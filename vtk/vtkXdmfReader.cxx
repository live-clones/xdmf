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
#include "vtkXdmfReader.h"

#include "vtkUnstructuredGrid.h"
#include "vtkStructuredGrid.h"
#include "vtkRectilinearGrid.h"
#include "vtkDoubleArray.h"

#include "vtkDataArrayCollection.h"
#include "vtkIdListCollection.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkXdmfReader, "1.1");
vtkStandardNewMacro(vtkXdmfReader);


//---------------------------------------------------------------------------
vtkXdmfReader::vtkXdmfReader() {
  vtkDebugMacro(<< "vtkXdmfReader Constructor" );

  this->Initialized = 0;
  this->InputFileName = NULL;
  this->Grid = NULL;
  this->DOM = NULL;
  this->CurrentOutput = NULL;
  this->ArrayConverter = new vtkXdmfDataArray;
  this->AttributeStatus = NULL;
  this->NumberOfAttributes = 0;
  this->GridIndex = 0;
  this->GridName = NULL;
  }

vtkXdmfReader::~vtkXdmfReader() {
  vtkDebugMacro(<< "vtkXdmfReader Destructor" );
  if ( this->InputFileName ) {
    delete [] this->InputFileName;
    }
  }
char *
vtkXdmfReader::GetXdmfGridHandle() {
return( XdmfObjectToHandle( this->Grid ) );
}

char *
vtkXdmfReader::GetXdmfDOMHandle() {
return( XdmfObjectToHandle( this->DOM ) );
}

vtkDataObject * vtkXdmfReader::GetOutput( int idx)
{
vtkDebugMacro(<< "GetOutput" );
if( !this->Grid->GetGeometry()->GetNumberOfPoints() <= 0  ){
  this->Execute();
  this->CurrentOutput = this->Outputs[0];
  }
return( vtkSource::GetOutput( idx ) );
}

vtkDataObject * vtkXdmfReader::GetOutput()
{
vtkDebugMacro(<< "GetOutput" );
return( this->GetOutput(0) );
}

void
vtkXdmfReader::PrintSelf(ostream& os, vtkIndent indent)
{
os << indent << "InputFileName : " << this->InputFileName << endl;
this->Outputs[0]->Print(os);
this->Superclass::PrintSelf(os,indent);
}

void
vtkXdmfReader::SetInputFileName( const char *fileName ) {
  vtkDebugMacro(<< "SetInputFileName " << fileName );
  if ( this->InputFileName ) {
    delete [] this->InputFileName;
    }
  if( fileName ) {
    this->InputFileName = new char[strlen(fileName)+1];
    strcpy( this->InputFileName, fileName );
    }
  }

