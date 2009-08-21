/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXdmfHeavyData.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkXdmfHeavyData_h
#define __vtkXdmfHeavyData_h

#include "vtkObject.h"

#include "XdmfDataItem.h"
#include "XdmfGrid.h"

#include "vtkDataArraySelection.h"

class vtkDataArray;
class vtkDataObject;
class vtkImageData;
class vtkPoints;
class vtkRectilinearGrid;
class vtkStructuredGrid;
class vtkUnstructuredGrid;
class vtkXdmfDomain;
class vtkDataSet;

// vtkXdmfHeavyData helps in reading heavy data from Xdmf and putting that into
// vtkDataObject subclasses.
class vtkXdmfHeavyData
{
  vtkXdmfDomain* Domain;
  XdmfDataItem DataItem;

public:
  // These must be set before using this class.
  int Piece;
  int NumberOfPieces;
  int GhostLevels;
  int Extents[6]; // < these are original extents without the stride taken in
                  //   consideration
  int Stride[3];
  XdmfFloat64 Time;

public:
  vtkXdmfHeavyData(vtkXdmfDomain* domain);
  ~vtkXdmfHeavyData();

  // Description:
  vtkDataObject* ReadData(XdmfGrid* xmfGrid);

  // Description:
  vtkDataObject* ReadData();

  // Description:
  // Returns the VTKCellType for the given xdmf topology. Returns VTK_EMPTY_CELL
  // on error and VTK_NUMBER_OF_CELL_TYPES for XDMF_MIXED.
  static int GetVTKCellType(XdmfInt32 topologyType);
  
  // Description:
  // Returns the number of points per cell. -1 for error. 0 when no fixed number
  // of points possible.
  static int GetNumberOfPointsPerCell(int vtk_cell_type);

private:
  // Description:
  // Read a temporal collection.
  vtkDataObject* ReadTemporalCollection(XdmfGrid* xmfTemporalCollection);

  // Description:
  // Read a spatial-collection or a tree.
  vtkDataObject* ReadComposite(XdmfGrid* xmfColOrTree);

  // Description:
  // Read a non-composite grid.
  vtkDataObject* ReadPrimitiveData(XdmfGrid* xmfGrid);

  // Description:
  // Reads the topology and geometry for an unstructured grid. Does not read any
  // data attributes or geometry.
  vtkUnstructuredGrid* ReadUnstructuredGrid(XdmfGrid* xmfGrid);

  // Description:
  // Read the image data. Simply initializes the extents and origin and spacing
  // for the image, doesn't really read any attributes including the active
  // point attributes.
  vtkImageData* RequestImageData(XdmfGrid* xmfGrid, bool use_uniform_grid);

  // Description:
  // Reads the geometry and topology for a vtkStructuredGrid.
  vtkStructuredGrid* RequestStructuredGrid(XdmfGrid* xmfGrid);

  // Description:
  // Reads the geometry and topology for a vtkRectilinearGrid.
  vtkRectilinearGrid* RequestRectilinearGrid(XdmfGrid* xmfGrid);

  // Description:
  // Reads geometry for vtkUnstructuredGrid or vtkStructuredGrid i.e. of
  // vtkPointSet subclasses. The extents only make sense when reading
  // vtkStructuredGrid. If non-null, then the only the points for the sub-grid
  // are read. 
  vtkPoints* ReadPoints(XdmfGeometry* xmfGeometry,
    int *update_extents=NULL,
    int *whole_extents=NULL);

  // Description:
  // Read attributes.
  bool ReadAttributes(vtkDataSet* dataSet, XdmfGrid* xmfGrid,
    int* update_extents=0);

  // Description:
  // Reads an attribute.
  // If update_extents are non-null, then we are reading structured attributes
  // and we read only the sub-set specified by update_extents.
  vtkDataArray* ReadAttribute(XdmfAttribute* xmfAttribute,
    int data_dimensionality, int* update_extents=0);
};

#endif


