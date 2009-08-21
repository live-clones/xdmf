/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXdmfWriter2.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkXdmfWriter2.h"
#include "vtkObjectFactory.h"

#include "vtkDataObject.h"

#include "vtkCompositeDataPipeline.h"
#include "vtkCompositeDataSet.h"
#include "vtkCompositeDataIterator.h"
#include "vtkInformation.h"
#include "vtkFieldData.h"
#include "vtkDataSet.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkImageData.h"
#include "vtkRectilinearGrid.h"
#include "vtkStructuredGrid.h"
#include "vtkSmartPointer.h"
#include "vtkCellType.h"
#include "vtkIdList.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPolyData.h"
#include "vtkGenericCell.h"
#include "vtkCellArray.h"

#include "XdmfDOM.h"
#include "XdmfRoot.h"
#include "XdmfDomain.h"
#include "XdmfGrid.h"
#include "XdmfTopology.h"
#include "XdmfDataDesc.h"
#include "XdmfGeometry.h"
#include "XdmfAttribute.h"
#include "XdmfArray.h"

#include <vtkstd/map>

//==============================================================================

struct vtkXdmfWriter2Internal
{
  class CellType
    {
  public:
    CellType() : VTKType(0), NumPoints(0) {}
    CellType(const CellType& ct) : VTKType(ct.VTKType), NumPoints(ct.NumPoints) {}
    vtkIdType VTKType;
    vtkIdType NumPoints;
    vtkstd_bool operator<(const CellType& ct) const
      {
      return this->VTKType < ct.VTKType || this->VTKType == ct.VTKType && this->NumPoints < ct.NumPoints;
      }
    vtkstd_bool operator==(const CellType& ct) const
      {
      return this->VTKType == ct.VTKType && this->NumPoints == ct.NumPoints;
      }
    CellType& operator=(const CellType& ct)
      {
      this->VTKType = ct.VTKType;
      this->NumPoints = ct.NumPoints;
      return *this;
      }

    };
  typedef vtkstd::map<CellType, vtkSmartPointer<vtkIdList> > MapOfCellTypes;
  static void DetermineCellTypes(vtkPointSet *t, MapOfCellTypes& vec);
};

//----------------------------------------------------------------------------
void vtkXdmfWriter2Internal::DetermineCellTypes(vtkPointSet * t, vtkXdmfWriter2Internal::MapOfCellTypes& vec)
{
  if ( !t )
    {
    return;
    }
  vtkIdType cc;
  vtkGenericCell* cell = vtkGenericCell::New();
  for ( cc = 0; cc < t->GetNumberOfCells(); cc ++ )
    {
    vtkXdmfWriter2Internal::CellType ct;
    t->GetCell(cc, cell);
    ct.VTKType = cell->GetCellType();
    ct.NumPoints = cell->GetNumberOfPoints();
    vtkXdmfWriter2Internal::MapOfCellTypes::iterator it = vec.find(ct);
    if ( it == vec.end() )
      {
      vtkIdList *l = vtkIdList::New();
      it = vec.insert(vtkXdmfWriter2Internal::MapOfCellTypes::value_type(ct, 
          vtkSmartPointer<vtkIdList>(l))).first;
      l->Delete();
      }
    // it->second->InsertUniqueId(cc);;
    it->second->InsertNextId(cc);;
    }
  cell->Delete();
}

//==============================================================================

vtkStandardNewMacro(vtkXdmfWriter2);
vtkCxxRevisionMacro(vtkXdmfWriter2, "1.1");

//----------------------------------------------------------------------------
vtkXdmfWriter2::vtkXdmfWriter2()
{
  this->FileName = NULL;
  this->DOM = NULL;
  this->Piece = 0;  //for parallel
  this->NumberOfPieces = 1;
}

//----------------------------------------------------------------------------
vtkXdmfWriter2::~vtkXdmfWriter2()
{
  this->SetFileName(NULL);
  if (this->DOM)
    {
    delete this->DOM;
    this->DOM = NULL;
    }

  //TODO: Verify memory isn't leaking
}

//-----------------------------------------------------------------------------
vtkExecutive* vtkXdmfWriter2::CreateDefaultExecutive()
{
  return vtkCompositeDataPipeline::New();
}