int
vtkXdmfReader::Initialize() {
  XdmfInt32  Index = this->GridIndex;
  XdmfXNode  *GridNode;
  char    *GridName = this->GridName;

  vtkDebugMacro(<< "Initialize" << GridName );
  if( this->Initialized ){
    return( XDMF_SUCCESS );
    }
  /*
  if( !this->DOM ) {
    vtkErrorMacro(<< "DOM is NULL" );
    return(XDMF_FAIL);
    }
  */
  if ( this->Parse() == XDMF_FAIL ) {
    vtkErrorMacro(<< "Error Parsing XML" );
    return(XDMF_FAIL);
    }
  GridNode = this->DOM->FindElement("Grid", this->GridIndex );
  if( GridName ) {
    Index = 0;
    while(GridNode) {
      char *Name;
  
      Name = this->DOM->Get( GridNode, "Name" );
      if( Name && strcmp(Name, GridName)) {
        break;
        }
      Index++;
      GridNode = this->DOM->FindElement("Grid", Index );
      }
  }
  if ( GridNode ) {
    this->Grid = new XdmfGrid;
    this->Grid->SetDOM( this->DOM );
    this->Grid->InitGridFromElement( GridNode );
    if( this->Grid->GetClass() == XDMF_UNSTRUCTURED ) {
      vtkDebugMacro(<< "Unstructured Grid Named " << this->DOM->Get(GridNode, "Name") );
      vtkUnstructuredGrid  *vGrid = vtkUnstructuredGrid::New();
      this->SetNthOutput(0, vGrid );
    } else {
      if( ( this->Grid->GetTopologyType() == XDMF_2DSMESH ) ||
        ( this->Grid->GetTopologyType() == XDMF_3DSMESH ) ){
        vtkDebugMacro(<< "Structured Grid Named " << this->DOM->Get(GridNode, "Name") );
        vtkStructuredGrid  *vGrid = vtkStructuredGrid::New();
        this->SetNthOutput(0, vGrid );
      } else {
        vtkDebugMacro(<< "Rectilinear Grid Named " << this->DOM->Get(GridNode, "Name") );
        vtkRectilinearGrid  *vGrid = vtkRectilinearGrid::New();
        this->SetNthOutput(0, vGrid );
      }
    }
  if (this->AttributeStatus) {
    delete [] this->AttributeStatus;
  }
  this->NumberOfAttributes = this->Grid->GetNumberOfAttributes();
  this->AttributeStatus = new int[ this->NumberOfAttributes + 1 ];
  this->AttributeStatus[ this->NumberOfAttributes ] = -1;
  this->SetAllAttributeStatusOn();
    this->Outputs[0]->ReleaseData();
    this->Outputs[0]->Delete();
  this->Outputs[0]->Register(this);
  this->Outputs[0]->Delete();
  this->Modified();
  } else {
    vtkErrorMacro(<< "No Grid Found with Name = " << GridName );
    return(XDMF_FAIL);
    }
  this->Initialized = 1;
  return( XDMF_SUCCESS );
  }

void
vtkXdmfReader::SetAttributeStatusOn( char *Name ) {
  char *AttributeName;
  int  i;
  for( i = 0 ; i < this->NumberOfAttributes ; i++ ){
    AttributeName = this->Grid->GetAttribute(i)->GetName();
    if(strcmp( Name, AttributeName) == 0 ){
      this->AttributeStatus[ i ] = 1;
    }
  }
}
void
vtkXdmfReader::SetAttributeStatusOff( char *Name ) {
  char *AttributeName;
  int  i;
  for( i = 0 ; i < this->NumberOfAttributes ; i++ ){
    AttributeName = this->Grid->GetAttribute(i)->GetName();
    if(strcmp( Name, AttributeName) == 0 ){
      this->AttributeStatus[ i ] = 0;
    }
  }
}
void
vtkXdmfReader::SetAttributeStatusOn( int Index ) {
  if ( Index < this->NumberOfAttributes ) {
    this->AttributeStatus[ Index ] = 1;    
    }
  }

void
vtkXdmfReader::SetAttributeStatusOff( int Index ) {
  if ( Index < this->NumberOfAttributes ) {
    this->AttributeStatus[ Index ] = 0;    
    }
  }

void
vtkXdmfReader::SetAllAttributeStatusOn() {
  int i;
  for( i = 0 ; i < this->NumberOfAttributes ; i++ ){
    this->SetAttributeStatusOn( i );
    }
  }

void
vtkXdmfReader::SetAllAttributeStatusOff() {
  int i;
  for( i = 0 ; i < this->NumberOfAttributes ; i++ ){
    this->SetAttributeStatusOff( i );
    }
  }

void vtkXdmfReader::Update() {

vtkDebugMacro(<< "Update" );
this->Execute();
}

