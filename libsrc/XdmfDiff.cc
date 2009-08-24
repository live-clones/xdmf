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
 *		RELATIVE_ERROR .15
 *		ABSOLUTE_ERROR 1
 *		INCLUDE_GRID grid1 grid2
 *		IGNORE_GRID grid1 grid2
 *		IGNORE_GEOMETRY
 *		IGNORE_TOPOLOGY
 *		INCLUDE_ATTRIBUTE attr1 attr2
 *		IGNORE_ATTRIBUTE attr1 attr2
 *		IGNORE_ALL_ATTRIBUTES
 *		DISPLAY_FAILURES_ONLY
 *		VERBOSE_OUTPUT
 *
 *		Settings can be commented out with #
 *
 *	For code testing purposes run XdmfDiff::AreEquivalent().
 */

#include "XdmfDiff.h"

#include <XdmfAttribute.h>
#include <XdmfGrid.h>
#include <XdmfGeometry.h>
#include <XdmfTime.h>
#include <XdmfTopology.h>

#include <cmath>

#include <sstream>
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

	XdmfDiff(myRefDOM, myNewDOM);

    refDOMIsMine = true;
    newDOMIsMine = true;

}

XdmfDiff::~XdmfDiff()
{
	if (this->refDOMIsMine) delete myRefDOM;
	if (this->newDOMIsMine) delete myNewDOM;
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
	myRefDOM = refDOM;
	myNewDOM = newDOM;
	myRelativeError = 0;
    myAbsoluteError = 0;
	IgnoreTime = false;
    IgnoreGeometry = false;
    IgnoreTopology = false;
    IgnoreAllAttributes = false;
    DisplayFailuresOnly = false;
    VerboseOutput = false;
    refDOMIsMine = false;
    newDOMIsMine = false;
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
	myAbsoluteError = 0;
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
	myRelativeError = 0;
}

/*
 * Determines whether the two files are equivalent.
 *
 * @return an XdmfBoolean true = equivalent, false = nonequivalent
 *
 */
XdmfBoolean XdmfDiff::AreEquivalent()
{
	XdmfDiffReportCollection myErrors = XdmfDiffReportCollection(DisplayFailuresOnly, VerboseOutput);
	this->GetDiffs(myErrors);
	if (myErrors.GetNumberOfErrors() == 0)
	{
		return true;
	}
	return false;
}

/*
 * Get the differences between the two Xdmf files as a string
 *
 */
std::string XdmfDiff::GetDiffsAsString()
{
	std::stringstream toReturn;
	XdmfDiffReportCollection myErrors = XdmfDiffReportCollection(DisplayFailuresOnly, VerboseOutput);
	this->GetDiffs(myErrors);
	toReturn << myErrors;
	return toReturn.str();
}

/*
 * Get the differences between a grid in the two Xdmf files as a string
 *
 * @param gridName the name of the grid to compare
 *
 */
std::string XdmfDiff::GetDiffsAsString(XdmfConstString gridName)
{
	std::stringstream toReturn;
	XdmfXmlNode currDomain = myRefDOM->FindElement("Domain");
	for (int i=0; i<myRefDOM->FindNumberOfElements("Grid", currDomain); i++)
	{
			XdmfGrid grid = XdmfGrid();
			grid.SetDOM(myRefDOM);
			grid.SetElement(myRefDOM->FindElement("Grid", i, currDomain));
			grid.Update();
			if (strcmp(grid.GetName(), gridName) == 0)
			{
				XdmfDiffReportCollection errorReports = XdmfDiffReportCollection(DisplayFailuresOnly, VerboseOutput);
				this->GetDiffs(errorReports);
				toReturn << errorReports;
				return toReturn.str();
			}
	}
	toReturn << "FAIL: Cannot Find Grid Named " << gridName;
	return toReturn.str();
}

/*
 * Returns the differences between the two XDMF files.  Compares each grid in the
 * reference file to the grid of the same name in the second file.  Stuffs the results
 * of the comparison in the error report
 *
 * @param errorReports an XdmfDiffReportCollection that collects all difference reports during the comparison
 *
 */
