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
#include "Xdmf.h"

#include "vtkCellDataToPointData.h"
#include "vtkContourFilter.h"
#include "vtkStructuredGrid.h"
#include "vtkUnstructuredGrid.h"

XdmfVtk::XdmfVtk( ) {
  this->Mpi = NULL;
  this->Polygons = NULL;
  this->NumberOfPolygons = 0;
  this->NumberOfPoints = 0;
  this->NumberOfConnections = 0;
  this->NumberOfPointScalars = 0;
  this->NumberOfCellScalars = 0;
}

XdmfVtk::~XdmfVtk( ) {
}

XdmfInt32
XdmfVtk::GenerateXML( XdmfString What ){

if( XDMF_WORD_CMP(What, "Head") ) {
  this->GenerateHead();
} else if ( XDMF_WORD_CMP(What, "Tail") ) {
  this->GenerateTail();
} else {
*this->Output << "<Domain><Grid Name=\"Polygons\">\n";
*this->Output << "<Topology Type=\"Triangle\" ";
*this->Output << "    NumElements=\"" << this->NumberOfPolygons << "\"></Topology>\n";
*this->Output << "<Geometry><Vertices>\n";
*this->Output << "<Index>" << this->Domain << ":" << this->FileName << ":/Geometry/Connections</Index>\n";
*this->Output << "<Points>" << this->Domain << ":" << this->FileName << ":/Geometry/Points</Points>\n";
*this->Output << "</Vertices></Geometry>\n";
*this->Output << "<Attribute Name=\"Scalars\">\n";
*this->Output << "<Values>" << this->Domain << ":" << this->FileName << ":/Geometry/Scalars</Values>\n";
*this->Output << "</Attribute>\n";
*this->Output << "</Grid></Domain>\n";
}

return( XDMF_SUCCESS );
}

