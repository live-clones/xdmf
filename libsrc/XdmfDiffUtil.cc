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