void XdmfDiff::GetDiffs(XdmfDiff::XdmfDiffReportCollection & errorReports)
{
	XdmfXmlNode currDomain = myRefDOM->FindElement("Domain");
	for (int i=0; i<myRefDOM->FindNumberOfElements("Grid", currDomain); i++)
	{
		XdmfGrid grid = XdmfGrid();
		grid.SetDOM(myRefDOM);
		grid.SetElement(myRefDOM->FindElement("Grid", i, currDomain));
		grid.Update();
		// Make sure we cleanup well
		for (int j=0; j<grid.GetNumberOfAttributes(); j++)
		{
			grid.GetAttribute(j)->SetDeleteOnGridDelete(true);
		}
		this->GetDiffs(grid, errorReports);
	}
}

/*
 * Returns the differences between two grids.
 *
 * @param refGrid the reference grid to compare (new grid is searched for using the reference grid's name)
 * @param errorReports an XdmfDiffReportCollection that collects all difference reports during the comparison
 *
 */
void XdmfDiff::GetDiffs(XdmfGrid & refGrid, XdmfDiff::XdmfDiffReportCollection & errorReports)
{
	// Check for user specified grid includes / excludes
	if (includedGrids.size() != 0)
	{
		if(includedGrids.find(refGrid.GetName()) == includedGrids.end())
		{
			return;
		}
	}

	if (ignoredGrids.size() != 0)
	{
		if(ignoredGrids.find(refGrid.GetName()) != ignoredGrids.end())
		{
			return;
		}
	}

	XdmfGrid newGrid = XdmfGrid();
	newGrid.SetDOM(myNewDOM);

	std::string refPath = myRefDOM->GetPath(refGrid.GetElement());
	std::string parentPath = refPath.substr(0,refPath.find_last_of("/"));

	XdmfXmlNode newNode;
	if(refGrid.GetGridType() == XDMF_GRID_COLLECTION)
	{
		newNode = myNewDOM->FindElementByPath(refPath.c_str());
	}
	else
	{
		newNode= myNewDOM->FindElementByAttribute("Name", refGrid.GetName(), 0, myNewDOM->FindElementByPath(parentPath.c_str()));
	}

	if (newNode == NULL || newGrid.SetElement(newNode) != XDMF_SUCCESS)
	{
		XdmfDiffReport diffReport = XdmfDiffReport("Grid Name");
		std::stringstream warning;
		warning << "Could Not Find Grid: " << refGrid.GetName();
		diffReport.AddError(warning.str());
		errorReports.AddReport(refGrid.GetName(), diffReport);
		return;
	}
	newGrid.Update();

	XdmfDiffReport diffReport = XdmfDiffReport("Grid Type");
	if (refGrid.GetGridType() != newGrid.GetGridType())
	{
		diffReport.AddError("Grid Type", refGrid.GetGridTypeAsString(), newGrid.GetGridTypeAsString());
	}
	errorReports.AddReport(refGrid.GetName(), diffReport);

	if (refGrid.GetGridType() == XDMF_GRID_COLLECTION)
	{
		if (newGrid.GetGridType() == XDMF_GRID_COLLECTION)
		{
			XdmfDiffReport diffReport = XdmfDiffReport("Collection Type");
			if (refGrid.GetCollectionType() != newGrid.GetCollectionType())
			{
				diffReport.AddError(0, refGrid.GetCollectionTypeAsString(), newGrid.GetCollectionTypeAsString());
			}
			errorReports.AddReport(refGrid.GetName(), diffReport);
		}
	}
	else
	{
		if (!IgnoreGeometry)
		{
			errorReports.AddReport(refGrid.GetName(), this->GetGeometryDiffs(refGrid.GetGeometry(), newGrid.GetGeometry()));
		}

		if (!IgnoreTopology)
		{
			errorReports.AddReport(refGrid.GetName(), this->GetTopologyDiffs(refGrid.GetTopology(), newGrid.GetTopology()));
		}

		if (!IgnoreAllAttributes)
		{
			for (int i=0; i<refGrid.GetNumberOfAttributes(); i++)
			{
				refGrid.GetAttribute(i)->Update();
				XdmfAttribute * newAttribute = NULL;
				for (int j=0; j<newGrid.GetNumberOfAttributes(); j++)
				{
					if (strcmp(newGrid.GetAttribute(j)->GetName(), refGrid.GetAttribute(i)->GetName()) == 0)
					{
						newAttribute = newGrid.GetAttribute(j);
						newAttribute->Update();
					}
				}
				if (newAttribute != NULL)
				{
					if (includedAttributes.size() != 0)
					{
						if(includedAttributes.find(refGrid.GetAttribute(i)->GetName()) != includedAttributes.end())
						{
							errorReports.AddReport(refGrid.GetName(), this->GetAttributeDiffs(refGrid.GetAttribute(i), newAttribute));
						}
					}
					else
					{
						if (ignoredAttributes.size() != 0)
						{
							if(ignoredAttributes.find(refGrid.GetAttribute(i)->GetName()) == ignoredAttributes.end())
							{
								errorReports.AddReport(refGrid.GetName(), this->GetAttributeDiffs(refGrid.GetAttribute(i), newAttribute));
							}
						}
						else
						{
							errorReports.AddReport(refGrid.GetName(), this->GetAttributeDiffs(refGrid.GetAttribute(i), newAttribute));
						}
					}
				}
				else
				{
					std::stringstream heading;
					heading << "Attribute " << refGrid.GetAttribute(i)->GetName();
					XdmfDiffReport diffReport = XdmfDiffReport(heading.str());
					std::stringstream warning;
					warning << "Could Not Find "<< "Attribute: " << refGrid.GetAttribute(i)->GetName();
					diffReport.AddError(warning.str());
					errorReports.AddReport(refGrid.GetName(), diffReport);
				}
			}
		}
	}

	if (!IgnoreTime)
	{
		XdmfDiffReport diffReport = XdmfDiffReport("Time");
		if (refGrid.GetTime()->GetValue() != newGrid.GetTime()->GetValue())
		{
			std::stringstream refTime;
			std::stringstream newTime;
			refTime << refGrid.GetTime()->GetValue();
			newTime << newGrid.GetTime()->GetValue();
			diffReport.AddError(0, refTime.str(), newTime.str());
			errorReports.AddReport(refGrid.GetName(), diffReport);
		}
	}

	for (int i=0; i<refGrid.GetNumberOfChildren(); i++)
	{
		XdmfGrid grid = XdmfGrid();
		grid.SetDOM(myRefDOM);
		grid.SetElement(refGrid.GetChild(i)->GetElement());
		grid.Update();
		this->GetDiffs(grid, errorReports);
	}
}