//----------------------------------------------------------------------------
void vtkXdmfWriter2::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "FileName: " <<
    (this->FileName ? this->FileName : "(none)") << "\n";
}

//------------------------------------------------------------------------------
void vtkXdmfWriter2::SetInput(vtkDataObject *input)
{
  if (input)
    {
    this->SetInputConnection(0, input->GetProducerPort());
    }
  else
    {
    // Setting a NULL input remove the connection.
    this->SetInputConnection(0, NULL);
    }
}

//------------------------------------------------------------------------------
int vtkXdmfWriter2::FillInputPortInformation(int, vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataObject");
  return 1;
}

//------------------------------------------------------------------------------
void vtkXdmfWriter2::WriteData()
{
  //TODO: Time isn't being recognized properly yet
  vtkInformation *inInfo = this->GetInputPortInformation(0);
  int nTimes = 0;
  double *times = NULL;
  if (inInfo->Has(vtkStreamingDemandDrivenPipeline::TIME_STEPS()))
    {    
    times = inInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
    nTimes = inInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
    }
  
  if (!this->DOM)
    {
    this->DOM = new XdmfDOM();
    }

  //TODO: Create the companion hdf5 heavy data file
/*
  h5 = XdmfHDF()
  h5.CopyType(arr)
  h5.CopyShape(arr)
  h5.Open('XdmfByHand.h5:/Mydata', 'w')
  h5.Write(arr)
  h5.Close()
  dv = XdmfValuesHDF()
  DataXml = dv.DataItemFromHDF('XdmfByHand.h5:/Mydata')
*/

  XdmfRoot root;
  root.SetDOM(this->DOM);
  root.SetVersion(2.2);
  root.Build();
  XdmfDomain *domain = new XdmfDomain();
  root.Insert(domain);

  if (nTimes > 0)
    {
    //TODO: For time 
    //make a new top level collection grid of type temporal
    //iterate myself to get data at each point
    //call what lies below this for each one
    cerr << "In times are: [";
    for (int i = 0; i < nTimes; i++)
      {
      cerr << times[i] << ",";
      }
    cerr << "]" << endl;
    }  
  else
    {
    cerr << "Input data is static" << endl;
    }

  XdmfGrid *grid = new XdmfGrid();
  domain->Insert(grid);
  this->WriteDataSet(this->GetInput(), grid);
  root.Build();
  this->DOM->Write(this->FileName);
  delete grid;
  delete domain;
}

//------------------------------------------------------------------------------
void vtkXdmfWriter2::WriteDataSet(vtkDataObject *dobj, XdmfGrid *grid)
{
  //TODO:
  // respect parallelism

  if (!dobj)
    {
    cerr << "Null DS, someone else will take care of it" << endl;
    return;
    }
  if (!grid)
    {
    cerr << "Something is wrong, grid should have already been created for " << dobj << endl;
    return;
    }

  vtkCompositeDataSet *cdobj = vtkCompositeDataSet::SafeDownCast(dobj);
  if (cdobj)//!dobj->IsTypeOf("vtkCompositeDataSet")) //TODO: Why doesn't IsTypeOf work?
    {
    this->WriteCompositeDataSet(cdobj, grid);
    return;
    }

  this->WriteAtomicDataSet(dobj, grid);
}

//------------------------------------------------------------------------------
void vtkXdmfWriter2::WriteCompositeDataSet(vtkCompositeDataSet *dobj, XdmfGrid *grid)
{
  cerr << "internal node " << dobj << " is a " << dobj->GetClassName() << endl;
  if (dobj->IsA("vtkMultiPieceDataSet"))
    {
    grid->SetGridType(XDMF_GRID_COLLECTION);
    grid->SetCollectionType(XDMF_GRID_COLLECTION_SPATIAL);
    }
  else
    {
    //fine for vtkMultiBlockDataSet
    //vtkHierarchicalBoxDataSet would be better served by a different xdmf tree type
    //vtkTemporalDataSet is internal to the VTK pipeline so I am ingnoring it
    grid->SetGridType(XDMF_GRID_TREE);
    }
 
  vtkCompositeDataIterator* iter = dobj->NewIterator();
  iter->VisitOnlyLeavesOff();
  iter->TraverseSubTreeOff();
  iter->GoToFirstItem();
  while (!iter->IsDoneWithTraversal())
    {
    XdmfGrid *childsGrid = new XdmfGrid();
    childsGrid->SetDeleteOnGridDelete(1);
    grid->Insert(childsGrid);    
    vtkDataObject* ds = iter->GetCurrentDataObject();
    this->WriteDataSet(ds, childsGrid);
    //delete childsGrid; //parent deletes children in Xdmf. Doing so here segfaults.
    iter->GoToNextItem();
    }
  iter->Delete();
  return;
}

