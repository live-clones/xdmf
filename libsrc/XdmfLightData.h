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

#include "XdmfDOM.h"

/*!
This is an abstract convenience object for reading and writing
LightData Files. LightData is stored in XML File or DOMs.
LightData "points" to HeavyData ; the giga-terabytes of HPC simulations.
This class points to a DOM, and a current node in that DOM which is presumeably
points to the "root" of the data object ( Grid, Geometry, Topology, etc.)
*/
class XdmfLightData : public XdmfObject {

public:
  XdmfLightData();
  ~XdmfLightData();

  const char * GetClassName() { return ( "XdmfLightData" ) ; };

//! Get the default NDGM Host
        XdmfGetValueMacro(NdgmHost, XdmfString);
//! Set the default NDGM Host 
        void SetNdgmHost( char *String ) { strcpy( this->NdgmHost, String ); }

//! Get the current name
  XdmfGetValueMacro(Name, XdmfString);
//! Set the current name
  void SetName( XdmfString File ) {
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

  XdmfInt32 HasBeenInited( void ) {
    if( this->DOM && this->CurrentElement ) {
      return( XDMF_SUCCESS );
      }
    return( XDMF_FAIL );
    };

protected:

  char    NdgmHost[XDMF_MAX_STRING_LENGTH];
  char    WorkingDirectory[XDMF_MAX_STRING_LENGTH];
  char    FileName[XDMF_MAX_STRING_LENGTH];
  char    Name[XDMF_MAX_STRING_LENGTH];
  XdmfDOM    *DOM;
  XdmfXNode    *CurrentElement;
};

#endif // __XdmfLightData_h