/*
 * Returns the differences in values between two XdmfGeometries
 *
 * @param refGeometry an XdmfGeometry to compare
 * @param newGeometry an XdmfGeometry to compare
 *
 * @return an XdmfDiffReport containing differences in values
 *
 */
XdmfDiff::XdmfDiffReport XdmfDiff::GetGeometryDiffs(XdmfGeometry * refGeometry, XdmfGeometry * newGeometry)
{
	XdmfDiffReport diffReport = XdmfDiffReport("Geometry");

	if (refGeometry->GetGeometryType() != newGeometry->GetGeometryType())
	{
		diffReport.AddError("Geometry Type", refGeometry->GetGeometryTypeAsString(), newGeometry->GetGeometryTypeAsString());
	}

	switch(refGeometry->GetGeometryType())
	{
	case XDMF_GEOMETRY_XYZ:
		this->CompareValues(diffReport, refGeometry->GetPoints(), newGeometry->GetPoints(), 0, refGeometry->GetPoints()->GetNumberOfElements(), 3);
		break;
	case XDMF_GEOMETRY_XY:
		this->CompareValues(diffReport, refGeometry->GetPoints(), newGeometry->GetPoints(), 0, refGeometry->GetPoints()->GetNumberOfElements(), 2);
		break;
	default:
		this->CompareValues(diffReport, refGeometry->GetPoints(), newGeometry->GetPoints(), 0, refGeometry->GetPoints()->GetNumberOfElements());
	}

	return diffReport;
}

