/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXdmfReader.cxx
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
#include "vtkXdmfReader.h"

#include "vtkCallbackCommand.h"
#include "vtkDataArraySelection.h"
#include "vtkDataSet.h"
#include "vtkDoubleArray.h"
#include "vtkObjectFactory.h"
#include "vtkRectilinearGrid.h"
#include "vtkStructuredGrid.h"
#include "vtkUnstructuredGrid.h"
#include "vtkXdmfDataArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkCell.h"
#include "vtkCellArray.h"

#include "XdmfArray.h"
#include "XdmfDOM.h"
#include "XdmfParameter.h"
#include "XdmfDataDesc.h"
#include "XdmfFormatMulti.h"
#include "XdmfTransform.h"
#include "XdmfGrid.h"
#include "XdmfXNode.h"

#include <sys/stat.h>
#include <string>
#include <vector>

vtkStandardNewMacro(vtkXdmfReader);
vtkCxxRevisionMacro(vtkXdmfReader, "1.13");

#if defined(_WIN32) && (defined(_MSC_VER) || defined(__BORLANDC__))
#  include <direct.h>
#  define GETCWD _getcwd
#else
#include <unistd.h>
#  define GETCWD getcwd
#endif


#define vtkMAX(x, y) (((x)>(y))?(x):(y))
#define vtkMIN(x, y) (((x)<(y))?(x):(y))

class vtkXdmfReaderInternal
{
public:
  typedef vtkstd::vector<vtkstd::string> vtkXdmfReaderList;
  vtkXdmfReaderList DomainList;
  vtkXdmfReaderList GridList;
};

//----------------------------------------------------------------------------
vtkXdmfReader::vtkXdmfReader()
{
  this->Internals = new vtkXdmfReaderInternal;

  // I have this flag because I do not want to change the initialization
  // of the output to the generic output.  It might break something;
  this->SetOutput(vtkDataObject::New());
  this->OutputsInitialized = 0;
  // Releasing data for pipeline parallism.
  // Filters will know it is empty. 
  this->Outputs[0]->ReleaseData();
  this->Outputs[0]->Delete();

  this->DOM = 0;
  this->FormatMulti = 0;
  this->Transform = 0;
  this->DataDescription = 0;
  this->ArrayConverter = vtkXdmfDataArray::New();

  this->PointDataArraySelection = vtkDataArraySelection::New();
  this->CellDataArraySelection = vtkDataArraySelection::New();

  // Setup the selection callback to modify this object when an array
  // selection is changed.
  this->SelectionObserver = vtkCallbackCommand::New();
  this->SelectionObserver->SetCallback(
    &vtkXdmfReader::SelectionModifiedCallback);
  this->SelectionObserver->SetClientData(this);
  this->PointDataArraySelection->AddObserver(vtkCommand::ModifiedEvent,
                                             this->SelectionObserver);
  this->CellDataArraySelection->AddObserver(vtkCommand::ModifiedEvent,
                                            this->SelectionObserver);

  this->DomainName = 0;
  this->GridName = 0;
  
  this->Grid = 0;

  for (int i = 0; i < 3; i ++ )
    {
    this->Stride[i] = 1;
    }

}

//----------------------------------------------------------------------------
vtkXdmfReader::~vtkXdmfReader()
{
  if ( this->DOM )
    {
    delete this->DOM;
    }
  if ( this->FormatMulti )
    {
    delete this->FormatMulti;
    }
  if ( this->Transform )
    {
    delete this->Transform;
    }
  this->ArrayConverter->Delete();

  this->CellDataArraySelection->RemoveObserver(this->SelectionObserver);
  this->PointDataArraySelection->RemoveObserver(this->SelectionObserver);
  this->SelectionObserver->Delete();
  this->CellDataArraySelection->Delete();
  this->PointDataArraySelection->Delete();

  this->SetDomainName(0);
  this->SetGridName(0);
  delete this->Internals;
}

vtkDataSet *vtkXdmfReader::GetOutput()
{
  if ( ! this->OutputsInitialized)
    {
    this->ExecuteInformation();
    }
  if (this->NumberOfOutputs < 1)
    {
    return NULL;
    }
  
  return static_cast<vtkDataSet *>(this->Outputs[0]);
}

//----------------------------------------------------------------------------
void vtkXdmfReader::SetOutput(vtkDataSet *output)
{
  this->OutputsInitialized = 1;
  this->vtkSource::SetNthOutput(0, output);
}

//----------------------------------------------------------------------------
void vtkXdmfReader::SetOutput(vtkDataObject *output)
{
  this->OutputsInitialized = 1;
  this->vtkSource::SetNthOutput(0, output);
}

//----------------------------------------------------------------------------
vtkDataSet *vtkXdmfReader::GetOutput(int idx)
{
  if ( ! this->OutputsInitialized)
    {
    this->ExecuteInformation();
    }
  return static_cast<vtkDataSet *>( this->Superclass::GetOutput(idx) ); 
}

