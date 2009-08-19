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

#ifndef XDMFDIFF_H_
#define XDMFDIFF_H_

#include <XdmfArray.h>
#include <XdmfGrid.h>
#include <XdmfDOM.h>
#include <map>
#include <set>
#include <vector>

class XdmfDiff{
public:

	class XdmfDiffReport{
	public:

		class XdmfDiffEntry{
		public:
			XdmfDiffEntry(std::string errorDescription, XdmfInt64 loc, std::string refVals, std::string newVals)
			{
				description = errorDescription;
				location = loc;
				refValues = refVals;
				newValues = newVals;
			}
			~XdmfDiffEntry(){}
			friend std::ostream &operator<<(std::ostream & toReturn, const XdmfDiffEntry &diffEntry)
			{
				if (diffEntry.location == -1)
				{
					toReturn << "For " << diffEntry.description << " | Expected : " << diffEntry.refValues << " | Got : " << diffEntry.newValues;
				}
				else
				{
					toReturn << "For " << diffEntry.description << " | At Tuple " << diffEntry.location << " | Expected : " << diffEntry.refValues << " | Got : " << diffEntry.newValues;
				}
				return toReturn;
			}
		private:
			XdmfInt64 location;
			std::string refValues;
			std::string newValues;
			std::string description;
		};

		XdmfDiffReport(std::string type)
		{
			valType = type;
		}
		~XdmfDiffReport(){}
		void AddError(std::string errorDescription, std::string refVals, std::string newVals)
		{
			this->AddError(errorDescription, -1, refVals, newVals);
		}
		void AddError(std::string errorDescription, XdmfInt64 loc, std::string refVals, std::string newVals)
		{
			errors.push_back(XdmfDiffEntry(errorDescription, loc, refVals, newVals));
		}
		void AddError(std::string warning)
		{
			warnings.push_back(warning);
		}
		XdmfInt64 GetNumberOfErrors()
		{
			return errors.size() + warnings.size();
		}

		friend std::ostream &operator<<(std::ostream & toReturn, const XdmfDiffReport &diffReport)
		{
			toReturn << diffReport.valType << "\n";
			for (unsigned int i=0; i<diffReport.warnings.size(); i++)
			{
				toReturn << "\t\t" << diffReport.warnings[i] << "\n";
			}
			for (unsigned int i=0; i<diffReport.errors.size(); i++)
			{
				toReturn << "\t\t" << diffReport.errors[i] << "\n";
			}
			return toReturn;
		}

	private:
		std::vector<XdmfDiffEntry> errors;
		std::vector<std::string> warnings;
		std::string valType;
	};

	class XdmfDiffReportCollection{
	public:
		XdmfDiffReportCollection(XdmfBoolean failuresOnly, XdmfBoolean verbose)
		{
			displayFailuresOnly = failuresOnly;
			verboseOutput = verbose;
		}
		~XdmfDiffReportCollection(){}
		void AddReport(std::string gridName, XdmfDiffReport report)
		{
			reports[gridName].push_back(report);
		}
		XdmfInt64 GetNumberOfErrors()
		{
			int numErrors = 0;
			for (std::map<std::string, std::vector<XdmfDiffReport> >::const_iterator iter = reports.begin(); iter!= reports.end(); iter++)
			{
				for (unsigned int i=0; i<iter->second.size(); i++)
				{
					std::vector<XdmfDiffReport> report = iter->second;
					numErrors += report[i].GetNumberOfErrors();
				}
			}
			return numErrors;
		}
		friend std::ostream &operator<<(std::ostream & toReturn, const XdmfDiffReportCollection &diffCollection)
		{
			for (std::map<std::string, std::vector<XdmfDiffReport> >::const_iterator iter = diffCollection.reports.begin(); iter!= diffCollection.reports.end(); iter++)
			{
				int numGridErrors = 0;
				for (unsigned int i=0; i<iter->second.size(); i++)
				{
					std::vector<XdmfDiffReport> report = iter->second;
					if (report[i].GetNumberOfErrors() > 0)
					{
						if (numGridErrors == 0 || diffCollection.verboseOutput)
						{
							toReturn << "|FAIL|  Grid Name: " << iter->first << "\n";
						}
						toReturn << "\t" << report[i];
						numGridErrors += report[i].GetNumberOfErrors();
					}
					else if (diffCollection.verboseOutput && !diffCollection.displayFailuresOnly)
					{
						toReturn << "|PASS|  Grid Name: " << iter->first;
						toReturn << "\t" << report[i];
					}
				}
				if (numGridErrors == 0 && !diffCollection.displayFailuresOnly && !diffCollection.verboseOutput)
				{
					toReturn << "|PASS|  Grid Name: " << iter->first << "\n";
				}
			}
			return toReturn;
		}
	private:
		std::map<std::string, std::vector<XdmfDiffReport> > reports;
		XdmfBoolean displayFailuresOnly;
		XdmfBoolean verboseOutput;
	};

	XdmfDiff(XdmfConstString refFileName, XdmfConstString newFileName);
	XdmfDiff(XdmfDOM * refDOM, XdmfDOM * newDOM);
	~XdmfDiff();

	std::string GetDiffsAsString();
	std::string GetDiffsAsString(XdmfConstString gridName);
	void ParseSettingsFile(XdmfConstString settingsFile);
	void SetRelativeError(XdmfFloat64 relativeError);
	void SetAbsoluteError(XdmfFloat64 absoluteError);
	XdmfSetValueMacro(IgnoreTime, XdmfBoolean);
	XdmfSetValueMacro(IgnoreGeometry, XdmfBoolean);
	XdmfSetValueMacro(IgnoreTopology, XdmfBoolean);
	XdmfSetValueMacro(IgnoreAllAttributes, XdmfBoolean);
	XdmfSetValueMacro(DisplayFailuresOnly, XdmfBoolean);
	XdmfSetValueMacro(VerboseOutput, XdmfBoolean);
	XdmfBoolean AreEquivalent();

private:
	void GetDiffs(XdmfDiffReportCollection & errorReports);
	void GetDiffs(XdmfGrid & grid, XdmfDiffReportCollection & errorReports);
	void GetDiffs(XdmfConstString gridName, XdmfDiffReportCollection & errorReports);
	XdmfDiffReport GetGeometryDiffs(XdmfGeometry * refGeometry, XdmfGeometry * newGeometry);
	XdmfDiffReport GetTopologyDiffs(XdmfTopology * refTopology, XdmfTopology * newTopology);
	XdmfDiffReport GetAttributeDiffs(XdmfAttribute * refAttribute, XdmfAttribute * newAttribute);
	void CompareValues(XdmfDiffReport & errorReport, XdmfArray * refArray, XdmfArray * newArray, XdmfInt64 startIndex, XdmfInt64 numValues, XdmfInt64 groupLength = 1);
	XdmfDOM * myRefDOM;
	XdmfDOM * myNewDOM;
	XdmfFloat64 myRelativeError;
	XdmfFloat64 myAbsoluteError;
	XdmfBoolean IgnoreTime;
	XdmfBoolean IgnoreGeometry;
	XdmfBoolean IgnoreTopology;
	XdmfBoolean IgnoreAllAttributes;
	XdmfBoolean DisplayFailuresOnly;
	XdmfBoolean VerboseOutput;
	XdmfBoolean refDOMIsMine;
	XdmfBoolean newDOMIsMine;
	std::set<std::string> includedGrids;
	std::set<std::string> ignoredGrids;
	std::set<std::string> includedAttributes;
	std::set<std::string> ignoredAttributes;
};

#endif /* XDMFDIFF_H_ */
