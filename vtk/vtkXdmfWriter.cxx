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
#include "XdmfAttribute.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkXdmfWriter);
vtkCxxRevisionMacro(vtkXdmfWriter, "1.7");

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

  this->InputList = 0;

  this->HDF5ArrayName = 0;
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
  this->SetHDF5ArrayName(0);
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
    vtkDebugMacro("Set Heavy Data Set Name: " << hname);
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
  this->IncrementIndent();
  this->Indent(ost);

  return 1;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteTail( ostream& ost )
{
  this->DecrementIndent();
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
  this->IncrementIndent();
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
    XdmfArray  Conns;
    XdmfHDF    H5;
    XdmfInt64  Dims[2];
    XdmfInt32  *Dp;

    const char* DataSetName = this->GenerateHDF5ArrayName("Connections");
    ost << " Format=\"HDF\">";
    this->IncrementIndent();
    this->Indent(ost);
    ost << " " << DataSetName;
    this->DecrementIndent();
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
  this->DecrementIndent();
  this->Indent(ost);
  ost << "</DataStructure>";
  return( NumberOfCells );
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WritePoints( ostream& ost, vtkPoints *Points )
{
  int dims[3] = { -1, -1, -1 };
  return this->WriteVTKArray( ost, Points->GetData(), dims, "XYZ", 0, 
    this->AllLight );
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::EndTopology( ostream& ost )
{
  this->DecrementIndent();
  this->Indent(ost);
  ost << "</Topology>";
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::StartTopology( ostream& ost, const char* toptype, int rank, int *dims)
{
  ost << "<Topology ";
  this->IncrementIndent();
  this->Indent(ost);
  ost << " Type=\"" << toptype << "\"";
  ost << " Dimensions=\"";
  int cc;
  for ( cc = rank-1; cc >= 0; cc -- )
    {
    if ( cc < rank - 1 )
      {
      ost << " ";
      }
    ost << dims[cc]; 
    }
  ost << "\">";
}
//----------------------------------------------------------------------------
void vtkXdmfWriter::StartTopology( ostream& ost, int Type, vtkCellArray *Cells )
{
  vtkIdType *Cp;

  Cp = Cells->GetPointer();
  ost << "<Topology ";
  this->IncrementIndent();
  switch( Type ) 
    {
  case VTK_EMPTY_CELL :
    vtkDebugMacro("Start Empty Cell");
  case VTK_VERTEX :
    vtkDebugMacro("Start " <<  " VERTEX");
    ost << " Type=\"POLYVERTEX\"";
    this->Indent(ost);
    break;
  case VTK_POLY_VERTEX :
    vtkDebugMacro("Start " <<  " POLY_VERTEX");
    ost << " Type=\"POLYVERTEX\"";
    this->Indent(ost);
    break;
  case VTK_LINE :
    vtkDebugMacro("Start " <<  " LINE");
    ost << " Type=\"POLYLINE\"";
    this->Indent(ost);
    ost << " NodesPerElement=\"" << *Cp << "\"";
    this->Indent(ost);
    break;
  case VTK_POLY_LINE :
    vtkDebugMacro("Start " <<  " POLY_LINE");
    ost << " Type=\"POLYLINE\"";
    this->Indent(ost);
    ost << " NodesPerElement=\"" << *Cp << "\"";
    this->Indent(ost);
    break;
  case VTK_TRIANGLE :
    vtkDebugMacro("Start " <<  " TRIANGLE");
    ost << " Type=\"TRIANGLE\"";
    this->Indent(ost);
    break;
  case VTK_TRIANGLE_STRIP :
    vtkDebugMacro("Start " <<  " TRIANGLE_STRIP");
    ost << " Type=\"TRIANGLE\"";
    this->Indent(ost);
    break;
  case VTK_POLYGON :
    vtkDebugMacro("Start " <<  " POLYGON");
    ost << " Type=\"POLYGON\"";
    this->Indent(ost);
    ost << " NodesPerElement=\"" << *Cp << "\"";
    this->Indent(ost);
    break;
  case VTK_PIXEL :
    vtkDebugMacro("Start " <<  " PIXEL");
    ost << " Type=\"QUADRILATERAL\"";
    this->Indent(ost);
    break;
  case VTK_QUAD :
    vtkDebugMacro("Start " <<  " QUAD");
    ost << " Type=\"QUADRILATERAL\"";
    this->Indent(ost);
    break;
  case VTK_TETRA :
    vtkDebugMacro("Start " <<  " TETRA");
    ost << " Type=\"TETRAHEDRON\"";
    this->Indent(ost);
    break;
  case VTK_VOXEL :
    vtkDebugMacro("Start " <<  " VOXEL");
    ost << " Type=\"HEXAHEDRON\"";
    this->Indent(ost);
    break;
  case VTK_HEXAHEDRON :
    vtkDebugMacro("Start " <<  " HEXAHEDRON");
    ost << " Type=\"HEXAHEDRON\"";
    this->Indent(ost);
    break;
  case VTK_WEDGE :
    vtkDebugMacro("Start " <<  " WEDGE");
    ost << " Type=\"WEDGE\"";
    this->Indent(ost);
    break;
  case VTK_PYRAMID :
    vtkDebugMacro("Start " <<  " PYRAMID");
    ost << " Type=\"PYRAMID\"";
    this->Indent(ost);
    break;
  default :
    vtkErrorMacro("Unknown Topology Type");
    break;
    }
  ost << " Dimensions=\"" << Cells->GetNumberOfCells() << "\">";
}

//----------------------------------------------------------------------------
template<class AType, class NType>
vtkIdType vtkXdmfWriterWriteXMLScalar(vtkXdmfWriter* self, ostream& ost, AType* array, 
  const char* dataName, const char* arrayName, 
  const char* scalar_type, NType value,
  int allLight, int type,
  int dims[3])
{
  if ( !array )
    {
    vtkErrorWithObjectMacro(self, "No array specified. Should be " << scalar_type << " array");
    return -2;
    }
  ost << "<DataStructure";
  self->IncrementIndent();
  if ( dataName )
    {
    self->Indent(ost);
    ost << " Name=\"" << dataName << "\"";
    }
  self->Indent(ost);
  ost << " DataType=\"" << scalar_type << "\"";
  self->Indent(ost);
  if ( type == XDMF_FLOAT64_TYPE )
    {
    ost << " Precision=\"8\"";
    self->Indent(ost);
    }
  else if ( type == XDMF_FLOAT32_TYPE )
    {
    ost << " Precision=\"4\"";
    self->Indent(ost);
    }
  ost << " Dimensions=\"";
  if ( dims[0] < 1 )
    {
    ost << array->GetNumberOfTuples();
    }
  else
    {
    ost << dims[2] << " " << dims[1] << " " << dims[0];
    }
  if ( array->GetNumberOfComponents() > 1 )
    {
    ost << " " << array->GetNumberOfComponents();
    }
  ost << "\"";
  self->Indent(ost);
  NType val = value;
  if ( allLight )
    {
    ost << " Format=\"XML\">";
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
    const char *DataSetName;
    XdmfHDF H5;
    XdmfArray Data;
    XdmfInt64 h5dims[5];
    int nh5dims = 0;

    DataSetName = self->GenerateHDF5ArrayName(arrayName);
    ost << " Format=\"HDF\">";
    self->Indent(ost);
    ost << " " << DataSetName;
    self->Indent(ost);

    if ( dims[0] < 1 )
      {
      h5dims[0] = array->GetNumberOfTuples();
      h5dims[1] = array->GetNumberOfComponents();
      nh5dims = 2;
      }
    else
      {
      h5dims[0] = dims[2];
      h5dims[1] = dims[1];
      h5dims[2] = dims[0];
      h5dims[3] = array->GetNumberOfComponents();
      nh5dims = 4;
      if ( h5dims[3] <= 1 )
        {
        nh5dims = 3;
        }
      //cout << "Use dims: " << h5dims[0] << " " << h5dims[1] << " " << h5dims[2]<< " " << h5dims[3] << " (" << nh5dims << ")" << endl;
      }

    Data.SetNumberType(type);
    //cout << "Data type: " << type << endl;
    Data.SetShape(nh5dims, h5dims);

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
    }
  self->DecrementIndent();
  self->Indent( ost );
  ost << "</DataStructure>";
  return( array->GetNumberOfTuples() );
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteDataArray( ostream& ost, vtkDataArray* array, int dims[3], const char* Name, const char* Center, int type )
{
  const char* arrayName = Name;
  if ( array->GetName() )
    {
    arrayName = array->GetName();
    }

  ost << "<Attribute";
  this->IncrementIndent();
  this->Indent(ost);
  switch ( type )
    {
  case XDMF_ATTRIBUTE_TYPE_SCALAR:
    ost << " Type=\"Scalar\"";
    break;
  case XDMF_ATTRIBUTE_TYPE_VECTOR:
    ost << " Type=\"Vector\"";
    break;
  case XDMF_ATTRIBUTE_TYPE_TENSOR:
    ost << " Type=\"Tensor\"";
    break;
  case XDMF_ATTRIBUTE_TYPE_MATRIX:
    ost << " Type=\"Matrix\"";
    break;
    }
  ost << " Center=\"" << Center << "\"";
  this->Indent(ost);
  ost << " Name=\"" << arrayName << "\">";
  this->Indent(ost);
  vtkIdType res = this->WriteVTKArray( ost, array, dims, arrayName, 0, 
    this->AllLight );
  this->DecrementIndent();
  this->Indent(ost);
  ost << "</Attribute>";
  this->Indent(ost);
  return res;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter::WriteVTKArray( ostream& ost, vtkDataArray* array, int dims[3], const char* Name, const char* dataName, int alllight )
{
  vtkIdType res = -1;
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
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkCharArray::SafeDownCast(array), dataName,
      Name, "Char", static_cast<short>(0), alllight, XDMF_INT8_TYPE, dims);
    break;
  case VTK_SHORT:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkShortArray::SafeDownCast(array), dataName,
      Name, "Int", static_cast<int>(0), alllight, int_type, dims);
    break;
  case VTK_INT:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkIntArray::SafeDownCast(array), dataName,
      Name, "Int", static_cast<int>(0), alllight, int_type, dims);
    break;
  case VTK_FLOAT:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkFloatArray::SafeDownCast(array), dataName,
      Name, "Float", static_cast<float>(0), alllight, XDMF_FLOAT32_TYPE, dims);
    break;
  case VTK_DOUBLE:
    res = vtkXdmfWriterWriteXMLScalar(this, ost, vtkDoubleArray::SafeDownCast(array), dataName,
      Name, "Float", static_cast<double>(0), alllight, XDMF_FLOAT64_TYPE, dims);
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
  int extent[6];
  int cdims[3] = { -1, -1, -1 };
  int pdims[3] = { -1, -1, -1 };

  DataSet->GetUpdateExtent(extent);
  //cout << "Extent: " << extent[0] << " "<< extent[1] << " "<< extent[2] << " "<< extent[3] << " "<< extent[4] << " " << extent[5] << endl;
  if ( extent[1] >= extent[0] && extent[3] >= extent[2] && extent[5] >= extent[4] )
    {
    cdims[0] = pdims[0] = extent[1] - extent[0] +1;
    cdims[1] = pdims[1] = extent[3] - extent[2] +1;
    cdims[2] = pdims[2] = extent[5] - extent[4] +1;
    cdims[0] --;
    cdims[1] --;
    cdims[2] --;
    //cout << "pDims: " << pdims[0] << " " << pdims[1] << " " << pdims[2] << endl;
    //cout << "CDims: " << cdims[0] << " " << cdims[1] << " " << cdims[2] << endl;
    if ( cdims[0] <= 0 && cdims[1] <= 0 && cdims[2] <= 0 )
      {
      // Bogus dimensions.
      pdims[0] = pdims[1] = pdims[2] = -1;
      cdims[0] = cdims[1] = cdims[2] = -1;
      }
    }

  vtkCellData *CellData = DataSet->GetCellData();
  vtkPointData *PointData = DataSet->GetPointData();
  int cc;

  if( CellData )
    {
    for ( cc = 0; cc < CellData->GetNumberOfArrays(); cc ++ )
      {
      vtkDataArray* array = CellData->GetArray(cc);
      int type = XDMF_ATTRIBUTE_TYPE_NONE;
      if ( array == CellData->GetScalars() )
        {
        type = XDMF_ATTRIBUTE_TYPE_SCALAR;
        }
      if ( array == CellData->GetVectors() )
        {
        type = XDMF_ATTRIBUTE_TYPE_VECTOR;
        }
      char buffer[100];
      sprintf(buffer, "UnnamedCellArray%d", cc);
      this->WriteDataArray( ost, array, cdims, buffer, "Cell", type);
      }
    }
  if( PointData )
    {
    for ( cc = 0; cc < PointData->GetNumberOfArrays(); cc ++ )
      {
      vtkDataArray* array = PointData->GetArray(cc);
      int type = XDMF_ATTRIBUTE_TYPE_NONE;
      if ( array == CellData->GetScalars() )
        {
        type = XDMF_ATTRIBUTE_TYPE_SCALAR;
        }
      if ( array == CellData->GetVectors() )
        {
        type = XDMF_ATTRIBUTE_TYPE_VECTOR;
        }
      if ( array == CellData->GetTensors() )
        {
        type = XDMF_ATTRIBUTE_TYPE_TENSOR;
        }
      char buffer[100];
      sprintf(buffer, "UnnamedNodeArray%d", cc);
      this->WriteDataArray( ost, array, pdims, buffer, "Node", type);
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
  this->IncrementIndent();
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
    this->StartTopology(ost, "3DCORECTMESH", 3, Dims);
    this->EndTopology(ost);
    this->Indent(ost);
    this->StartGeometry( ost, "ORIGIN_DXDYDZ" );
    this->Indent(ost);

    // Origin
    ost << "<DataStructure";
    this->IncrementIndent();
    this->Indent(ost);
    ost << " DataType=\"Float\"";
    this->Indent(ost);
    ost << " Dimensions=\"3\"";
    this->Indent(ost);
    ost << " Format=\"XML\">";
    this->Indent(ost);
    ost << Origin[0] << " " << Origin[1] << " " << Origin[2];
    this->DecrementIndent();
    this->Indent(ost);
    ost << "</DataStructure>";
    this->Indent(ost);

    // DX DY DZ
    ost << "<DataStructure";
    this->IncrementIndent();
    this->Indent(ost);
    ost << " DataType=\"Float\"";
    this->Indent(ost);
    ost << " Dimensions=\"3\"";
    this->Indent(ost);
    ost << " Format=\"XML\">";
    this->Indent(ost);
    ost << Spacing[0] << " " << Spacing[1] << " " << Spacing[2];
    this->DecrementIndent();
    this->Indent(ost);
    ost << "</DataStructure>";

    this->EndGeometry( ost );
    }
  else if ( type == VTK_STRUCTURED_GRID )
    {
    int     Dims[3];
    vtkStructuredGrid *SGrid = ( vtkStructuredGrid *)DataSet;
    SGrid->GetDimensions( Dims );
    this->StartTopology(ost, "3DSMESH", 3, Dims);
    this->EndTopology(ost);
    this->Indent(ost);
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
    int     Dims[3];
    vtkDataArray  *Coord;
    vtkRectilinearGrid *RGrid = ( vtkRectilinearGrid *)DataSet;
    RGrid->GetDimensions( Dims );
    this->StartTopology(ost, "3DRECTMESH", 3, Dims);
    this->EndTopology( ost );
    this->Indent(ost);
    this->StartGeometry(ost, "VXVYVZ");
    int dummydims[3];
    dummydims[0] = dummydims[1] = dummydims[2] = -1;
    // X Coordinated
    Coord = RGrid->GetXCoordinates();
    this->WriteVTKArray( ost, Coord, dummydims, "X Coordinates", "X", 1);
    this->Indent(ost);
    // Y Coordinated
    Coord = RGrid->GetYCoordinates();
    this->WriteVTKArray( ost, Coord, dummydims, "Y Coordinates", "Y", 1);
    this->Indent(ost);
    // Z Coordinated
    Coord = RGrid->GetZCoordinates();
    this->WriteVTKArray( ost, Coord, dummydims, "Z Coordinates", "Z", 1);
    this->EndGeometry(ost);
    }
  this->Indent(ost);
  this->WriteAttributes(ost);

  this->DecrementIndent();
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
  this->IncrementIndent();

  this->Indent(ofs);
  this->WriteGrid(ofs);

  this->DecrementIndent();
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
  this->IncrementIndent();
  this->Indent(ost);
}

//----------------------------------------------------------------------------
void vtkXdmfWriter::EndGeometry( ostream& ost )
{
  this->DecrementIndent();
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
const char* vtkXdmfWriter::GenerateHDF5ArrayName(const char* array)
{
  if ( !this->HeavyDataSetNameString )
    {
    vtkErrorMacro("HeavyDataSetName is not yet specified");
    return 0;
    }
  size_t namelen = strlen(this->HeavyDataSetNameString) + strlen(array);
  if ( this->GridName )
    {
    namelen += strlen(this->GridName);
    }
  char *name = new char [ namelen + 10 ];
  if ( this->GridName )
    {
    sprintf(name, "%s:/%s/%s", this->HeavyDataSetNameString, 
      this->GridName, array);
    }
  else
    {
    sprintf(name, "%s:/%s", this->HeavyDataSetNameString, array);
    }
  this->SetHDF5ArrayName(name);
  delete [] name;
  return this->HDF5ArrayName;
}
//----------------------------------------------------------------------------
void vtkXdmfWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

