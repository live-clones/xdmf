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
#include "XdmfTopology.h"
#include "XdmfTransform.h"
#include "XdmfFormatMulti.h"

#include <strstream>

using namespace std;


XdmfTopology *HandleToXdmfTopology( char *Source ){
  XdmfObject  *TempObj;
  XdmfTopology   *Topology;

  TempObj = HandleToXdmfObject( Source );
  Topology = (XdmfTopology *)TempObj;
  return( Topology );
  }

XdmfTopology::XdmfTopology() {
  XdmfInt64 Dimensions = 1;
  this->TopologyType = XDMF_NOTOPOLOGY;
  this->NodesPerElement = 0;
  this->Shape.SetShape( 1, &Dimensions );
  this->Connectivity = NULL;
  this->ConnectivityIsMine = 1;
  this->OrderIsDefault = 1;
  this->BaseOffset = 0;
  }

XdmfTopology::~XdmfTopology() {
  if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
  }

XdmfInt32
XdmfTopology::SetConnectivity( XdmfArray *Array ){
if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
this->ConnectivityIsMine = 0;
this->Connectivity = Array;
return( XDMF_SUCCESS );
}


XdmfInt32
XdmfTopology::SetTopologyTypeFromString( XdmfString TopologyType ) {

XdmfInt32  newTopologyType = XDMF_NOTOPOLOGY;


if ( XDMF_WORD_CMP( TopologyType, "NOTOPOLOGY") ){
  newTopologyType = XDMF_NOTOPOLOGY;
} else if( XDMF_WORD_CMP( TopologyType, "POLYVERTEX") ){
  newTopologyType = XDMF_POLYVERTEX;
} else if( XDMF_WORD_CMP( TopologyType, "POLYLINE") ){
  newTopologyType = XDMF_POLYLINE;
} else if( XDMF_WORD_CMP( TopologyType, "POLYGON") ){
  newTopologyType = XDMF_POLYGON;
} else if( XDMF_WORD_CMP( TopologyType, "TRIANGLE") ){
  newTopologyType = XDMF_TRI;
} else if( XDMF_WORD_CMP( TopologyType, "QUADRILATERAL") ){
  newTopologyType = XDMF_QUAD;
} else if( XDMF_WORD_CMP( TopologyType, "TETRAHEDRON") ){
  newTopologyType = XDMF_TET;
} else if( XDMF_WORD_CMP( TopologyType, "PYRAMID") ){
  newTopologyType = XDMF_PYRAMID;
} else if( XDMF_WORD_CMP( TopologyType, "WEDGE") ){
  newTopologyType = XDMF_WEDGE;
} else if( XDMF_WORD_CMP( TopologyType, "HEXAHEDRON") ){
  newTopologyType = XDMF_HEX;
} else if( XDMF_WORD_CMP( TopologyType, "2DSMESH") ){
  newTopologyType = XDMF_2DSMESH;
} else if( XDMF_WORD_CMP( TopologyType, "2DRECTMESH") ){
  newTopologyType = XDMF_2DRECTMESH;
} else if( XDMF_WORD_CMP( TopologyType, "2DCORECTMESH") ){
  newTopologyType = XDMF_2DCORECTMESH;
} else if( XDMF_WORD_CMP( TopologyType, "3DSMESH") ){
  newTopologyType = XDMF_3DSMESH;
} else if( XDMF_WORD_CMP( TopologyType, "3DRECTMESH") ){
  newTopologyType = XDMF_3DRECTMESH;
} else if( XDMF_WORD_CMP( TopologyType, "3DCORECTMESH") ){
  newTopologyType = XDMF_3DCORECTMESH;
}

if( newTopologyType != XDMF_NOTOPOLOGY ){
  this->SetTopologyType( newTopologyType );
  return( XDMF_SUCCESS );
  }

return( XDMF_FAIL );
}

XdmfString
XdmfTopology::GetTopologyTypeAsString( void ) {


  switch ( this->TopologyType ){
    case  XDMF_POLYVERTEX :
      return("Polyvertex");
    case  XDMF_POLYLINE :
      return("Polyline");
    case  XDMF_POLYGON :
      return("Polygon");
    case  XDMF_TRI :
      return("Triangle");
    case  XDMF_QUAD :
      return("Quadralateral");
    case  XDMF_TET :
      return("Tetrahedron");
    case  XDMF_PYRAMID :
      return("Pyramid");
    case  XDMF_WEDGE :
      return("Wedge");
    case  XDMF_HEX :
      return("Hexahedron");
    case  XDMF_2DSMESH :
      return("2DSMesh");
    case  XDMF_2DRECTMESH :
      return("2DRectMesh");
    case  XDMF_2DCORECTMESH :
      return("2DCORECTMesh");
    case  XDMF_3DSMESH :
      return("3DSMesh");
    case  XDMF_3DRECTMESH :
      return("3DRECTMesh");
    case  XDMF_3DCORECTMESH :
      return("3DCORECTMesh");
    default :
      break;
  }
return( "NOTOPOLOGY");
}

