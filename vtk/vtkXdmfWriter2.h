/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXdmfWriter2.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkXdmfWriter2 - write eXtensible Data Model and Format files
// .SECTION Description
// vtkXdmfWriter2 converts vtkDataObjects to XDMF format. This is intended to
// replace vtkXdmfWriter, which is not up to date with the capabilities of the
// newer XDMF2 library. This writer understands VTK's composite data types and
// produces full trees in the output XDMF files.

#ifndef _vtkXdmfWriter2_h
#define _vtkXdmfWriter2_h

#include "vtkWriter.h"

class vtkExecutive;

class vtkCompositeDataSet;
class vtkDataObject;
class vtkFieldData;
class vtkDataArray;

//BTX
class XdmfDOM;
class XdmfGrid;
class XdmfArray;
//ETX

class VTK_EXPORT vtkXdmfWriter2 : public vtkWriter
{
public:
  static vtkXdmfWriter2 *New();
  vtkTypeRevisionMacro(vtkXdmfWriter2,vtkWriter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set or get the file name of the xdmf file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  // Description:
  // Set the input data set.
  virtual void SetInput(vtkDataObject* dobj);

  // Description:
  // Called in parallel runs to identify the portion this process is responsible for
  // TODO: respect this
  vtkSetMacro(Piece, int);
  vtkSetMacro(NumberOfPieces, int);

  //TODO: These controls are available in vtkXdmfWriter, but are not used here.
  //AllLight/AllHeavy
  //GridsOnly
  //Append to Domain

  //TODO: I think there is a limit in the XDMF library about when hdf is used for heavy data and when it is in XML
  //In any case we need to be able to choose the heavy data format (xml, hdf5, sql) somehow

protected:
  vtkXdmfWriter2();
  ~vtkXdmfWriter2();

  // Does the work
  virtual void WriteData();
  virtual void WriteDataSet(vtkDataObject *dobj, XdmfGrid *grid);
  virtual void WriteCompositeDataSet(vtkCompositeDataSet *dobj, XdmfGrid *grid);
  virtual void WriteAtomicDataSet(vtkDataObject *dobj, XdmfGrid *grid);
  virtual void WriteArrays(vtkFieldData* dsa, XdmfGrid *grid, int association );
  virtual void ConvertVToXArray(vtkDataArray *vda, XdmfArray *xda);

  // Create a default executive.
  virtual vtkExecutive* CreateDefaultExecutive();

  virtual int FillInputPortInformation(int port, vtkInformation *info);

  char *FileName;
  XdmfDOM *DOM;

  int Piece;
  int NumberOfPieces;
private:
  vtkXdmfWriter2(const vtkXdmfWriter2&); // Not implemented
  void operator=(const vtkXdmfWriter2&); // Not implemented

  vtkDataObject *InDataNow;
};

#endif /* _vtkXdmfWriter2_h */
