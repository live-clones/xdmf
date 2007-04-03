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
/*     Copyright @ 2007 US Army Research Laboratory                */
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

#include "XdmfDataItem.h"
#include "XdmfArray.h"
#include "XdmfDOM.h"
#include "XdmfHDF.h" // For CopyArray()

XdmfTopology *HandleToXdmfTopology( XdmfString Source ){
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
  this->Shape = new XdmfDataDesc;
  this->Shape->SetShape( 1, &Dimensions );
  this->Connectivity = NULL;
  this->ConnectivityIsMine = 1;
  this->OrderIsDefault = 1;
  this->BaseOffset = 0;
  }

XdmfTopology::~XdmfTopology() {
  if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
  delete this->Shape;
  }

XdmfInt64 XdmfTopology::GetNumberOfElements( void ) {
  return( this->Shape->GetNumberOfElements() );
  }

XdmfInt32
XdmfTopology::SetConnectivity( XdmfArray *Array ){
if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
this->ConnectivityIsMine = 0;
this->Connectivity = Array;
return( XDMF_SUCCESS );
}


XdmfInt32
XdmfTopology::SetTopologyTypeFromString( XdmfConstString topologyType ) {

XdmfInt32  newTopologyType = XDMF_NOTOPOLOGY;


if ( XDMF_WORD_CMP( topologyType, "NOTOPOLOGY") ){
  newTopologyType = XDMF_NOTOPOLOGY;
} else if( XDMF_WORD_CMP( topologyType, "POLYVERTEX") ){
  newTopologyType = XDMF_POLYVERTEX;
} else if( XDMF_WORD_CMP( topologyType, "POLYLINE") ){
  newTopologyType = XDMF_POLYLINE;
} else if( XDMF_WORD_CMP( topologyType, "POLYGON") ){
  newTopologyType = XDMF_POLYGON;
} else if( XDMF_WORD_CMP( topologyType, "TRIANGLE") ){
  newTopologyType = XDMF_TRI;
} else if( XDMF_WORD_CMP( topologyType, "QUADRILATERAL") ){
  newTopologyType = XDMF_QUAD;
} else if( XDMF_WORD_CMP( topologyType, "TETRAHEDRON") ){
  newTopologyType = XDMF_TET;
} else if( XDMF_WORD_CMP( topologyType, "PYRAMID") ){
  newTopologyType = XDMF_PYRAMID;
} else if( XDMF_WORD_CMP( topologyType, "WEDGE") ){
  newTopologyType = XDMF_WEDGE;
} else if( XDMF_WORD_CMP( topologyType, "HEXAHEDRON") ){
  newTopologyType = XDMF_HEX;
} else if( XDMF_WORD_CMP( topologyType, "MIXED") ){
  newTopologyType = XDMF_MIXED;
} else if( XDMF_WORD_CMP( topologyType, "2DSMESH") ){
  newTopologyType = XDMF_2DSMESH;
} else if( XDMF_WORD_CMP( topologyType, "2DRECTMESH") ){
  newTopologyType = XDMF_2DRECTMESH;
} else if( XDMF_WORD_CMP( topologyType, "2DCORECTMESH") ){
  newTopologyType = XDMF_2DCORECTMESH;
} else if( XDMF_WORD_CMP( topologyType, "3DSMESH") ){
  newTopologyType = XDMF_3DSMESH;
} else if( XDMF_WORD_CMP( topologyType, "3DRECTMESH") ){
  newTopologyType = XDMF_3DRECTMESH;
} else if( XDMF_WORD_CMP( topologyType, "3DCORECTMESH") ){
  newTopologyType = XDMF_3DCORECTMESH;
}

if( newTopologyType != XDMF_NOTOPOLOGY ){
  this->SetTopologyType( newTopologyType );
  return( XDMF_SUCCESS );
  }

return( XDMF_FAIL );
}

XdmfConstString
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
      return("Quadrilateral");
    case  XDMF_TET :
      return("Tetrahedron");
    case  XDMF_PYRAMID :
      return("Pyramid");
    case  XDMF_WEDGE :
      return("Wedge");
    case  XDMF_HEX :
      return("Hexahedron");
    case XDMF_MIXED :
      return("Mixed");
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
XdmfTopology::SetTopologyType( XdmfInt32 topologyType ) {

XdmfInt32  nodesPerElement = 0;

  switch ( topologyType ){
    case  XDMF_NOTOPOLOGY :
      break;
    case  XDMF_POLYVERTEX :
      nodesPerElement = 1;
      break;
    case  XDMF_POLYLINE :
      nodesPerElement = 1;
      break;
    case  XDMF_POLYGON :
      nodesPerElement = 1;
      break;
    case  XDMF_TRI :
      nodesPerElement = 3;
      break;
    case  XDMF_QUAD :
      nodesPerElement = 4;
      break;
    case  XDMF_TET :
      nodesPerElement = 4;
      break;
    case  XDMF_PYRAMID :
      nodesPerElement = 5;
      break;
    case  XDMF_WEDGE :
      nodesPerElement = 6;
      break;
    case  XDMF_HEX :
      nodesPerElement = 8;
      break;
    case  XDMF_MIXED :
      nodesPerElement = 0;
      break;
    case  XDMF_2DSMESH :
      nodesPerElement = 1;
      break;
    case  XDMF_2DRECTMESH :
      nodesPerElement = 1;
      break;
    case  XDMF_2DCORECTMESH :
      nodesPerElement = 1;
      break;
    case  XDMF_3DSMESH :
      nodesPerElement = 1;
      break;
    case  XDMF_3DRECTMESH :
      nodesPerElement = 1;
      break;
    case  XDMF_3DCORECTMESH :
      nodesPerElement = 1;
      break;
    default :
      return( XDMF_FAIL );
  }

this->TopologyType = topologyType;
this->NodesPerElement = nodesPerElement;
return ( XDMF_SUCCESS );
}

