/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Jerry A. Clarke                                             */
/*     clarke@arl.army.mil                                         */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2002 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef __XdmfLightData_h
#define __XdmfLightData_h

#include "XdmfObject.h"

class XdmfDOM;
class XdmfXNode;

//! Base object for Light Data (XML)
/*!
This is an abstract convenience object for reading and writing
LightData Files. LightData is stored in XML File or DOMs.
LightData "points" to HeavyData ; the giga-terabytes of HPC simulations.
This class points to a DOM, and a current node in that DOM which is presumeably
points to the "root" of the data object ( Grid, Geometry, Topology, etc.)

XdmfLightData is typically not used by itself. Rather one of the derived
classes like XdmfGrid or XdmfFormatMulti is used and these derived methods
are used from that class.
*/

class XDMF_EXPORT XdmfLightData : public XdmfObject {

public:
  XdmfLightData();
  ~XdmfLightData();

  XdmfConstString GetClassName() { return ( "XdmfLightData" ) ; };

//! Get the default NDGM Host
        XdmfGetValueMacro(NdgmHost, XdmfString);
//! Set the default NDGM Host 
        void SetNdgmHost( XdmfString String ) { strcpy( this->NdgmHost, String ); }

//! Get the current name
/*!
	In the XML, it is \b HIGHLY reccomended to give a Name to every
	XdmfGrid and XdmfAttribute. Example :
\verbatim
	<Grid Name="Concrete Block"
		.
		.
		.
		<Attribute Name="Pressure" ...
\endverbatim
	Visualization tools, in particular, look for this information to
	distinguish grids and scalars.
*/
  XdmfGetValueMacro(Name, XdmfString);
//! Set the current name
  void SetName( XdmfConstString File ) {
    strcpy( this->Name, File );
    } ;

//! Get the current filename
  XdmfGetValueMacro(FileName, XdmfString);
//! Set the current filename
  void SetFileName( XdmfString File ) {
    strcpy( this->FileName, File );
    } ;

//! Get the current WorkingDirectory
  XdmfGetValueMacro(WorkingDirectory, XdmfString);
//! Set the current WorkingDirectory
/*!
	This alleviates the need to hard code pathnames in the
	light data. i.e. the heavy and light data can be in 
	one directory and accessed from another.
*/
  void SetWorkingDirectory( XdmfString File ) {
    strcpy( this->WorkingDirectory, File );
    } ;

//! Set the current DOM used for finding the data
  XdmfSetValueMacro(DOM, XdmfDOM *);
//! Get the current DOM used for finding the data
  XdmfGetValueMacro(DOM, XdmfDOM *);

//! Set the "root" element in the DOM for the associated LightData type
  XdmfSetValueMacro(CurrentElement, XdmfXNode *);
//! Get the "root" element in the DOM for the associated LightData type
  XdmfGetValueMacro(CurrentElement, XdmfXNode *);

//! For internal use
  XdmfInt32 HasBeenInited( void ) {
    if( this->DOM && this->CurrentElement ) {
      return( XDMF_SUCCESS );
      }
    return( XDMF_FAIL );
    };

protected:

//! How to connect to NDGM for Heavy Data
  char    NdgmHost[XDMF_MAX_STRING_LENGTH];
//! Where to find data
  char    WorkingDirectory[XDMF_MAX_STRING_LENGTH];
//! XML filename
  char    FileName[XDMF_MAX_STRING_LENGTH];
//! Unique Identifier
  char    Name[XDMF_MAX_STRING_LENGTH];
  XdmfDOM    *DOM;
  XdmfXNode    *CurrentElement;
};

#endif // __XdmfLightData_h