/*
 * Returns the differences in values between two XdmfTopologies
 *
 * @param refTopology an XdmfTopology to compare
 * @param newTopology an XdmfTopology to compare
 *
 * @return an XdmfDiffReport containing differences in values
 *
 */
XdmfDiff::XdmfDiffReport XdmfDiff::GetTopologyDiffs(XdmfTopology * refTopology, XdmfTopology * newTopology)
{
	XdmfDiffReport diffReport = XdmfDiffReport("Topology");

	if (refTopology->GetTopologyType() != newTopology->GetTopologyType())
	{
		diffReport.AddError("Topology Type", refTopology->GetTopologyTypeAsString(), newTopology->GetTopologyTypeAsString());
	}

	this->CompareValues(diffReport, refTopology->GetConnectivity(), newTopology->GetConnectivity(), 0, refTopology->GetNumberOfElements(), refTopology->GetNodesPerElement());
	return diffReport;
}

/*
 * Returns the differences in values between two XdmfAttributes
 *
 * @param refAttribute an XdmfAttribute to compare
 * @param newAttribute an XdmfAttribute to compare
 *
 * @return an XdmfDiffReport containing differences in values
 *
 */
XdmfDiff::XdmfDiffReport XdmfDiff::GetAttributeDiffs(XdmfAttribute * refAttribute, XdmfAttribute * newAttribute)
{
	std::stringstream valType;
	valType << "Attribute " << refAttribute->GetName();
	XdmfDiffReport diffReport = XdmfDiffReport(valType.str());

	int numValsPerNode = 1;
	switch (refAttribute->GetAttributeType())
	{
		case XDMF_ATTRIBUTE_TYPE_VECTOR:
			numValsPerNode = 3;
			break;
		case XDMF_ATTRIBUTE_TYPE_TENSOR6:
			numValsPerNode = 6;
			break;
		case XDMF_ATTRIBUTE_TYPE_TENSOR:
			numValsPerNode = 9;
			break;
		default:
			numValsPerNode = 1;
			break;
	}

	if (refAttribute->GetAttributeCenter() != newAttribute->GetAttributeCenter())
	{
		diffReport.AddError("Attribute Center", refAttribute->GetAttributeCenterAsString(), newAttribute->GetAttributeCenterAsString());
	}

	if (refAttribute->GetAttributeType() != newAttribute->GetAttributeType())
	{
		numValsPerNode = 1;
		diffReport.AddError("Attribute Type", refAttribute->GetAttributeTypeAsString(), newAttribute->GetAttributeTypeAsString());
	}

	this->CompareValues(diffReport, refAttribute->GetValues(), newAttribute->GetValues(), 0, refAttribute->GetValues()->GetNumberOfElements(), numValsPerNode);
	return diffReport;
}

/*
 * Compares values between two XdmfArrays
 *
 * @param errorReport an XdmfDiffReport to add comparison results to
 * @param refArray an XdmfArray containing values to compare
 * @param newArray an XdmfArray containing values to compare
 * @param startIndex an index to start comparison at
 * @param numValues the number of values to compare
 * @param groupLength how many values are contained together.
 * 		  Useful for reporting changes in multiple values i.e. XYZ geometry (default: 1)
 *
 */
