/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Kenneth Leiter                                              */
/*     kenneth.leiter@arl.army.mil                                 */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2009 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/

/**
 * Determines whether two XDMF files contain equivalent data.
 *
 * Intended to be used as both a command line utility and a framework for code
 * testing purposes.
 *
 * Command Line:
 * 		There are two ways to run the command line utility:
 *
 * 		XdmfDiff referenceFile newFile
 * 			Compares all information contained in referenceFile to newFile .  Extra grids
 * 			contained in newFile that are not in referenceFile are ignored.
 *
 * 		XdmfDiff referenceFile newFile settingsFile
 * 			Compares information contained in referenceFile to newFile according to settings
 * 			specified in the settingsFile.  All settings options are outlined below:
 *
 * Settings Options:
 *		WITH_RELATIVE_ERROR .15
 *		WITH_ABSOLUTE_ERROR 1
 *		INCLUDE_GRID grid1 grid2
 *		IGNORE_GRID grid1 grid2
 *		IGNORE_GEOMETRY
 *		IGNORE_TOPOLOGY
 *		INCLUDE_ATTRIBUTE attr1 attr2
 *		IGNORE_ATTRIBUTE attr1 attr2
 *		IGNORE_ALL_ATTRIBUTES
 *		DISPLAY_FAILURES_ONLY
 *
 *		Settings are commented out with #
 *
 *	For code testing purposes run XdmfDiff::AreEquivalent().
 */

#include "XdmfDiff.h"

#include <XdmfAttribute.h>
#include <XdmfGrid.h>
#include <XdmfGeometry.h>
#include <XdmfTopology.h>

#include <cmath>

#include <sstream>
#include <vector>
#include <string>

/*
 * Constructs an XdmfDiff object to compare two Xdmf Files
 *
 * @param refFileName the path to an Xdmf file to compare
 * @param newFileName the path to an Xdmf file to compare
 *
 */
XdmfDiff::XdmfDiff(XdmfConstString refFileName, XdmfConstString newFileName)
{
	myRefDOM = new XdmfDOM();
	myNewDOM = new XdmfDOM();

	std::string refstr = refFileName;
	size_t reffound = refstr.find_last_of("/\\");
	if (reffound != std::string::npos)
	{
		myRefDOM->SetWorkingDirectory(refstr.substr(0,reffound).substr().c_str());
	}

	std::string newstr = newFileName;
	size_t newfound = newstr.find_last_of("/\\");
	if (newfound != std::string::npos)
	{
		myNewDOM->SetWorkingDirectory(newstr.substr(0,newfound).substr().c_str());
	}

	myRefDOM->Parse(refFileName);
	myNewDOM->Parse(newFileName);

	relErrorOn = false;
	absErrorOn = false;
	this->SetIgnoreGeometry(false);
	this->SetIgnoreGeometry(false);
	this->SetIgnoreAllAttributes(false);
	areEquivalent = false;
	diffRun = false;
}

XdmfDiff::~XdmfDiff()
{
	delete myRefDOM;
	delete myNewDOM;
}

/*
 * Constructs an XdmfDiff object to compare two Xdmf Files
 *
 * @param refDOM an XdmfDOM to compare
 * @param newDOM an XdmfDOM to compare
 *
 */
XdmfDiff::XdmfDiff(XdmfDOM * refDOM, XdmfDOM * newDOM)
{
	myRefDOM = new XdmfDOM();
	myNewDOM = new XdmfDOM();

	myRefDOM = refDOM;
	myNewDOM = newDOM;

	relErrorOn = false;
	absErrorOn = false;
	this->SetIgnoreGeometry(false);
	this->SetIgnoreGeometry(false);
	this->SetIgnoreAllAttributes(false);
	areEquivalent = false;
	diffRun = false;
}

/*
 * Sets the acceptable relative error between values.  Relative Errors and Absolute Errors
 * can not be used at the same time.
 *
 * @param relativeError the acceptable relative error in decimal form
 *
 */
