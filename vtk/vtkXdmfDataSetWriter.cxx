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
#include <vtkXdmfDataSetWriter.h>
#include <vtkImageData.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkRectilinearGrid.h>

#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCell.h>
#include <vtkCellTypes.h>

#include <vtkObjectFactory.h>
#include <vtkCommand.h>

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkPointData.h"


#include <XdmfHDF.h>
#include <XdmfArray.h>

class vtkXdmfDataSetWriterInternals
{
public:
  ostrstream *XMLStream;
};

//----------------------------------------------------------------------------
vtkXdmfDataSetWriter* vtkXdmfDataSetWriter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkXdmfDataSetWriter");
  if(ret)
    {
    return (vtkXdmfDataSetWriter*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkXdmfDataSetWriter;
}

vtkXdmfDataSetWriter::vtkXdmfDataSetWriter()
{
  strcpy( this->HeavyDataSetName, "XdmfData.h5" );
  this->FastWrite = 1;
  this->AllLight = 0;
  this->Internals = new vtkXdmfDataSetWriterInternals;
  this->Internals->XMLStream = new ostrstream();
}

void vtkXdmfDataSetWriter::ResetXML( void ) {
  delete this->Internals->XMLStream;
  this->Internals->XMLStream = new ostrstream();
  }

char *vtkXdmfDataSetWriter::GetXML( void ) {
  char  *String, *ptr;
  
  ptr = this->Internals->XMLStream->str();
  String = new char[ strlen( ptr ) + 1 ];
  strcpy( String, ptr );
  this->Internals->XMLStream->rdbuf()->freeze( 0 );
  return( String );
  }
int
vtkXdmfDataSetWriter::WriteHead( void ){
}

int
vtkXdmfDataSetWriter::WriteTail( void ){
}

int
vtkXdmfDataSetWriter::WriteCellArray( vtkCellArray *Cells ){

vtkIdType  *Cp;
int    NumberOfCells;
int    PointsInPoly;
int    i, j;

Cp = Cells->GetPointer();
NumberOfCells = Cells->GetNumberOfCells();
PointsInPoly = *Cp;
*this->Internals->XMLStream << "\t<DataStructure" << endl;
*this->Internals->XMLStream << "\t\tDataType=\"Int\"" << endl;
*this->Internals->XMLStream << "\t\tDimensions=\"" << NumberOfCells << " " << PointsInPoly << "\"" << endl;
if( this->AllLight ){
  *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
  for( i = 0 ; i < NumberOfCells ; i++ ){
    PointsInPoly = *Cp++;
    *this->Internals->XMLStream << "\t\t";
    for( j = 0 ; j < PointsInPoly ; j++ ){
      *this->Internals->XMLStream << *Cp++ << " ";
      }
    *this->Internals->XMLStream << endl;
    }
} else {
  // Create HDF File
  char    DataSetName[256];
  XdmfArray  Conns;
  XdmfHDF    H5;
  XdmfInt64  Dims[2];
  XdmfInt32  *Dp;

  sprintf(DataSetName, "%s:/Connections" , this->HeavyDataSetName);
  *this->Internals->XMLStream << "\t\tFormat=\"HDF\">" << endl;
  *this->Internals->XMLStream << "\t\t" << DataSetName << endl;
  Conns.SetNumberType( XDMF_INT32_TYPE );
  Dims[0] = NumberOfCells;
  Dims[1] = PointsInPoly;
  Conns.SetShape( 2, Dims );
  Dp = (XdmfInt32 *)Conns.GetDataPointer();
  for( i = 0 ; i < NumberOfCells ; i++ ){
    PointsInPoly = *Cp++;
    for( j = 0 ; j < PointsInPoly ; j++ ){
      *Dp++ = *Cp++;
      }
    }
  H5.CopyType( &Conns );
  H5.CopyShape( &Conns );
  if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL ){
    if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) {
      cerr << "Can't Create Heavy Dataset " << DataSetName << endl;
      return( -1 );
      }
  }
  H5.Write( &Conns );
  H5.Close();
  
  
}
*this->Internals->XMLStream << "</DataStructure>" << endl;
return( NumberOfCells );
}