//------------------------------------------------------------------------------
void vtkXdmfWriter2::WriteAtomicDataSet(vtkDataObject *dobj, XdmfGrid *grid)
{
  vtkDataSet *ds = vtkDataSet::SafeDownCast(dobj);
  if (!ds)
    {
    //TODO: Fill in non Vis data types
    cerr << "Can not convert " << dobj->GetClassName() << " to XDMF yet." << endl;
    return;
    }

  cerr << "Leaf node " << ds << " is a " << ds->GetClassName() << endl;
  grid->SetGridType(XDMF_GRID_UNIFORM);

  //Topology
  XdmfTopology *t = grid->GetTopology();
  switch (ds->GetDataObjectType()) {
  case VTK_STRUCTURED_POINTS:
  case VTK_IMAGE_DATA:
  case VTK_UNIFORM_GRID:
    {
    t->SetTopologyType(XDMF_3DCORECTMESH);
    vtkImageData *id = vtkImageData::SafeDownCast(ds);
    int wExtent[6];
    id->GetWholeExtent(wExtent);
    XdmfInt64 Dims[3];
    Dims[0] = wExtent[1] - wExtent[0] + 1;
    Dims[1] = wExtent[3] - wExtent[2] + 1;
    Dims[2] = wExtent[5] - wExtent[4] + 1;
    XdmfDataDesc *dd = t->GetShapeDesc();
    dd->SetShape(3, Dims);
    //TODO: verify row/column major ordering
    }
    break;
  case VTK_RECTILINEAR_GRID:
    {
    t->SetTopologyType(XDMF_3DCORECTMESH);
    vtkRectilinearGrid *rgrid = vtkRectilinearGrid::SafeDownCast(ds);
    int wExtent[6];
    rgrid->GetWholeExtent(wExtent);
    XdmfInt64 Dims[3];
    Dims[0] = wExtent[1] - wExtent[0] + 1;
    Dims[1] = wExtent[3] - wExtent[2] + 1;
    Dims[2] = wExtent[5] - wExtent[4] + 1;
    XdmfDataDesc *dd = t->GetShapeDesc();
    dd->SetShape(3, Dims);
    //TODO: verify row/column major ordering
    }
    break;
  case VTK_STRUCTURED_GRID:
    {
    t->SetTopologyType(XDMF_3DSMESH);
    vtkStructuredGrid *sgrid = vtkStructuredGrid::SafeDownCast(ds);
    int wExtent[6];
    sgrid->GetWholeExtent(wExtent);
    XdmfInt64 Dims[3];
    Dims[0] = wExtent[1] - wExtent[0] + 1;
    Dims[1] = wExtent[3] - wExtent[2] + 1;
    Dims[2] = wExtent[5] - wExtent[4] + 1;
    XdmfDataDesc *dd = t->GetShapeDesc();
    dd->SetShape(3, Dims); 
    //TODO: verify row/column major ordering
    }
    break;
  case VTK_POLY_DATA:
  case VTK_UNSTRUCTURED_GRID:
    {
    //TODO: flag polydata so it can be read back in as PD instead of UG
    vtkXdmfWriter2Internal::MapOfCellTypes cellTypes;
    vtkXdmfWriter2Internal::DetermineCellTypes(vtkPointSet::SafeDownCast(ds), cellTypes);
    //TODO: When is it beneficial to take advantage of a homogenous topology?
    //if no compelling reason not to used MIXED, then this should go away because
    //this special case code is bound to cause problems and because 
    //it requires and in memory copy instead of pointer reference
    if ( cellTypes.size() == 1 && false)  
      {
      const vtkXdmfWriter2Internal::CellType* ct = &cellTypes.begin()->first;
      vtkIdType ppCell = ct->NumPoints;
      switch(ct->VTKType) 
        {
        case VTK_VERTEX :
        case VTK_POLY_VERTEX :
          t->SetTopologyType(XDMF_POLYVERTEX);
          break;
        case VTK_LINE : 
        case VTK_POLY_LINE :
          t->SetTopologyType(XDMF_POLYLINE);
          t->SetNodesPerElement(ppCell);
          break;
        case VTK_TRIANGLE :
        case VTK_TRIANGLE_STRIP :
          t->SetTopologyType(XDMF_TRI);
          break;
        case VTK_POLYGON :
          t->SetTopologyType(XDMF_POLYGON);
          t->SetNodesPerElement(ppCell);
          break;
        case VTK_PIXEL :
        case VTK_QUAD :
          t->SetTopologyType(XDMF_QUAD);
          break;
        case VTK_TETRA :
          t->SetTopologyType(XDMF_TET);
          break;
        case VTK_VOXEL :
        case VTK_HEXAHEDRON :
          t->SetTopologyType(XDMF_HEX);
          break;
        case VTK_WEDGE :
          t->SetTopologyType(XDMF_WEDGE);
          break;
        case VTK_PYRAMID :
          t->SetTopologyType(XDMF_PYRAMID);
          break;
        case VTK_EMPTY_CELL :
        default :
          t->SetTopologyType(XDMF_NOTOPOLOGY);
          break;
        }
      XdmfArray *di = t->GetConnectivity();
      di->SetNumberType(XDMF_INT32_TYPE); //TODO: What if 64 bit ids?
      XdmfInt64 xppCell = ppCell;
      di->SetShape(ds->GetNumberOfCells(), &xppCell);
      vtkIdList* il = cellTypes[*ct].GetPointer();
      vtkIdList* cellPoints = vtkIdList::New();
      vtkIdType cvnt=0;
      for(vtkIdType i = 0 ; i < ds->GetNumberOfCells(); i++ )
        {
        ds->GetCellPoints(il->GetId(i), cellPoints);
        if ( ct->VTKType == VTK_VOXEL )
          {
          // Hack for VTK_VOXEL          
          di->SetValue(cvnt++, cellPoints->GetId(0));
          di->SetValue(cvnt++, cellPoints->GetId(1));
          di->SetValue(cvnt++, cellPoints->GetId(3));
          di->SetValue(cvnt++, cellPoints->GetId(2));
          di->SetValue(cvnt++, cellPoints->GetId(4));
          di->SetValue(cvnt++, cellPoints->GetId(5));
          di->SetValue(cvnt++, cellPoints->GetId(7));
          di->SetValue(cvnt++, cellPoints->GetId(6));
          }
        else if ( ct->VTKType == VTK_PIXEL )
          {
          // Hack for VTK_PIXEL
          di->SetValue(cvnt++, cellPoints->GetId(0));
          di->SetValue(cvnt++, cellPoints->GetId(1));
          di->SetValue(cvnt++, cellPoints->GetId(3));
          di->SetValue(cvnt++, cellPoints->GetId(2));
          }
        else
          {
          for( vtkIdType j = 0 ; j < ppCell ; j++ )
            {
            di->SetValue(cvnt++, cellPoints->GetId(j));
            }
          }//pd has 4 arrays, so it is rarely homogeoneous
        }
      cellPoints->Delete();
      } //homogenous
    else
      {
      cerr << "NON HOMOGENEOUS" << endl;
      //Non Homogeneous, used mixed topology type to dump them all
      t->SetTopologyType(XDMF_MIXED);
      XdmfArray *di = t->GetConnectivity();
      di->SetNumberType(XDMF_INT32_TYPE); //TODO: What if 64 bit ids?
      di->SetNumberOfElements(ds->GetNumberOfCells()); //TODO: Fill out dimensions in topology and full length here?
      vtkIdTypeArray *da = NULL;
      vtkUnstructuredGrid *ugrid = vtkUnstructuredGrid::SafeDownCast(ds);
      if (ugrid)
        {
        da = ugrid->GetCells()->GetData();
        this->ConvertVToXArray(da, di);
        }
      else
        {
        //TODO: add markers to record starting index of lines, polys and strips
        vtkPolyData *pd = vtkPolyData::SafeDownCast(ds);
        vtkIdType vsize = pd->GetNumberOfVerts();
        vtkIdType lsize = pd->GetNumberOfLines();
        vtkIdType psize = pd->GetNumberOfPolys();
        vtkIdType ssize = pd->GetNumberOfStrips();
        vtkIdType total = vsize+lsize+psize+ssize;
        cerr 
          << vsize << " "
          << lsize << " "
          << psize << " "
          << ssize << " "
          << total << endl;
        if (total == vsize)
          {
          da = pd->GetVerts()->GetData();
          }
        else if (total == lsize)
          {
          da = pd->GetLines()->GetData();
          }
        else if (total == psize)
          {
          da = pd->GetPolys()->GetData();
          }
        else if (total == ssize)
          {
          da = pd->GetStrips()->GetData();
          }
        if (da != NULL)
          {
          this->ConvertVToXArray(da, di);
          }
        else
          {
          //more than one cell array, have to accumulate them together
          //TODO: Is there a valid XDMF way of dumping each one independently? If so we could avoid the copies
          vtkIdTypeArray *accum = vtkIdTypeArray::New();
          accum->SetNumberOfComponents(1);
          accum->SetNumberOfTuples(total);
          vtkIdType *out = accum->GetPointer(0);
          vtkIdType *ptr = out;
          memcpy(ptr, pd->GetVerts()->GetPointer(), vsize*sizeof(vtkIdType));
          ptr+=vsize;
          memcpy(ptr, pd->GetLines()->GetPointer(), lsize*sizeof(vtkIdType));
          ptr+=lsize;
          memcpy(ptr, pd->GetPolys()->GetPointer(), psize*sizeof(vtkIdType));
          ptr+=psize;
          memcpy(ptr, pd->GetStrips()->GetPointer(), ssize*sizeof(vtkIdType));
          this->ConvertVToXArray(accum, di);
          //accum->Delete(); //TODO: Delete this, but not before it is used
          }
        }
      }
    }
    break;
  default:
    t->SetTopologyType(XDMF_GEOMETRY_NONE);
    cerr << "Unrecognized dataset type" << endl;
  }
  
  //Geometry
  XdmfGeometry *geo = grid->GetGeometry();
  switch (ds->GetDataObjectType()) {
  case VTK_STRUCTURED_POINTS:
  case VTK_IMAGE_DATA:
  case VTK_UNIFORM_GRID:
    {
    geo->SetGeometryType(XDMF_GEOMETRY_ORIGIN_DXDYDZ);
    vtkImageData *id = vtkImageData::SafeDownCast(ds);
    geo->SetOrigin(id->GetOrigin());
    geo->SetDxDyDz(id->GetSpacing());
    }
    break;
  case VTK_RECTILINEAR_GRID:
    {
    geo->SetGeometryType(XDMF_GEOMETRY_VXVYVZ);
    vtkRectilinearGrid *rgrid = vtkRectilinearGrid::SafeDownCast(ds);
    vtkDataArray *da;
    da = rgrid->GetXCoordinates();
    XdmfArray *xdax = new XdmfArray;
    this->ConvertVToXArray(da, xdax);
    geo->SetVectorX(xdax);
    da = rgrid->GetYCoordinates();
    XdmfArray *xday = new XdmfArray;
    this->ConvertVToXArray(da, xday);
    geo->SetVectorY(xday);
    da = rgrid->GetZCoordinates();
    XdmfArray *xdaz = new XdmfArray;
    this->ConvertVToXArray(da, xdaz);
    geo->SetVectorZ(xdaz);
    }
    break;
  case VTK_STRUCTURED_GRID:
  case VTK_POLY_DATA:
  case VTK_UNSTRUCTURED_GRID:
    {
    geo->SetGeometryType(XDMF_GEOMETRY_XYZ);
    vtkPointSet *pset = vtkPointSet::SafeDownCast(ds);
    vtkPoints *pts = pset->GetPoints();
    vtkDataArray *da = pts->GetData();
    XdmfArray *xda = new XdmfArray;
    this->ConvertVToXArray(da, xda);
    geo->SetPoints(xda);
    }
    break;
  default:
    geo->SetGeometryType(XDMF_GEOMETRY_NONE);
    //TODO: Support non-canonical vtkDataSets (callout)
    cerr << "Unrecognized dataset type" << endl;
  }
    
  //Attributes
  this->WriteArrays(ds->GetFieldData(),grid,XDMF_ATTRIBUTE_CENTER_GRID);
  this->WriteArrays((vtkFieldData*)ds->GetCellData(),grid,XDMF_ATTRIBUTE_CENTER_CELL); //TODO: why do I need this cast?
  this->WriteArrays((vtkFieldData*)ds->GetPointData(),grid,XDMF_ATTRIBUTE_CENTER_NODE); //TODO: why do I need this cast?
}

