/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMyXdmfReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkMyXdmfReader - read PNG files
// .SECTION Description
// vtkRectilinearGridReader is a source object that reads ASCII or binary 
// rectilinear grid data files in vtk format (see text for format details).
// The output of this reader is a single vtkRectilinearGrid data object.
// The superclass of this class, vtkDataReader, provides many methods for
// controlling the reading of the data file, see vtkDataReader for more
// information.
// .SECTION Caveats
// Binary files written on one system may not be readable on other systems.
// .SECTION See Also
// vtkRectilinearGrid vtkDataReader

#ifndef __vtkMyXdmfReader_h
#define __vtkMyXdmfReader_h

#include "vtkDataReader.h"

class vtkDoubleArray;
class vtkDataArray;
class vtkRectilinearGrid;
class vtkDataObject;
class vtkRectilinearGrid;
class vtkDataArraySelection;
class vtkCallbackCommand;
class vtkDataSet;

//BTX
class XdmfDOM;
class XdmfFormatMulti;
class XdmfDataDesc;
class XdmfGrid;
class vtkXdmfDataArray;

class vtkMyXdmfReaderInternal;

//ETX

class VTK_EXPORT vtkMyXdmfReader : public vtkDataReader
{
public:
  static vtkMyXdmfReader* New();
  vtkTypeRevisionMacro(vtkMyXdmfReader, vtkDataReader);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get and set the output of this reader.
  vtkDataSet *GetOutput();
  vtkDataSet *GetOutput(int idx);
  void SetOutput(vtkDataSet *output);
  void SetOutput(vtkDataObject *output);

  // Description:
  // Get the data array selection tables used to configure which data
  // arrays are loaded by the reader.
  vtkGetObjectMacro(PointDataArraySelection, vtkDataArraySelection);
  vtkGetObjectMacro(CellDataArraySelection, vtkDataArraySelection);
  
  // Description:  
  // Get the number of point or cell arrays available in the input.
  int GetNumberOfPointArrays();
  int GetNumberOfCellArrays();
  
  // Description:
  // Get the name of the point or cell array with the given index in
  // the input.
  const char* GetPointArrayName(int index);
  const char* GetCellArrayName(int index);
  
  // Description:
  // Get/Set whether the point or cell array with the given name is to
  // be read.
  int GetPointArrayStatus(const char* name);
  int GetCellArrayStatus(const char* name);
  void SetPointArrayStatus(const char* name, int status);  
  void SetCellArrayStatus(const char* name, int status);  

  // Description:
  // Get/Set the current domain name.
  vtkSetStringMacro(DomainName);
  vtkGetStringMacro(DomainName);

  // Description:
  // Get/Set the current grid name.
  vtkSetStringMacro(GridName);
  vtkGetStringMacro(GridName);

  // Description:
  // Get number of domains and grids.
  int GetNumberOfDomains();
  int GetNumberOfGrids();
  
  // Description:
  // Get the name of domain or grid at index.
  const char* GetDomainName(int idx);
  const char* GetGridName(int idx);

  // Description:
  // Set / get stride
  vtkSetVector3Macro(Stride, int);
  vtkGetVector3Macro(Stride, int);

protected:
  vtkMyXdmfReader();
  ~vtkMyXdmfReader();   

  virtual void Execute();
  virtual void ExecuteInformation();

  // Callback registered with the SelectionObserver.
  static void SelectionModifiedCallback(vtkObject* caller, unsigned long eid,
                                        void* clientdata, void* calldata);
  
  // The array selections.
  vtkDataArraySelection* PointDataArraySelection;
  vtkDataArraySelection* CellDataArraySelection;

  // The observer to modify this object when the array selections are
  // modified.
  vtkCallbackCommand* SelectionObserver;

  XdmfDOM         *DOM;
  XdmfFormatMulti *FormatMulti;
  XdmfDataDesc    *DataDescription;
  XdmfGrid        *Grid;

  // For converting arrays from XDMF to VTK format
  vtkXdmfDataArray *ArrayConverter;

  char* DomainName;
  char* GridName;

  vtkMyXdmfReaderInternal* Internals;

  int Stride[3];

private:
  vtkMyXdmfReader(const vtkMyXdmfReader&); // Not implemented
  void operator=(const vtkMyXdmfReader&); // Not implemented  
};

#endif //__vtkMyXdmfReader_h