void XdmfDiff::SetRelativeError(XdmfFloat64 relativeError)
{
	myRelativeError = relativeError;
	relErrorOn = true;
	absErrorOn = false;
}

/*
 * Sets the acceptable absolute error between values.  Relative Errors and Absolute Errors
 * can not be used at the same time.
 *
 * @param absoluteError the acceptable absolute error
 *
 */
void XdmfDiff::SetAbsoluteError(XdmfFloat64 absoluteError)
{
	myAbsoluteError = absoluteError;
	relErrorOn = false;
	absErrorOn = true;
}

/*
 * Sets the differ to ignore topology values.
 *
 * @param value whether to ignore topology values (default: true)
 *
 */
void XdmfDiff::SetIgnoreTopology(XdmfBoolean value)
{
	ignoreTopology = value;
}

/*
 * Sets the differ to ignore geometry values.
 *
 * @param value whether to ignore geometry values (default: true)
 *
 */
void XdmfDiff::SetIgnoreGeometry(XdmfBoolean value)
{
	ignoreGeometry = value;
}

/*
 * Sets the differ to ignore all attribute values.
 *
 * @param value whether to ignore all attribute values (default: true)
 *
 */
void XdmfDiff::SetIgnoreAllAttributes(XdmfBoolean value)
{
	ignoreAllAttributes = value;
}

/*
 * Determines whether the two grids are equivalent.
 *
 */
XdmfBoolean XdmfDiff::AreEquivalent()
{
	if (!diffRun)
	{
		this->GetDiffs();
	}
	return areEquivalent;
}

/*
 * Only return failure notices. No "PASS" notifications.
 *
 * @param value whether to return only failure notices (default: true)
 *
 */
void XdmfDiff::DisplayFailuresOnly(XdmfBoolean value)
{
	displayFailuresOnly = value;
}

/*
 * Returns the differences between the two XDMF files.  Compares each grid in the
 * reference file to the grid of the same name in the second file.  Returns a string
 * containing the result of the comparison.
 *
 */
std::string XdmfDiff::GetDiffs()
{
	areEquivalent = true;
	std::stringstream toReturn;

	XdmfXmlNode currDomain = myRefDOM->FindElement("Domain");
	for (int i=0; i<myRefDOM->FindNumberOfElements("Grid", currDomain); i++)
	{
		XdmfGrid * grid = new XdmfGrid();
		grid->SetDOM(myRefDOM);
		grid->SetElement(myRefDOM->FindElement("Grid", i, currDomain));
		grid->Update();
		toReturn << this->GetDiffs(grid->GetName());
		delete grid;
	}
	diffRun = true;
	return toReturn.str();
}

/*
 * Returns the differences between the grid specified in two XDMF files.  If the grid
 * can not be found, comparison quits and a notification of failure is returned.
 *
 * @param gridName the name of the grid to compare
 *
 */