//----------------------------------------------------------------------------
void vtkXdmfWriter2::WriteArrays(vtkFieldData* dsa, XdmfGrid *grid, int association )
{
  if (dsa)
    {
    for (int i = 0; i < dsa->GetNumberOfArrays(); i++)
      {
      vtkDataArray *da = dsa->GetArray(i);
      if (!da)
        {
        //TODO: Dump non numeric arrays too
        continue;
        }
      XdmfAttribute *attr = new XdmfAttribute;    
      attr->SetName(da->GetName());
      attr->SetAttributeCenter(association);

      //TODO: Mark the active scalar, vector, global id, etc
      if ( da->GetNumberOfComponents() == 1 )
        {
        attr->SetAttributeType(XDMF_ATTRIBUTE_TYPE_SCALAR);
        }
      else if ( da->GetNumberOfComponents() == 3 )
        {
        attr->SetAttributeType(XDMF_ATTRIBUTE_TYPE_VECTOR);
        }
      else if ( da->GetNumberOfComponents() == 6 )
        {
        attr->SetAttributeType(XDMF_ATTRIBUTE_TYPE_TENSOR);
        }

      XdmfArray *xda = new XdmfArray;
      this->ConvertVToXArray(da, xda);
      attr->SetValues(xda);
      grid->Insert(attr);
      }
    }
}