int
vtkXdmfDataSetWriter::WritePoints( vtkPoints *Points ){
int  NumberOfPoints;
int  i;
float  *Pp;

NumberOfPoints = Points->GetNumberOfPoints();
Pp = Points->GetPoint(0);
cerr << NumberOfPoints << " Points: " << endl;
*this->Internals->XMLStream << "\t<DataStructure" << endl;
*this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
*this->Internals->XMLStream << "\t\tDimensions=\"" << NumberOfPoints << " 3\"" << endl;
if( this->AllLight ){
  *this->Internals->XMLStream << "\tFormat=\"XML\">" << endl;
  for( i = 0 ; i < NumberOfPoints ; i++ ){
    *this->Internals->XMLStream << "\t\t" << *Pp++ << " ";
    *this->Internals->XMLStream << *Pp++ << " ";
    *this->Internals->XMLStream << *Pp++ << endl;
    }
} else {
  // Create HDF File
  char    DataSetName[256];
  XdmfArray  Geo;
  XdmfHDF    H5;
  XdmfInt64  Dims[2];
  XdmfFloat64  *Dp;

  sprintf(DataSetName, "%s:/XYZ" , this->HeavyDataSetName);
  *this->Internals->XMLStream << "\t\tFormat=\"HDF\">" << endl;
  *this->Internals->XMLStream << "\t\t" << DataSetName << endl;
  Geo.SetNumberType( XDMF_FLOAT64_TYPE );
  Dims[0] = NumberOfPoints;
  Dims[1] = 3;
  Geo.SetShape( 2, Dims );
  Dp = (XdmfFloat64 *)Geo.GetDataPointer();
  for( i = 0 ; i < NumberOfPoints * 3 ; i++ ){
      *Dp++ = *Pp++;
    }
  H5.CopyType( &Geo );
  H5.CopyShape( &Geo );
  if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL ){
    if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) {
      cerr << "Can't Create Heavy Dataset " << DataSetName << endl;
      return( -1 );
      }
  }
  H5.Write( &Geo );
  H5.Close();
}
*this->Internals->XMLStream << "\t</DataStructure>" << endl;
return( NumberOfPoints );
}

void vtkXdmfDataSetWriter::StartTopology( int Type, vtkCellArray *Cells ){
  vtkIdType *Cp;

  Cp = Cells->GetPointer();
  *this->Internals->XMLStream << "<Topology " << endl;
  switch( Type ) {
    case VTK_EMPTY_CELL :
      cerr << "Start Empty Cell" << endl;
    case VTK_VERTEX :
      cerr << "Start " <<  " VERTEX" << endl;
      *this->Internals->XMLStream << "\tType=\"POLYVERTEX\"" << endl;
      break;
    case VTK_POLY_VERTEX :
      cerr << "Start " <<  " POLY_VERTEX" << endl;
      *this->Internals->XMLStream << "\tType=\"POLYVERTEX\"" << endl;
      break;
    case VTK_LINE :
      cerr << "Start " <<  " LINE" << endl;
      *this->Internals->XMLStream << "\tType=\"POLYLINE\"" << endl;
      *this->Internals->XMLStream << "\tNodesPerElement=\"" << *Cp << "\"" << endl;
      break;
    case VTK_POLY_LINE :
      cerr << "Start " <<  " POLY_LINE" << endl;
      *this->Internals->XMLStream << "\tType=\"POLYLINE\"" << endl;
      *this->Internals->XMLStream << "\tNodesPerElement=\"" << *Cp << "\"" << endl;
      break;
    case VTK_TRIANGLE :
      cerr << "Start " <<  " TRIANGLE" << endl;
      *this->Internals->XMLStream << "\tType=\"TRIANGLE\"" << endl;
      break;
    case VTK_TRIANGLE_STRIP :
      cerr << "Start " <<  " TRIANGLE_STRIP" << endl;
      *this->Internals->XMLStream << "\tType=\"TRIANGLE\"" << endl;
      break;
    case VTK_POLYGON :
      cerr << "Start " <<  " POLYGON" << endl;
      *this->Internals->XMLStream << "\tType=\"POLYGON\"" << endl;
      *this->Internals->XMLStream << "\tNodesPerElement=\"" << *Cp << "\"" << endl;
      break;
    case VTK_PIXEL :
      cerr << "Start " <<  " PIXEL" << endl;
      *this->Internals->XMLStream << "\tType=\"QUADRILATERAL\"" << endl;
      break;
    case VTK_QUAD :
      cerr << "Start " <<  " QUAD" << endl;
      *this->Internals->XMLStream << "\tType=\"QUADRILATERAL\"" << endl;
      break;
    case VTK_TETRA :
      cerr << "Start " <<  " TETRA" << endl;
      *this->Internals->XMLStream << "\tType=\"TETRAHEDRON\"" << endl;
      break;
    case VTK_VOXEL :
      cerr << "Start " <<  " VOXEL" << endl;
      *this->Internals->XMLStream << "\tType=\"HEXAHEDRON\"" << endl;
      break;
    case VTK_HEXAHEDRON :
      cerr << "Start " <<  " HEXAHEDRON" << endl;
      *this->Internals->XMLStream << "\tType=\"HEXAHEDRON\"" << endl;
      break;
    case VTK_WEDGE :
      cerr << "Start " <<  " WEDGE" << endl;
      *this->Internals->XMLStream << "\tType=\"WEDGE\"" << endl;
      break;
    case VTK_PYRAMID :
      cerr << "Start " <<  " PYRAMID" << endl;
      *this->Internals->XMLStream << "\tType=\"PYRAMID\"" << endl;
      break;
    default :
      cerr << "Unknown Topology Type" << endl;
      break;
    }
  *this->Internals->XMLStream << "\tDimensions=\"" << Cells->GetNumberOfCells() << "\">" << endl;
}

