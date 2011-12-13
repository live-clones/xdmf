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

#include <vector>

namespace {

  //
  // local methods
  //
  void
  InsertQuadInHash(XdmfInt64 a, XdmfInt64 b, XdmfInt64 c, XdmfInt64 d,
		   std::vector<std::vector<std::vector<XdmfInt64> > > & hash)
  {
    XdmfInt64 tmp;
    
    // Reorder to get smallest id in a.
    if (b < a && b < c && b < d) {
      tmp = a;
      a = b;
      b = c;
      c = d;
      d = tmp;
    }
    else if (c < a && c < b && c < d) {
      tmp = a;
      a = c;
      c = tmp;
      tmp = b;
      b = d;
      d = tmp;
    }
    else if (d < a && d < b && d < c) {
      tmp = a;
      a = d;
      d = c;
      c = b;
      b = tmp;
    }

    // Look for existing cell in the hash;
    std::vector<std::vector<XdmfInt64> > & currHash = hash[a];
    for(std::vector<std::vector<XdmfInt64> >::iterator iter = 
	  currHash.begin(); iter != currHash.end(); ++iter) {
      std::vector<XdmfInt64> & currFace = *iter;
      // 3 because b + c + d
      if(currFace.size() == 3) {
	if ((b == currFace[0] && d == currFace[2]) || 
	    (b == currFace[2] && d == currFace[0])) {
	  currHash.erase(iter);
	  return;
	}
      }
    }
    
    std::vector<XdmfInt64> newFace(3);
    newFace[0] = b;
    newFace[1] = c;
    newFace[2] = d;
    currHash.push_back(newFace);
  }

  void 
  InsertQuad8InHash(XdmfInt64 a, XdmfInt64 b, XdmfInt64 c, XdmfInt64 d,
		    XdmfInt64 e, XdmfInt64 f, XdmfInt64 g, XdmfInt64 h,
		    std::vector<std::vector<std::vector<XdmfInt64> > > & hash)
  {
    XdmfInt64 tmp;
    
    // Reorder to get smallest id in a.
    if (b < a && b < c && b < d) {
      tmp = a;
      a = b;
      b = c;
      c = d;
      d = tmp;
      tmp = e;
      e = f;
      f = g;
      g = h;
      h = tmp;
    }
    else if (c < a && c < b && c < d) {
      tmp = a;
      a = c;
      c = tmp;
      tmp = b;
      b = d;
      d = tmp;
      tmp = e;
      e = g;
      g = tmp;
      tmp = f;
      f = h;
      h = tmp;
    }
    else if (d < a && d < b && d < c) {
      tmp = a;
      a = d;
      d = c;
      c = b;
      b = tmp;
      tmp = e;
      e = h;
      h = g;
      g = f;
      f = tmp;
    }

    // Look for existing cell in the hash;
    std::vector<std::vector<XdmfInt64> > & currHash = hash[a];
    for(std::vector<std::vector<XdmfInt64> >::iterator iter = 
	  currHash.begin(); iter != currHash.end(); ++iter) {
      std::vector<XdmfInt64> & currFace = *iter;
      // 7 because b + c + d + e + f + g + h
      if(currFace.size() == 7) {
	if ((b == currFace[0] && d == currFace[2]) || 
	    (b == currFace[2] && d == currFace[0])) {
	  currHash.erase(iter);
	  return;
	}
      }
    }
    
    std::vector<XdmfInt64> newFace(7);
    newFace[0] = b;
    newFace[1] = c;
    newFace[2] = d;
    newFace[3] = e;
    newFace[4] = f;
    newFace[5] = g;
    newFace[6] = h;
    currHash.push_back(newFace);
  }

  void
  InsertTriInHash(XdmfInt64 a, XdmfInt64 b, XdmfInt64 c, 
		  std::vector<std::vector<std::vector<XdmfInt64> > > & hash)
  {
    XdmfInt64 tmp;

    // Reorder to get smallest id in a.
    if (b < a && b < c) {
      tmp = a;
      a = b;
      b = c;
      c = tmp;
    }
    else if (c < a && c < b) {
      tmp = a;
      a = c;
      c = b;
      b = tmp;
    }

    // Look for existing cell in the hash;
    std::vector<std::vector<XdmfInt64> > & currHash = hash[a];
    for(std::vector<std::vector<XdmfInt64> >::iterator iter = 
	  currHash.begin(); iter != currHash.end(); ++iter) {
      std::vector<XdmfInt64> & currFace = *iter;
      // 2 because b + c
      if(currFace.size() == 2) {
	if ((b == currFace[0] && c == currFace[1]) || 
	    (b == currFace[1] && c == currFace[0])) {
	  currHash.erase(iter);
	  return;
	}
      }
    }
    
    std::vector<XdmfInt64> newFace(2);
    newFace[0] = b;
    newFace[1] = c;
    currHash.push_back(newFace);
  }