std::string XdmfDiff::GetDiffs(XdmfConstString gridName)
{

	if (!diffRun)
	{
		areEquivalent = true;
		diffRun = true;
	}

	// Check for user specified grid includes / excludes
	if (includedGrids.size() != 0)
	{
		if(includedGrids.find(gridName) == includedGrids.end())
		{
			return "";
		}
	}

	if (ignoredGrids.size() != 0)
	{
		if(ignoredGrids.find(gridName) != ignoredGrids.end())
		{
			return "";
		}
	}

	std::stringstream toReturn;

	XdmfGrid * refGrid = this->GetGrid(gridName, myRefDOM);
	XdmfGrid * newGrid = this->GetGrid(gridName, myNewDOM);

	if (refGrid == NULL)
	{
		areEquivalent = false;
		if(newGrid == NULL)
		{
			toReturn << "Files do not contain grid named: " << gridName;
			return toReturn.str().c_str();
		}
		toReturn << "Reference file does not contain grid named: " << gridName;
		return toReturn.str().c_str();
	}

	if (newGrid == NULL)
	{
		areEquivalent = false;
		toReturn << "New file does not contain grid named: " << gridName;
		return toReturn.str().c_str();
	}

	std::string pointDiffs;
	if (!ignoreGeometry)
	{
		pointDiffs = (std::string)this->GetPointDiffs(refGrid, newGrid);
	}

	std::string connDiffs;
	if (!ignoreTopology)
	{
		connDiffs = (std::string)this->GetConnectionDiffs(refGrid, newGrid);
	}
	toReturn << pointDiffs << connDiffs;

	for (int i=0; i<refGrid->GetNumberOfAttributes(); i++)
	{
		std::string currAttributeDiffs;
		if (includedAttributes.size() != 0)
		{
			if(includedAttributes.find(refGrid->GetAttribute(i)->GetName()) != includedAttributes.end())
			{
				currAttributeDiffs = (std::string)this->GetAttributeDiffs(refGrid, newGrid, refGrid->GetAttribute(i)->GetName());
			}
		}
		else
		{
			if (ignoredAttributes.size() != 0)
			{
				if(ignoredAttributes.find(refGrid->GetAttribute(i)->GetName()) == ignoredAttributes.end())
				{
					currAttributeDiffs = (std::string)this->GetAttributeDiffs(refGrid, newGrid, refGrid->GetAttribute(i)->GetName());
				}
			}
			else
			{
				currAttributeDiffs = (std::string)this->GetAttributeDiffs(refGrid, newGrid, refGrid->GetAttribute(i)->GetName());
			}
		}

		toReturn << currAttributeDiffs;
	}

	delete refGrid;
	delete newGrid;
	return toReturn.str();
}

/*
 * Returns the differences in geometry between the two grids.
 *
 * @param refGrid the XdmfGrid containing geometry to compare
 * @param newGrid the XdmfGrid containing geometry to compare
 *
 */
std::string XdmfDiff::GetPointDiffs(XdmfGrid * refGrid, XdmfGrid * newGrid)
{
	std::stringstream toReturn;
	if (refGrid->GetGeometry()->GetGeometryType() != newGrid->GetGeometry()->GetGeometryType())
	{
		toReturn << "FAIL: Geometry Types of Grid \"" << refGrid->GetName() << "\" do not match";
		return toReturn.str();
	}

	XdmfArray * refPoints = refGrid->GetGeometry()->GetPoints();
	XdmfArray * newPoints = newGrid->GetGeometry()->GetPoints();

	std::string returned;
	switch(refGrid->GetGeometry()->GetGeometryType())
	{
	case XDMF_GEOMETRY_XYZ:
		returned = (std::string)this->CompareValues(refPoints, newPoints, 0, refPoints->GetNumberOfElements(), 3);
		break;
	case XDMF_GEOMETRY_XY:
		returned = (std::string)this->CompareValues(refPoints, newPoints, 0, refPoints->GetNumberOfElements(), 2);
		break;
	default:
		returned = (std::string)this->CompareValues(refPoints, newPoints, 0, refPoints->GetNumberOfElements());
	}

	if(returned.compare("") == 0)
	{
		if(!displayFailuresOnly)
		{
			toReturn << "PASS: Acceptable Geometry Values in Grid \"" << refGrid->GetName() << "\"" << "\n";
		}
	}
	else
	{
		areEquivalent = false;
		toReturn << "FAIL: Unacceptable Geometry Values in Grid \"" << refGrid->GetName() << "\"" << ":\n" << returned;
	}
	return toReturn.str();
}

/*
 * Returns the differences in topology between the two grids.
 *
 * @param refGrid the XdmfGrid containing topology to compare
 * @param newGrid the XdmfGrid containing topology to compare
 *
 */
