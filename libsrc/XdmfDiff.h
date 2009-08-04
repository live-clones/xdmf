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

#ifndef XDMFDIFF_H_
#define XDMFDIFF_H_

#include <XdmfArray.h>
#include <XdmfGrid.h>
#include <XdmfDOM.h>

#include <set>

class XdmfDiff{
public:
	XdmfDiff(XdmfConstString refFileName, XdmfConstString newFileName);
	XdmfDiff(XdmfDOM * refDOM, XdmfDOM * newDOM);
	~XdmfDiff();
	std::string GetDiffs();
	std::string GetDiffs(XdmfConstString gridName);
	void ParseSettingsFile(XdmfConstString settingsFile);
	void SetRelativeError(XdmfFloat64 relativeError);
	void SetAbsoluteError(XdmfFloat64 absoluteError);
	void SetIgnoreTopology(XdmfBoolean value = true);
	void SetIgnoreGeometry(XdmfBoolean value = true);
	void SetIgnoreAllAttributes(XdmfBoolean value = true);
	void DisplayFailuresOnly(XdmfBoolean value = true);
	XdmfBoolean AreEquivalent();
private:
	XdmfGrid * GetGrid(XdmfConstString gridName, XdmfDOM * DOM);
	std::string GetPointDiffs(XdmfGrid * refGrid, XdmfGrid * newGrid);
	std::string GetConnectionDiffs(XdmfGrid * refGrid, XdmfGrid * newGrid);
	std::string GetAttributeDiffs(XdmfGrid * refGrid, XdmfGrid * newGrid, XdmfConstString attributeName);
	std::string CompareValues(XdmfArray * refArray, XdmfArray * newArray, XdmfInt64 startIndex, XdmfInt64 numValues, XdmfInt64 groupLength = 1);
	XdmfDOM * myRefDOM;
	XdmfDOM * myNewDOM;
	XdmfFloat64 myRelativeError;
	XdmfFloat64 myAbsoluteError;
	XdmfBoolean relErrorOn;
	XdmfBoolean absErrorOn;
	XdmfBoolean ignoreGeometry;
	XdmfBoolean ignoreTopology;
	XdmfBoolean ignoreAllAttributes;
	XdmfBoolean displayFailuresOnly;
	XdmfBoolean areEquivalent;
	XdmfBoolean diffRun;
	XdmfBoolean refDOMIsMine;
	XdmfBoolean newDOMIsMine;
	std::set<std::string> includedGrids;
	std::set<std::string> ignoredGrids;
	std::set<std::string> includedAttributes;
	std::set<std::string> ignoredAttributes;
};

#endif /* XDMFDIFF_H_ */