  void
  InsertTri6InHash(XdmfInt64 a, XdmfInt64 b, XdmfInt64 c, 
		   XdmfInt64 d, XdmfInt64 e, XdmfInt64 f,
		   std::vector<std::vector<std::vector<XdmfInt64> > > & hash)
  {
    XdmfInt64 tmp;

    // Reorder to get smallest id in a.
    if (b < a && b < c) {
      tmp = a;
      a = b;
      b = c;
      c = tmp;
      tmp = d;
      d = e;
      e = f;
      f = tmp;
    }
    else if (c < a && c < b) {
      tmp = a;
      a = c;
      c = b;
      b = tmp;
      tmp = d;
      d = f;
      f = e;
      e = tmp;
    }

    // Look for existing cell in the hash;
    std::vector<std::vector<XdmfInt64> > & currHash = hash[a];
    for(std::vector<std::vector<XdmfInt64> >::iterator iter = 
	  currHash.begin(); iter != currHash.end(); ++iter) {
      std::vector<XdmfInt64> & currFace = *iter;
      // 5 because b + c + d + e + f
      if(currFace.size() == 5) {
	if ((b == currFace[0] && c == currFace[1]) || 
	    (b == currFace[1] && c == currFace[0])) {
	  currHash.erase(iter);
	  return;
	}
      }
    }
    
    std::vector<XdmfInt64> newFace(5);
    newFace[0] = b;
    newFace[1] = c;
    newFace[2] = d;
    newFace[3] = e;
    newFace[4] = f;
    currHash.push_back(newFace);
  }
}

XdmfTopology *HandleToXdmfTopology( XdmfString Source ){
  XdmfObject  *TempObj;
  XdmfTopology   *Topology;

  TempObj = HandleToXdmfObject( Source );
  Topology = (XdmfTopology *)TempObj;
  return( Topology );
  }

XdmfTopology::XdmfTopology() {
  XdmfInt64 Dimensions = 1;
  this->SetElementName("Topology");
  this->TopologyType = XDMF_NOTOPOLOGY;
  this->NodesPerElement = 0;
  this->Shape = new XdmfDataDesc;
  this->Shape->SetShape( 1, &Dimensions );
  this->Connectivity = NULL;
  this->CellOffsets = NULL;
  this->ConnectivityIsMine = 1;
  this->OrderIsDefault = 1;
  this->BaseOffset = 0;
  this->LightDataLimit = 100;
  }

XdmfTopology::~XdmfTopology() {
  if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
  if(this->CellOffsets) delete this->CellOffsets;
  delete this->Shape;
  }

XdmfInt32
XdmfTopology::Release(){
    if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
    if(this->CellOffsets) delete this->CellOffsets;
    this->Connectivity = NULL;
    this->CellOffsets = NULL;
    return(XDMF_SUCCESS);
}

XdmfDataItem * XdmfTopology::GetDataItem(){
    XdmfDataItem *di = NULL;
    XdmfXmlNode Node = this->DOM->FindDataElement(0, this->GetElement());

    if(Node) {
        di = (XdmfDataItem *)this->GetCurrentXdmfElement(Node);
    }
    if(!di){
        di = new XdmfDataItem;
        Node = this->DOM->InsertNew(this->GetElement(), "DataItem");
        di->SetDOM(this->DOM);
        di->SetElement(Node);
    }
    return(di);
} 

XdmfInt32
XdmfTopology::Build(){
    if(XdmfElement::Build() != XDMF_SUCCESS) return(XDMF_FAIL);
    this->Set("TopologyType", this->GetTopologyTypeAsString());
    if(this->GetNumberOfElements()){
        this->Set("Dimensions", this->Shape->GetShapeAsString());
    }
    if( this->OrderIsDefault == 0 ){
        this->Set("Order", this->GetOrderAsString());
    } 
    if( this->BaseOffset != 0 ){
        char Offset[80];
        ostrstream OffsetStream(Offset, 80);
        OffsetStream << this->BaseOffset << ends;
        this->Set("BaseOffset", Offset);
    }
    if(this->BuildFromDataXml() == XDMF_SUCCESS) return(XDMF_SUCCESS);
    if(this->Connectivity){
        XdmfDataItem * di = this->GetDataItem();
        di->SetArray(this->Connectivity);
        if(this->Connectivity->GetNumberOfElements() > this->LightDataLimit) di->SetFormat(XDMF_FORMAT_HDF);
        di->Build();
        this->SetCurrentXdmfElement(di->GetElement(), NULL);
        delete di;
    }
    return(XDMF_SUCCESS);
}

XdmfInt32
XdmfTopology::Insert( XdmfElement *Child){
    if(Child && (
        XDMF_WORD_CMP(Child->GetElementName(), "DataItem") ||
        XDMF_WORD_CMP(Child->GetElementName(), "Information")
        )){
        return(XdmfElement::Insert(Child));
    }else{
        XdmfErrorMessage("Topology can only Insert DataItem or Information elements");
    }
    return(XDMF_FAIL);
}

