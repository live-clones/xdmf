/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXdmfWriter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 1993-2001 Ken Martin, Will Schroeder, Bill Lorensen  
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkXdmfWriter.h"

#include "vtkCell.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkCellTypes.h"
#include "vtkCommand.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkRectilinearGrid.h"
#include "vtkStructuredGrid.h"
#include "vtkUnstructuredGrid.h"

#include "XdmfHDF.h"
#include "XdmfArray.h"

//----------------------------------------------------------------------------
//============================================================================
class vtkXdmfWriterInternals
{
public:
  ostrstream *XMLStream;
};
//============================================================================
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkXdmfWriter);
vtkCxxRevisionMacro(vtkXdmfWriter, "1.2");

//----------------------------------------------------------------------------
vtkXdmfWriter::vtkXdmfWriter()
{
  this->FileName = 0;
  this->HeavyDataSetName = 0;
  this->GridName = 0;
  this->SetHeavyDataSetName( "XdmfData.h5" );
  this->SetGridName( "Unnamed" );

  this->AllLight = 0;

  this->Internals = new vtkXdmfWriterInternals;
  this->Internals->XMLStream = 0;
  this->ResetXML();

}

//----------------------------------------------------------------------------
vtkXdmfWriter::~vtkXdmfWriter()
{
  this->SetHeavyDataSetName(0);
  this->SetFileName(0);
  delete this->Internals->XMLStream;
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::SetHeavyDataSetName( const char *name) 
{
  if ( this->HeavyDataSetName == name )
    {
    return;
    }
  if ( this->HeavyDataSetName && name && strcmp(this->HeavyDataSetName, name) == 0 )
    {
    return;
    }
  if ( this->HeavyDataSetName )
    {
    delete [] this->HeavyDataSetName;
    this->HeavyDataSetName = 0;
    }
  if( name ) 
    {
    this->HeavyDataSetName = new char [ strlen(name) + 1 ];
    strcpy( this->HeavyDataSetName, name);
    this->AllLight = 0;
    }
  else
    {
    this->AllLight = 1;
    }
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::ResetXML( void ) 
{
  if ( this->Internals->XMLStream )
    {
    delete this->Internals->XMLStream;
    this->Internals->XMLStream = 0;
    }
  this->Internals->XMLStream = new ostrstream();
}

//----------------------------------------------------------------------------
char *vtkXdmfWriter::GetXML( void ) 
{
  char  *String, *ptr;

  *this->Internals->XMLStream << ends;
  ptr = this->Internals->XMLStream->str();
  if ( ptr ){
    String = new char[ strlen( ptr ) + 1 ];
    strcpy( String, ptr );
  }
  else {
    String = 0;
  }
  this->Internals->XMLStream->rdbuf()->freeze( 0 );
  return( String );
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteHead( void )
{
  return 0;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteTail( void )
{
  return 0;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteCellArray( vtkCellArray *Cells )
{

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
  if( this->AllLight )
    {
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    for( i = 0 ; i < NumberOfCells ; i++ )
      {
      PointsInPoly = *Cp++;
      *this->Internals->XMLStream << "\t\t";
      for( j = 0 ; j < PointsInPoly ; j++ )
        {
        *this->Internals->XMLStream << *Cp++ << " ";
        }
      *this->Internals->XMLStream << endl;
      }
    } 
  else 
    {
    // Create HDF File
    char    DataSetName[256];
    XdmfArray  Conns;
    XdmfHDF    H5;
    XdmfInt64  Dims[2];
    XdmfInt32  *Dp;

    sprintf(DataSetName, "%s:/Connections" , this->HeavyDataSetName);
    cout << "DataSetName: " << DataSetName << endl;
    *this->Internals->XMLStream << "\t\tFormat=\"HDF\">" << endl;
    *this->Internals->XMLStream << "\t\t" << DataSetName << endl;
    Conns.SetNumberType( XDMF_INT32_TYPE );
    Dims[0] = NumberOfCells;
    Dims[1] = PointsInPoly;
    Conns.SetShape( 2, Dims );
    Dp = (XdmfInt32 *)Conns.GetDataPointer();
    for( i = 0 ; i < NumberOfCells ; i++ )
      {
      PointsInPoly = *Cp++;
      for( j = 0 ; j < PointsInPoly ; j++ )
        {
        *Dp++ = *Cp++;
        }
      }
    H5.CopyType( &Conns );
    H5.CopyShape( &Conns );
    if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL )
      {
      if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) 
        {
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

//----------------------------------------------------------------------------
int vtkXdmfWriter::WritePoints( vtkPoints *Points )
{
  int  NumberOfPoints;
  int  i;
  float  *Pp;

  NumberOfPoints = Points->GetNumberOfPoints();
  Pp = Points->GetPoint(0);
  cerr << NumberOfPoints << " Points: " << endl;
  *this->Internals->XMLStream << "\t<DataStructure" << endl;
  *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
  *this->Internals->XMLStream << "\t\tDimensions=\"" << NumberOfPoints << " 3\"" << endl;
  if( this->AllLight )
    {
    *this->Internals->XMLStream << "\tFormat=\"XML\">" << endl;
    for( i = 0 ; i < NumberOfPoints ; i++ )
      {
      *this->Internals->XMLStream << "\t\t" << *Pp++ << " ";
      *this->Internals->XMLStream << *Pp++ << " ";
      *this->Internals->XMLStream << *Pp++ << endl;
      }
    } 
  else 
    {
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
    for( i = 0 ; i < NumberOfPoints * 3 ; i++ )
      {
      *Dp++ = *Pp++;
      }
    H5.CopyType( &Geo );
    H5.CopyShape( &Geo );
    if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL )
      {
      if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) 
        {
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

//----------------------------------------------------------------------------
void vtkXdmfWriter::StartTopology( int Type, vtkCellArray *Cells )
{
  vtkIdType *Cp;

  Cp = Cells->GetPointer();
  *this->Internals->XMLStream << "<Topology " << endl;
  switch( Type ) 
    {
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

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteScalar( vtkDataArray *Scalars, const char *Name, const char *Center ) 
{
  int i, j;
  const char* arrayName = Name;
  if ( Scalars->GetName() )
    {
    arrayName = Scalars->GetName();
    }

  *this->Internals->XMLStream << "<Attribute Center=\"" <<
    Center << "\"" <<
    " Name=\"" << arrayName << "\">" << endl;
  *this->Internals->XMLStream << "\t<DataStructure" << endl;
  *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
  *this->Internals->XMLStream << "\t\tDimensions=\"" << 
    Scalars->GetNumberOfTuples() << "\"" << endl;
  if( this->AllLight )
    {
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t";
    j = 0;
    for( i = 0 ; i < Scalars->GetNumberOfTuples() ; i++ )
      {
      if( j >= 10 )
        {
        *this->Internals->XMLStream << endl << "\t\t";
        j = 0;
        }
      *this->Internals->XMLStream << Scalars->GetTuple1( i ) << " ";
      j++;
      }
    } 
  else 
    {
    // Create HDF File
    char    DataSetName[256];
    XdmfArray  Data;
    XdmfHDF    H5;
    XdmfFloat32  *Dp;

    sprintf(DataSetName, "%s:/%s" , this->HeavyDataSetName, arrayName);
    *this->Internals->XMLStream << "\t\tFormat=\"HDF\">" << endl;
    *this->Internals->XMLStream << "\t\t" << DataSetName << endl;
    Data.SetNumberType( XDMF_FLOAT32_TYPE );
    Data.SetNumberOfElements( Scalars->GetNumberOfTuples() );
    Dp = (XdmfFloat32 *)Data.GetDataPointer();
    for( j = 0 ; j < Scalars->GetNumberOfTuples(); j++ )
      {
      *Dp++ = Scalars->GetTuple1(j);
      }
    H5.CopyType( &Data);
    H5.CopyShape( &Data);
    if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL )
      {
      if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) 
        {
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

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteVector( vtkDataArray *Vectors, const char *Name, const char *Center ) 
{
  int i, j;
  double  VectorData[3];
  const char* arrayName = Name;
  if ( Vectors->GetName() )
    {
    arrayName = Vectors->GetName();
    }

  *this->Internals->XMLStream << "<Attribute Center=\"" <<
    Center << "\"" <<
    " Name=\"" << arrayName << "\">" << endl;
  *this->Internals->XMLStream << "\t<DataStructure" << endl;
  *this->Internals->XMLStream << "\t\tDataType=\"Float\"" << endl;
  *this->Internals->XMLStream << "\t\tDimensions=\"" << 
    Vectors->GetNumberOfTuples() * 3 << "\"" << endl;
  if( this->AllLight )
    {
    *this->Internals->XMLStream << "\t\tFormat=\"XML\">" << endl;
    *this->Internals->XMLStream << "\t\t";
    j = 0;
    for( i = 0 ; i < Vectors->GetNumberOfTuples() ; i++ )
      {
      Vectors->GetTuple( i, VectorData );
      if( j >= 2 )
        {
        *this->Internals->XMLStream << endl << "\t\t";
        j = 0;
        }
      *this->Internals->XMLStream <<
        VectorData[0] << " " <<
        VectorData[1] << " " <<
        VectorData[2] << "      ";
      j++;
      }
    } 
  else 
    {
    // Create HDF File
    char    DataSetName[256];
    XdmfArray  Data;
    XdmfHDF    H5;
    XdmfFloat32  *Dp;

    sprintf(DataSetName, "%s:/%s" , this->HeavyDataSetName, arrayName);
    *this->Internals->XMLStream << "\t\tFormat=\"HDF\">" << endl;
    *this->Internals->XMLStream << "\t\t" << DataSetName << endl;
    Data.SetNumberType( XDMF_FLOAT32_TYPE );
    Data.SetNumberOfElements( Vectors->GetNumberOfTuples() * 3  );
    Dp = (XdmfFloat32 *)Data.GetDataPointer();
    for( j = 0 ; j < Vectors->GetNumberOfTuples(); j++ )
      {
      Vectors->GetTuple( j, VectorData );
      *Dp++ = VectorData[0];
      *Dp++ = VectorData[1];
      *Dp++ = VectorData[2];
      }
    H5.CopyType( &Data);
    H5.CopyShape( &Data);
    if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL )
      {
      if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) 
        {
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

//----------------------------------------------------------------------------
void vtkXdmfWriter::WriteAttributes( void )
{
  vtkDataSet *DataSet = this->GetInputDataSet();
  vtkCellData *CellData = DataSet->GetCellData();
  vtkPointData *PointData = DataSet->GetPointData();
  int cc;

  if( CellData )
    {
    vtkDataArray *Scalars = CellData->GetScalars();
    vtkDataArray *Vectors= CellData->GetVectors();
    if( Scalars )
      {
      this->WriteScalar( Scalars, "CellScalars", "Cell" );  
      }
    if( Vectors )
      {
      this->WriteVector( Vectors, "CellVectors", "Cell" );  
      }
    for ( cc = 0; cc < CellData->GetNumberOfArrays(); cc ++ )
      {
      vtkDataArray* array = CellData->GetArray(cc);
      if ( array != CellData->GetScalars() &&
        array != CellData->GetVectors() )
        {
        if ( array->GetNumberOfComponents() == 3 )
          {
          this->WriteScalar( array, "CellVectors", "Cell");
          }
        else
          {
          this->WriteScalar( array, "CellScalars", "Cell");
          }
        }
      }
    }
  if( PointData )
    {
    vtkDataArray *Scalars = PointData->GetScalars();
    vtkDataArray *Vectors= PointData->GetVectors();
    if( Scalars )
      {
      this->WriteScalar( Scalars, "NodeScalars", "Node" );  
      }
    if( Vectors )
      {
      this->WriteVector( Vectors, "NodeVectors", "Node" );  
      }
    for ( cc = 0; cc < PointData->GetNumberOfArrays(); cc ++ )
      {
      vtkDataArray* array = PointData->GetArray(cc);
      if ( array != PointData->GetScalars() &&
        array != PointData->GetVectors() )
        {
        if ( array->GetNumberOfComponents() == 3 )
          {
          this->WriteScalar( array, "CellVectors", "Node");
          }
        else
          {
          this->WriteScalar( array, "CellScalars", "Node");
          }
        }
      }
    }
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteGrid( void )
{
  vtkDataSet *DataSet = this->GetInputDataSet();
  int type; 

  if( !DataSet ) 
    {
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
    for( i = 0 ; i < NumberOfPoints ; i++ )
      {
      if( j >= 10 )
        {
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
    for( i = 0 ; i < NumberOfPoints ; i++ )
      {
      if( j >= 10 )
        {
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
    for( i = 0 ; i < NumberOfPoints ; i++ )
      {
      if( j >= 10 )
        {
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

//----------------------------------------------------------------------------
void vtkXdmfWriter::Write()
{
  if ( !this->FileName )
    {
    vtkErrorMacro("No file name specified");
    return;
    }

  vtkDataSet* ds = this->GetInputDataSet();
  if ( !ds )
    {
    vtkErrorMacro("No input or input of the wrong type");
    return;
    }
  ofstream ofs(this->FileName);
  if ( !ofs )
    {
    vtkErrorMacro("Cannot open file: " << this->FileName);
    return;
    }

  ds->Update();
  this->ResetXML();
  this->WriteHead();
  this->WriteGrid();
  this->WriteAttributes();
  this->WriteTail();
  char* str = this->GetXML();
  if ( str )
    {
    ofs << "<?xml version=\"1.0\" ?>" << endl
      << "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" [" << endl
      << "<!ENTITY HeavyData \"" << this->HeavyDataSetName << "\">" <<endl
      << "]>" << endl << endl << endl
      << "<Xdmf>" << endl
      << "<Domain>" << endl
      << "<Grid Name=\"" << this->GridName << "\">" << endl;
    ofs << str;
    ofs << "</Grid>" << endl
      << "</Domain>" << endl
      << "</Xdmf>" << endl;
    delete [] str;
    }
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::SetInput(vtkDataSet* ds)
{
  this->SetNthInput(0, ds);
}

//----------------------------------------------------------------------------
vtkDataSet* vtkXdmfWriter::GetInputDataSet()
{
  return vtkDataSet::SafeDownCast(this->Inputs[0]);
}