XdmfInt32
XdmfVtk::CollectPolygons( void ) {

XdmfMpi    *Mpi = this->Mpi;
vtkPolyData  *Polygons = this->Polygons;
XdmfInt32  i;
XdmfInt32Array  InfoArray;
XdmfInt32Array  PolysFromNode;

Mpi->Barrier();
if( Mpi->GetId() == 0 ) {
  XdmfInt32    NumberOfPolygons;
  XdmfInt32    CellType;
  XdmfInt32    NumberOfPoints, NumberOfConnections, NumberOfScalars;
  XdmfInt32Array    PointOffset;
  XdmfInt64    Counter;
  XdmfArray    *Points;
  XdmfArray    *Scalars;
  XdmfArray    *Connections;
  XdmfMemory    Mem;

  this->SetAccess("rw");
  this->Open();
  
  InfoArray.SetNumberOfElements( 5 );

  NumberOfPolygons = Polygons->GetNumberOfPolys();
  Points = XdmfConvertVtkPolyData( Polygons, "Points" );
  Connections = XdmfConvertVtkPolyData( Polygons, "Connections" );
  Scalars = XdmfConvertVtkPolyData( Polygons, "PointScalars" );
  NumberOfPoints = Points->GetNumberOfElements();
  NumberOfConnections = Connections->GetNumberOfElements();
  NumberOfScalars = Scalars->GetNumberOfElements();

  XdmfDebug( "Node 0 has " << NumberOfPolygons << " Polygons " <<
    NumberOfPoints / 3  << " Points " <<  
    NumberOfConnections << " Connections " <<  
    NumberOfScalars << " Scalars");

  PointOffset.SetNumberOfElements( Mpi->GetSize() );
  PointOffset.SetValue( 0, 0 );
  // Collect Information
  PolysFromNode.SetNumberOfElements( Mpi->GetSize() );
  PolysFromNode.Generate( 0, 0 );
  for( i = 1 ; i < Mpi->GetSize() ; i++ ) {
    PointOffset.SetValue( i, NumberOfPoints / 3 );
    Mpi->Recv( &InfoArray, i  );
    PolysFromNode.SetValue( i, InfoArray.GetValue( 1 ) );
    NumberOfPolygons += InfoArray.GetValue( 1 );
    NumberOfPoints += InfoArray.GetValue( 2 );
    NumberOfConnections += InfoArray.GetValue( 3 );
    NumberOfScalars += InfoArray.GetValue( 4 );
  }
  XdmfDebug( "Totals : " <<
    NumberOfPolygons << " Polygons " <<
    NumberOfPoints << " Points " <<
    NumberOfConnections << " Connections " <<
    NumberOfScalars << " Scalars");

  this->NumberOfPolygons = NumberOfPolygons;
  this->NumberOfPoints = NumberOfPoints;
  this->NumberOfConnections = NumberOfConnections;
  this->NumberOfPointScalars = NumberOfPointScalars;
  this->NumberOfCellScalars = NumberOfCellScalars;

  this->Mkdir("/Geometry");
  this->Cd("/Geometry");
  Counter = 0;

  // Collect And Write Points
  this->SetRank( 2 );
  this->SetDimension( 0, NumberOfPoints / 3 );
  this->SetDimension( 1, 3 );
  this->SetSlabToAll();
  this->SetTypeFromArray( Points );
  this->SetPath( "Points" );
  this->CreateDataset();
  
  Counter = 0;
  Mem.SetArray( Points );
  this->SetStart( 0, Counter );
  this->SetCount( 0, Points->GetNumberOfElements() / 3 );
  this->SetSlab();
  this->Write( &Mem );
  Counter += ( Points->GetNumberOfElements() / 3 );

  for( i = 1 ; i < Mpi->GetSize() ; i++ ) {

    XdmfDebug("Expecting " << PolysFromNode.GetValue( i ) <<
      " Polygons from Node " << i );
    if ( PolysFromNode.GetValue( i ) > 0 ){

    Mpi->Recv( Points, i );
    XdmfDebug( " Node " << i << " Sent " << Points->GetNumberOfElements()/ 3  << " Points");
    Mem.SetArray( Points );
    this->SetStart( 0, Counter );
    this->SetCount( 0, Points->GetNumberOfElements() / 3 );
    this->SetSlab();
    this->Write( &Mem );
    Counter += ( Points->GetNumberOfElements() / 3 );
    }
  }
  // Collect And Write Connections
  this->SetRank( 1 );
  this->SetDimension( 0, NumberOfConnections );
  this->SetSlabToAll();
  this->SetTypeFromArray( Connections );
  this->SetPath( "Connections" );
  this->CreateDataset();
  
  Counter = 0;
  Mem.SetArray( Connections );
  this->SetStart( 0, Counter );
  this->SetCount( 0, Connections->GetNumberOfElements() );
  this->SetSlab();
  this->Write( &Mem );
  Counter += Connections->GetNumberOfElements();
  for( i = 1 ; i < Mpi->GetSize() ; i++ ) {
    XdmfInt64  j, Offset;
    if ( PolysFromNode.GetValue( i ) > 0 ){

    Mpi->Recv( Connections, i );
    Offset = PointOffset.GetValue( i );
    XdmfDebug( " Node " << i << " Sent " <<
      Connections->GetNumberOfElements() << " Connections " <<
      "Offset = " << Offset );
    for( j = 0 ; j < Connections->GetNumberOfElements() ; j++ ){
      Connections->SetValueFromInt64( j,
        Offset + Connections->GetValueAsInt64( j ) );
      }
    Mem.SetArray( Connections );
    this->SetStart( 0, Counter );
    this->SetCount( 0, Connections->GetNumberOfElements() );
    this->SetSlab();
    this->Write( &Mem );
    Counter += Connections->GetNumberOfElements();
    }
  }
  // Collect And Write Scalars
  this->SetRank( 1 );
  this->SetDimension( 0, NumberOfScalars );
  this->SetSlabToAll();
  this->SetTypeFromArray( Scalars );
  this->SetPath( "Scalars" );
  this->CreateDataset();
  
  Counter = 0;
  Mem.SetArray( Scalars );
  this->SetStart( 0, Counter );
  this->SetCount( 0, Scalars->GetNumberOfElements() );
  this->SetSlab();
  this->Write( &Mem );
  Counter += Scalars->GetNumberOfElements();
  for( i = 1 ; i < Mpi->GetSize() ; i++ ) {

    if ( PolysFromNode.GetValue( i ) > 0 ){
    Mpi->Recv( Scalars, i );
    XdmfDebug( " Node " << i << " Sent " << Scalars->GetNumberOfElements() << " Scalars");
    Mem.SetArray( Scalars );
    this->SetStart( 0, Counter );
    this->SetCount( 0, Scalars->GetNumberOfElements() );
    this->SetSlab();
    this->Write( &Mem );
    Counter += Scalars->GetNumberOfElements();
    }
    }
  this->Close();
} else {
  XdmfArray  *Points;
  XdmfArray  *Connections;
  XdmfArray  *Scalars;
  XdmfInt32  CellType;
  XdmfInt32  j;

  XdmfDebug( "Node " << Mpi->GetId() << " has " <<
    Polygons->GetNumberOfPolys() << " " << 
    Polygons->GetCell( 0 )->GetClassName() << "s " <<
    " VtkType = " << Polygons->GetCell( 0 )->GetCellType() );

  if( Polygons->GetNumberOfPolys() <= 0 ) {
    InfoArray.SetNumberOfElements( 5 );
    InfoArray.SetValue( 0 , 0);
    InfoArray.SetValue( 1 , 0);
    InfoArray.SetValue( 2 , 0);
    InfoArray.SetValue( 3 , 0);
    InfoArray.SetValue( 4 , 0);
    Mpi->Send( &InfoArray, 0 );
    return( XDMF_SUCCESS );
  }

  switch ( Polygons->GetCell( 0 )->GetCellType() ) {
    case VTK_TRIANGLE :
      CellType = xdmfTriangle;
      break;
    case VTK_QUAD :
      CellType = xdmfQuad;
      break;
    default :
      XdmfErrorMessage("Can't send vtk cell type " << Polygons->GetCell( 0 )->GetCellType() );
      break;
    }
  Points = XdmfConvertVtkPolyData( Polygons, "Points" );
  XdmfDebug( "Sending " << Points->GetNumberOfElements() / 3 << " Points" );
  Connections = XdmfConvertVtkPolyData( Polygons, "Connections" );
  XdmfDebug( "Sending " << Connections->GetNumberOfElements() << " Connections") ;
  Scalars = XdmfConvertVtkPolyData( Polygons, "PointScalars" );
  XdmfDebug( "Sending " << Scalars->GetNumberOfElements() << " PointScalars");
  InfoArray.SetNumberOfElements( 5 );
  InfoArray.SetValue( 0 , CellType );
  InfoArray.SetValue( 1 , Polygons->GetNumberOfPolys() );
  InfoArray.SetValue( 2 , Points->GetNumberOfElements() );
  InfoArray.SetValue( 3 , Connections->GetNumberOfElements() );
  InfoArray.SetValue( 4 , Scalars->GetNumberOfElements() );
  Mpi->Send( &InfoArray, 0 );
  Mpi->Send( Points, 0 );
  Mpi->Send( Connections, 0 );
  Mpi->Send( Scalars, 0 );
  }
return( XDMF_SUCCESS );
}