int
vtkXdmfReader::SetBaseAttributes(){

XdmfAttribute  *Attribute;
XdmfInt32  Index = 0, Status;

for( Index = 0 ; Index < this->Grid->GetNumberOfAttributes() ; Index++ ){
  Attribute = this->Grid->GetAttribute( Index );
  Status = this->AttributeStatus[ Index ];
  if( Attribute && Status ) {
    vtkDataSet  *vGrid = ( vtkDataSet *)this->Outputs[0];
    XdmfArray  *Values;
    vtkDataArray  *vtkValues;
    char    *Name;
    XdmfInt32  AttributeType, AttributeCenter;

    vtkDebugMacro( << "Reading Attribute " << Attribute->GetName());
    Attribute->Update();  
    Values = Attribute->GetValues();
    this->ArrayConverter->SetVtkArray( NULL );
    vtkValues = this->ArrayConverter->FromXdmfArray(Values->GetTagName());
    Name = Attribute->GetName();
    if( !Name ){
      Name = Values->GetTagName();
      }
    vtkValues->SetName( Name );
    vtkDebugMacro(<< "vtkValues has " << vtkValues->GetNumberOfComponents() << " Components" );
    vtkDebugMacro(<< "vtkValues has " << vtkValues->GetNumberOfTuples() << " Tuples" );
    AttributeType = Attribute->GetAttributeType();
    AttributeCenter = Attribute->GetAttributeCenter();
    // Special Cases
    if( AttributeCenter == XDMF_ATTRIBUTE_CENTER_GRID ) {
      // Implement XDMF_ATTRIBUTE_CENTER_GRID as PointData
      XdmfArray *tmpArray = new XdmfArray;

      vtkDebugMacro(<< "Setting Grid Centered Values");
      tmpArray->CopyType( Values );
      tmpArray->SetNumberOfElements( vGrid->GetNumberOfPoints() );
      tmpArray->Generate( Values->GetValueAsFloat64(0), Values->GetValueAsFloat64(0) );
      vtkValues->Delete();
      this->ArrayConverter->SetVtkArray( NULL );
      vtkValues = this->ArrayConverter->FromXdmfArray(tmpArray->GetTagName());
      Name = Attribute->GetName();
      if( !Name ){
        Name = Values->GetTagName();
        }
      vtkValues->SetName( Name );
      delete tmpArray;
      AttributeCenter = XDMF_ATTRIBUTE_CENTER_NODE;
      }
    switch (AttributeCenter){
      case XDMF_ATTRIBUTE_CENTER_NODE :
        vGrid->GetPointData()->RemoveArray(Name);
        vGrid->GetPointData()->AddArray(vtkValues);
        switch( AttributeType ){
            case XDMF_ATTRIBUTE_TYPE_SCALAR :
              vGrid->GetPointData()->SetActiveScalars( Name );
              break;
            case XDMF_ATTRIBUTE_TYPE_VECTOR :
              vGrid->GetPointData()->SetActiveVectors( Name );
              break;
            case XDMF_ATTRIBUTE_TYPE_TENSOR :
              vGrid->GetPointData()->SetActiveTensors( Name );
              break;
            default :
              break;
          }
        break;
      case XDMF_ATTRIBUTE_CENTER_CELL :
        vGrid->GetCellData()->RemoveArray(Name);
        vGrid->GetCellData()->AddArray(vtkValues);
        switch( AttributeType ){
            case XDMF_ATTRIBUTE_TYPE_SCALAR :
              vGrid->GetCellData()->SetActiveScalars( Name );
              break;
            case XDMF_ATTRIBUTE_TYPE_VECTOR :
              vGrid->GetCellData()->SetActiveVectors( Name );
              break;
            case XDMF_ATTRIBUTE_TYPE_TENSOR :
              vGrid->GetCellData()->SetActiveTensors( Name );
              break;
            default :
              break;
          }
        break;
      default : 
        vtkErrorMacro(<< "Can't Handle Values at " << Attribute->GetAttributeCenterAsString());
        break;
    }
    vGrid->Modified();
  } else {
    if ( Attribute == NULL ){
      vtkErrorMacro(<< "Can't get Attribute at Index " << Index );
    } else {
      vtkDebugMacro(<< "Skipping Attribute " << Attribute->GetName() );
    }
     }
  }
return( XDMF_SUCCESS );
}

