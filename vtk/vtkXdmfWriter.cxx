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
#include "vtkDataSetCollection.h"

#include "vtkCharArray.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkLongArray.h"
#include "vtkShortArray.h"

#include "XdmfHDF.h"
#include "XdmfArray.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkXdmfWriter);
vtkCxxRevisionMacro(vtkXdmfWriter, "1.6");

//----------------------------------------------------------------------------
vtkXdmfWriter::vtkXdmfWriter()
{
  this->FileNameString = 0;
  this->HeavyDataSetNameString = 0;
  this->GridName = 0;
  this->SetHeavyDataSetName( "XdmfData.h5" );
  this->SetGridName( "Unnamed" );

  this->AllLight = 0;
  this->CurrIndent = 0;
}

//----------------------------------------------------------------------------
vtkXdmfWriter::~vtkXdmfWriter()
{
  this->SetHeavyDataSetName(0);
  this->SetFileNameString(0);
  if (this->InputList != NULL)
    {
    this->InputList->Delete();
    this->InputList = NULL;
    }
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::SetFileName(const char* fname)
{
  if ( fname )
    {
    char* hname = new char [ strlen(fname) + 10 ]; // space for extension
    strcpy(hname, fname);
    size_t cc;
    for ( cc = strlen(hname); cc > 0; cc -- )
      {
      if ( hname[cc] == '.' )
        {
        break;
        }
      }
    if ( cc > 0 )
      {
      hname[cc+1] = 0;
      }
    strcat(hname, "h5");
    this->SetHeavyDataSetName(hname);
    cout << "Set Heavy Data Set Name: " << hname << endl;
    delete [] hname;
    }
  this->SetFileNameString(fname);
}

//----------------------------------------------------------------------------
const char* vtkXdmfWriter::GetFileName()
{
  return this->FileNameString;
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::SetHeavyDataSetName( const char *name) 
{
  this->SetHeavyDataSetNameString(name);
  if ( name )
    {
    this->AllLight = 0;
    }
  else
    {
    this->AllLight = 1;
    }
  this->Modified();
}

//----------------------------------------------------------------------------
const char* vtkXdmfWriter::GetHeavyDataSetName()
{
  return this->HeavyDataSetNameString;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteHead( ostream& ost )
{
  ost << "<?xml version=\"1.0\" ?>" << endl
    << "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" [" << endl
    << "<!ENTITY HeavyData \"" << this->HeavyDataSetNameString << "\">" <<endl
    << "]>" << endl << endl << endl;
  this->Indent(ost);
  ost << "<Xdmf>";
  this->CurrIndent++;
  this->Indent(ost);

  return 1;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteTail( ostream& ost )
{
  this->CurrIndent--;
  this->Indent(ost);
  ost << "</Xdmf>";
  this->Indent(ost);
  return 0;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteCellArray( ostream& ost, vtkCellArray *Cells )
{

  vtkIdType  *Cp;
  int    NumberOfCells;
  int    PointsInPoly;
  int    i, j;

  Cp = Cells->GetPointer();
  NumberOfCells = Cells->GetNumberOfCells();
  PointsInPoly = *Cp;
  ost << "<DataStructure";
  this->CurrIndent++;
  this->Indent(ost);
  ost << " DataType=\"Int\"";
  this->Indent(ost);
  ost << " Dimensions=\"" << NumberOfCells << " " << PointsInPoly << "\"";
  this->Indent(ost);
  if( this->AllLight )
    {
    ost << " Format=\"XML\">";
    for( i = 0 ; i < NumberOfCells ; i++ )
      {
      this->Indent(ost);
      PointsInPoly = *Cp++;
      for( j = 0 ; j < PointsInPoly ; j++ )
        {
        ost << " " << *Cp++;
        }
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

    sprintf(DataSetName, "%s:/Connections" , this->HeavyDataSetNameString);
    cout << "DataSetName: " << DataSetName << endl;
    ost << " Format=\"HDF\">";
    this->CurrIndent++;
    this->Indent(ost);
    ost << " " << DataSetName;
    this->CurrIndent--;
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
        vtkErrorMacro( "Can't Create Heavy Dataset " << DataSetName );
        return( -1 );
        }
      }
    H5.Write( &Conns );
    H5.Close();


    }
  this->CurrIndent--;
  this->Indent(ost);
  ost << "</DataStructure>";
  return( NumberOfCells );
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WritePoints( ostream& ost, vtkPoints *Points )
{
  return this->WriteVTKArray( ost, Points->GetData(), "XYZ" );
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::EndTopology( ostream& ost )
{
  this->CurrIndent--;
  this->Indent(ost);
  ost << "</Topology>";
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::StartTopology( ostream& ost, int Type, vtkCellArray *Cells )
{
  vtkIdType *Cp;

  Cp = Cells->GetPointer();
  ost << "<Topology ";
  this->CurrIndent++;
  switch( Type ) 
    {
  case VTK_EMPTY_CELL :
    cerr << "Start Empty Cell" << endl;
  case VTK_VERTEX :
    cerr << "Start " <<  " VERTEX" << endl;
    ost << " Type=\"POLYVERTEX\"";
    this->Indent(ost);
    break;
  case VTK_POLY_VERTEX :
    cerr << "Start " <<  " POLY_VERTEX" << endl;
    ost << " Type=\"POLYVERTEX\"";
    this->Indent(ost);
    break;
  case VTK_LINE :
    cerr << "Start " <<  " LINE" << endl;
    ost << " Type=\"POLYLINE\"";
    this->Indent(ost);
    ost << " NodesPerElement=\"" << *Cp << "\"";
    this->Indent(ost);
    break;
  case VTK_POLY_LINE :
    cerr << "Start " <<  " POLY_LINE" << endl;
    ost << " Type=\"POLYLINE\"";
    this->Indent(ost);
    ost << " NodesPerElement=\"" << *Cp << "\"";
    this->Indent(ost);
    break;
  case VTK_TRIANGLE :
    cerr << "Start " <<  " TRIANGLE" << endl;
    ost << " Type=\"TRIANGLE\"";
    this->Indent(ost);
    break;
  case VTK_TRIANGLE_STRIP :
    cerr << "Start " <<  " TRIANGLE_STRIP" << endl;
    ost << " Type=\"TRIANGLE\"";
    this->Indent(ost);
    break;
  case VTK_POLYGON :
    cerr << "Start " <<  " POLYGON" << endl;
    ost << " Type=\"POLYGON\"";
    this->Indent(ost);
    ost << " NodesPerElement=\"" << *Cp << "\"";
    this->Indent(ost);
    break;
  case VTK_PIXEL :
    cerr << "Start " <<  " PIXEL" << endl;
    ost << " Type=\"QUADRILATERAL\"";
    this->Indent(ost);
    break;
  case VTK_QUAD :
    cerr << "Start " <<  " QUAD" << endl;
    ost << " Type=\"QUADRILATERAL\"";
    this->Indent(ost);
    break;
  case VTK_TETRA :
    cerr << "Start " <<  " TETRA" << endl;
    ost << " Type=\"TETRAHEDRON\"";
    this->Indent(ost);
    break;
  case VTK_VOXEL :
    cerr << "Start " <<  " VOXEL" << endl;
    ost << " Type=\"HEXAHEDRON\"";
    this->Indent(ost);
    break;
  case VTK_HEXAHEDRON :
    cerr << "Start " <<  " HEXAHEDRON" << endl;
    ost << " Type=\"HEXAHEDRON\"";
    this->Indent(ost);
    break;
  case VTK_WEDGE :
    cerr << "Start " <<  " WEDGE" << endl;
    ost << " Type=\"WEDGE\"";
    this->Indent(ost);
    break;
  case VTK_PYRAMID :
    cerr << "Start " <<  " PYRAMID" << endl;
    ost << " Type=\"PYRAMID\"";
    this->Indent(ost);
    break;
  default :
    vtkErrorMacro("Unknown Topology Type");
    break;
    }
  ost << " Dimensions=\"" << Cells->GetNumberOfCells() << "\">";
  this->Indent(ost);
}

//----------------------------------------------------------------------------
template<class AType, class NType>
vtkIdType vtkXdmfWriterWriteXMLScalar(vtkXdmfWriter* self, ostream& ost, AType* array, 
  const char* heavyData, const char* arrayName, 
  const char* scalar_type, NType value,
  int allLight, int type)
{
  if ( !array )
    {
    return -2;
    }
  ost << "<DataStructure";
  self->Indent(ost);
  ost << "  DataType=\"" << scalar_type << "\"";
  self->Indent(ost);
  ost << "  Dimensions=\"" << array->GetNumberOfTuples() 
    << " " << array->GetNumberOfComponents() << "\"";
  self->Indent(ost);
  NType val = value;
  if ( allLight )
    {
    ost << "  Format=\"XML\">";
    vtkIdType jj, kk;
    for ( jj = 0; jj < array->GetNumberOfTuples(); jj ++ )
      {
      if ( jj % 3 == 0 )
        {
        self->Indent(ost);
        }
      for ( kk = 0; kk < array->GetNumberOfComponents(); kk ++ )
        {
        val = array->GetValue(jj * array->GetNumberOfComponents() + kk);
        ost << " " << val;
        }
      }
    }
  else
    {
    char *DataSetName;
    XdmfHDF H5;
    XdmfArray Data;
    XdmfInt64 dims[2];

    DataSetName = new char [ strlen(heavyData) + strlen(arrayName) + 10 ];
    sprintf(DataSetName, "%s:/%s", heavyData, arrayName);
    ost << "  Format=\"HDF\">";
    self->Indent(ost);
    ost << "  " << DataSetName;
    self->Indent(ost);

    dims[0] = array->GetNumberOfTuples();
    dims[1] = array->GetNumberOfComponents();

    Data.SetNumberType(type);
    cout << "Data type: " << type << endl;
    Data.SetShape(2, dims);

    vtkIdType jj, kk;
    vtkIdType pos = 0;
    for ( jj = 0; jj < array->GetNumberOfTuples(); jj ++ )
      {
      for ( kk = 0; kk < array->GetNumberOfComponents(); kk ++ )
        {
        Data.SetValue(pos, array->GetValue(pos));
        pos ++;
        }
      }
    H5.CopyType( &Data );
    H5.CopyShape( &Data );

    if( H5.Open( DataSetName, "rw" ) == XDMF_FAIL )
      {
      if( H5.CreateDataset( DataSetName ) == XDMF_FAIL ) 
        {
        vtkErrorWithObjectMacro(self, "Can't Create Heavy Dataset " << DataSetName);
        return( -1 );
        }
      }
    H5.Write( &Data );
    H5.Close();

    delete [] DataSetName;
    }

  self->Indent( ost );
  ost << "</DataStructure>";
  return( array->GetNumberOfTuples() );
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteDataArray( ostream& ost, vtkDataArray* array, const char* Name, const char* Center )
{
  const char* arrayName = Name;
  if ( array->GetName() )
    {
    arrayName = array->GetName();
    }

  ost << "<Attribute Center=\"" <<
    Center << "\"" <<
    " Name=\"" << arrayName << "\">";
  this->CurrIndent ++;
  this->Indent(ost);
  vtkIdType res = this->WriteVTKArray( ost, array, arrayName );
  this->CurrIndent --;
  this->Indent(ost);
  ost << "</Attribute>";
  this->Indent(ost);
  return res;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteVTKArray( ostream& ost, vtkDataArray* array, const char* Name )
{
  vtkIdType res = -1;
  const char* hd = this->HeavyDataSetNameString;
  int int_type;
  if ( sizeof(long) == 4 )
    {
    int_type = XDMF_INT32_TYPE;
    }
  else
    {
    int_type = XDMF_INT64_TYPE;
    }
  switch ( array->GetDataType() )
    {
  case VTK_CHAR:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkCharArray::SafeDownCast(array), hd,
      Name, "Char", static_cast<short>(0), this->AllLight, XDMF_INT8_TYPE);
    break;
  case VTK_SHORT:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkShortArray::SafeDownCast(array), hd,
      Name, "Int", static_cast<int>(0), this->AllLight, int_type);
    break;
  case VTK_INT:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkIntArray::SafeDownCast(array), hd,
      Name, "Int", static_cast<int>(0), this->AllLight, int_type);
    break;
  case VTK_FLOAT:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkFloatArray::SafeDownCast(array), hd,
      Name, "Float", static_cast<float>(0), this->AllLight, XDMF_FLOAT32_TYPE);
    break;
  case VTK_DOUBLE:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkFloatArray::SafeDownCast(array), hd,
      Name, "Float", static_cast<double>(0), this->AllLight, XDMF_FLOAT64_TYPE);
    break;
  default:
    vtkErrorMacro("Unknown scalar type: " << array->GetDataType());
    }
  if ( res == -2 )
    {
    vtkErrorMacro("Cannot convert array to specified type");
    }
  return res;
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::WriteAttributes( ostream& ost )
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
      //this->WriteScalar( Scalars, "CellScalars", "Cell" );  
      this->WriteDataArray( ost, Scalars, "CellScalars", "Cell" );
      }
    if( Vectors )
      {
      //this->WriteVector( Vectors, "CellVectors", "Cell" );  
      this->WriteDataArray( ost, Vectors, "CellVectors", "Cell" );  
      }
    for ( cc = 0; cc < CellData->GetNumberOfArrays(); cc ++ )
      {
      vtkDataArray* array = CellData->GetArray(cc);
      if ( array != CellData->GetScalars() &&
        array != CellData->GetVectors() )
        {
        if ( array->GetNumberOfComponents() == 3 )
          {
          //this->WriteScalar( array, "CellVectors", "Cell");
          this->WriteDataArray( ost, array, "CellVectors", "Cell");
          }
        else
          {
          //this->WriteScalar( array, "CellScalars", "Cell");
          this->WriteDataArray( ost, array, "CellScalars", "Cell");
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
      //this->WriteScalar( Scalars, "NodeScalars", "Node" );  
      this->WriteDataArray( ost, Scalars, "NodeScalars", "Node" );  
      }
    if( Vectors )
      {
      //this->WriteVector( Vectors, "NodeVectors", "Node" );  
      this->WriteDataArray( ost, Vectors, "NodeVectors", "Node" );  
      }
    for ( cc = 0; cc < PointData->GetNumberOfArrays(); cc ++ )
      {
      vtkDataArray* array = PointData->GetArray(cc);
      if ( array != PointData->GetScalars() &&
        array != PointData->GetVectors() )
        {
        if ( array->GetNumberOfComponents() == 3 )
          {
          //this->WriteScalar( array, "CellVectors", "Node");
          this->WriteDataArray( ost, array, "CellVectors", "Node");
          }
        else
          {
          //this->WriteScalar( array, "CellScalars", "Node");
          this->WriteDataArray( ost, array, "CellScalars", "Node");
          }
        }
      }
    }
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteGrid( ostream& ost )
{
  vtkDataSet *DataSet = this->GetInputDataSet();
  int type; 

  if( !DataSet ) 
    {
    vtkErrorMacro("No Input Data Set");
    return( -1 );
    }
  ost << "<Grid Name=\"" << this->GridName << "\">";
  this->CurrIndent ++;
  this->Indent(ost);
  type = DataSet->GetDataObjectType();
  if ( type == VTK_POLY_DATA )
    {
    vtkPolyData *Polys = ( vtkPolyData *)DataSet;
    this->StartTopology( ost, Polys->GetCell(0)->GetCellType(), Polys->GetPolys());
    this->WriteCellArray( ost, Polys->GetPolys());
    this->EndTopology( ost );
    this->Indent(ost);

    this->StartGeometry( ost, "XYZ" );
    this->WritePoints( ost, Polys->GetPoints());
    this->EndGeometry( ost );
    }
  else if ( type == VTK_STRUCTURED_POINTS || type == VTK_IMAGE_DATA)
    {
    int     Dims[3];
    double Origin[3], Spacing[3];
    vtkImageData *SGrid = ( vtkImageData *)DataSet;
    SGrid->GetDimensions( Dims );
    SGrid->GetOrigin( Origin );
    SGrid->GetSpacing( Spacing );
    ost << "<Topology Type=\"3DCORECTMESH\"";
    this->Indent(ost);
    ost << " Dimensions=\"" << Dims[2] << " " << Dims[1] << " " << Dims[0] << "\"/>";
    this->Indent(ost);
    this->StartGeometry( ost, "ORIGIN_DXDYDZ" );
    this->Indent(ost);

    // Origin
    ost << "<DataStructure";
    this->CurrIndent ++;
    this->Indent(ost);
    ost << "  DataType=\"Float\"";
    this->Indent(ost);
    ost << "  Dimensions=\"3\"";
    this->Indent(ost);
    ost << "  Format=\"XML\">";
    this->Indent(ost);
    ost << Origin[0] << " " << Origin[1] << " " << Origin[2];
    this->CurrIndent--;
    this->Indent(ost);
    ost << "</DataStructure>";

    // DX DY DZ
    ost << "<DataStructure";
    this->CurrIndent ++;
    this->Indent(ost);
    ost << "  DataType=\"Float\"";
    this->Indent(ost);
    ost << "  Dimensions=\"3\"";
    this->Indent(ost);
    ost << "  Format=\"XML\">";
    this->Indent(ost);
    ost << Spacing[0] << " " << Spacing[1] << " " << Spacing[2];
    this->CurrIndent--;
    this->Indent(ost);
    ost << "</DataStructure>";

    this->EndGeometry( ost );
    }
  else if ( type == VTK_STRUCTURED_GRID )
    {
    int     Dims[3];
    vtkStructuredGrid *SGrid = ( vtkStructuredGrid *)DataSet;
    SGrid->GetDimensions( Dims );
    ost << "<Topology Type=\"3DSMESH\"";
    this->CurrIndent++;
    this->Indent(ost);
    ost << " Dimensions=\"" << Dims[2] << " " << Dims[1] << " " << Dims[0] << "\"/>";
    this->EndTopology( ost );
    this->StartGeometry(ost, "XYZ");
    this->WritePoints( ost, SGrid->GetPoints());
    this->EndGeometry(ost);
    }
  else if ( type == VTK_UNSTRUCTURED_GRID )
    {
    vtkUnstructuredGrid *UGrid = ( vtkUnstructuredGrid *)DataSet;
    this->StartTopology( ost, UGrid->GetCell(0)->GetCellType(), UGrid->GetCells());
    this->WriteCellArray( ost, UGrid->GetCells());
    this->EndTopology( ost );
    this->Indent(ost);
    this->StartGeometry(ost, "XYZ");
    this->WritePoints( ost, UGrid->GetPoints());
    this->EndGeometry(ost);
    }
  else if ( type == VTK_RECTILINEAR_GRID )
    {
    int    i, j;
    int     Dims[3], NumberOfPoints;
    vtkDataArray  *Coord;
    vtkRectilinearGrid *RGrid = ( vtkRectilinearGrid *)DataSet;
    RGrid->GetDimensions( Dims );
    ost << "<Topology Type=\"3DRECTMESH\"";
    this->CurrIndent ++;
    this->Indent(ost);
    ost << " Dimensions=\"" << Dims[2] << " " << Dims[1] << " " << Dims[0] << "\"/>";
    this->Indent(ost);
    this->EndTopology( ost );
    this->StartGeometry(ost, "VXVYVZ");
    // X Coordinated
    Coord = RGrid->GetXCoordinates();
    NumberOfPoints = Coord->GetNumberOfTuples();
    ost << "<DataStructure";
    this->CurrIndent ++;
    this->Indent(ost);
    ost << "  DataType=\"Float\"";
    this->Indent(ost);
    ost << "  Dimensions=\"" << NumberOfPoints << "\"";
    this->Indent(ost);
    ost << "  Format=\"XML\">";
    for( i = 0 ; i < NumberOfPoints ; i++ )
      {
      if( i % 3 == 0 )
        {
        this->Indent(ost);
        }
      ost << " " << *Coord->GetTuple( i );
      }
    this->CurrIndent --;
    this->Indent(ost);
    ost << "</DataStructure>";
    this->Indent(ost);
    // Y Coordinated
    Coord = RGrid->GetYCoordinates();
    NumberOfPoints = Coord->GetNumberOfTuples();
    ost << "<DataStructure";
    this->CurrIndent ++;
    this->Indent(ost);
    ost << "  DataType=\"Float\"";
    this->Indent(ost);
    ost << "  Dimensions=\"" << NumberOfPoints << "\"";
    this->Indent(ost);
    ost << "  Format=\"XML\">";
    this->Indent(ost);
    j = 0;
    for( i = 0 ; i < NumberOfPoints ; i++ )
      {
      if( j >= 10 )
        {
        this->Indent(ost);
        j = 0;
        }
      ost << " " << *Coord->GetTuple( i );
      j++;
      }
    this->CurrIndent --;
    this->Indent(ost);
    ost << "</DataStructure>";
    this->Indent(ost);
    // Z Coordinated
    Coord = RGrid->GetZCoordinates();
    NumberOfPoints = Coord->GetNumberOfTuples();
    ost << "<DataStructure";
    this->CurrIndent ++;
    this->Indent(ost);
    ost << "  DataType=\"Float\"";
    this->Indent(ost);
    ost << "  Dimensions=\"" << NumberOfPoints << "\"";
    this->Indent(ost);
    ost << "  Format=\"XML\">";
    this->Indent(ost);
    j = 0;
    for( i = 0 ; i < NumberOfPoints ; i++ )
      {
      if( j >= 10 )
        {
        this->Indent(ost);
        j = 0;
        }
      ost << " " << *Coord->GetTuple( i );
      j++;
      }
    this->CurrIndent --;
    this->Indent(ost);
    ost << "</DataStructure>";
    this->EndGeometry(ost);
    }
  this->Indent(ost);
  this->WriteAttributes(ost);

  this->CurrIndent --;
  this->Indent(ost);
  ost << "</Grid>";

  return( 1 );
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::Write()
{
  if ( !this->FileNameString )
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
  ofstream ofs(this->FileNameString);
  if ( !ofs )
    {
    vtkErrorMacro("Cannot open file: " << this->FileNameString);
    return;
    }

  ds->Update();
  this->WriteHead(ofs);
  ofs << "<Domain>";
  this->CurrIndent ++;

  this->Indent(ofs);
  this->WriteGrid(ofs);

  this->CurrIndent --;
  this->Indent( ofs );
  ofs << "</Domain>";
  this->WriteTail(ofs);
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::SetInput(vtkDataSet* ds)
{
  this->SetNthInput(0, ds);
}

//----------------------------------------------------------------------------
vtkDataSet* vtkXdmfWriter::GetInputDataSet()
{
  vtkDataSet* ds = vtkDataSet::SafeDownCast(this->Inputs[0]);
  return ds;
}

//----------------------------------------------------------------------------
// Add a dataset to the list of data to append.
void vtkXdmfWriter::AddInput(vtkDataObject *ds)
{
  vtkErrorMacro("AddInput does not work as it should yet. Use SetInput.");
  this->vtkProcessObject::AddInput(ds);
}

//----------------------------------------------------------------------------
vtkDataObject *vtkXdmfWriter::GetInput(int idx)
{
  if (idx >= this->NumberOfInputs || idx < 0)
    {
    return NULL;
    }

  return (vtkDataObject *)(this->Inputs[idx]);
}

//----------------------------------------------------------------------------
// Remove a dataset from the list of data to append.
void vtkXdmfWriter::RemoveInput(vtkDataObject *ds)
{
  this->vtkProcessObject::RemoveInput(ds);
}

//----------------------------------------------------------------------------
vtkDataSetCollection *vtkXdmfWriter::GetInputList()
{
  int idx;

  if (this->InputList)
    {
    this->InputList->Delete();
    }
  this->InputList = vtkDataSetCollection::New();

  for (idx = 0; idx < this->NumberOfInputs; ++idx)
    {
    if (this->Inputs[idx] != NULL)
      {
      this->InputList->AddItem((vtkDataSet*)(this->Inputs[idx]));
      }
    }  

  return this->InputList;
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::StartGeometry( ostream& ost, const char* type )
{
  ost << "<Geometry Type=\"" << type << "\">";
  this->CurrIndent ++;
  this->Indent(ost);
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::EndGeometry( ostream& ost )
{
  this->CurrIndent --;
  this->Indent(ost);
  ost << "</Geometry>";
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::Indent(ostream& os)
{
  int cc;

  os << endl;
  for ( cc = 0; cc < this->CurrIndent; cc ++ )
    {
    os << "  ";
    }
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