void
*XdmfGetVtkAddress( VtkScriptObject VtkObject ) {

return(( void *)VtkObject);
}

void
*XdmfCopyArrayToVtkData( XdmfArray *Array, VtkScriptObject VtkObject ) {

vtkDataArray  *VtkArray = ( vtkDataArray *)VtkObject;
void    *VtkPointer = VtkArray->GetVoidPointer( 0 );
XdmfInt64  Length;

Length = MIN( Array->GetNumberOfElements(), VtkArray->GetSize() );

memcpy( VtkPointer,
  Array->GetVoidPointer(),
  Length * Array->GetPrecision() );


return(( void *)VtkPointer);
}

void
*XdmfCopyVtkDataToArray( VtkScriptObject VtkObject, XdmfArray *Array ) {

vtkDataArray  *VtkArray = ( vtkDataArray *)VtkObject;
void    *VtkPointer = VtkArray->GetVoidPointer( 0 );
XdmfInt64  Length;

// Length = MIN( Array->GetNumberOfElements(), VtkArray->GetSize() );
Length = VtkArray->GetSize();
Array->SetNumberOfElements( Length );

memcpy( Array->GetVoidPointer(),
  VtkPointer,
  Length * Array->GetPrecision() );


return(( void *)VtkPointer);
}

VtkScriptObject
XdmfVtkAddressToCommand( void *VtkObject ) {

return(( VtkScriptObject )VtkObject);
}

