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
#include "vtkInformationVector.h"
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
#include "vtkDataSetAttributes.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkTypeTraits.h"

#include "XdmfArray.h"
#include "XdmfAttribute.h"
#include "XdmfDataDesc.h"
#include "XdmfDOM.h"
#include "XdmfDomain.h"
#include "XdmfGeometry.h"
#include "XdmfGrid.h"
#include "XdmfRoot.h"
#include "XdmfTime.h"
#include "XdmfTopology.h"

#include <vtkstd/map>
#include <stdio.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
# define SNPRINTF _snprintf
#else
# define SNPRINTF snprintf
#endif

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
vtkCxxRevisionMacro(vtkXdmfWriter2, "1.6");

//----------------------------------------------------------------------------
vtkXdmfWriter2::vtkXdmfWriter2()
{
  this->FileName = NULL;
  this->DOM = NULL;
  this->Piece = 0;  //for parallel
  this->NumberOfPieces = 1;
  this->LightDataLimit = 100;
  this->WriteAllTimeSteps = 0;
  this->NumberOfTimeSteps = 1;
  this->CurrentTimeIndex = 0;
  this->Domain = NULL;
  this->TopTemporalGrid = NULL;
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
  if (this->Domain)
    {
    delete this->Domain;
    this->Domain = NULL;
    }
  if (this->TopTemporalGrid)
    {
    delete this->TopTemporalGrid;
    this->TopTemporalGrid = NULL;
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
    (this->FileName ? this->FileName : "(none)") << endl;
  os << indent << "LightDataLimit: " <<
    this->LightDataLimit << endl;
  os << indent << "WriteAllTimeSteps: " <<
    (this->WriteAllTimeSteps?"ON":"OFF") << endl;
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
int vtkXdmfWriter2::Write()
{
  // Make sure we have input.
  if (this->GetNumberOfInputConnections(0) < 1)
    {
    vtkErrorMacro("No input provided!");
    return 0;
    }

  // always write even if the data hasn't changed
  this->Modified();

  //TODO: Specify name of heavy data companion file?
  //TODO: Respect time
  if (!this->DOM)
    {
    this->DOM = new XdmfDOM();
    }

  XdmfRoot root;
  root.SetDOM(this->DOM);  
  root.SetVersion(2.2);
  root.Build();

  if (this->Domain)
    {
    delete this->Domain;
    }
  this->Domain = new XdmfDomain();
  root.Insert(this->Domain);
//  this->Domain->Build();

  this->Update();

  root.Build();
  this->DOM->Write(this->FileName);
  delete this->Domain;
  this->Domain = NULL;

  return 1;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter2::RequestInformation(
  vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector,
  vtkInformationVector* vtkNotUsed(outputVector))
{
  // Does the input have timesteps?
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  if ( inInfo->Has(vtkStreamingDemandDrivenPipeline::TIME_STEPS()) )
    {
    this->NumberOfTimeSteps = 
      inInfo->Length( vtkStreamingDemandDrivenPipeline::TIME_STEPS() );
    }
  else
    {
    this->NumberOfTimeSteps = 1;
    }

  return 1;
}

//----------------------------------------------------------------------------
int vtkXdmfWriter2::RequestUpdateExtent(
  vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector,
  vtkInformationVector* vtkNotUsed(outputVector))
{
  double *inTimes = inputVector[0]->GetInformationObject(0)->Get(
      vtkStreamingDemandDrivenPipeline::TIME_STEPS());
  if (inTimes && this->WriteAllTimeSteps)
    {
    //TODO:? Add a user ivar to specify a particular time, 
    //which is different from current time. Can do it by updating
    //to a particular time then writing without writealltimesteps, 
    //but that is annoying.
    double timeReq = inTimes[this->CurrentTimeIndex];
    inputVector[0]->GetInformationObject(0)->Set( 
        vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS(), 
        &timeReq, 1);
    }

  return 1;  
}

//----------------------------------------------------------------------------
int vtkXdmfWriter2::RequestData(
  vtkInformation* request,
  vtkInformationVector** inputVector,
  vtkInformationVector* outputVector)
{
  if (this->CurrentTimeIndex == 0 && this->WriteAllTimeSteps)
    {
    // Tell the pipeline to start looping.
    request->Set(vtkStreamingDemandDrivenPipeline::CONTINUE_EXECUTING(), 1);
    
    // make a top level temporal grid just under domain
    if (this->TopTemporalGrid)
      {
      delete this->TopTemporalGrid;
      this->TopTemporalGrid = NULL;
      }

    XdmfGrid *tgrid = new XdmfGrid();
    tgrid->SetGridType(XDMF_GRID_COLLECTION);
    tgrid->SetCollectionType(XDMF_GRID_COLLECTION_TEMPORAL);
    XdmfTopology *t = tgrid->GetTopology();
    t->SetTopologyType(XDMF_NOTOPOLOGY);
    XdmfGeometry *geo = tgrid->GetGeometry();
    geo->SetGeometryType(XDMF_GEOMETRY_NONE);

    this->Domain->Insert(tgrid);    

    this->TopTemporalGrid = tgrid;
//    cerr << "BUILDING TGRID" << endl;
//    this->TopTemporalGrid->Build();
    }

  XdmfGrid *grid = new XdmfGrid();
  if (this->TopTemporalGrid)
    {
    this->TopTemporalGrid->Insert(grid);
    }
  else
    {
    this->Domain->Insert(grid);
    }

  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkDataObject* input = inInfo->Get(vtkDataObject::DATA_OBJECT());
  vtkInformation *inDataInfo = input->GetInformation();
  if (inDataInfo->Has(vtkDataObject::DATA_TIME_STEPS()))
    {
    //I am assuming we are not given a temporal data object and getting just one time.
    double *dataT = input->GetInformation()->Get(vtkDataObject::DATA_TIME_STEPS());
    //cerr << "Writing " << this->CurrentTimeIndex << " " << *dataT << endl;

    //TODO: Shouldn't grid->GetTime be used instead of making my own here?
    XdmfTime *xT = new XdmfTime();
    xT->SetTimeType(XDMF_TIME_SINGLE);
    xT->SetValue(*dataT);
    grid->Insert(xT);
    }

  this->WriteDataSet(input, grid);
  //cerr << "BUILDING GRID" << endl;
  //grid->Build();
  //delete grid; //domain takes care of it?

  this->CurrentTimeIndex++;
  if (this->CurrentTimeIndex >= this->NumberOfTimeSteps && 
      this->WriteAllTimeSteps)
    {
    // Tell the pipeline to stop looping.
    request->Remove(vtkStreamingDemandDrivenPipeline::CONTINUE_EXECUTING());
    this->CurrentTimeIndex = 0;
    this->TopTemporalGrid = NULL; //domain takes care of it deletion?
    }

  return 1;
}
 
//------------------------------------------------------------------------------
void vtkXdmfWriter2::WriteDataSet(vtkDataObject *dobj, XdmfGrid *grid)
{
  //TODO:
  // respect parallelism
  if (!dobj)
    {
    //cerr << "Null DS, someone else will take care of it" << endl;
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

  //cerr << "internal node " << dobj << " is a " << dobj->GetClassName() << endl;
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
  
 
  XdmfTopology *t = grid->GetTopology();
  t->SetTopologyType(XDMF_NOTOPOLOGY);
  XdmfGeometry *geo = grid->GetGeometry();
  geo->SetGeometryType(XDMF_GEOMETRY_NONE);
  //geo->SetPoints(NULL);

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
  //cerr << "Writing " << dobj << " a " << dobj->GetClassName() << endl;
  vtkDataSet *ds = vtkDataSet::SafeDownCast(dobj);
  if (!ds)
    {
    //TODO: Fill in non Vis data types
    cerr << "Can not convert " << dobj->GetClassName() << " to XDMF yet." << endl;
    return;
    }

  grid->SetGridType(XDMF_GRID_UNIFORM);

  //Topology
  XdmfTopology *t = grid->GetTopology();
  switch (ds->GetDataObjectType()) {
  case VTK_STRUCTURED_POINTS:
  case VTK_IMAGE_DATA:
  case VTK_UNIFORM_GRID:
    {
    t->SetTopologyType(XDMF_3DCORECTMESH);
    t->SetLightDataLimit(this->LightDataLimit);
    vtkImageData *id = vtkImageData::SafeDownCast(ds);
    int wExtent[6];
    id->GetExtent(wExtent);
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
    rgrid->GetExtent(wExtent);
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
    sgrid->GetExtent(wExtent);
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
    vtkUnstructuredGrid *ugrid = vtkUnstructuredGrid::SafeDownCast(ds);
    if (!ugrid)
      {
      //flag polydata so vtkXdmfReader2 can be read back in as vtkPolyData
      t->Set("Polygonal", "1");
      }
    vtkXdmfWriter2Internal::MapOfCellTypes cellTypes;
    vtkXdmfWriter2Internal::DetermineCellTypes(vtkPointSet::SafeDownCast(ds), cellTypes);

    //TODO: When is it beneficial to take advantage of a homogenous topology?
    //If no compelling reason not to used MIXED, then this should go away.
    //This special case code requires an in memory copy just to get rid of 
    //each cell's preceeding number of points int.
    //If don't have to do that, could use pointer sharing, and the
    //and the extra code path is bound to cause problems eventually.
    if ( cellTypes.size() == 1 )
      {
      //cerr << "Homogeneous topology" << endl;
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
      if (VTK_SIZEOF_ID_TYPE==sizeof(XDMF_64_INT))
        {
        di->SetNumberType(XDMF_INT64_TYPE);
        }
      else
        {
        di->SetNumberType(XDMF_INT32_TYPE);
        }
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
      //cerr << "Nonhomogeneous topology" << endl;
      //Non Homogeneous, used mixed topology type to dump them all
      t->SetTopologyType(XDMF_MIXED);
      XdmfArray *di = t->GetConnectivity();
      if (VTK_SIZEOF_ID_TYPE==sizeof(XDMF_64_INT))
        {
        di->SetNumberType(XDMF_INT64_TYPE);
        }
      else
        {
        di->SetNumberType(XDMF_INT32_TYPE);
        }
      vtkIdTypeArray *da = NULL;
      if (ugrid)
        {
        da = ugrid->GetCells()->GetData();
        this->ConvertVToXArray(da, di);
        }
      else
        {
        vtkPolyData *pd = vtkPolyData::SafeDownCast(ds);
        vtkIdType numv = pd->GetNumberOfVerts();
        vtkIdType numl = pd->GetNumberOfLines();
        vtkIdType nump = pd->GetNumberOfPolys();
        vtkIdType nums = pd->GetNumberOfStrips();
        vtkIdType total = numv+numl+nump+nums;
        if (total == numv)
          {
          da = pd->GetVerts()->GetData();
          t->Set("AllVerts","1");
          }
        else if (total == numl)
          {
          da = pd->GetLines()->GetData();
          t->Set("AllLine","1");
          }
        else if (total == nump)
          {
          da = pd->GetPolys()->GetData();
          t->Set("AllPolys","1");
          }
        else if (total == nums)
          {
          da = pd->GetStrips()->GetData();
          t->Set("AllStrips","1");
          }
        if (da != NULL)
          {
          //cerr << "Single poly cell type" << endl;
          this->ConvertVToXArray(da, di);
          }
        else
          {
          //cerr << "Multiple poly cell types" << endl;
          //more than one cell array, have to merge them into one array
          //TODO: Can XdmfTopology have more than one Connectivity array?
          //If so we could avoid copies and just pass pointers.
          vtkIdType sizev = pd->GetVerts()->GetData()->GetNumberOfTuples();
          vtkIdType sizel = pd->GetLines()->GetData()->GetNumberOfTuples();
          vtkIdType sizep = pd->GetPolys()->GetData()->GetNumberOfTuples();
          vtkIdType sizes = pd->GetStrips()->GetData()->GetNumberOfTuples();
          vtkIdType rtotal = sizev+sizel+sizep+sizes;
          di->SetNumberOfElements(rtotal);
          vtkIdType *out = (vtkIdType*)di->GetDataPointer();
          vtkIdType *ptr = out; 
          memcpy(ptr, pd->GetVerts()->GetData()->GetVoidPointer(0), sizev*sizeof(vtkIdType));
          ptr+=sizev;
          memcpy(ptr, pd->GetLines()->GetData()->GetVoidPointer(0), sizel*sizeof(vtkIdType));
          ptr+=sizel;
          memcpy(ptr, pd->GetPolys()->GetData()->GetVoidPointer(0), sizep*sizeof(vtkIdType));
          ptr+=sizep;
          memcpy(ptr, pd->GetStrips()->GetData()->GetVoidPointer(0), sizes*sizeof(vtkIdType));

          char buf[20];
          SNPRINTF(buf, 20, vtkTypeTraits<vtkIdType>::ParseFormat(),
                   vtkTypeTraits<vtkIdType>::PrintType(sizev));
          t->Set("VertSize",buf);
          SNPRINTF(buf, 20, vtkTypeTraits<vtkIdType>::ParseFormat(),
                   vtkTypeTraits<vtkIdType>::PrintType(sizel));
          t->Set("LineSize",buf);
          SNPRINTF(buf, 20, vtkTypeTraits<vtkIdType>::ParseFormat(),
                   vtkTypeTraits<vtkIdType>::PrintType(sizep));
          t->Set("PolySize",buf);
          SNPRINTF(buf, 20, vtkTypeTraits<vtkIdType>::ParseFormat(),
                   vtkTypeTraits<vtkIdType>::PrintType(sizes));
          t->Set("StripSize",buf);
          }
        }
      }
    }
    break;
  default:
    t->SetTopologyType(XDMF_NOTOPOLOGY);
    cerr << "Unrecognized dataset type" << endl;
  }
  
  //Geometry
  XdmfGeometry *geo = grid->GetGeometry();
  geo->SetLightDataLimit(this->LightDataLimit);
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
  this->WriteArrays(ds->GetCellData(),grid,XDMF_ATTRIBUTE_CENTER_CELL);
  this->WriteArrays(ds->GetPointData(),grid,XDMF_ATTRIBUTE_CENTER_NODE);
}

//----------------------------------------------------------------------------
void vtkXdmfWriter2::WriteArrays(vtkFieldData* fd, XdmfGrid *grid, int association )
{
  if (fd)
    {
    vtkDataSetAttributes *dsa = vtkDataSetAttributes::SafeDownCast(fd);

    for (int i = 0; i < fd->GetNumberOfArrays(); i++)
      {
      vtkDataArray *da = fd->GetArray(i);
      if (!da)
        {
        //TODO: Dump non numeric arrays too
        continue;
        }

      XdmfAttribute *attr = new XdmfAttribute;    
      attr->SetLightDataLimit(this->LightDataLimit);
      if (da->GetName())
        {
        attr->SetName(da->GetName());
        }
      else
        {
        attr->SetName("ANONYMOUS");
        }
      attr->SetAttributeCenter(association);

      int attributeType = 0;
      if (dsa)
        {
        attributeType = dsa->IsArrayAnAttribute(i);
        switch (attributeType) {
        case vtkDataSetAttributes::SCALARS:
          attributeType = XDMF_ATTRIBUTE_TYPE_SCALAR; //TODO: Is XDMF ok with 3 component(RGB) active scalars?
          break;
        case vtkDataSetAttributes::VECTORS:
          attributeType = XDMF_ATTRIBUTE_TYPE_VECTOR; 
          break;
        case vtkDataSetAttributes::GLOBALIDS:
          attributeType = XDMF_ATTRIBUTE_TYPE_GLOBALID; 
          break;
        case vtkDataSetAttributes::TENSORS: //TODO: vtk tensors are 9 component, xdmf tensors are 6?
        case vtkDataSetAttributes::NORMALS: //TODO: mark as vectors?
        case vtkDataSetAttributes::TCOORDS: //TODO: mark as vectors?
        case vtkDataSetAttributes::PEDIGREEIDS: //TODO: ? type is variable
        default:
          attributeType = 0;
          break;
        }
        }

      if (attributeType != 0)
        {
        attr->SetActive(1);
        attr->SetAttributeType(attributeType);
        }
      else
        {
        //vtk doesn't mark it as a special array, use width to tell xdmf what to call it
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
  switch (vda->GetDataType())
    {
    case VTK_DOUBLE:
      xda->SetNumberType(XDMF_FLOAT64_TYPE);
      break;        
    case VTK_FLOAT:
      xda->SetNumberType(XDMF_FLOAT32_TYPE);
      break;
    case VTK_ID_TYPE:      
      xda->SetNumberType((VTK_SIZEOF_ID_TYPE==sizeof(XDMF_64_INT)?XDMF_INT64_TYPE:XDMF_INT32_TYPE));
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
      xda->SetNumberType(XDMF_INT8_TYPE); //TODO: Do we ever want unicode?
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

  //TODO: Pass by reference is preferable, but we have to be sure data sticks around until write occurs.
  if (!this->TopTemporalGrid)
    {
    //Do not let xdmf allocate its own buffer. xdmf just borrows vtk's and doesn't double mem size.
    xda->SetAllowAllocate(0); 
    xda->SetNumberOfElements(vda->GetNumberOfTuples()*vda->GetNumberOfComponents());
    xda->SetDataPointer(vda->GetVoidPointer(0));
    }
  else
    {
    //Unfortunately data doesn't stick around with temporal updates, which is exactly when you want it most.
    xda->SetAllowAllocate(1);  
    xda->SetNumberOfElements(vda->GetNumberOfTuples()*vda->GetNumberOfComponents());
    void *p = xda->GetDataPointer();
    memcpy(p, vda->GetVoidPointer(0), vda->GetNumberOfTuples()*vda->GetNumberOfComponents()*vda->GetElementComponentSize());
    }
}