XdmfInt64 XdmfTopology::GetNumberOfElements( void ) {
  return( this->Shape->GetNumberOfElements() );
  }

XdmfInt32
XdmfTopology::SetNumberOfElements( XdmfInt64 NumberOfElements){
    return(this->Shape->SetNumberOfElements(NumberOfElements));
    }

XdmfInt32
XdmfTopology::SetConnectivity( XdmfArray *Array ){
    if(this->Connectivity == Array) return(XDMF_SUCCESS);
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
} else if( XDMF_WORD_CMP( topologyType, "EDGE_3") ){
  newTopologyType = XDMF_EDGE_3;
} else if( XDMF_WORD_CMP( topologyType, "TRI_6") ){
  newTopologyType = XDMF_TRI_6;
} else if( XDMF_WORD_CMP( topologyType, "TRIANGLE_6") ){
  newTopologyType = XDMF_TRI_6;
} else if( XDMF_WORD_CMP( topologyType, "TRI_7") ){
  newTopologyType = XDMF_TRI_7;
} else if( XDMF_WORD_CMP( topologyType, "TRIANGLE_7") ){
  newTopologyType = XDMF_TRI_7;
} else if( XDMF_WORD_CMP( topologyType, "QUAD_6") ){
  newTopologyType = XDMF_QUAD_6;
} else if( XDMF_WORD_CMP( topologyType, "QUAD_8") ){
  newTopologyType = XDMF_QUAD_8;
} else if( XDMF_WORD_CMP( topologyType, "QUAD_9") ){
  newTopologyType = XDMF_QUAD_9;
} else if( XDMF_WORD_CMP( topologyType, "QUADRILATERAL_6") ){
  newTopologyType = XDMF_QUAD_6;
} else if( XDMF_WORD_CMP( topologyType, "QUADRILATERAL_8") ){
  newTopologyType = XDMF_QUAD_8;
} else if( XDMF_WORD_CMP( topologyType, "QUADRILATERAL_9") ){
  newTopologyType = XDMF_QUAD_9;
} else if( XDMF_WORD_CMP( topologyType, "TET_10") ){
  newTopologyType = XDMF_TET_10;
} else if( XDMF_WORD_CMP( topologyType, "TETRAHEDRON_10") ){
  newTopologyType = XDMF_TET_10;
} else if( XDMF_WORD_CMP( topologyType, "PYRAMID_13") ){
  newTopologyType = XDMF_PYRAMID_13;
} else if( XDMF_WORD_CMP( topologyType, "WEDGE_12") ){
  newTopologyType = XDMF_WEDGE_12;
} else if( XDMF_WORD_CMP( topologyType, "WEDGE_15") ){
  newTopologyType = XDMF_WEDGE_15;
} else if( XDMF_WORD_CMP( topologyType, "WEDGE_18") ){
  newTopologyType = XDMF_WEDGE_18;
} else if( XDMF_WORD_CMP( topologyType, "HEX_20") ){
  newTopologyType = XDMF_HEX_20;
} else if( XDMF_WORD_CMP( topologyType, "HEXAHEDRON_20") ){
  newTopologyType = XDMF_HEX_20;
} else if( XDMF_WORD_CMP( topologyType, "HEXAHEDRON_24") ){
  newTopologyType = XDMF_HEX_24;
} else if( XDMF_WORD_CMP( topologyType, "HEXAHEDRON_27") ){
  newTopologyType = XDMF_HEX_27;
} else if( XDMF_WORD_CMP( topologyType, "HEXAHEDRON_64") ){
  newTopologyType = XDMF_HEX_64;
} else if( XDMF_WORD_CMP( topologyType, "HEXAHEDRON_125") ){
  newTopologyType = XDMF_HEX_125;
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
    case  XDMF_EDGE_3 :
      return("Edge_3");
    case  XDMF_TRI_6 :
      return("Triangle_6");
    case  XDMF_TRI_7:
      return("Triangle_7");
    case XDMF_QUAD_6:
      return("Quadrilateral_6");
    case  XDMF_QUAD_8 :
      return("Quadrilateral_8");
    case  XDMF_QUAD_9 :
      return("Quadrilateral_9");
    case  XDMF_TET_10 :
      return("Tetrahedron_10");
    case  XDMF_PYRAMID_13 :
      return("Pyramid_13");
    case  XDMF_WEDGE_12:
      return("Wedge_12");
    case  XDMF_WEDGE_15 :
      return("Wedge_15");
    case  XDMF_WEDGE_18 :
      return("Wedge_18");
    case  XDMF_HEX_20 :
      return("Hexahedron_20");
    case  XDMF_HEX_24 :
      return("Hexahedron_24");
    case  XDMF_HEX_27 :
      return("Hexahedron_27");
    case  XDMF_HEX_64 :
      return("Hexahedron_64");
    case  XDMF_HEX_125 :
      return("Hexahedron_125");
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
    case  XDMF_EDGE_3 :
      nodesPerElement = 3;
      break;
    case  XDMF_TRI_6 :
      nodesPerElement = 6;
      break;
    case  XDMF_TRI_7 :
      nodesPerElement = 7;
      break;
    case  XDMF_QUAD_6:
      nodesPerElement = 6;
      break;
    case  XDMF_QUAD_8 :
      nodesPerElement = 8;
      break;
    case  XDMF_QUAD_9 :
      nodesPerElement = 9;
      break;
    case  XDMF_TET_10 :
      nodesPerElement = 10;
      break;
    case  XDMF_PYRAMID_13 :
      nodesPerElement = 13;
      break;
    case  XDMF_WEDGE_12 :
      nodesPerElement = 12;
      break;
    case  XDMF_WEDGE_15 :
      nodesPerElement = 15;
      break;
    case  XDMF_WEDGE_18 :
      nodesPerElement = 18;
      break;
    case  XDMF_HEX_20 :
      nodesPerElement = 20;
      break;
    case  XDMF_HEX_24 :
      nodesPerElement = 24;
      break;
    case  XDMF_HEX_27 :
      nodesPerElement = 27;
      break;
    case  XDMF_HEX_64 :
      nodesPerElement = 64;
      break;
    case  XDMF_HEX_125 :
      nodesPerElement = 125;
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
XdmfTopology::GetCellOffsets( XdmfArray *Array ){
    XdmfInt64   *offsets;
    XdmfInt64   *conns;

    // Only Necessary for Mixed Topology but will work for any unstructured
if(this->GetClass() == XDMF_STRUCTURED){
    XdmfErrorMessage("Grid is Structured: No Connectivity");
    return(NULL);
}
if(!this->Connectivity){
    XdmfErrorMessage("Cannot get Cell Offsets without Connectivity");
    return(NULL);
}
if(!this->CellOffsets){
    this->CellOffsets = new XdmfArray;
    this->CellOffsets->SetNumberType(XDMF_INT64_TYPE);
    this->CellOffsets->SetNumberOfElements(1);
}
if((this->GetNumberOfElements() + 1)  != this->CellOffsets->GetNumberOfElements()){
    this->CellOffsets->SetNumberOfElements(this->GetNumberOfElements() + 1);
}else{
    // Already done
    if( Array ){
      CopyArray( this->CellOffsets, Array );
      return( Array );
    }
    return(this->CellOffsets);
}
// We don't know what number type Connectivity is 
conns = new XdmfInt64[ this->Connectivity->GetNumberOfElements() ];
this->Connectivity->GetValues(0, conns, this->Connectivity->GetNumberOfElements());
offsets = (XdmfInt64 *)this->CellOffsets->GetDataPointer();
if(this->TopologyType == XDMF_MIXED){
    XdmfInt64   i, index = 0;
    XdmfInt32   npe, celltype;
    for(i=0;i<this->GetNumberOfElements();i++){
        // cout << " index = " << index << " *conns = " << *conns << endl;
        celltype = *conns;
        *offsets++ = index;
        // Skip Cell Type
        index++;
        conns++;
        // cout << " celltype = " << celltype << endl;
        switch ( celltype ){
            case  XDMF_POLYVERTEX :
            case  XDMF_POLYLINE :
            case  XDMF_POLYGON :
              npe = *conns++;
              index++;
              break;
            case  XDMF_TRI :
              npe = 3;
              break;
            case  XDMF_QUAD :
              npe = 4;
              break;
            case  XDMF_TET :
              npe = 4;
              break;
            case  XDMF_PYRAMID :
              npe = 5;
              break;
            case  XDMF_WEDGE :
              npe = 6;
              break;
            case  XDMF_HEX :
              npe = 8;
              break;
            case  XDMF_EDGE_3 :
              npe = 3;
              break;
            case  XDMF_TRI_6 :
              npe = 6;
              break;
            case  XDMF_TRI_7 :
              npe = 7;
              break;
            case  XDMF_QUAD_6 :
              npe = 6;
              break;
            case  XDMF_QUAD_8 :
              npe = 8;
              break;
            case  XDMF_QUAD_9 :
              npe = 9;
              break;
            case  XDMF_TET_10 :
              npe = 10;
              break;
            case  XDMF_PYRAMID_13 :
              npe = 13;
              break;
            case  XDMF_WEDGE_12 :
              npe = 12;
              break;
            case  XDMF_WEDGE_15 :
              npe = 15;
              break;
            case  XDMF_WEDGE_18 :
              npe = 18;
              break;
            case  XDMF_HEX_20 :
              npe = 20;
              break;
            case  XDMF_HEX_24 :
              npe = 24;
              break;
            case  XDMF_HEX_27 :
              npe = 27;
              break;
            case XDMF_HEX_64 :
              npe = 64;
              break;
            case XDMF_HEX_125 :
              npe = 125;
              break;
            default :
                XdmfErrorMessage("Unknown Cell Type = " << celltype);
                break;
        }
        // cout << " npe = " << npe << endl;
        conns += npe;
        index += npe;
    }
    *offsets = index; // index at nelements + 1
}else{
    // NodesPerElement are constant
    XdmfInt64   i, index = 0;

    for(i=0;i<this->GetNumberOfElements();i++){
        *offsets++ = index;
        index += this->NodesPerElement;
    }
    *offsets = index; // index at nelements + 1
}
if(conns) delete conns;
if( Array ){
  CopyArray( this->CellOffsets, Array );
  return( Array );
}
return( this->CellOffsets );
}

XdmfArray *
XdmfTopology::GetConnectivity( XdmfArray *Array, XdmfInt32 Create ){
    if(!this->Connectivity && Create){
        this->Connectivity = new XdmfArray;
        this->ConnectivityIsMine = 1;
    }
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
Attribute = this->Get( "TopologyType" );
if(!Attribute) Attribute = this->Get( "Type" );
if( this->SetTopologyTypeFromString( Attribute ) == XDMF_FAIL ){
  XdmfErrorMessage("Bad Topology Type : " << Attribute );
  free((void*)Attribute);
  return( XDMF_FAIL );
  }
free((void*)Attribute);
// Set Shape Either Way
Attribute = this->Get( "NumberOfElements" );
if( Attribute ){
  this->GetShapeDesc()->SetShapeFromString( Attribute );
  }
free((void*)Attribute);
Attribute = this->Get( "Dimensions" );
if( Attribute ){
  this->GetShapeDesc()->SetShapeFromString( Attribute );
}
free((void*)Attribute);
Attribute = this->Get( "NodesPerElement" );
if( Attribute ){
  XdmfInt64 nodesPerElement;

  nodesPerElement = strtol( Attribute, (XdmfString *)NULL, 0 );
  this->SetNodesPerElement( nodesPerElement );
  }
free((void*)Attribute);
Attribute = this->Get( "Order" );
if( Attribute ){
  this->SetOrderFromString( Attribute );
  }
free((void*)Attribute);
Attribute = this->Get( "BaseOffset" );
if( Attribute ){
  this->BaseOffset = strtol( Attribute, (XdmfString *)NULL, 0);
  }
free((void*)Attribute);
if(!this->Name) this->SetName(GetUnique("Topology_"));
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfTopology::Update() {

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
  ConnectionElement = this->DOM->FindDataElement(0, this->Element );
  if( ConnectionElement ){
      XdmfDataItem  Connections;
  
    // cout << "Connection DataItem = " << &Connections << endl;
    XdmfDebug("Reading Connections from DataItem");
    if(Connections.SetDOM(this->DOM) == XDMF_FAIL) return(XDMF_FAIL);
    Connections.SetDsmBuffer(this->DsmBuffer);
    if( this->ConnectivityIsMine && this->Connectivity ) delete this->Connectivity;
    if(Connections.SetElement(ConnectionElement, 0) == XDMF_FAIL) return(XDMF_FAIL);
    if(Connections.UpdateInformation() == XDMF_FAIL) return(XDMF_FAIL);
    if(Connections.Update() == XDMF_FAIL) return(XDMF_FAIL);
    // Steal the Array so it doesn't get deleted in the destructor of the DataItem
    this->Connectivity = Connections.GetArray();
    Connections.SetArrayIsMine(0);
    this->ConnectivityIsMine = 1;

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
    XdmfDebug("Using Default Connectivity");
    XdmfInt64  Length;
    if( !this->Connectivity ){
      this->Connectivity = new XdmfArray;
      this->Connectivity->SetNumberType( XDMF_INT64_TYPE );
      this->ConnectivityIsMine = 1;
    }
    Length = this->GetShapeDesc()->GetNumberOfElements() * this->NodesPerElement;
    this->Connectivity->SetNumberOfElements( Length );
    this->Connectivity->Generate( 0 , Length - 1 );
    XdmfDebug("Default Connectivity Set");
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

XdmfInt32
XdmfTopology::GetEdgesPerElement() {

  switch ( this->TopologyType ){
    case  XDMF_NOTOPOLOGY :
      return 0;
    case  XDMF_POLYVERTEX :
      return 0;
    case  XDMF_POLYLINE :
      return this->NodesPerElement - 1;
    case  XDMF_POLYGON :
      return this->NodesPerElement;
    case  XDMF_TRI :
      return 3;
    case  XDMF_QUAD :
      return 4;
    case  XDMF_TET :
      return 6;
    case  XDMF_PYRAMID :
      return 8;
    case  XDMF_WEDGE :
      return 9;
    case  XDMF_HEX :
      return 12;
    case  XDMF_EDGE_3 :
      return 1;
    case  XDMF_TRI_6 :
    case  XDMF_TRI_7 :
      return 3;
    case  XDMF_QUAD_6 :
    case  XDMF_QUAD_8 :
    case  XDMF_QUAD_9 :
      return 4;
    case  XDMF_TET_10 :
      return 6;
    case  XDMF_PYRAMID_13 :
      return 8;
    case  XDMF_WEDGE_12 :
    case  XDMF_WEDGE_15 :
    case  XDMF_WEDGE_18 :
      return 9;
    case  XDMF_HEX_20 :
    case  XDMF_HEX_24 :
    case  XDMF_HEX_27 :
    case  XDMF_HEX_64 :
    case  XDMF_HEX_125 :
      return 12;
    case  XDMF_MIXED :
    case  XDMF_2DSMESH :
    case  XDMF_2DRECTMESH :
    case  XDMF_2DCORECTMESH :
    case  XDMF_3DSMESH :
    case  XDMF_3DRECTMESH :
    case  XDMF_3DCORECTMESH :
    default :
      return 0;
  }
}

XdmfInt32
XdmfTopology::GetFacesPerElement() {

  switch ( this->TopologyType ){
    case  XDMF_NOTOPOLOGY :
      return 0;
    case  XDMF_POLYVERTEX :
      return 0;
    case  XDMF_POLYLINE :
      return 0;
    case  XDMF_POLYGON :
      return 1;
    case  XDMF_TRI :
      return 1;
    case  XDMF_QUAD :
      return 1;
    case  XDMF_TET :
      return 4;
    case  XDMF_PYRAMID :
      return 5;
    case  XDMF_WEDGE :
      return 5;
    case  XDMF_HEX :
      return 6;
    case  XDMF_EDGE_3 :
      return 0;
    case  XDMF_TRI_6 :
    case  XDMF_TRI_7 :
      return 1;
    case  XDMF_QUAD_6 :
    case  XDMF_QUAD_8 :
    case  XDMF_QUAD_9 :
      return 1;
    case  XDMF_TET_10 :
      return 4;
    case  XDMF_PYRAMID_13 :
      return 5;
    case  XDMF_WEDGE_12 :
    case  XDMF_WEDGE_15 :
    case  XDMF_WEDGE_18 :
      return 5;
    case  XDMF_HEX_20 :
    case  XDMF_HEX_24 :
    case  XDMF_HEX_27 :
    case  XDMF_HEX_64 :
    case  XDMF_HEX_125 :
      return 6;
    case  XDMF_MIXED :
    case  XDMF_2DSMESH :
    case  XDMF_2DRECTMESH :
    case  XDMF_2DCORECTMESH :
    case  XDMF_3DSMESH :
    case  XDMF_3DRECTMESH :
    case  XDMF_3DCORECTMESH :
    default :
      return 0;
  }
}

XdmfTopology * 
XdmfTopology::GetExternalSurface() {

  const XdmfInt64 numberCells = this->GetNumberOfElements();
  const XdmfInt64 largestId = Connectivity->GetMaxAsInt64();
  
  std::vector<std::vector<std::vector<XdmfInt64> > > hash(largestId);
  
  if(TopologyType == XDMF_TET) {
    
    const int stopValue = numberCells * 4;
    for(int arrayOffset=0; arrayOffset<stopValue; arrayOffset += 4) {
      const XdmfInt64 a = Connectivity->GetValueAsInt64(arrayOffset);
      const XdmfInt64 b = Connectivity->GetValueAsInt64(arrayOffset + 1);
      const XdmfInt64 c = Connectivity->GetValueAsInt64(arrayOffset + 2);
      const XdmfInt64 d = Connectivity->GetValueAsInt64(arrayOffset + 3);
      InsertTriInHash(a, b, d, hash);
      InsertTriInHash(a, c, b, hash);
      InsertTriInHash(a, d, c, hash);
      InsertTriInHash(b, c, d, hash);
    }

    // create new topology
    XdmfTopology * toReturn = new XdmfTopology();
    toReturn->SetTopologyType(XDMF_TRI);
    std::vector<XdmfInt64> newCells;
    int index = 0;
    for(std::vector<std::vector<std::vector<XdmfInt64> > >::const_iterator
	  hashIter = hash.begin(); hashIter != hash.end(); 
	++hashIter, ++index) {
      const std::vector<std::vector<XdmfInt64> > & currHash = *hashIter;
      for(std::vector<std::vector<XdmfInt64> >::const_iterator currHashIter = 
	    currHash.begin(); currHashIter != currHash.end(); ++currHashIter) {
	const std::vector<XdmfInt64> & currFaceIds = *currHashIter;
	newCells.push_back(index);
	newCells.push_back(currFaceIds[0]);
	newCells.push_back(currFaceIds[1]);
      }
    }
    toReturn->SetNumberOfElements(newCells.size() / 3);
    XdmfArray * toReturnCells = toReturn->GetConnectivity();
    toReturnCells->SetNumberType(XDMF_INT64_TYPE);
    toReturnCells->SetNumberOfElements(newCells.size());
    toReturnCells->SetValues(0, &newCells[0], newCells.size());
    return toReturn;
  }
  else if(TopologyType == XDMF_HEX) {
    const int stopValue = numberCells * 8;
    for(XdmfInt64 arrayOffset=0; arrayOffset<stopValue; arrayOffset += 8) {
      const XdmfInt64 a = Connectivity->GetValueAsInt64(arrayOffset);
      const XdmfInt64 b = Connectivity->GetValueAsInt64(arrayOffset + 1);
      const XdmfInt64 c = Connectivity->GetValueAsInt64(arrayOffset + 2);
      const XdmfInt64 d = Connectivity->GetValueAsInt64(arrayOffset + 3);
      const XdmfInt64 e = Connectivity->GetValueAsInt64(arrayOffset + 4);
      const XdmfInt64 f = Connectivity->GetValueAsInt64(arrayOffset + 5);
      const XdmfInt64 g = Connectivity->GetValueAsInt64(arrayOffset + 6);
      const XdmfInt64 h = Connectivity->GetValueAsInt64(arrayOffset + 7);
      InsertQuadInHash(a, b, f, e, hash);
      InsertQuadInHash(a, d, c, b, hash);
      InsertQuadInHash(a, e, h, d, hash);
      InsertQuadInHash(b, c, g, f, hash);
      InsertQuadInHash(c, d, h, g, hash);
      InsertQuadInHash(e, f, g, h, hash);
    }
    
    // create new topology
    XdmfTopology * toReturn = new XdmfTopology();
    toReturn->SetTopologyType(XDMF_QUAD);
    std::vector<XdmfInt64> newCells;
    int index = 0;
    for(std::vector<std::vector<std::vector<XdmfInt64> > >::const_iterator
	  hashIter = hash.begin(); hashIter != hash.end(); 
	++hashIter, ++index) {
      const std::vector<std::vector<XdmfInt64> > & currHash = *hashIter;
      for(std::vector<std::vector<XdmfInt64> >::const_iterator currHashIter = 
	    currHash.begin(); currHashIter != currHash.end(); ++currHashIter) {
	const std::vector<XdmfInt64> & currFaceIds = *currHashIter;
	newCells.push_back(index);
	newCells.push_back(currFaceIds[0]);
	newCells.push_back(currFaceIds[1]);
	newCells.push_back(currFaceIds[2]);
      }
    }
    toReturn->SetNumberOfElements(newCells.size() / 4);
    XdmfArray * toReturnCells = toReturn->GetConnectivity();
    toReturnCells->SetNumberType(XDMF_INT64_TYPE);
    toReturnCells->SetNumberOfElements(newCells.size());
    toReturnCells->SetValues(0, &newCells[0], newCells.size());
    return toReturn;
  }
  else if(TopologyType == XDMF_TET_10) {
    const int stopValue = numberCells * 10;
    for(XdmfInt64 arrayOffset=0; arrayOffset<stopValue; arrayOffset += 10) {
      const XdmfInt64 a = Connectivity->GetValueAsInt64(arrayOffset);
      const XdmfInt64 b = Connectivity->GetValueAsInt64(arrayOffset + 1);
      const XdmfInt64 c = Connectivity->GetValueAsInt64(arrayOffset + 2);
      const XdmfInt64 d = Connectivity->GetValueAsInt64(arrayOffset + 3);
      const XdmfInt64 e = Connectivity->GetValueAsInt64(arrayOffset + 4);
      const XdmfInt64 f = Connectivity->GetValueAsInt64(arrayOffset + 5);
      const XdmfInt64 g = Connectivity->GetValueAsInt64(arrayOffset + 6);
      const XdmfInt64 h = Connectivity->GetValueAsInt64(arrayOffset + 7);
      const XdmfInt64 i = Connectivity->GetValueAsInt64(arrayOffset + 8);
      const XdmfInt64 j = Connectivity->GetValueAsInt64(arrayOffset + 9);
      InsertTri6InHash(a, b, d, e, i, h, hash); // 0 1 3 4 8 7
      InsertTri6InHash(a, c, b, g, f, e, hash); // 0 2 1 6 5 4
      InsertTri6InHash(a, d, c, h, j, g, hash); // 0 3 2 7 9 6
      InsertTri6InHash(b, c, d, f, j, i, hash); // 1 2 3 5 9 8
    }

    // create new topology
    XdmfTopology * toReturn = new XdmfTopology();
    toReturn->SetTopologyType(XDMF_TRI_6);
    std::vector<XdmfInt64> newCells;
    int index = 0;
    for(std::vector<std::vector<std::vector<XdmfInt64> > >::const_iterator
	  hashIter = hash.begin(); hashIter != hash.end(); 
	++hashIter, ++index) {
      const std::vector<std::vector<XdmfInt64> > & currHash = *hashIter;
      for(std::vector<std::vector<XdmfInt64> >::const_iterator currHashIter = 
	    currHash.begin(); currHashIter != currHash.end(); ++currHashIter) {
	const std::vector<XdmfInt64> & currFaceIds = *currHashIter;
	newCells.push_back(index);
	newCells.push_back(currFaceIds[0]);
	newCells.push_back(currFaceIds[1]);
	newCells.push_back(currFaceIds[2]);
	newCells.push_back(currFaceIds[3]);
	newCells.push_back(currFaceIds[4]);
      }
    }
    toReturn->SetNumberOfElements(newCells.size() / 6);
    XdmfArray * toReturnCells = toReturn->GetConnectivity();
    toReturnCells->SetNumberType(XDMF_INT64_TYPE);
    toReturnCells->SetNumberOfElements(newCells.size());
    toReturnCells->SetValues(0, &newCells[0], newCells.size());
    return toReturn;
  }
  else if(TopologyType == XDMF_HEX_20) {
    const int stopValue = numberCells * 20;
    for(XdmfInt64 arrayOffset=0; arrayOffset<stopValue; arrayOffset += 20) {
      const XdmfInt64 a = Connectivity->GetValueAsInt64(arrayOffset);
      const XdmfInt64 b = Connectivity->GetValueAsInt64(arrayOffset + 1);
      const XdmfInt64 c = Connectivity->GetValueAsInt64(arrayOffset + 2);
      const XdmfInt64 d = Connectivity->GetValueAsInt64(arrayOffset + 3);
      const XdmfInt64 e = Connectivity->GetValueAsInt64(arrayOffset + 4);
      const XdmfInt64 f = Connectivity->GetValueAsInt64(arrayOffset + 5);
      const XdmfInt64 g = Connectivity->GetValueAsInt64(arrayOffset + 6);
      const XdmfInt64 h = Connectivity->GetValueAsInt64(arrayOffset + 7);
      const XdmfInt64 i = Connectivity->GetValueAsInt64(arrayOffset + 8);
      const XdmfInt64 j = Connectivity->GetValueAsInt64(arrayOffset + 9);
      const XdmfInt64 k = Connectivity->GetValueAsInt64(arrayOffset + 10);
      const XdmfInt64 l = Connectivity->GetValueAsInt64(arrayOffset + 11);
      const XdmfInt64 m = Connectivity->GetValueAsInt64(arrayOffset + 12);
      const XdmfInt64 n = Connectivity->GetValueAsInt64(arrayOffset + 13);
      const XdmfInt64 o = Connectivity->GetValueAsInt64(arrayOffset + 14);
      const XdmfInt64 p = Connectivity->GetValueAsInt64(arrayOffset + 15);
      const XdmfInt64 q = Connectivity->GetValueAsInt64(arrayOffset + 16);
      const XdmfInt64 r = Connectivity->GetValueAsInt64(arrayOffset + 17);
      const XdmfInt64 s = Connectivity->GetValueAsInt64(arrayOffset + 18);
      const XdmfInt64 t = Connectivity->GetValueAsInt64(arrayOffset + 19);
      InsertQuad8InHash(a, b, f, e, i, r, m, q, hash);
      InsertQuad8InHash(a, d, c, b, l, k, j, i, hash);
      InsertQuad8InHash(a, e, h, d, q, p, t, l, hash);
      InsertQuad8InHash(b, c, g, f, j, s, n, r, hash);
      InsertQuad8InHash(c, d, h, g, k, t, o, s, hash);
      InsertQuad8InHash(e, f, g, h, m, n, o, p, hash);
    }
    
    // create new topology
    XdmfTopology * toReturn = new XdmfTopology();
    toReturn->SetTopologyType(XDMF_QUAD_8);
    std::vector<XdmfInt64> newCells;
    int index = 0;
    for(std::vector<std::vector<std::vector<XdmfInt64> > >::const_iterator
	  hashIter = hash.begin(); hashIter != hash.end(); 
	++hashIter, ++index) {
      const std::vector<std::vector<XdmfInt64> > & currHash = *hashIter;
      for(std::vector<std::vector<XdmfInt64> >::const_iterator currHashIter = 
	    currHash.begin(); currHashIter != currHash.end(); ++currHashIter) {
	const std::vector<XdmfInt64> & currFaceIds = *currHashIter;
	newCells.push_back(index);
	newCells.push_back(currFaceIds[0]);
	newCells.push_back(currFaceIds[1]);
	newCells.push_back(currFaceIds[2]);
	newCells.push_back(currFaceIds[3]);
	newCells.push_back(currFaceIds[4]);
	newCells.push_back(currFaceIds[5]);
	newCells.push_back(currFaceIds[6]);
      }
    }
    toReturn->SetNumberOfElements(newCells.size() / 8);
    XdmfArray * toReturnCells = toReturn->GetConnectivity();
    toReturnCells->SetNumberType(XDMF_INT64_TYPE);
    toReturnCells->SetNumberOfElements(newCells.size());
    toReturnCells->SetValues(0, &newCells[0], newCells.size());
    return toReturn;
  }

  XdmfErrorMessage("Unsupported TopologyType when computing external surface");
  return NULL;

}