XdmfArray *
XdmfConvertVtkPolyData ( VtkScriptObject VtkObject , XdmfString What ) {

vtkPolyData *PolyData = ( vtkPolyData *)VtkObject;
if( strcmp( PolyData->GetClassName(), "vtkPolyData" ) != 0 ){
  XdmfErrorMessage("Object is not vtkPolyData");
  return( NULL );
  }

if( XDMF_WORD_CMP( What, "Points" ) ) {
  XdmfInt64    NumberOfPoints;
  vtkPoints    *Points;
  XdmfFloat32Array  *xyz = new XdmfFloat32Array;
  float      *fp;

  
  NumberOfPoints = PolyData->GetNumberOfPoints();
  Points = PolyData->GetPoints();

  xyz->SetNumberOfElements( NumberOfPoints * 3 );
  fp = ( float *)Points->GetPoint( 0 );
  memcpy( xyz->GetVoidPointer(), fp, NumberOfPoints * 3 * sizeof( float ) );
  return( xyz );
}

if( XDMF_WORD_CMP( What, "Connections" ) ) {
  XdmfInt64  i, j, k, Size, AdjustedSize;
  vtkCellArray  *Cells;
  int    *Cp;
  XdmfInt32Array  *Connections = new XdmfInt32Array;

  Cells = PolyData->GetPolys();
  Size = Cells->GetSize();
  Cp = Cells->GetPointer();
  Connections->SetNumberOfElements( Size );
  i = AdjustedSize = 0;
  while( i < Size ) {
    j = *Cp++;
    for( k = 0 ; k < j ; k++ ){
      Connections->SetValueFromInt64( AdjustedSize++, *Cp++ );
      }
    i += ( j + 1 );
    }
  Connections->SetNumberOfElements( AdjustedSize );
  return( Connections );
}

if( XDMF_WORD_CMP( What, "CellScalars" ) || XDMF_WORD_CMP( What, "PointScalars" ) ) {
  vtkDataArray *Scalars;
  XdmfArray  *ScalarArray;
  XdmfInt64  Size, cntr;

  if( XDMF_WORD_CMP( What, "CellScalars" ) ) {
    Scalars = PolyData->GetCellData()->GetScalars();
  } else {
    Scalars = PolyData->GetPointData()->GetScalars();
  }
  if( Scalars == NULL ) {
    return( NULL );  
  }
  switch( Scalars->GetDataType() ) {
    case VTK_FLOAT :
      ScalarArray = new XdmfFloat32Array;
      break;
    case VTK_DOUBLE :
      ScalarArray = new XdmfFloat64Array;
      break;
    default :
      ScalarArray = new XdmfInt32Array;
      break;
    }
  Size = Scalars->GetNumberOfTuples();
  ScalarArray->SetNumberOfElements( Size );
  for( cntr = 0 ; cntr < Size ; cntr++ ){
    ScalarArray->SetValueFromFloat64( cntr, Scalars->GetTuple1( cntr ) );
    }
  return( ScalarArray );
}

return( NULL );
}

