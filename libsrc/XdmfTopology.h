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
#ifndef __XdmfTopology_h
#define __XdmfTopology_h

#include "XdmfLightData.h"
#include "XdmfDataDesc.h"

#define XDMF_MAX_ORDER  10


// General Organization
#define XDMF_STRUCTURED    0
#define XDMF_UNSTRUCTURED  1

// Topologies
#define  XDMF_NOTOPOLOGY    0
#define XDMF_POLYVERTEX    1
#define XDMF_POLYLINE    2
#define XDMF_POLYGON    3
#define XDMF_TRI    4
#define XDMF_QUAD    5
#define XDMF_TET    6
#define XDMF_PYRAMID    7
#define XDMF_WEDGE    8
#define XDMF_HEX    9
#define  XDMF_2DSMESH    0x0100
#define XDMF_2DRECTMESH    0x0101
#define XDMF_2DCORECTMESH  0x0102
#define  XDMF_3DSMESH    0x1100
#define XDMF_3DRECTMESH    0x1101
#define XDMF_3DCORECTMESH  0x1102

/*!
Container class for base topologies
Currently supported base topologies are :
      POLYVERTEX
      POLYLINE
      POLYGON
      TRIANGLE
      QUADRILATERAL
      TETRAHEDRON
      PYRAMID
      WEDGE
      HEXAHEDRON
      2DSMESH .............. General ( Curved )
      2DRECTMESH ........... Rectilinear
      2DCORECTMESH.......... Co-Rectilinear
      3DSMESH  
      3DRECTMESH
      3DCORECTMESH
*/
class XdmfTopology : public XdmfLightData {

public:
  XdmfTopology();
  ~XdmfTopology();

  const char * GetClassName() { return ( "XdmfTopology" ) ; };

//! Get General Class
  XdmfInt32 GetClass ( void ) {
    if( ( this->TopologyType & 0x100 ) == 0  ) {
      return( XDMF_UNSTRUCTURED );
    }
    return( XDMF_STRUCTURED );
    };
//! Get the General TopologyType as a String
  XdmfString GetClassAsString( void ) {
    if( this->GetClass() == XDMF_STRUCTURED ){ 
      return( "XDMF_STRUCTURED" );
    }
    return( "XDMF_UNSTRUCTURED" );
    };
//! Set the base topology type
  XdmfInt32 SetTopologyType( XdmfInt32 TopologyType );
  XdmfInt32 SetTopologyTypeFromString( XdmfString TopologyType );
//! Get the base topology type
  XdmfGetValueMacro( TopologyType, XdmfInt32 );
  XdmfString GetTopologyTypeAsString( void );


  XdmfInt32  *GetOrder( void );
  XdmfString  GetOrderAsString( void );
  XdmfInt32  SetOrderFromString( XdmfString Order );
  XdmfInt32  SetOrder( XdmfInt32 Length, XdmfInt32 *Order );
  XdmfSetValueMacro( NodesPerElement, XdmfInt32 );
  XdmfGetValueMacro( NodesPerElement, XdmfInt32 );
  XdmfSetValueMacro( BaseOffset, XdmfInt32 );
  XdmfGetValueMacro( BaseOffset, XdmfInt32 );

  XdmfDataDesc *
  GetShapeDesc( void ) {
    return( &this->Shape );
    }
  XdmfInt64 GetNumberOfElements( void ) {
    return( this->Shape.GetNumberOfElements() );
    }
  XdmfInt32 InitTopologyFromElement( XdmfXNode *Element );
  XdmfInt32 SetTopologyFromElement( XdmfXNode *Element );
  XdmfArray *GetConnectivity( XdmfArray *Array = NULL );
  XdmfInt32 SetConnectivity( XdmfArray *Array );
  XdmfInt32 Update( void );
#ifndef DOXYGEN_SKIP 
  // XdmfInt32 SetBaseConnectivity( XdmfPointer Grid );
#endif
/*
  XdmfXNode *SetElementFromTopology( XdmfXNode *Element = NULL );
*/

protected:

  XdmfInt32  TopologyType;
  XdmfInt32  Order[ XDMF_MAX_ORDER ];
  XdmfInt32  NodesPerElement;
  XdmfArray  *Connectivity;
  XdmfInt32  OrderIsDefault;
  XdmfInt32  ConnectivityIsMine;
  XdmfInt32  BaseOffset;
  XdmfDataDesc  Shape;
};

extern XdmfTopology *HandleToXdmfTopology( char *Source );
#endif // __XdmfTopology_h