XdmfInt32
XdmfTopology::SetTopologyType( XdmfInt32 TopologyType ) {

XdmfInt32  NodesPerElement = 0;

  switch ( TopologyType ){
    case  XDMF_NOTOPOLOGY :
      break;
    case  XDMF_POLYVERTEX :
      NodesPerElement = 1;
      break;
    case  XDMF_POLYLINE :
      NodesPerElement = 1;
      break;
    case  XDMF_POLYGON :
      NodesPerElement = 1;
      break;
    case  XDMF_TRI :
      NodesPerElement = 3;
      break;
    case  XDMF_QUAD :
      NodesPerElement = 4;
      break;
    case  XDMF_TET :
      NodesPerElement = 4;
      break;
    case  XDMF_PYRAMID :
      NodesPerElement = 5;
      break;
    case  XDMF_WEDGE :
      NodesPerElement = 6;
      break;
    case  XDMF_HEX :
      NodesPerElement = 8;
      break;
    case  XDMF_2DSMESH :
      NodesPerElement = 1;
      break;
    case  XDMF_2DRECTMESH :
      NodesPerElement = 1;
      break;
    case  XDMF_2DCORECTMESH :
      NodesPerElement = 1;
      break;
    case  XDMF_3DSMESH :
      NodesPerElement = 1;
      break;
    case  XDMF_3DRECTMESH :
      NodesPerElement = 1;
      break;
    case  XDMF_3DCORECTMESH :
      NodesPerElement = 1;
      break;
    default :
      return( XDMF_FAIL );
  }

this->TopologyType = TopologyType;
this->NodesPerElement = NodesPerElement;
return ( XDMF_SUCCESS );
}

XdmfInt32 *
XdmfTopology::GetOrder( void ){
  return( this->Order );
}

XdmfString
XdmfTopology::GetOrderAsString( void ) {
static char    Value[ 80 ];
ostrstream  OutStream( Value, 80 );
XdmfInt32  i;

for( i = 0 ; i < this->NodesPerElement ; i++ ){
  OutStream << this->Order[i] << " ";
  }
OutStream << ends;
return( Value );
}

