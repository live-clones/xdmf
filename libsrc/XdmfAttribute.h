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
#ifndef __XdmfAttribute_h
#define __XdmfAttribute_h

#include "XdmfTopology.h"

// Value Types
#define XDMF_ATTRIBUTE_TYPE_NONE  0
#define XDMF_ATTRIBUTE_TYPE_SCALAR  1
#define XDMF_ATTRIBUTE_TYPE_VECTOR  2
#define XDMF_ATTRIBUTE_TYPE_TENSOR  3
#define XDMF_ATTRIBUTE_TYPE_MATRIX  4

// Where Values are Assigned
#define XDMF_ATTRIBUTE_CENTER_GRID  0
#define XDMF_ATTRIBUTE_CENTER_CELL  1
#define XDMF_ATTRIBUTE_CENTER_FACE  2
#define XDMF_ATTRIBUTE_CENTER_EDGE  3
#define XDMF_ATTRIBUTE_CENTER_NODE  4


class XdmfAttribute : public XdmfLightData {

public:
  XdmfAttribute();
  ~XdmfAttribute();

  const char * GetClassName() { return ( "XdmfAttribute" ) ; };

  XdmfSetValueMacro( AttributeType, XdmfInt32 );
  XdmfGetValueMacro( AttributeType, XdmfInt32 );

  XdmfInt32 SetAttributeTypeFromString( XdmfString AttributeType );
  XdmfString GetAttributeTypeAsString( void );

  XdmfInt32 SetAttributeCenterFromString( XdmfString AttributeCenter );
  XdmfString GetAttributeCenterAsString( void );

  XdmfSetValueMacro( AttributeCenter, XdmfInt32 );
  XdmfGetValueMacro( AttributeCenter, XdmfInt32 );

  XdmfDataDesc *GetShapeDesc( void ) { return( &this->ShapeDesc ); };

  XdmfSetValueMacro( Values, XdmfArray *);
  XdmfGetValueMacro( Values, XdmfArray *);

  XdmfInt32 InitAttributeFromElement( XdmfXNode *Element );
  XdmfInt32 SetAttributeFromElement( XdmfXNode *Element );

//  XdmfInt32 SetBaseAttribute( XdmfTopology *Topology, XdmfPointer Grid );

  XdmfInt32 Update();

protected:

  XdmfInt32  AttributeType;
  XdmfInt32  AttributeCenter;
  XdmfDataDesc  ShapeDesc;
  XdmfInt32  ValuesAreMine;
  XdmfArray  *Values;
};

#endif // __XdmfAttribute_h