int
vtkXdmfReader::SetBaseTopology(){

vtkDataObject   *Grid = NULL;
XdmfGrid  *MyGrid = this->Grid;

vtkDebugMacro(<< "SetBaseTopology" );
if( !MyGrid ) {
  return( XDMF_FAIL );
  }

if( MyGrid->GetClass() == XDMF_UNSTRUCTURED ) {
  vtkUnstructuredGrid  *vGrid = ( vtkUnstructuredGrid *)this->Outputs[0];
  vtkCellArray    *verts;
  XdmfInt32    vType;
  XdmfInt32    NodesPerElement;
  XdmfInt64    NumberOfElements;
  XdmfInt64    i, j, index;  
  XdmfInt64    Length, *Connections;
  vtkIdType    *connections;
  int      *cell_types, *ctp;

  vtkDebugMacro(<< "Unstructred Topology is " << MyGrid->GetTopologyTypeAsString() );
  switch ( MyGrid->GetTopologyType() ){
    case  XDMF_POLYVERTEX :
      vType = VTK_POLY_VERTEX;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_POLYLINE :
      vType = VTK_POLY_LINE;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_POLYGON :
      vType = VTK_POLYGON;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_TRI :
      vType = VTK_TRIANGLE;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_QUAD :
      vType = VTK_QUAD;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_TET :
      vType = VTK_TETRA;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_PYRAMID :
      vType = VTK_PYRAMID;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_WEDGE :
      vType = VTK_WEDGE;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    case  XDMF_HEX :
      vType = VTK_HEXAHEDRON;
      NodesPerElement = MyGrid->GetNodesPerElement();
      break;
    default :
      XdmfErrorMessage("Unknown Topology Type");
      return( XDMF_FAIL );
      break;
  }
  
  /* Create Cell Type Array */
  Length = MyGrid->GetConnectivity()->GetNumberOfElements();
  Connections = new XdmfInt64[ Length ];
  MyGrid->GetConnectivity()->GetValues( 0, Connections, Length );
  
  NumberOfElements = MyGrid->GetShapeDesc()->GetNumberOfElements();
  ctp = cell_types = new int[ NumberOfElements ];

  /* Create Cell Array */
  verts = vtkCellArray::New();

  /* Get the pointer */
  connections = verts->WritePointer(NumberOfElements,
      NumberOfElements * ( 1 + NodesPerElement ));

  /* Connections : N p1 p2 ... pN */
  /* i.e. Triangles : 3 0 1 2    3 3 4 5   3 6 7 8 */
  index = 0;
    for( j = 0 ; j < NumberOfElements; j++ ){
            *ctp++ = vType;
            *connections++ = NodesPerElement;
            for( i = 0 ; i < NodesPerElement; i++ ){
        *connections++ = Connections[index++];
                    }
     }
  delete [] Connections;
  vGrid->SetCells(cell_types, verts);
  /* OK, because of reference counting */
  verts->Delete();
  delete [] cell_types;
  vGrid->Modified();
  Grid = vGrid;
} else {
  // Connectivity is Implicit
  XdmfInt32    Rank;
  XdmfInt64    Dimensions[ XDMF_MAX_DIMENSION ];

  Rank = MyGrid->GetShapeDesc()->GetShape( Dimensions );
  if( Rank < 3 ) Dimensions[2] = 1;
  if( ( MyGrid->GetTopologyType() == XDMF_2DSMESH ) ||
    ( MyGrid->GetTopologyType() == XDMF_3DSMESH ) ){
    vtkStructuredGrid  *vGrid = (vtkStructuredGrid *)this->Outputs[0];
    // XDMF and HDF5 use Kdim JDim IDim
    // Vtk wants Idim Jdim Kdim
    vtkDebugMacro(<< "Structred Topology is " << MyGrid->GetTopologyTypeAsString() );
    vGrid->SetDimensions( Dimensions[2], Dimensions[1], Dimensions[0] );
    vGrid->SetWholeExtent(0, Dimensions[2] - 1,
        0, Dimensions[1] - 1,
        0, Dimensions[0] - 1);
    Grid = vGrid;
  } else {
    vtkRectilinearGrid  *vGrid = (vtkRectilinearGrid *)this->Outputs[0];
    // RectilinearGrids Are Specified via X,Y,Z
    vtkDebugMacro(<< "Rectilinear Topology is " << MyGrid->GetTopologyTypeAsString() );
    vGrid->SetDimensions( Dimensions[2], Dimensions[1], Dimensions[0] );
    vGrid->SetWholeExtent(0, Dimensions[2] - 1,
        0, Dimensions[1] - 1,
        0, Dimensions[0] - 1);
    Grid = vGrid;
  }
}
if ( !Grid ) {
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
}

int
vtkXdmfReader::SetBaseGeometry() {

XdmfTopology  *Topology = this->Grid;
XdmfGeometry  *Geometry = this->Grid->GetGeometry();
XdmfInt64  Length;
float    *pp;

vtkDebugMacro(<< "SetBaseGeometry " );
if( !Topology ){
  vtkErrorMacro(<< "No XdmfGrid" );
  return( XDMF_FAIL );
  }
if( !Geometry ){
  vtkErrorMacro(<< "Grid has no Geometry" );
  return( XDMF_FAIL );
  }
if( !this->Outputs[0] ) {
  vtkErrorMacro(<< "Reader has no Output");
  return( XDMF_FAIL );
  }
if( ( Geometry->GetGeometryType() == XDMF_GEOMETRY_X_Y_Z ) ||
  ( Geometry->GetGeometryType() == XDMF_GEOMETRY_XYZ ) ||
  ( Geometry->GetGeometryType() == XDMF_GEOMETRY_XY ) ){
   vtkPoints  *Points;
   vtkPointSet  *Pointset = ( vtkPointSet *)this->Outputs[0];

  Points = Pointset->GetPoints();
  if( !Points ){
    vtkDebugMacro(<<  "Creating vtkPoints" );
    Points = vtkPoints::New();
    Pointset->SetPoints( Points );
    // OK Because of Reference Counting
    Points->Delete();
    }
  
/*
   if( Topology->GetClass() == XDMF_STRUCTURED ){
  vtkStructuredGrid  *vGrid = ( vtkStructuredGrid *)this->Outputs[0];
  Points = vGrid->GetPoints();
  if( !Points ){
    vtkDebugMacro(<<  "Creating vtkPoints" );
    Points = vtkPoints::New();
    vGrid->SetPoints( Points );
    // OK Because of Reference Counting
    Points->Delete();
    }
   } else {
  vtkUnstructuredGrid  *vGrid = ( vtkUnstructuredGrid *)this->Outputs[0];
  Points = vGrid->GetPoints();
  if( !Points ){
    vtkDebugMacro(<<  "Creating vtkPoints" );
    Points = vtkPoints::New();
    vGrid->SetPoints( Points );
    // OK Because of Reference Counting
    Points->Delete();
    }
   }
*/
   if( Geometry->GetPoints() ){
  if( Points ){
    Length = Geometry->GetPoints()->GetNumberOfElements();
    vtkDebugMacro( << "Setting Array of " << Length << " = " << Geometry->GetNumberOfPoints() << " Points");
    Points->SetNumberOfPoints( Geometry->GetNumberOfPoints() );
    // XdmfDebug("Getting Pointer");
    pp = Points->GetPoint( 0 );
    // XdmfDebug("Have Data Pointer");
    if( sizeof( float ) == sizeof( XdmfFloat32 ) ) {
      Geometry->GetPoints()->GetValues( 0, (XdmfFloat32 *)pp, Length );
    } else if( sizeof( float ) == sizeof( XdmfFloat64 )) {
      Geometry->GetPoints()->GetValues( 0, (XdmfFloat64 *)pp, Length );
    } else {
      XdmfFloat64  *TmpPp, *TmpPoints = new XdmfFloat64[ Length ];
      XdmfInt64  i;

      Geometry->GetPoints()->GetValues( 0, TmpPoints, Length );
      TmpPp = TmpPoints;
      for( i = 0 ; i < Length ; i++ ){
        *pp++ = *TmpPp++;
        }
      delete TmpPoints;
    }
    Points->Modified();
    Pointset->Modified();
  } else {
    XdmfErrorMessage("Base Grid Has No Points");
    return( XDMF_FAIL );
  }
   } else {
  XdmfErrorMessage("No Points to Set");
  return( XDMF_FAIL );
  }
} else {
  XdmfInt64  Index;
  vtkRectilinearGrid *vGrid = (vtkRectilinearGrid *)this->Outputs[0];
  vtkDoubleArray  *XCoord, *YCoord, *ZCoord;
  XdmfFloat64  *Origin;
  XdmfInt64  Dimensions[3];

  // Make Sure Grid Has Coordinates
  Topology->GetShapeDesc()->GetShape( Dimensions );
  vtkDebugMacro( << "Dimensions of Grid = " << Dimensions[0] <<
       " x " << Dimensions[1] << " x " << Dimensions[2] );
    XCoord = vtkDoubleArray::New();
    vGrid->SetXCoordinates( XCoord );
    // OK Because of Reference Counting
    XCoord->Delete();  
  XCoord->SetNumberOfValues( Dimensions[2] );
    YCoord = vtkDoubleArray::New();
    vGrid->SetYCoordinates( YCoord );
    // OK Because of Reference Counting
    YCoord->Delete();  
  YCoord->SetNumberOfValues( Dimensions[1] );
    ZCoord = vtkDoubleArray::New();
    vGrid->SetZCoordinates( ZCoord );
    // OK Because of Reference Counting
    ZCoord->Delete();  
  ZCoord->SetNumberOfValues( Dimensions[0] );
  // Build Vectors if nescessary
  if( Geometry->GetGeometryType() == XDMF_GEOMETRY_ORIGIN_DXDYDZ ){
    if( Geometry->GetVectorX() == NULL ){
      Geometry->SetVectorX( new XdmfArray );
      Geometry->GetVectorX()->SetNumberType( XDMF_FLOAT32_TYPE );
      }
    if( Geometry->GetVectorY() == NULL ){
      Geometry->SetVectorY( new XdmfArray );
      Geometry->GetVectorY()->SetNumberType( XDMF_FLOAT32_TYPE );
      }
    if( Geometry->GetVectorZ() == NULL ){
      Geometry->SetVectorZ( new XdmfArray );
      Geometry->GetVectorZ()->SetNumberType( XDMF_FLOAT32_TYPE );
      }
    Geometry->GetVectorX()->SetNumberOfElements( Dimensions[2] );
    Geometry->GetVectorY()->SetNumberOfElements( Dimensions[1] );
    Geometry->GetVectorZ()->SetNumberOfElements( Dimensions[0] );
    Origin = Geometry->GetOrigin();
    Geometry->GetVectorX()->Generate( Origin[0],
        Origin[0] + ( Geometry->GetDx() * ( Dimensions[2] - 1 ) ) );
    Geometry->GetVectorY()->Generate( Origin[1],
        Origin[1] + ( Geometry->GetDy() * ( Dimensions[1] - 1 ) ) );
    Geometry->GetVectorZ()->Generate( Origin[2],
        Origin[2] + ( Geometry->GetDz() * ( Dimensions[0] - 1 ) ) );
    }
  // Set the Points
  for( Index = 0 ; Index < Dimensions[2] ; Index++ ){
    XCoord->SetValue( Index , Geometry->GetVectorX()->GetValueAsFloat32( Index ) );
    }  
  for( Index = 0 ; Index < Dimensions[1] ; Index++ ){
    YCoord->SetValue( Index , Geometry->GetVectorY()->GetValueAsFloat32( Index ) );
    }  
  for( Index = 0 ; Index < Dimensions[0] ; Index++ ){
    ZCoord->SetValue( Index , Geometry->GetVectorZ()->GetValueAsFloat32( Index ) );
    }  
}
return( XDMF_SUCCESS );
}

int
vtkXdmfReader::Parse() {
  vtkDebugMacro(<< "Parse" );
  if( !this->DOM ) {
    this->DOM = new XdmfDOM;
    }
  if( this->InputFileName ) {
    this->DOM->SetInputFileName( this->InputFileName );
    }
  return( this->DOM->Parse() );
  }

void vtkXdmfReader::Execute() {

  vtkDebugMacro(<<  "Execute" );
  // node = this->DOM->FindElement("Grid");
  // this->Grid->SetGridFromElement( node );
  this->Initialize();
  this->Grid->Update();
  this->SetBaseTopology();
  this->SetBaseGeometry();
  this->SetBaseAttributes();
  }