std::string XdmfDiff::GetConnectionDiffs(XdmfGrid * refGrid, XdmfGrid * newGrid)
{
	std::stringstream toReturn;
	if (refGrid->GetTopology()->GetTopologyType() != newGrid->GetTopology()->GetTopologyType())
	{
		toReturn << "FAIL: Topology Types of Grid \"" << refGrid->GetName() << "\" do not match";
		return toReturn.str();
	}

	XdmfArray * refConns = refGrid->GetTopology()->GetConnectivity();
	XdmfArray * newConns = newGrid->GetTopology()->GetConnectivity();

	std::string returned = (std::string)this->CompareValues(refConns, newConns, 0, refConns->GetNumberOfElements(), refGrid->GetTopology()->GetNodesPerElement());
	if(returned.compare("") == 0)
	{
		if(!displayFailuresOnly)
		{
			toReturn << "PASS: Acceptable Topology Values in Grid \"" << refGrid->GetName() << "\"" << "\n";
		}
	}
	else
	{
		areEquivalent = false;
		toReturn << "FAIL: Unacceptable Topology Values in Grid \"" << refGrid->GetName() << "\"" << ":\n" << returned;
	}
	return toReturn.str();
}

/*
 * Returns the differences in attribute between the two grids.
 *
 * @param refGrid the XdmfGrid containing attribute to compare
 * @param newGrid the XdmfGrid containing attribute to compare
 * @param attributeName the name of the attribute to compare
 *
 */
std::string XdmfDiff::GetAttributeDiffs(XdmfGrid * refGrid, XdmfGrid * newGrid, XdmfConstString attributeName)
{

	if (ignoreAllAttributes)
	{
		return "";
	}

	XdmfConstString attributeCenter;

	XdmfArray * refAttrVals;
	for (int j=0; j<refGrid->GetNumberOfAttributes(); j++)
	{
		if (strcmp(refGrid->GetAttribute(j)->GetName(), attributeName) == 0)
		{
			XdmfAttribute * currAttribute = refGrid->GetAttribute(j);
			currAttribute->Update();
			refAttrVals = currAttribute->GetValues();
			attributeCenter = currAttribute->GetAttributeCenterAsString();
			break;
		}
	}

	XdmfArray * newAttrVals;
	for (int j=0; j<newGrid->GetNumberOfAttributes(); j++)
	{
		if (strcmp(newGrid->GetAttribute(j)->GetName(), attributeName) == 0)
		{
			XdmfAttribute * currAttribute = newGrid->GetAttribute(j);
			currAttribute->Update();
			newAttrVals = currAttribute->GetValues();
			break;
		}
	}

	std::stringstream toReturn;
	std::string returned = (std::string)this->CompareValues(refAttrVals, newAttrVals, 0, newAttrVals->GetNumberOfElements());
	if(returned.compare("") == 0)
	{
		if(!displayFailuresOnly)
		{
			toReturn << "PASS: Acceptable Attribute Values in Grid \"" << refGrid->GetName() << "\"" << " for " << attributeCenter << " Attribute \"" << attributeName << "\"\n";
		}
	}
	else
	{
		areEquivalent = false;
		toReturn << "FAIL: Unacceptable Attribute Values in Grid \"" << refGrid->GetName() << "\"" << " for " << attributeCenter << " Attribute \"" << attributeName << "\":\n" << returned;
	}
	return toReturn.str();
}

/*
 * Returns an XdmfGrid of name gridName that exists in the DOM
 *
 * @param gridName the name of the grid to find in the DOM
 * @param DOM containing a grid named gridName
 *
 */
XdmfGrid * XdmfDiff::GetGrid(XdmfConstString gridName, XdmfDOM * DOM)
{
	XdmfXmlNode currDomain = DOM->FindElement("Domain");
	for (int i=0; i<DOM->FindNumberOfElements("Grid", currDomain); i++)
	{
		XdmfGrid * grid = new XdmfGrid();
		grid->SetDOM(DOM);
		grid->SetElement(DOM->FindElement("Grid", i, currDomain));
		grid->Update();
		if (strcmp(grid->GetName(), gridName) == 0)
		{
			return grid;
		}
	}
	return NULL;
}

/*
 * Compares values between two XdmfArrays
 *
 * @param refArray an XdmfArray containing values to compare
 * @param newArray an XdmfArray containing values to compare
 * @param startIndex an index to start comparison at
 * @param numValues the number of values to compare
 * @param groupLength how many values are contained together.
 * 		  Useful for reporting changes in multiple values i.e. XYZ geometry (default: 1)
 */