int
vtkXdmfDataSetWriter::WriteScalar( vtkDataArray *Scalars, char *Name, char *Center ) {

  int i, j;

  *this->Internals->XMLStream << "<Attribute Center=\"" <<
      Center << "\"" <<
      " Name=\"" << Name << "\">" << endl;
  *this->Internals->XMLStream << "\t<DataStructure" << endl;
  *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
  *this->Internals->XMLStream << "\t\tDimensions=\"" << 
    Scalars->GetNumberOfTuples() << "\"" << endl;
  if( this->AllLight ){
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t";
    j = 0;
    for( i = 0 ; i < Scalars->GetNumberOfTuples() ; i++ ){
      if( j >= 10 ){
        *this->Internals->XMLStream << endl << "\t\t";
        j = 0;
        }
      *this->Internals->XMLStream << Scalars->GetTuple1( i ) << " ";
      j++;
    }
  } else {
    // Create HDF File
    char    DataSetName[256];
    XdmfArray  Data;
    XdmfHDF    H5;
    XdmfFloat32  *Dp;

    sprintf(DataSetName, "%s:/%s" , this->HeavyDataSetName, Name);
    *this->Internals->XMLStream << "\t\tFormat=\"HDF\">" << endl;
    *this->Internals->XMLStream << "\t\t" << DataSetName << endl;
    Data.SetNumberType( XDMF_FLOAT32_TYPE );
    Data.SetNumberOfElements( Scalars->GetNumberOfTuples() );
    Dp = (XdmfFloat32 *)Data.GetDataPointer();
    for( j = 0 ; j < Scalars->GetNumberOfTuples(); j++ ){
      *Dp++ = Scalars->GetTuple1(j);
      }
    H5.CopyType( &Data);
    H5.CopyShape( &Data);
    if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL ){
      if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) {
        cerr << "Can't Create Heavy Dataset " <<
          DataSetName << endl;
        return( -1 );
        }
    }
    H5.Write( &Data );
    H5.Close();
  }
  *this->Internals->XMLStream << "</DataStructure>" << endl;
  *this->Internals->XMLStream << "</Attribute>" << endl;
  return( Scalars->GetNumberOfTuples() );
}

int
vtkXdmfDataSetWriter::WriteVector( vtkDataArray *Vectors, char *Name, char *Center ) {

  int i, j;
  double  VectorData[3];

  *this->Internals->XMLStream << "<Attribute Center=\"" <<
      Center << "\"" <<
      " Name=\"" << Name << "\">" << endl;
  *this->Internals->XMLStream << "\t<DataStructure" << endl;
  *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
  *this->Internals->XMLStream << "\t\tDimensions=\"" << 
    Vectors->GetNumberOfTuples() * 3 << "\"" << endl;
  if( this->AllLight ){
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t";
    j = 0;
    for( i = 0 ; i < Vectors->GetNumberOfTuples() ; i++ ){
      Vectors->GetTuple( i, VectorData );
      if( j >= 2 ){
        *this->Internals->XMLStream << endl << "\t\t";
        j = 0;
        }
      *this->Internals->XMLStream <<
        VectorData[0] << " " <<
        VectorData[1] << " " <<
        VectorData[2] << "      ";
      j++;
    }
  } else {
    // Create HDF File
    char    DataSetName[256];
    XdmfArray  Data;
    XdmfHDF    H5;
    XdmfFloat32  *Dp;

    sprintf(DataSetName, "%s:/%s" , this->HeavyDataSetName, Name);
    *this->Internals->XMLStream << "\t\tFormat=\"HDF\">" << endl;
    *this->Internals->XMLStream << "\t\t" << DataSetName << endl;
    Data.SetNumberType( XDMF_FLOAT32_TYPE );
    Data.SetNumberOfElements( Vectors->GetNumberOfTuples() * 3  );
    Dp = (XdmfFloat32 *)Data.GetDataPointer();
    for( j = 0 ; j < Vectors->GetNumberOfTuples(); j++ ){
      Vectors->GetTuple( i, VectorData );
      *Dp++ = VectorData[0];
      *Dp++ = VectorData[1];
      *Dp++ = VectorData[2];
      }
    H5.CopyType( &Data);
    H5.CopyShape( &Data);
    if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL ){
      if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) {
        cerr << "Can't Create Heavy Dataset " <<
          DataSetName << endl;
        return( -1 );
        }
    }
    H5.Write( &Data );
    H5.Close();
  }
  *this->Internals->XMLStream << "</DataStructure>" << endl;
  *this->Internals->XMLStream << "</Attribute>" << endl;
  return( Vectors->GetNumberOfTuples() );
}