//----------------------------------------------------------------------------
void vtkXdmfReader::Execute()
{
  if ( !this->FileName )
    {
    vtkErrorMacro("File name not set");
    return;
    }

  if ( !this->DOM )
    {
    return;
    }
  if ( !this->FormatMulti )
    {
    return;
    }
  if ( !this->Transform)
    {
    return;
    }
  if ( !this->DataDescription )
    {
    return;
    }

  this->Grid->Update();

  vtkIdType cc;
  // XdmfXNode *attrNode = this->DOM->FindElement("Attribute");
  // XdmfXNode *dataNode = this->DOM->FindElement("DataStructure", 0, attrNode);
  XdmfXNode *attrNode;
  XdmfXNode *dataNode;
  // XdmfInt64 start[3]  = { 0, 0, 0 };
  // XdmfInt64 stride[3] = { 1, 1, 1 };
  // XdmfInt64 count[3] = { 0, 0, 0 };
  // XdmfInt64 end[3] = { 0, 0, 0 };
  // Must Account for Vectors and Tensors
  XdmfInt64 start[4]  = { 0, 0, 0, 0 };
  XdmfInt64 stride[4] = { 1, 1, 1, 1 };
  XdmfInt64 count[4] = { 0, 0, 0, 0 };
  // XdmfInt64 end[4] = { 0, 0, 0, 0 };
  this->DataDescription->GetShape(count);

  int upext[6];
  int whext[6];
  this->GetOutput()->GetUpdateExtent(upext);
  this->GetOutput()->GetWholeExtent(whext);

  vtkDebugMacro( << "In Execute: Update Extents = " << upext[0] << ", " << upext[1] << ", " << upext[2] << ", " << upext[3] << ", " << upext[4] << ", " << upext[5]);

  int ext_fixed =0;
  for ( cc = 0; cc < 3; cc ++ )
    {
    if ( upext[cc*2] == upext[cc*2+1] )
      {
      if ( upext[cc*2] == whext[cc*2+1] )
        {
        upext[cc*2] = whext[cc*2+1]-1;
        }
      else
        {
        upext[cc*2+1] = upext[cc*2+1] + 1;
        }
      ext_fixed = 1;
      }
    }
  vtkDebugMacro( << "After fixing extent: Update Extents = " << upext[0] << ", " << upext[1] << ", " << upext[2] << ", " << upext[3] << ", " << upext[4] << ", " << upext[5]);

  start[2] = vtkMAX(0, upext[0]);
  start[1] = vtkMAX(0, upext[2]);
  start[0] = vtkMAX(0, upext[4]);

  count[2] = upext[1] - upext[0];
  count[1] = upext[3] - upext[2];
  count[0] = upext[5] - upext[4];
  
  XdmfGeometry  *Geometry = this->Grid->GetGeometry();
  // Read Topology for Unstructured Grid
  if( this->Grid->GetClass() == XDMF_UNSTRUCTURED ) 
    {
    vtkUnstructuredGrid  *vGrid = 
      static_cast<vtkUnstructuredGrid *>(this->Outputs[0]);
    vtkCellArray                *verts;
    XdmfInt32           vType;
    XdmfInt32           NodesPerElement;
    vtkIdType           NumberOfElements;
    vtkIdType           i, j, index;    
    XdmfInt64           Length, *Connections;
    vtkIdType           *connections;
    int                 *cell_types, *ctp;

    vtkDebugMacro(<< "Unstructred Topology is " 
                  << this->Grid->GetTopologyTypeAsString() );
    switch ( this->Grid->GetTopologyType() )
      {
      case  XDMF_POLYVERTEX :
        vType = VTK_POLY_VERTEX;
        break;
      case  XDMF_POLYLINE :
        vType = VTK_POLY_LINE;
        break;
      case  XDMF_POLYGON :
        vType = VTK_POLYGON;
        break;
      case  XDMF_TRI :
        vType = VTK_TRIANGLE;
        break;
      case  XDMF_QUAD :
        vType = VTK_QUAD;
        break;
      case  XDMF_TET :
        vType = VTK_TETRA;
        break;
      case  XDMF_PYRAMID :
        vType = VTK_PYRAMID;
        break;
      case  XDMF_WEDGE :
        vType = VTK_WEDGE;
        break;
      case  XDMF_HEX :
        vType = VTK_HEXAHEDRON;
        break;
      default :
        XdmfErrorMessage("Unknown Topology Type");
        return;
      }
    NodesPerElement = this->Grid->GetNodesPerElement();
        
    /* Create Cell Type Array */
    Length = this->Grid->GetConnectivity()->GetNumberOfElements();
    Connections = new XdmfInt64[ Length ];
    this->Grid->GetConnectivity()->GetValues( 0, Connections, Length );
        
    NumberOfElements = this->Grid->GetShapeDesc()->GetNumberOfElements();
    ctp = cell_types = new int[ NumberOfElements ];

    /* Create Cell Array */
    verts = vtkCellArray::New();

    /* Get the pointer */
    connections = verts->WritePointer(NumberOfElements,
                                      NumberOfElements * ( 1 + NodesPerElement ));

    /* Connections : N p1 p2 ... pN */
    /* i.e. Triangles : 3 0 1 2    3 3 4 5   3 6 7 8 */
    index = 0;
    for( j = 0 ; j < NumberOfElements; j++ )
      {
      *ctp++ = vType;
      *connections++ = NodesPerElement;
      for( i = 0 ; i < NodesPerElement; i++ )
        {
        *connections++ = Connections[index++];
        }
      }
    delete [] Connections;
    vGrid->SetCells(cell_types, verts);
    /* OK, because of reference counting */
    verts->Delete();
    delete [] cell_types;
    vGrid->Modified();
    }  // if( this->Grid->GetClass() == XDMF_UNSTRUCTURED ) 
    else
    {
    if( (this->Grid->GetTopologyType() == XDMF_2DSMESH ) ||
		(this->Grid->GetTopologyType() == XDMF_3DSMESH ) ){
		vtkDebugMacro( << "Setting Extents for vtkStructuredGrid");
		vtkStructuredGrid  *vGrid = (vtkStructuredGrid *)this->Outputs[0];
    		vGrid->SetExtent(upext);    
    } else {
		vtkRectilinearGrid *vGrid = (vtkRectilinearGrid *)this->Outputs[0];
    		vGrid->SetExtent(upext);    
	}
    }
  // Read Geometry
  if( ( Geometry->GetGeometryType() == XDMF_GEOMETRY_X_Y_Z ) ||
      ( Geometry->GetGeometryType() == XDMF_GEOMETRY_XYZ ) ||
      ( Geometry->GetGeometryType() == XDMF_GEOMETRY_XY ) )
    {
    float *pp;
    XdmfInt64   Length;
    vtkPoints   *Points;
    vtkPointSet *Pointset = ( vtkPointSet *)this->Outputs[0];

    Points = Pointset->GetPoints();
    if( !Points )
      {
      vtkDebugMacro(<<  "Creating vtkPoints" );
      Points = vtkPoints::New();
      Pointset->SetPoints( Points );
      // OK Because of Reference Counting
      Points->Delete();
      }
        
    if( Geometry->GetPoints() )
      {
      if( Points )
        {
        Length = Geometry->GetPoints()->GetNumberOfElements();
        vtkDebugMacro( << "Setting Array of " << (int)Length << " = " 
                       << (int)Geometry->GetNumberOfPoints() << " Points");
        Points->SetNumberOfPoints( Geometry->GetNumberOfPoints() );
        pp = Points->GetPoint( 0 );
        if( sizeof( float ) == sizeof( XdmfFloat32 ) ) 
          {
          Geometry->GetPoints()->GetValues( 0, (XdmfFloat32 *)pp, Length );
          } 
        else if( sizeof( float ) == sizeof( XdmfFloat64 )) 
          {
          Geometry->GetPoints()->GetValues( 0, (XdmfFloat64 *)pp, Length );
          } 
        else 
          {
          XdmfFloat64   *TmpPp, *TmpPoints = new XdmfFloat64[ Length ];
          XdmfInt64     i;

          Geometry->GetPoints()->GetValues( 0, TmpPoints, Length );
          TmpPp = TmpPoints;
          for( i = 0 ; i < Length ; i++ )
            {
            *pp++ = *TmpPp++;
            }
          delete TmpPoints;
          }
        Points->Modified();
        Pointset->Modified();
        } 
      else 
        {
        XdmfErrorMessage("Base Grid Has No Points");
        return;
        }
      } 
    else 
      {
      XdmfErrorMessage("No Points to Set");
      return;
      }
    }
  else
    {
    // Special Rectilinear and CoRectilinear Geometries
    XdmfTopology        *Topology = this->Grid;
    vtkIdType Index;
    vtkRectilinearGrid *vGrid = (vtkRectilinearGrid *)this->Outputs[0];

    vtkDoubleArray      *XCoord, *YCoord, *ZCoord;
    XdmfFloat64 *Origin;
    XdmfInt64   Dimensions[3] = { 0, 0, 0 };

    // Make Sure Grid Has Coordinates
    Topology->GetShapeDesc()->GetShape( Dimensions );
    
    XCoord = vtkDoubleArray::New();
    vGrid->SetXCoordinates( XCoord );
    // OK Because of Reference Counting
    XCoord->Delete();   
    XCoord->SetNumberOfValues( count[2]+1 );
    YCoord = vtkDoubleArray::New();
    vGrid->SetYCoordinates( YCoord );
    // OK Because of Reference Counting
    YCoord->Delete();   
    YCoord->SetNumberOfValues( count[1]+1 );
    ZCoord = vtkDoubleArray::New();
    vGrid->SetZCoordinates( ZCoord );
    // OK Because of Reference Counting
    ZCoord->Delete();   
    ZCoord->SetNumberOfValues( count[0]+1 );

    // Build Vectors if nescessary
    if( Geometry->GetGeometryType() == XDMF_GEOMETRY_ORIGIN_DXDYDZ )
      {
      if( !Geometry->GetVectorX() )
        {
        Geometry->SetVectorX( new XdmfArray );
        Geometry->GetVectorX()->SetNumberType( XDMF_FLOAT32_TYPE );
        }
      if( !Geometry->GetVectorY() )
        {
        Geometry->SetVectorY( new XdmfArray );
        Geometry->GetVectorY()->SetNumberType( XDMF_FLOAT32_TYPE );
        }
      if( !Geometry->GetVectorZ() )
        {
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
    int sstart[3];
    sstart[0] = start[0];
    sstart[1] = start[1];
    sstart[2] = start[2];

    vtkIdType cstart[3] = { 0, 0, 0 };
    vtkIdType cend[3];
    cstart[0] = vtkMAX(0, sstart[2]);
    cstart[1] = vtkMAX(0, sstart[1]);
    cstart[2] = vtkMAX(0, sstart[0]);

    cend[0] = start[2] + count[2]*this->Stride[0]+1;
    cend[1] = start[1] + count[1]*this->Stride[1]+1;
    cend[2] = start[0] + count[0]*this->Stride[2]+1;

    vtkDebugMacro ( << "CStart: " << cstart[0] << ", " 
                                  << cstart[1] << ", " 
                                  << cstart[2] );
    vtkDebugMacro ( << "CEnd: " << cend[0] << ", " 
                                << cend[1] << ", " 
                                << cend[2] );

    // Set the Points
    for( Index = cstart[0], cc = 0 ; Index < cend[0] ; Index += this->Stride[0] )
      {
      XCoord->SetValue( cc++, 
                        Geometry->GetVectorX()->GetValueAsFloat32( Index ) ) ;
      } 
    for( Index = cstart[1], cc = 0 ; Index < cend[1] ; Index+= this->Stride[1] )
      {
      YCoord->SetValue( cc++ , 
                        Geometry->GetVectorY()->GetValueAsFloat32( Index ) );
      } 
    for( Index = cstart[2], cc = 0 ; Index < cend[2] ; Index += this->Stride[2] )
      {
      ZCoord->SetValue( cc++ , 
                        Geometry->GetVectorZ()->GetValueAsFloat32( Index ) );
      }

    stride[2] = this->Stride[0];
    stride[1] = this->Stride[1];
    stride[0] = this->Stride[2];

    // vGrid->SetExtent(upext);    
    }
  vtkDataSet* dataSet = ( vtkDataSet * )this->Outputs[0];
  for ( cc = 0; cc < dataSet->GetPointData()->GetNumberOfArrays(); cc ++ )
    {
    dataSet->GetPointData()->RemoveArray(
      dataSet->GetPointData()->GetArrayName(cc));
    }
  for( cc = 0 ; cc < this->Grid->GetNumberOfAttributes() ; cc++ )
    {
    XdmfInt32 AttributeCenter;
    XdmfAttribute       *Attribute;
    char		*NodeType;
    Attribute = this->Grid->GetAttribute( cc );
    char *name = Attribute->GetName();
    int status = 1;
    AttributeCenter = Attribute->GetAttributeCenter();
    if (name )
      {
      if ( AttributeCenter == XDMF_ATTRIBUTE_CENTER_GRID || 
           AttributeCenter == XDMF_ATTRIBUTE_CENTER_NODE)
        {
        status = this->PointDataArraySelection->ArrayIsEnabled(name);
        }
      else
        {
        status = this->CellDataArraySelection->ArrayIsEnabled(name);
        }
      }
    // attrNode = this->DOM->FindElement("Attribute", cc);
    attrNode = Attribute->GetCurrentElement();
    // dataNode = this->DOM->FindElement("DataStructure", 0, attrNode);
    // Find the DataTransform or DataStructure below the <Attribute>
    dataNode = this->DOM->FindElement(NULL, 0, attrNode);
    NodeType = this->DOM->Get(dataNode, "NodeType" );
    // this->DataDescription is a Copy so Delete it later
    if( XDMF_WORD_CMP(NodeType, "DataTransform") ){
  	this->DataDescription = this->Transform->ElementToDataDesc( dataNode );
    } else {
  	this->DataDescription = this->FormatMulti->ElementToDataDesc( dataNode );
    }

    if ( Attribute && status )
      {
      //Attribute->Update();
      XdmfInt32 AttributeType;
      XdmfTransform Trans;
      XdmfArray *values;

      this->DataDescription->SelectHyperSlab(start, stride, count);
      // vtkDebugMacro( << "Reading From Element " << this->DOM->Serialize(dataNode) );
      // vtkDebugMacro( << "Dataset = " << this->DOM->Get(dataNode, "CData"));
      vtkDebugMacro( << "Dims = " << this->DataDescription->GetShapeAsString());
      vtkDebugMacro( << "Slab = " << this->DataDescription->GetHyperSlabAsString());
      /*
      XdmfArray *values = this->FormatMulti->ElementToArray( 
        dataNode, this->DataDescription );
      */
    // if( !XDMF_WORD_CMP(NodeType, "DataTransform") &&
    //   ( this->Grid->GetTopologyType() == XDMF_3DRECTMESH ) ){
    if( XDMF_WORD_CMP(NodeType, "DataStructure") &&
	( this->Grid->GetTopologyType() == XDMF_3DRECTMESH )){
	// Only works for the CTH-like special case
        vtkDebugMacro( << "Preparing to Read :" << this->DOM->Get(dataNode, "CData"));
        values = this->FormatMulti->ElementToArray(dataNode, this->DataDescription);
    } else {
	Trans.SetDOM(this->DOM);
	values = Trans.ElementToArray( dataNode );
	}
      this->ArrayConverter->SetVtkArray( NULL );
      vtkDataArray* vtkValues = this->ArrayConverter->FromXdmfArray(
        values->GetTagName());
      
      vtkDebugMacro ( << "Reading array: " << name );
      vtkValues->SetName(name);
      AttributeType = Attribute->GetAttributeType();
      // Special Cases
      if( AttributeCenter == XDMF_ATTRIBUTE_CENTER_GRID ) 
        {
        // Implement XDMF_ATTRIBUTE_CENTER_GRID as PointData
        XdmfArray *tmpArray = new XdmfArray;

        vtkDebugMacro(<< "Setting Grid Centered Values");
        tmpArray->CopyType( values );
        tmpArray->SetNumberOfElements( dataSet->GetNumberOfPoints() );
        tmpArray->Generate( values->GetValueAsFloat64(0), 
                            values->GetValueAsFloat64(0) );
        vtkValues->Delete();
        this->ArrayConverter->SetVtkArray( NULL );
        vtkValues = this->ArrayConverter->FromXdmfArray(tmpArray->GetTagName());
        if( !name )
          {
          name = values->GetTagName();
          }
        vtkValues->SetName( name );
        delete tmpArray;
        AttributeCenter = XDMF_ATTRIBUTE_CENTER_NODE;
        }
      switch (AttributeCenter)
        {
        case XDMF_ATTRIBUTE_CENTER_NODE :
          dataSet->GetPointData()->RemoveArray(name);
          dataSet->GetPointData()->AddArray(vtkValues);
          switch( AttributeType )
            {
            case XDMF_ATTRIBUTE_TYPE_SCALAR :
              dataSet->GetPointData()->SetActiveScalars( name );
              break;
            case XDMF_ATTRIBUTE_TYPE_VECTOR :
              dataSet->GetPointData()->SetActiveVectors( name );
              break;
            case XDMF_ATTRIBUTE_TYPE_TENSOR :
              dataSet->GetPointData()->SetActiveTensors( name );
              break;
            default :
              break;
            }
          break;
        case XDMF_ATTRIBUTE_CENTER_CELL :
          dataSet->GetCellData()->RemoveArray(name);
          dataSet->GetCellData()->AddArray(vtkValues);
          switch( AttributeType )
            {
            case XDMF_ATTRIBUTE_TYPE_SCALAR :
              dataSet->GetCellData()->SetActiveScalars( name );
              break;
            case XDMF_ATTRIBUTE_TYPE_VECTOR :
              dataSet->GetCellData()->SetActiveVectors( name );
              break;
            case XDMF_ATTRIBUTE_TYPE_TENSOR :
              dataSet->GetCellData()->SetActiveTensors( name );
              break;
            default :
              break;
            }
          break;
        default : 
          vtkErrorMacro(<< "Can't Handle Values at " 
                        << Attribute->GetAttributeCenterAsString());
          break;
        }
      if ( vtkValues )
        {
        vtkValues->Delete();
        }
      if ( this->DataDescription ) {
	 delete this->DataDescription;
	}
      }
    }
}

//----------------------------------------------------------------------------
void vtkXdmfReader::ExecuteInformation()
{
  vtkIdType cc;
  char         *CurrentFileName;
  XdmfInt32    Rank;
  XdmfInt64    Dimensions[ XDMF_MAX_DIMENSION ];
  XdmfInt64    EndExtent[ XDMF_MAX_DIMENSION ];
  
  if ( !this->FileName )
    {
    vtkErrorMacro("File name not set");
    return;
    }
  // First make sure the file exists.  This prevents an empty file
  // from being created on older compilers.
  struct stat fs;
  if(stat(this->FileName, &fs) != 0)
    {
    vtkErrorMacro("Error opening file " << this->FileName);
    return;
    }
  if ( !this->DOM )
    {
    this->DOM = new XdmfDOM();
    }
  if ( !this->Transform )
    {
    this->Transform = new XdmfTransform();
    this->Transform->SetDOM(this->DOM);
    }
  if ( !this->FormatMulti )
    {
    this->FormatMulti = new XdmfFormatMulti();
    this->FormatMulti->SetDOM(this->DOM);
    }
  CurrentFileName = this->DOM->GetInputFileName();
  // Don't Re-Parse : Could Reset Parameters
  if((CurrentFileName == NULL) || (STRCASECMP(CurrentFileName, this->FileName) != 0 )){
  	this->DOM->SetInputFileName(this->FileName);
  	vtkDebugMacro( << "...............Preparing to Parse " << this->FileName);
  	this->DOM->Parse();
	}

  XdmfXNode *domain = 0;
  int done = 0;
  char buffer[100];
  this->Internals->DomainList.erase(this->Internals->DomainList.begin(),
                                    this->Internals->DomainList.end());
  for ( cc = 0; !done; cc ++ )
    {
    domain = this->DOM->FindElement("Domain", cc);
    if ( !domain )
      {
      break;
      }
    char *Name = this->DOM->Get( domain, "Name" );
    if ( !Name )
      {
      sprintf(buffer, "Domain%d", cc);
      Name = buffer;
      }
    this->Internals->DomainList.push_back(Name);
    }
  if ( this->DomainName )
    {
    for ( cc = 0; !done; cc ++ )
      {
      domain = this->DOM->FindElement("Domain", cc);
      if ( !domain )
        {
        break;
        }
      char *Name = this->DOM->Get( domain, "Name" );
      if ( !Name )
        {
        sprintf(buffer, "Domain%d", cc);
        Name = buffer;
        }
      if( Name && strcmp(Name, this->DomainName) == 0) 
        {
        break;
        }      
      }
    }
  
  if ( !domain )
    {
    domain = this->DOM->FindElement("Domain", 0); // 0 - domain index    
    }

  if ( !domain )
    {
    vtkErrorMacro("Cannot find any domain...");
    return;
    }

  
  XdmfXNode *gridNode = 0;
  this->Internals->GridList.erase(this->Internals->GridList.begin(),
                                  this->Internals->GridList.end());
  for ( cc = 0; !done; cc ++ )
    {
    gridNode = this->DOM->FindElement("Grid", cc, domain);
    if ( !gridNode )
      {
      break;
      }
    char *Name = this->DOM->Get( gridNode, "Name" );
    if ( !Name )
      {
      sprintf(buffer, "Grid%d", cc);
      Name = buffer;
      }
    this->Internals->GridList.push_back(Name);
    }
  if ( this->GridName )
    {
    for ( cc = 0; !done; cc ++ )
      {
      gridNode = this->DOM->FindElement("Grid", cc, domain);
      if ( !gridNode )
        {
        break;
        }
      char *Name = this->DOM->Get( gridNode, "Name" );
      if ( !Name )
        {
        sprintf(buffer, "Grid%d", cc);
        Name = buffer;
        }
      if( Name && strcmp(Name, this->GridName) == 0) 
        {
        break;
        }      
      }
    }
  if ( !gridNode )
    {
    gridNode = this->DOM->FindElement("Grid", 0, domain); // 0 - grid index    
    }

  if ( !gridNode )
    {
    vtkErrorMacro("Cannot find any grid...");
    return;
    }

  if (!this->Grid)
    {
    this->Grid = new XdmfGrid;
    }
  this->Grid->SetDOM( this->DOM );
  this->Grid->InitGridFromElement( gridNode );

  char* filename = 0;
  if ( this->FileName )
    {
    filename = new char[ strlen(this->FileName)+ 1];
    strcpy(filename, this->FileName);
    }
  int len = static_cast<int>(strlen(filename));
  for ( cc = len-1; cc >= 0; cc -- )
    {
    if ( filename[cc] != '/' && filename[cc] != '\\' )
      {
      filename[cc] = 0;
      }
    else
      {
      break;
      }
    }
  if ( filename[0] == 0 )
    {
    char buffer[1024];
    if ( GETCWD(buffer, 1023) )
      {
      delete [] filename;
      if ( buffer )
        {
        filename = new char[ strlen(buffer)+ 1];
        strcpy(filename, buffer);
        }
      }
    }
  this->DOM->SetWorkingDirectory(filename);
  delete [] filename;

  vtkDataObject* vGrid = 0;

  if( this->Grid->GetClass() == XDMF_UNSTRUCTURED ) 
    {
    vGrid = vtkUnstructuredGrid::New();
    vGrid->SetMaximumNumberOfPieces(1);
    } 
  else 
    {
    if( ( this->Grid->GetTopologyType() == XDMF_2DSMESH ) ||
        ( this->Grid->GetTopologyType() == XDMF_3DSMESH ) )
      {
      vGrid = vtkStructuredGrid::New();
      } 
    else 
      {
      vGrid = vtkRectilinearGrid::New();
      }
    }
  int type_changed = 0;
  if ( vGrid )
    {
    // Put this here so that GetOutput 
    // does not call ExecuteInfomrtion again.
    this->OutputsInitialized = 1;
    if ( this->GetOutput()->GetClassName() != vGrid->GetClassName() )
      {
      type_changed = 1;
      this->SetOutput(vGrid );
      this->Outputs[0]->ReleaseData();
      }
    vGrid->Delete();
    }


  if ( type_changed )
    {
    this->PointDataArraySelection->RemoveAllArrays();
    this->CellDataArraySelection->RemoveAllArrays();
    }
  for( cc = 0 ; cc < this->Grid->GetNumberOfAttributes() ; cc++ )
    {
    XdmfAttribute       *Attribute;
    Attribute = this->Grid->GetAttribute( cc );
    char *name = Attribute->GetName();
    if (name )
      {
      XdmfInt32 AttributeCenter = Attribute->GetAttributeCenter();
      if ( AttributeCenter == XDMF_ATTRIBUTE_CENTER_GRID || 
           AttributeCenter == XDMF_ATTRIBUTE_CENTER_NODE)
        {
        if ( !this->PointDataArraySelection->ArrayExists(name) )
          {
          this->PointDataArraySelection->AddArray(name);
          }
        }
      else
        {
        if ( !this->CellDataArraySelection->ArrayExists(name) )
          {
          this->CellDataArraySelection->AddArray(name);
          }
        }
      }
    }

  //this->Grid->Update();

  /* Bad Idea -- Use the DataDesc of the Grid Instead */
/*
  XdmfXNode *attrNode = this->DOM->FindElement("Attribute");
  XdmfXNode *dataNode = this->DOM->FindElement(NULL, 0, attrNode);
  if( XDMF_WORD_CMP(this->DOM->Get(dataNode, "NodeType" ), "DataTransform") ){
  	this->DataDescription = this->Transform->ElementToDataDesc( dataNode );
  } else {
  	this->DataDescription = this->FormatMulti->ElementToDataDesc( dataNode );
  }
  XdmfInt64 shape[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  XdmfInt32 res = 0;
  if (this->Grid->GetGeometry()->GetGeometryType() == XDMF_GEOMETRY_VXVYVZ)
    {
    XdmfXNode* geoNode = this->DOM->FindElement("Geometry", 0, gridNode );
    XdmfXNode* XDataStructure = this->DOM->FindElement("DataStructure", 
                                                       0, 
                                                       geoNode);
    XdmfXNode* YDataStructure = this->DOM->FindElement("DataStructure", 
                                                       1, 
                                                       geoNode);
    XdmfXNode* ZDataStructure = this->DOM->FindElement("DataStructure", 
                                                       2, 
                                                       geoNode);
    char *sxdim = this->DOM->Get( XDataStructure, "Dimensions" );
    char *sydim = this->DOM->Get( YDataStructure, "Dimensions" );
    char *szdim = this->DOM->Get( ZDataStructure, "Dimensions" ); 
    shape[2] = atoi(sxdim)-1;
    shape[1] = atoi(sydim)-1;
    shape[0] = atoi(szdim)-1;
    res = 3;
    }
  else
    {
    res = this->DataDescription->GetShape(shape);
    }
 */ 
 // Revised Initial Setup
  this->DataDescription = this->Grid->GetShapeDesc();
  Rank = this->DataDescription->GetShape( Dimensions );
  for(int i = Rank ; i < 3 ; i++){
    Dimensions[i] = 1;
  }
  // End Extent is Dim - 1
  EndExtent[0] = Dimensions[0] - 1;
  EndExtent[1] = Dimensions[1] - 1;
  EndExtent[2] = Dimensions[2] - 1;
  // vtk Dims are i,j,k XDMF are k,j,i
  EndExtent[0] = vtkMAX(1, EndExtent[0]) / this->Stride[2];
  EndExtent[1] = vtkMAX(1, EndExtent[1]) / this->Stride[1];
  EndExtent[2] = vtkMAX(1, EndExtent[2]) / this->Stride[0];
  vtkDebugMacro( << "EndExtents = " << (vtkIdType)EndExtent[0] << ", " 
     << (vtkIdType)EndExtent[1] << ", " << (vtkIdType)EndExtent[2]);
  this->Outputs[0]->SetWholeExtent(0, EndExtent[2],
                                   0, EndExtent[1],
                                   0, EndExtent[0]);
  vtkDebugMacro( << "Grid Type = " << Grid->GetTopologyTypeAsString() << " = " << this->Grid->GetTopologyType());
  if( this->Grid->GetClass() != XDMF_UNSTRUCTURED ) {
	if( (this->Grid->GetTopologyType() == XDMF_2DSMESH ) ||
		(this->Grid->GetTopologyType() == XDMF_3DSMESH ) ){
		vtkDebugMacro( << "Setting Extents for vtkStructuredGrid");
		vtkStructuredGrid  *vGrid = (vtkStructuredGrid *)this->Outputs[0];
		vGrid->SetDimensions(EndExtent[2] + 1,
				EndExtent[1] + 1,
				EndExtent[0] + 1);
  		vGrid->SetWholeExtent(0, EndExtent[2],
                                   0, EndExtent[1],
                                   0, EndExtent[0]);
  		vGrid->SetExtent(0, EndExtent[2],
                                   0, EndExtent[1],
                                   0, EndExtent[0]);
	} else {
		vtkDebugMacro( << "Setting Extents for vtkRectilinearGrid");
		vtkRectilinearGrid *vGrid = (vtkRectilinearGrid *)this->Outputs[0];
		vGrid->SetDimensions(EndExtent[2] + 1,
				EndExtent[1] + 1,
				EndExtent[0] + 1);
  		vGrid->SetWholeExtent(0, EndExtent[2],
                                   0, EndExtent[1],
                                   0, EndExtent[0]);
  		vGrid->SetExtent(0, EndExtent[2],
                                   0, EndExtent[1],
                                   0, EndExtent[0]);
	}

  int *upext = this->GetOutput()->GetUpdateExtent();
  vtkDebugMacro( << "Update Extents: " << upext[0] << ", " 
                                << upext[1] << ", " 
                                << upext[2] << ", " 
                                << upext[3] << ", " 
                                << upext[4] << ", " 
                                << upext[5] )
 }
}

//----------------------------------------------------------------------------
void vtkXdmfReader::SelectionModifiedCallback(vtkObject*, unsigned long,
                                             void* clientdata, void*)
{
  static_cast<vtkXdmfReader*>(clientdata)->Modified();
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetNumberOfPointArrays()
{
  return this->PointDataArraySelection->GetNumberOfArrays();
}

//----------------------------------------------------------------------------
const char* vtkXdmfReader::GetPointArrayName(int index)
{
  return this->PointDataArraySelection->GetArrayName(index);
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetPointArrayStatus(const char* name)
{
  return this->PointDataArraySelection->ArrayIsEnabled(name);
}

//----------------------------------------------------------------------------
void vtkXdmfReader::SetPointArrayStatus(const char* name, int status)
{
  if(status)
    {
    this->PointDataArraySelection->EnableArray(name);
    }
  else
    {
    this->PointDataArraySelection->DisableArray(name);
    }
}

//----------------------------------------------------------------------------
void vtkXdmfReader::EnableAllArrays()
{
this->PointDataArraySelection->EnableAllArrays();
this->CellDataArraySelection->EnableAllArrays();
}
//----------------------------------------------------------------------------
void vtkXdmfReader::DisableAllArrays()
{
this->PointDataArraySelection->DisableAllArrays();
this->CellDataArraySelection->DisableAllArrays();
}
//----------------------------------------------------------------------------
int vtkXdmfReader::GetNumberOfCellArrays()
{
  return this->CellDataArraySelection->GetNumberOfArrays();
}

//----------------------------------------------------------------------------
const char* vtkXdmfReader::GetCellArrayName(int index)
{
  return this->CellDataArraySelection->GetArrayName(index);
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetCellArrayStatus(const char* name)
{
  return this->CellDataArraySelection->ArrayIsEnabled(name);
}

//----------------------------------------------------------------------------
void vtkXdmfReader::SetCellArrayStatus(const char* name, int status)
{
  if(status)
    {
    this->CellDataArraySelection->EnableArray(name);
    }
  else
    {
    this->CellDataArraySelection->DisableArray(name);
    }
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetNumberOfDomains()
{
  return this->Internals->DomainList.size();
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetNumberOfGrids()
{
  return this->Internals->GridList.size();
}

//----------------------------------------------------------------------------
const char* vtkXdmfReader::GetDomainName(int idx)
{
  return this->Internals->DomainList[idx].c_str();
}

//----------------------------------------------------------------------------
const char* vtkXdmfReader::GetGridName(int idx)
{
  return this->Internals->GridList[idx].c_str();
}


//----------------------------------------------------------------------------
int vtkXdmfReader::GetNumberOfParameters(){
if(!this->DOM) {
	return(-1);
	}
return(this->DOM->FindNumberOfParameters());
}

//----------------------------------------------------------------------------
const char *vtkXdmfReader::GetParameterName(int index){
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->GetParameter(index);
if(Param) {
	return(Param->GetParameterName());
} else {
	return(0);
}

}
//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterType(const char *Name){
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->FindParameter(Name);
if(Param) {
	return(Param->GetParameterType());
} else {
	return(0);
}

}
//----------------------------------------------------------------------------
char *vtkXdmfReader::GetParameterTypeAsString(const char *Name){

if (this->GetParameterType(Name) == XDMF_PARAMETER_RANGE_TYPE) {
	return("RANGE");
} 
return("LIST");
}
//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterType(int index){
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->GetParameter(index);
if(Param) {
	return(Param->GetParameterType());
} else {
	return(0);
}

}

//----------------------------------------------------------------------------
char *vtkXdmfReader::GetParameterTypeAsString(int index){

if (this->GetParameterType(index) == XDMF_PARAMETER_RANGE_TYPE) {
	return("RANGE");
} 
return("LIST");
}
//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterRange(int index, int Shape[3]){
XdmfParameter *Param;
XdmfArray  *Parray;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->GetParameter(index);
if(Param) {
	if( Param->GetParameterType() == XDMF_PARAMETER_RANGE_TYPE ){
		Parray = Param->GetArray();
		Shape[0] = Parray->GetValueAsInt64(0);
		Shape[1] = Parray->GetValueAsInt64(1);
		Shape[2] = Parray->GetValueAsInt64(2);
	} else {
		Shape[0] = 0;
		Shape[1] = 1;
		Shape[2] = Param->GetNumberOfElements();
	}
	return(Shape[2]);
}
return(0);
}
//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterRange(const char *Name, int Shape[3]){
XdmfParameter *Param;
XdmfArray  *Parray;

if(!this->DOM) {
	return(0);
	}
Param = this->DOM->FindParameter(Name);
if(Param) {
	if( Param->GetParameterType() == XDMF_PARAMETER_RANGE_TYPE ){
		Parray = Param->GetArray();
		Shape[0] = Parray->GetValueAsInt64(0);
		Shape[1] = Parray->GetValueAsInt64(1);
		Shape[2] = Parray->GetValueAsInt64(2);
	} else {
		Shape[0] = 0;
		Shape[1] = 1;
		Shape[2] = Param->GetNumberOfElements();
	}
	return(Shape[2]);
}
return(0);
}

//----------------------------------------------------------------------------
char *vtkXdmfReader::GetParameterRangeAsString(int index){
int Range[3];
ostrstream StringOutput;

if(this->GetParameterRange(index, Range) <= 0){
	return(NULL);
}
StringOutput << ICE_64BIT_CAST Range[0] << " ";
StringOutput << ICE_64BIT_CAST Range[1] << " ";
StringOutput << ICE_64BIT_CAST Range[2] << ends;
return(StringOutput.str());
}
//----------------------------------------------------------------------------
char *vtkXdmfReader::GetParameterRangeAsString(const char *Name){
int Range[3];
ostrstream StringOutput;

if (this->GetParameterRange(Name, Range) <= 0) {
	return(NULL);
}
StringOutput << ICE_64BIT_CAST Range[0] << " ";
StringOutput << ICE_64BIT_CAST Range[1] << " ";
StringOutput << ICE_64BIT_CAST Range[2] << ends;
return(StringOutput.str());
}
//----------------------------------------------------------------------------
int vtkXdmfReader::SetParameterIndex(int Index, int CurrentIndex) {
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->GetParameter(Index);
if(!Param) {
	return(-1);
}
this->Modified();
return(Param->SetCurrentIndex(CurrentIndex));
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterIndex(int Index) {
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->GetParameter(Index);
if(!Param) {
	return(-1);
}
return(Param->GetCurrentIndex());
}

//----------------------------------------------------------------------------
int vtkXdmfReader::SetParameterIndex(const char *ParameterName, int CurrentIndex) {
XdmfParameter *Param;
int Status;

if(!this->DOM) {
	return(0);
	}
for(int i=0 ; i < this->DOM->FindNumberOfParameters() ;  i++){
	Param = this->DOM->GetParameter(i);
	if(!Param) {
		return(-1);
	}
	if(XDMF_WORD_CMP(Param->GetParameterName(), ParameterName)){
		Status = Param->SetCurrentIndex(CurrentIndex);
    this->Modified();
		if(Status <= 0 ) return(Status);
		}
}
return(Status);
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterIndex(const char *Name) {
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->FindParameter(Name);
if(!Param) {
	return(-1);
}
return(Param->GetCurrentIndex());
}

//----------------------------------------------------------------------------
const char *vtkXdmfReader::GetParameterValue(const char *Name) {
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->FindParameter(Name);
if(!Param) {
	return(0);
}
Param->Update();
return(Param->GetParameterValue());
}

//----------------------------------------------------------------------------
const char *vtkXdmfReader::GetParameterValue(int index) {
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->GetParameter(index);
if(!Param) {
	return(0);
}
Param->Update();
return(Param->GetParameterValue());
}


//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterLength(int index){
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->GetParameter(index);
if(Param) {
	return(Param->GetNumberOfElements());
} else {
	return(0);
}
}

//----------------------------------------------------------------------------
int vtkXdmfReader::GetParameterLength(const char *Name){
XdmfParameter *Param;


if(!this->DOM) {
	return(0);
	}
Param = this->DOM->FindParameter(Name);
if(Param) {
	return(Param->GetNumberOfElements());
} else {
	return(0);
}
}

//----------------------------------------------------------------------------
const char * vtkXdmfReader::GetXdmfDOMHandle() {
 return( XdmfObjectToHandle( this->DOM ) );
}
//----------------------------------------------------------------------------
const char * vtkXdmfReader::GetXdmfGridHandle() {
 return( XdmfObjectToHandle( this->Grid ) );
}
//----------------------------------------------------------------------------
void vtkXdmfReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "CellDataArraySelection: " << this->CellDataArraySelection 
     << endl;
  os << indent << "PointDataArraySelection: " << this->PointDataArraySelection 
     << endl;
  this->Outputs[0]->PrintSelf(os, indent.GetNextIndent());
}
