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
#ifndef __XdmfGrid_h
#define __XdmfGrid_h

#include "XdmfTopology.h"

class XdmfGeometry;
class XdmfAttribute;

class XDMF_EXPORT XdmfGrid : public XdmfTopology {

public:
  XdmfGrid();
  ~XdmfGrid();

  XdmfConstString GetClassName() { return ( "XdmfGrid" ) ; };

  XdmfSetValueMacro( Geometry, XdmfGeometry *);
  XdmfGetValueMacro( Geometry, XdmfGeometry *);
  XdmfGetValueMacro( NumberOfAttributes, XdmfInt32 );
  XdmfGetIndexValueMacro( Attribute, XdmfAttribute * );

  XdmfGetValueMacro( BaseGrid, XdmfPointer);

  // XdmfPointer GetBaseGridCopy( void );

  XdmfInt32 AssignAttribute( XdmfInt64 Index );
#ifndef SWIG
  XdmfInt32 AssignAttribute( XdmfAttribute *Attribute );
#endif
  XdmfInt32 AssignAttributeByIndex( XdmfInt64 Index );
  XdmfInt32 AssignAttributeByName( XdmfString Name );

  XdmfAttribute *GetAssignedAttribute( void ) { return( this->AssignedAttribute ); };
  XdmfInt64 GetAssignedAttributeIndex( void );

  //! Light Version
  XdmfInt32  InitGridFromElement( XdmfXNode *Element );
  XdmfInt32  SetGridFromElement( XdmfXNode *Element );
  // XdmfInt32  SetBaseGrid( XdmfPointer Grid = NULL );

//! Overloads Topology->Update();
  XdmfInt32  Update( void );

protected:

  XdmfPointer  BaseGrid;
  XdmfGeometry  *Geometry;
  XdmfInt32  GeometryIsMine;
  XdmfInt32  BaseGridIsMine;
  XdmfInt32  NumberOfAttributes;
  XdmfAttribute  **Attribute;
  XdmfAttribute  *AssignedAttribute;
};

extern XDMF_EXPORT XdmfGrid *HandleToXdmfGrid( XdmfString Source);
#endif // __XdmfGrid_h