void vtkXdmfDataSetWriter::WriteAttributes( void ){
  vtkDataSet *DataSet = this->Input;
  vtkCellData *CellData = DataSet->GetCellData();
  vtkPointData *PointData = DataSet->GetPointData();

  if( CellData ){
    int     i, j ;
    vtkDataArray *Scalars = CellData->GetScalars();
    vtkDataArray *Vectors= CellData->GetVectors();
    if( Scalars ){
      this->WriteScalar( Scalars, "CellScalars", "Cell" );  
    }
    if( Vectors ){
      this->WriteVector( Vectors, "CellVectors", "Cell" );  
    }
  }
  if( PointData ){
    int     i, j ;
    vtkDataArray *Scalars = PointData->GetScalars();
    vtkDataArray *Vectors= PointData->GetVectors();
    if( Scalars ){
      this->WriteScalar( Scalars, "NodeScalars", "Node" );  
    }
    if( Vectors ){
      this->WriteVector( Vectors, "NodeVectors", "Node" );  
    }
  }
}

int vtkXdmfDataSetWriter::WriteGrid( void )
{
  vtkDataSet *DataSet = this->Input;
  int type; 

  if( !DataSet ) {
    cerr << "No Input Data Set" << endl;
    return( -1 );
    }
  type = DataSet->GetDataObjectType();
  if ( type == VTK_POLY_DATA )
  {
    vtkPolyData *Polys = ( vtkPolyData *)DataSet;
    this->StartTopology( Polys->GetCell(0)->GetCellType(), Polys->GetPolys());
    this->WriteCellArray( Polys->GetPolys());
    *this->Internals->XMLStream << "</Topology>" << endl;
    
    *this->Internals->XMLStream << "<Geometry Type=\"XYZ\">" << endl;
    this->WritePoints( Polys->GetPoints());
    *this->Internals->XMLStream << "</Geometry>" << endl;
  }
  else if ( type == VTK_STRUCTURED_POINTS || type == VTK_IMAGE_DATA)
  {
    int     Dims[3];
    float    Origin[3], Spacing[3];
    vtkImageData *SGrid = ( vtkImageData *)DataSet;
    SGrid->GetDimensions( Dims );
    SGrid->GetOrigin( Origin );
    SGrid->GetSpacing( Spacing );
    *this->Internals->XMLStream << "<Topology Type=\"3DCORECTMESH\"" << endl;
    *this->Internals->XMLStream << "\tDimensions=\"" << 
      Dims[2] << " " <<
      Dims[1] << " " <<
      Dims[0] <<
      "\"/>" << endl;
    *this->Internals->XMLStream << "<Geometry Type=\"ORIGIN_DXDYDZ\">" << endl;
    // Origin
    *this->Internals->XMLStream << "\t<DataStructure" << endl;
    *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
    *this->Internals->XMLStream << "\t\tDimensions=\"3\"" << endl;
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t" <<
      Origin[0] <<
      " " << Origin[1] <<
      " " << Origin[2] << endl;
    *this->Internals->XMLStream << "\t</DataStructure>" << endl;
    // DX DY DZ
    *this->Internals->XMLStream << "\t<DataStructure" << endl;
    *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
    *this->Internals->XMLStream << "\t\tDimensions=\"3\"" << endl;
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t" <<
      Spacing[0] <<
      " " << Spacing[1] <<
      " " << Spacing[2] << endl;
    *this->Internals->XMLStream << "\t</DataStructure>" << endl;


    *this->Internals->XMLStream << "</Geometry>" << endl;
  }
  else if ( type == VTK_STRUCTURED_GRID )
  {
    int     Dims[3];
    vtkStructuredGrid *SGrid = ( vtkStructuredGrid *)DataSet;
    SGrid->GetDimensions( Dims );
    *this->Internals->XMLStream << "<Topology Type=\"3DSMESH\"" << endl;
    *this->Internals->XMLStream << "\tDimensions=\"" << 
      Dims[2] << " " <<
      Dims[1] << " " <<
      Dims[0] <<
      "\"/>" << endl;
    *this->Internals->XMLStream << "<Geometry Type=\"XYZ\">" << endl;
    this->WritePoints( SGrid->GetPoints());
    *this->Internals->XMLStream << "</Geometry>" << endl;
  }
  else if ( type == VTK_UNSTRUCTURED_GRID )
  {
    vtkUnstructuredGrid *UGrid = ( vtkUnstructuredGrid *)DataSet;
    this->StartTopology( UGrid->GetCell(0)->GetCellType(), UGrid->GetCells());
    this->WriteCellArray( UGrid->GetCells());
    *this->Internals->XMLStream << "</Topology>" << endl;
    
    *this->Internals->XMLStream << "<Geometry Type=\"XYZ\">" << endl;
    this->WritePoints( UGrid->GetPoints());
    *this->Internals->XMLStream << "</Geometry>" << endl;
  }
  else if ( type == VTK_RECTILINEAR_GRID )
  {
    int    i, j;
    int     Dims[3], NumberOfPoints;
    vtkDataArray  *Coord;
    vtkRectilinearGrid *RGrid = ( vtkRectilinearGrid *)DataSet;
    RGrid->GetDimensions( Dims );
    *this->Internals->XMLStream << "<Topology Type=\"3DRECTMESH\"" << endl;
    *this->Internals->XMLStream << "\tDimensions=\"" << 
      Dims[2] << " " <<
      Dims[1] << " " <<
      Dims[0] <<
      "\"/>" << endl;
    *this->Internals->XMLStream << "<Geometry Type=\"VXVYVZ\">" << endl;
    // X Coordinated
    Coord = RGrid->GetXCoordinates();
    NumberOfPoints = Coord->GetNumberOfTuples();
    *this->Internals->XMLStream << "\t<DataStructure" << endl;
    *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
    *this->Internals->XMLStream << "\t\tDimensions=\"" << NumberOfPoints << "\"" << endl;
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t";
    j = 0;
    for( i = 0 ; i < NumberOfPoints ; i++ ){
      if( j >= 10 ){
        *this->Internals->XMLStream << endl << "\t\t";
        j = 0;
        }
      *this->Internals->XMLStream << *Coord->GetTuple( i ) << " ";
      j++;
      }
    *this->Internals->XMLStream << endl;
    *this->Internals->XMLStream << "</DataStructure>" << endl;
    // Y Coordinated
    Coord = RGrid->GetYCoordinates();
    NumberOfPoints = Coord->GetNumberOfTuples();
    *this->Internals->XMLStream << "\t<DataStructure" << endl;
    *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
    *this->Internals->XMLStream << "\t\tDimensions=\"" << NumberOfPoints << "\"" << endl;
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t";
    j = 0;
    for( i = 0 ; i < NumberOfPoints ; i++ ){
      if( j >= 10 ){
        *this->Internals->XMLStream << endl << "\t\t";
        j = 0;
        }
      *this->Internals->XMLStream << *Coord->GetTuple( i ) << " ";
      j++;
      }
    *this->Internals->XMLStream << endl;
    *this->Internals->XMLStream << "</DataStructure>" << endl;
    // Z Coordinated
    Coord = RGrid->GetZCoordinates();
    NumberOfPoints = Coord->GetNumberOfTuples();
    *this->Internals->XMLStream << "\t<DataStructure" << endl;
    *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
    *this->Internals->XMLStream << "\t\tDimensions=\"" << NumberOfPoints << "\"" << endl;
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t";
    j = 0;
    for( i = 0 ; i < NumberOfPoints ; i++ ){
      if( j >= 10 ){
        *this->Internals->XMLStream << endl << "\t\t";
        j = 0;
        }
      *this->Internals->XMLStream << *Coord->GetTuple( i ) << " ";
      j++;
      }
    *this->Internals->XMLStream << endl;
    *this->Internals->XMLStream << "</DataStructure>" << endl;
    *this->Internals->XMLStream << "</Geometry>" << endl;
  }

  return( 1 );
}