XdmfInt32 *
XdmfTopology::GetOrder( void ){
  return( this->Order );
}

XdmfConstString
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
XdmfTopology::SetOrder( XdmfInt32 Length, XdmfInt32 *order ) {
XdmfInt32  i;

if( Length > XDMF_MAX_ORDER ) {
  return( XDMF_FAIL );
  }
this->OrderIsDefault = 1;
for( i = 0 ; i < Length ; i++ ){
  XdmfDebug("Set Order[" << i << "] = " << order[i] );
  this->Order[i] = order[i];
  if ( order[i] != i ){
    this->OrderIsDefault = 0;
    }
  }
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfTopology::SetOrderFromString( XdmfConstString order ){
XdmfInt32  i = 0, List[ XDMF_MAX_ORDER];
char *NewOrder = new char[ strlen(order) + 1];
strcpy(NewOrder, order);
istrstream  InStream( NewOrder, strlen(NewOrder) );

while( InStream >> List[i] ) i++;
delete [] NewOrder;
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
XdmfTopology::UpdateInformation() {
XdmfConstString Attribute;

if(XdmfElement::UpdateInformation() != XDMF_SUCCESS) return(XDMF_FAIL);
if( XDMF_WORD_CMP(this->GetElementType(), "Topology") == 0){
    XdmfErrorMessage("Element type" << this->GetElementType() << " is not of type 'Topology'");
    return(XDMF_FAIL);
}
Attribute = this->Get( "Type" );
if( this->SetTopologyTypeFromString( Attribute ) == XDMF_FAIL ){
  XdmfErrorMessage("Bad Topology Type : " << Attribute );
  return( XDMF_FAIL );
  }
// Set Shape Either Way
Attribute = this->Get( "NumberOfElements" );
if( Attribute ){
  this->GetShapeDesc()->SetShapeFromString( Attribute );
  }
Attribute = this->Get( "Dimensions" );
if( Attribute ){
  this->GetShapeDesc()->SetShapeFromString( Attribute );
}
Attribute = this->Get( "NodesPerElement" );
if( Attribute ){
  XdmfInt64 nodesPerElement;

  nodesPerElement = strtol( Attribute, (XdmfString *)NULL, 0 );
  this->SetNodesPerElement( nodesPerElement );
  }
Attribute = this->Get( "Order" );
if( Attribute ){
  this->SetOrderFromString( Attribute );
  }
Attribute = this->Get( "BaseOffset" );
if( Attribute ){
  this->BaseOffset = strtol( Attribute, (XdmfString *)NULL, 0);
  }
if(!this->Name) this->SetName(GetUnique("Topology_"));
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfTopology::Update() {
XdmfConstString Attribute;
XdmfXmlNode     ConnectionElement;

if(XdmfElement::Update() != XDMF_SUCCESS) return(XDMF_FAIL);
if( this->TopologyType == XDMF_NOTOPOLOGY ){
  if( this->UpdateInformation() == XDMF_FAIL ) {
    XdmfErrorMessage("Can't Initialize from Element");
    return( XDMF_FAIL );
    }  
  }
if( this->GetClass() == XDMF_UNSTRUCTURED ){
  // Read Connection
  ConnectionElement = this->DOM->FindElement( NULL, 0, this->Element );
  if( ConnectionElement ){
      XdmfDataItem  Connections;
  
    XdmfDebug("Reading Connections from DataItem");
    if(Connections.SetDOM(this->DOM) == XDMF_FAIL) return(XDMF_FAIL);
    if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
    if(Connections.SetElement(ConnectionElement) == XDMF_FAIL) return(XDMF_FAIL);
    if(Connections.UpdateInformation() == XDMF_FAIL) return(XDMF_FAIL);
    if(Connections.Update() == XDMF_FAIL) return(XDMF_FAIL);
    // Steal the Array so it doesn't get deleted in the destructor of the DataItem
    this->Connectivity = Connections.GetArray();
    Connections.SetArrayIsMine(0);

    if( this->BaseOffset ) {
      XdmfDebug("Adjusting due to BaseOffset");
      if(this->TopologyType == XDMF_MIXED){
        XdmfDebug("Cannot Adjust BaseOffset of Mixed Topology Yet");
      } else {
        *this->Connectivity -= this->BaseOffset;
        }
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
return( XDMF_SUCCESS );
}