//------------------------------------------------------------------------------
void vtkXdmfWriter2::ConvertVToXArray(vtkDataArray *vda, XdmfArray *xda)
{
  xda->SetNumberOfElements(vda->GetNumberOfTuples() * vda->GetNumberOfComponents());
  switch (vda->GetDataType())
    {
    case VTK_DOUBLE:
      xda->SetNumberType(XDMF_FLOAT64_TYPE);
      break;        
    case VTK_FLOAT:
      xda->SetNumberType(XDMF_FLOAT32_TYPE);
      break;
    case VTK_ID_TYPE:
      xda->SetNumberType(XDMF_INT64_TYPE); //TODO: This isn't always true, depends on use 64 bit ids eh?
      break;
    case VTK_LONG:
      xda->SetNumberType(XDMF_INT64_TYPE);
      break;
    case VTK_INT:
      xda->SetNumberType(XDMF_INT32_TYPE);
      break;
    case VTK_UNSIGNED_INT:
      xda->SetNumberType(XDMF_UINT32_TYPE);
      break;
    case VTK_SHORT:
      xda->SetNumberType(XDMF_INT16_TYPE);
      break;
    case VTK_UNSIGNED_SHORT:
      xda->SetNumberType(XDMF_INT16_TYPE);
      break;
    case VTK_CHAR:
    case VTK_SIGNED_CHAR:
      xda->SetNumberType(XDMF_INT8_TYPE); //TODO: Do we want unicode support?
      break;
    case VTK_UNSIGNED_CHAR:
      xda->SetNumberType(XDMF_UINT8_TYPE);
      break;
    case VTK_LONG_LONG:
    case VTK_UNSIGNED_LONG_LONG:
    case VTK___INT64:
    case VTK_UNSIGNED___INT64:
    case VTK_UNSIGNED_LONG:
    case VTK_STRING:
      {
      xda->SetNumberType(XDMF_UNKNOWN_TYPE);
      break;
      }
    }        
  xda->SetDataPointer(vda->GetVoidPointer(0));
}