std::string XdmfDiff::CompareValues(XdmfArray * refArray, XdmfArray * newArray, XdmfInt64 startIndex, XdmfInt64 numValues, XdmfInt64 groupLength)
{
	if (groupLength < 1)
	{
		return "Invalid Group Length Entered";
	}

	std::stringstream toReturn;
	if (refArray->GetNumberType() == newArray->GetNumberType())
	{
		switch(refArray->GetNumberType())
		{
			case XDMF_INT8_TYPE:
			{
				XdmfInt8 * refVals = new XdmfInt8[numValues];
				XdmfInt8 * newVals = new XdmfInt8[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_INT16_TYPE:
			{
				XdmfInt16 * refVals = new XdmfInt16[numValues];
				XdmfInt16 * newVals = new XdmfInt16[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_INT32_TYPE:
			{
				XdmfInt32 * refVals = new XdmfInt32[numValues];
				XdmfInt32 * newVals = new XdmfInt32[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_INT64_TYPE:
			{
				XdmfInt64 * refVals = new XdmfInt64[numValues];
				XdmfInt64 * newVals = new XdmfInt64[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_FLOAT32_TYPE:
			{
				XdmfFloat32 * refVals = new XdmfFloat32[numValues];
				XdmfFloat32 * newVals = new XdmfFloat32[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_FLOAT64_TYPE:
			{
				XdmfFloat64 * refVals = new XdmfFloat64[numValues];
				XdmfFloat64 * newVals = new XdmfFloat64[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_UINT8_TYPE:
			{
				XdmfUInt8 * refVals = new XdmfUInt8[numValues];
				XdmfUInt8 * newVals = new XdmfUInt8[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_UINT16_TYPE:
			{
				XdmfUInt16 * refVals = new XdmfUInt16[numValues];
				XdmfUInt16 * newVals = new XdmfUInt16[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			case XDMF_UINT32_TYPE:
			{
				XdmfUInt32 * refVals = new XdmfUInt32[numValues];
				XdmfUInt32 * newVals = new XdmfUInt32[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) + groupLength - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
			default:
			{
				XdmfFloat64 * refVals = new XdmfFloat64[numValues];
				XdmfFloat64 * newVals = new XdmfFloat64[numValues];

				refArray->GetValues(startIndex, refVals, numValues, 1, 1);
				newArray->GetValues(startIndex, newVals, numValues, 1, 1);

				for (int i=0; i<numValues; i++)
				{
					XdmfFloat64 acceptableError = 0;
					if (relErrorOn)
					{
						acceptableError = fabs(refVals[i] * myRelativeError);
					}
					if (absErrorOn)
					{
						acceptableError = fabs(myAbsoluteError);
					}
					if(fabs((double)(newVals[i] - refVals[i])) > acceptableError)
					{
						XdmfInt64 groupIndex = (int)((startIndex+i) / groupLength);
						std::stringstream refValsReturn;
						std::stringstream newValsReturn;
						for (int j=0; j<groupLength; j++)
						{
							refValsReturn << refVals[(groupIndex * groupLength) + j];
							newValsReturn << newVals[(groupIndex * groupLength) + j];
							if (j+1 < groupLength)
							{
								refValsReturn << ", ";
								newValsReturn << ", ";
							}
						}
						toReturn << "At ID " << groupIndex << " | Expected: " << refValsReturn.str() << " | Got: " << newValsReturn.str() << "\n";
						i = (groupIndex * groupLength) - 1;
					}
				}
				delete [] refVals;
				delete [] newVals;
				return toReturn.str();
			}
		}
	}
	else
	{
		return "Arrays not of the same number type";
	}
	return "ERROR";
}

/**
 *
 * Parses a file containing settings for the comparison.  Settings are outlined at the top
 * of this file.  Commented lines starting with '#' are ignored.
 *
 */
void XdmfDiff::ParseSettingsFile(XdmfConstString settingsFile)
{
       std::string str;
       ifstream ifs(settingsFile, ifstream::in);
       while(std::getline(ifs,str))
       {
    	   if (str != "")
    	   {
    		   if(str.substr(0,1).compare("#") != 0)
    		   {
    			   std::string buf;
    			   std::stringstream ss(str);

    			   std::vector<std::string> tokens;
    			   while (ss >> buf)
    			   {
    				   tokens.push_back(buf);
    			   }

    			   if (tokens[0].compare("WITH_RELATIVE_ERROR") == 0)
    			   {
    				   std::istringstream stm;
    				   stm.str(tokens[1]);
    				   double d;
    				   stm >> d;
    				   this->SetRelativeError(d);
    			   }

    			   if (tokens[0].compare("WITH_ABSOLUTE_ERROR") == 0)
    			   {
    				   std::istringstream stm;
    				   stm.str(tokens[1]);
    				   double d;
    				   stm >> d;
    				   this->SetAbsoluteError(d);
    			   }

    			   if (tokens[0].compare("INCLUDE_GRID") == 0)
    			   {
    				   for (unsigned int i=1; i<tokens.size(); i++)
    			       {
    					   includedGrids.insert(tokens[i]);
    			       }
    			   }

    			   if (tokens[0].compare("IGNORE_GRID") == 0)
    			   {
    				   for (unsigned int i=1; i<tokens.size(); i++)
    			       {
    					   ignoredGrids.insert(tokens[i]);
    			       }
    			   }

    			   if (tokens[0].compare("IGNORE_GEOMETRY") == 0)
    			   {
    				   this->SetIgnoreGeometry(true);
    			   }

    			   if (tokens[0].compare("IGNORE_TOPOLOGY") == 0)
    			   {
    				   this->SetIgnoreTopology(true);
    			   }

    			   if (tokens[0].compare("INCLUDE_ATTRIBUTE") == 0)
    			   {
    				   for (unsigned int i=1; i<tokens.size(); i++)
    			       {
    					   includedAttributes.insert(tokens[i]);
    			       }
    			   }

    			   if (tokens[0].compare("IGNORE_ATTRIBUTE") == 0)
    			   {
    				   for (unsigned int i=1; i<tokens.size(); i++)
    			       {
    					   ignoredAttributes.insert(tokens[i]);
    			       }
    			   }

    			   if (tokens[0].compare("IGNORE_ALL_ATTRIBUTES") == 0)
    			   {
    				   this->SetIgnoreAllAttributes(true);
    			   }

    			   if (tokens[0].compare("DISPLAY_FAILURES_ONLY") == 0)
    			   {
    			       displayFailuresOnly = true;
    			   }
    		   }
    	   }
       }
       ifs.close();
}

/**
 * Entry point for command line utility
 *
 */
int main(int argc, char* argv[]) {

   XdmfDiff * diffFramework;

   if (argc < 3)
   {
	   cout << "Improper Number Of Arguments Given" << endl;
	   return 1;
   }

   if (argc >= 3)
   {
	   FILE * refFile = fopen(argv[1],"r");
	   if(refFile)
	   {
		   // Success
		   fclose(refFile);
	   }
	   else
	   {
		   cout << "Cannot open: " << argv[1] << endl;
		   return 1;
	   }

	   FILE * newFile = fopen(argv[2],"r");
	   if(newFile)
	   {
		   // Success
		   fclose(newFile);
	   }
	   else
	   {
		   cout << "Cannot open: " << argv[2] << endl;
		   return 1;
	   }

	   diffFramework = new XdmfDiff(argv[1], argv[2]);

	   if(argc >= 4)
	   {
		   FILE * defFile = fopen(argv[3],"r");
		   if(defFile)
		   {
			   // Success
			   fclose(defFile);
		   }
		   else
		   {
			   cout << "Cannot open: " << argv[3] << endl;
			   return 1;
		   }
		   diffFramework->ParseSettingsFile(argv[3]);
	   }
   }

   std::string output = diffFramework->GetDiffs();
   cout << output << endl;
   delete diffFramework;
   return 0;
}