void XdmfDiff::CompareValues(XdmfDiffReport & errorReport, XdmfArray * refArray, XdmfArray * newArray, XdmfInt64 startIndex, XdmfInt64 numValues, XdmfInt64 groupLength)
{
	if (groupLength < 1)
	{
		return;
	}

	if (refArray->GetNumberOfElements() != newArray->GetNumberOfElements())
	{
		errorReport.AddError("Number of Elements" , refArray->GetNumberOfElements() + "", refArray->GetNumberOfElements() + "");
	}

	if (strcmp(refArray->GetShapeAsString(), newArray->GetShapeAsString()) != 0)
	{
		errorReport.AddError("Shape" , refArray->GetShapeAsString(), newArray->GetShapeAsString());
	}

	if (refArray->GetNumberType() != newArray->GetNumberType())
	{
		errorReport.AddError("Number Type" , refArray->GetNumberTypeAsString(), newArray->GetNumberTypeAsString());
	}

	switch(refArray->GetNumberType())
	{
		case XDMF_FLOAT64_TYPE:
		{
			XdmfFloat64 * refVals = (XdmfFloat64*)refArray->GetDataPointer(startIndex);
			XdmfFloat64 * newVals = (XdmfFloat64*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_FLOAT32_TYPE:
		{
			XdmfFloat32 * refVals = (XdmfFloat32*)refArray->GetDataPointer(startIndex);
			XdmfFloat32 * newVals = (XdmfFloat32*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_INT64_TYPE:
		{
			XdmfInt64 * refVals = (XdmfInt64*)refArray->GetDataPointer(startIndex);
			XdmfInt64 * newVals = (XdmfInt64*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_INT32_TYPE:
		{
			XdmfInt32 * refVals = (XdmfInt32*)refArray->GetDataPointer(startIndex);
			XdmfInt32 * newVals = (XdmfInt32*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_INT16_TYPE:
		{
			XdmfInt16 * refVals = (XdmfInt16*)refArray->GetDataPointer(startIndex);
			XdmfInt16 * newVals = (XdmfInt16*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_INT8_TYPE:
		{
			XdmfInt8 * refVals = (XdmfInt8*)refArray->GetDataPointer(startIndex);
			XdmfInt8 * newVals = (XdmfInt8*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_UINT32_TYPE:
		{
			XdmfUInt32 * refVals = (XdmfUInt32*)refArray->GetDataPointer(startIndex);
			XdmfUInt32 * newVals = (XdmfUInt32*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_UINT16_TYPE:
		{
			XdmfUInt16 * refVals = (XdmfUInt16*)refArray->GetDataPointer(startIndex);
			XdmfUInt16 * newVals = (XdmfUInt16*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		case XDMF_UINT8_TYPE:
		{
			XdmfUInt8 * refVals = (XdmfUInt8*)refArray->GetDataPointer(startIndex);
			XdmfUInt8 * newVals = (XdmfUInt8*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) + groupLength - 1;
				}
			}
			return;
		}
		default:
		{
			XdmfFloat64 * refVals = (XdmfFloat64*)refArray->GetDataPointer(startIndex);
			XdmfFloat64 * newVals = (XdmfFloat64*)newArray->GetDataPointer(startIndex);

			for (int i=0; i<numValues; i++)
			{
				XdmfFloat64 acceptableError = fabs(myAbsoluteError);
				if (acceptableError == 0)
				{
					acceptableError = fabs(refVals[i] * myRelativeError);
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
					errorReport.AddError("Values", groupIndex, refValsReturn.str(), newValsReturn.str());
					i = (groupIndex * groupLength) - 1;
				}
			}
			return;
		}
	}
	return;
}

/**
 *
 * Parses a file containing settings for the comparison.  Settings are outlined at the top
 * of this file.  Commented lines starting with '#' are ignored.
 *
 * @param settingsFile the file path to the settings file
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
   			std::string buf;
   			std::stringstream ss(str);

   			std::vector<std::string> tokens;
   			while (ss >> buf)
   			{
   				tokens.push_back(buf);
   			}

   			if (tokens[0].compare("RELATIVE_ERROR") == 0)
   			{
   				std::istringstream stm;
   				stm.str(tokens[1]);
   				double d;
   				stm >> d;
   				this->SetRelativeError(d);
   			}

   			if (tokens[0].compare("ABSOLUTE_ERROR") == 0)
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

    		if (tokens[0].compare("IGNORE_TIME") == 0)
    		{
    			this->SetIgnoreTime(true);
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
   				this->SetDisplayFailuresOnly(true);
   			}

   			if (tokens[0].compare("VERBOSE_OUTPUT") == 0)
   			{
   				this->SetVerboseOutput(true);
   			}
    	}
    }
    ifs.close();
}
