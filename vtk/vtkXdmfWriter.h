/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXdmfWriter.h
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
// .NAME vtkXdmfWriter - write eXtensible Data Model and Format files
// .SECTION Description
// vtkXdmfWriter is a process object that writes XDMF data.  The input to this
// writer is a single vtkDataSet object.
// .SECTION Caveats
// used the XDMF API
// .SECTION See Also
// vtkDataReader
#ifndef _vtkXdmfWriter_h
#define _vtkXdmfWriter_h

#include "vtkProcessObject.h"

class vtkDataSet;
class vtkPoints;
class vtkCellArray;
class vtkDataArray;
class vtkDataSetCollection;

class VTK_EXPORT vtkXdmfWriter : public vtkProcessObject
{
public:
  static vtkXdmfWriter *New();
  vtkTypeRevisionMacro(vtkXdmfWriter,vtkProcessObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set or get the AllLight flag. If set, all data will be written as light
  // data (inlined).
  vtkSetClampMacro(AllLight, int, 0, 1);
  vtkBooleanMacro(AllLight, int);
  vtkGetMacro(AllLight, int);

  // Description:
  // Set or get the file name of the xdmf file.
  virtual void SetFileName(const char* fname);
  virtual const char* GetFileName();

  // Description:
  // Set or get the grid name of the dataset.
  vtkSetStringMacro(GridName);
  vtkGetStringMacro(GridName);

  // Description:
  // Set or ger the domain name.
  vtkSetStringMacro(DomainName);
  vtkGetStringMacro(DomainName);

  // Description:
  // Set or get the name of the heavy data file name.
  virtual void SetHeavyDataSetName( const char *name);
  virtual const char* GetHeavyDataSetName();

  // Description:
  // Set the input data set.
  virtual void SetInput(vtkDataSet* ds);

  // Description:
  // Write the XDMF file.
  void Write();

  // Description:
  // Add a dataset to the list of data to append.
  void AddInput(vtkDataObject *in);

  // Description:
  // Get any input of this filter.
  vtkDataObject *GetInput(int idx);
  vtkDataObject *GetInput() 
    {return this->GetInput( 0 );}
  
  // Description:
  // Remove a dataset from the list of data to append.
  void RemoveInput(vtkDataObject *in);

  // Description:
  // Returns a copy of the input array.  Modifications to this list
  // will not be reflected in the actual inputs.
  vtkDataSetCollection *GetInputList();

  // Description:
  // Indent xml 
  void Indent(ostream& ost);
  void IncrementIndent() { this->CurrIndent ++; }
  void DecrementIndent() { this->CurrIndent --; }

  // Description:
  // Generate hdf5 name for array
  const char* GenerateHDF5ArrayName(const char* arrayName);

protected:
  vtkXdmfWriter();
  ~vtkXdmfWriter();

  void WriteAttributes( ostream& ost );
  void StartTopology( ostream& ost, int Type, vtkCellArray *Cells);
  void StartTopology( ostream& ost, const char* toptype, int rank, int *dims);
  void EndTopology( ostream& ost );
  void StartGeometry( ostream& ost, const char* type );
  void EndGeometry( ostream& ost );
  virtual int WriteHead( ostream& ost );
  virtual int WriteTail( ostream& ost );
  virtual int WriteGrid( ostream& ost );
  virtual int WriteCellArray( ostream& ost, vtkCellArray *Cells );
  virtual int WritePoints( ostream& ost, vtkPoints *Points );
  virtual int WriteDataArray( ostream& ost, vtkDataArray* array, int dims[3], const char* Name, const char* Center, int type );
  virtual int WriteVTKArray( ostream& ost, vtkDataArray* array, int dims[3], const char* name, const char* dataName, int alllight);

  vtkDataSet* GetInputDataSet();

  vtkSetStringMacro(HeavyDataSetNameString);
  char    *HeavyDataSetNameString;

  vtkSetStringMacro(FileNameString);
  char    *FileNameString;
  char    *GridName;
  char    *DomainName;
  
  int    AllLight;

  int CurrIndent;

  vtkSetStringMacro(HDF5ArrayName);
  char* HDF5ArrayName;

  // list of data sets to append together.
  // Here as a convenience.  It is a copy of the input array.
  vtkDataSetCollection *InputList;

private:
  vtkXdmfWriter(const vtkXdmfWriter&); // Not implemented
  void operator=(const vtkXdmfWriter&); // Not implemented
};

#endif /* _vtkXdmfWriter_h */
