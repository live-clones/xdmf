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

class vtkXdmfWriterInternals;

class VTK_EXPORT vtkXdmfWriter : public vtkProcessObject
{
public:
  static vtkXdmfWriter *New();
  vtkTypeRevisionMacro(vtkXdmfWriter,vtkObject);

  // Description:
  // Set or get the AllLight flag. If set, all data will be written as light
  // data (inlined).
  vtkSetClampMacro(AllLight, int, 0, 1);
  vtkBooleanMacro(AllLight, int);
  vtkGetMacro(AllLight, int);

  // Description:
  // Set or get the file name of the xdmf file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  // Description:
  // Set or get the grid name of the dataset.
  vtkSetStringMacro(GridName);
  vtkGetStringMacro(GridName);

  // Description:
  // Set or get the name of the heavy data file name.
  void SetHeavyDataSetName( const char *name);
  vtkGetStringMacro(HeavyDataSetName);

  // Description:
  // Set the input data set.
  virtual void SetInput(vtkDataSet* ds);

  // Description:
  // Write the XDMF file.
  void Write();

protected:
  vtkXdmfWriter();
  ~vtkXdmfWriter();

  void ResetXML( void );
  char *GetXML( void );

  void WriteAttributes( void );
  void StartTopology( int Type, vtkCellArray *Cells);
  int WriteScalar( vtkDataArray *Scalars, const char *Name, const char *Center );
  int WriteVector( vtkDataArray *Vectors, const char *Name, const char *Center );
  virtual int WriteHead( void );
  virtual int WriteTail( void );
  virtual int WriteGrid( void );
  virtual int WriteCellArray( vtkCellArray *Cells );
  virtual int WritePoints( vtkPoints *Points );

  vtkDataSet* GetInputDataSet();

  char    *HeavyDataSetName;
  char    *FileName;
  char    *GridName;
  
  int    AllLight;
  vtkXdmfWriterInternals *Internals;

private:
  vtkXdmfWriter(const vtkXdmfWriter&); // Not implemented
  void operator=(const vtkXdmfWriter&); // Not implemented
};

#endif /* _vtkXdmfWriter_h */