XdmfInt32
XdmfTopology::SetOrder( XdmfInt32 Length, XdmfInt32 *Order ) {
XdmfInt32  i;

if( Length > XDMF_MAX_ORDER ) {
  return( XDMF_FAIL );
  }
this->OrderIsDefault = 1;
for( i = 0 ; i < Length ; i++ ){
  XdmfDebug("Set Order[" << i << "] = " << Order[i] );
  this->Order[i] = Order[i];
  if ( Order[i] != i ){
    this->OrderIsDefault = 0;
    }
  }
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfTopology::SetOrderFromString( XdmfString Order ){
XdmfInt32  i = 0, List[ XDMF_MAX_ORDER];
istrstream  InStream( Order, strlen(Order) );

while( InStream >> List[i] ) i++;
this->SetOrder( i, List );
return( XDMF_SUCCESS );
}


XdmfArray *
XdmfTopology::GetConnectivity( XdmfArray *Array ){
if( this->Connectivity ) {
  if( Array ){
    CopyArray( this->Connectivity, Array );
    return( Array );
  } else {
    return( this->Connectivity );
  }
}
return( NULL );
}

XdmfInt32
XdmfTopology::InitTopologyFromElement( XdmfXNode *Element ) {

XdmfString  Attribute;

if( !Element ) {
  XdmfErrorMessage("NULL Element");
  return( XDMF_FAIL );
  }
Attribute = this->DOM->Get( Element, "NodeType");
if( XDMF_WORD_CMP( Attribute, "Topology" ) == 0 ){
  Element = this->DOM->FindElement( "Topology", 0, Element );
  if( !Element ){
    XdmfErrorMessage("Can't Find Topology Node");
    return( XDMF_FAIL );
    }
  }
Attribute = this->DOM->Get( Element, "Type" );
if( this->SetTopologyTypeFromString( Attribute ) == XDMF_FAIL ){
  XdmfErrorMessage("Bad Topology Type : " << Attribute );
  return( XDMF_FAIL );
  }
// Set Shape Either Way
Attribute = this->DOM->Get( Element, "NumberOfElements" );
if( Attribute ){
  this->GetShapeDesc()->SetShapeFromString( Attribute );
  }
Attribute = this->DOM->Get( Element, "Dimensions" );
if( Attribute ){
  this->GetShapeDesc()->SetShapeFromString( Attribute );
}
Attribute = this->DOM->Get( Element, "NodesPerElement" );
if( Attribute ){
  XdmfInt64 NodesPerElement;

  NodesPerElement = strtol( Attribute, (char **)NULL, 0 );
  this->SetNodesPerElement( NodesPerElement );
  }
Attribute = this->DOM->Get( Element, "Order" );
if( Attribute ){
  this->SetOrderFromString( Attribute );
  }
Attribute = this->DOM->Get( Element, "BaseOffset" );
if( Attribute ){
  this->BaseOffset = strtol( Attribute, (char **)NULL, 0);
  }
Attribute = this->DOM->Get( Element, "Name" );
if( Attribute ) {
  this->SetName( Attribute );
} else {
  this->SetName( GetUnique("Topology_" ) );
}
this->CurrentElement = Element;
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfTopology::SetTopologyFromElement( XdmfXNode *Element ) {

XdmfString  Attribute;
XdmfXNode    *ConnectionElement;

if( !Element ) {
  XdmfErrorMessage("NULL Element");
  return( XDMF_FAIL );
  }
if( this->TopologyType == XDMF_NOTOPOLOGY ){
  if( this->InitTopologyFromElement( Element ) == XDMF_FAIL ) {
    XdmfErrorMessage("Can't Init from Element");
    return( XDMF_FAIL );
    }  
  }
Attribute = this->DOM->Get( Element, "NodeType");
if( XDMF_WORD_CMP( Attribute, "Topology" ) == 0 ){
  Element = this->DOM->FindElement( "Topology", 0, Element );
  if( !Element ){
    XdmfErrorMessage("Can't Find Topology Node");
    return( XDMF_FAIL );
    }
  }
if( this->GetClass() == XDMF_UNSTRUCTURED ){
  // Read Connection
  // ConnectionElement = this->DOM->FindElement("DataTransform", 0, Element );
  ConnectionElement = this->DOM->FindElement( NULL, 0, Element );
  if( ConnectionElement ){
    XdmfTransform  ConnectionReader;
  
    XdmfDebug("Reading Connections from Transform");
    ConnectionReader.SetDOM( this->DOM );
    if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
    this->Connectivity = ConnectionReader.ElementToArray( ConnectionElement );
    if( this->BaseOffset ) {
      XdmfDebug("Adjusting due to BaseOffset");
      *this->Connectivity -= this->BaseOffset;
      }
  } else {
    // Default Connectivity
    XdmfInt64  Length;
    if( !this->Connectivity ){
      this->Connectivity = new XdmfArray;
      this->Connectivity->SetNumberType( XDMF_INT64_TYPE );
      this->ConnectivityIsMine = 1;
    }
    Length = this->GetShapeDesc()->GetNumberOfElements() * this->NodesPerElement;
    this->Connectivity->SetNumberOfElements( Length );
    this->Connectivity->Generate( 0 , Length - 1 );
  }
  if( this->OrderIsDefault == 0 ){
    XdmfInt64  i,j;
    XdmfInt64  Length;
    XdmfInt64  *NewConnsPtr, *NewConns, *ElementConns;

    XdmfDebug("Re-Arranging Connections Due to Order");
    Length = this->Connectivity->GetNumberOfElements();
    NewConnsPtr = NewConns = new XdmfInt64[ Length ];
    Connectivity->GetValues( 0, NewConns, Length );
    ElementConns = new XdmfInt64[ this->NodesPerElement ];
    Length = this->Connectivity->GetNumberOfElements() / this->NodesPerElement;
    for( i = 0 ; i < Length ; i++ ){
      for( j = 0 ; j < this->NodesPerElement ; j++ ){
        ElementConns[j] = NewConnsPtr[ this->Order[j] ];
//        XdmfDebug("Conn " << j << " = " << ElementConns[j]);
      }
      memcpy( NewConnsPtr, ElementConns, this->NodesPerElement * sizeof( XdmfInt64 ) );
      NewConnsPtr += this->NodesPerElement;
    }
    Length = this->Connectivity->GetNumberOfElements();
    Connectivity->SetValues( 0, NewConns, Length );
    delete [] NewConns;
  }
}
this->CurrentElement = Element;
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfTopology::Update( void ){

if( this->DOM && this->CurrentElement ){
  return( this->SetTopologyFromElement( this->CurrentElement ) );
}
XdmfErrorMessage("No Current Element set for DOM");
return( XDMF_FAIL );
}
