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

#include "vtkDataObjectAlgorithm.h"

class vtkExecutive;

class vtkCompositeDataSet;
class vtkDataObject;
class vtkFieldData;
class vtkDataArray;
class vtkInformation;
class vtkInformationVector;

//BTX
class XdmfDOM;
class XdmfDomain;
class XdmfGrid;
class XdmfArray;
//ETX

class VTK_EXPORT vtkXdmfWriter2 : public vtkDataObjectAlgorithm
{
public:
  static vtkXdmfWriter2 *New();
  vtkTypeRevisionMacro(vtkXdmfWriter2,vtkDataObjectAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the input data set.
  virtual void SetInput(vtkDataObject* dobj);

  // Description:
  // Set or get the file name of the xdmf file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

    // Description:
  // Write data to output. Method executes subclasses WriteData() method, as 
  // well as StartMethod() and EndMethod() methods.
  // Returns 1 on success and 0 on failure.
  virtual int Write();

  // Description:
  // Topology Geometry and Attribute arrays smaller than this are written in line into the XML.
  // Default is 100.
  vtkSetMacro(LightDataLimit, int);
  vtkGetMacro(LightDataLimit, int);

  //Description:
  //Controls whether writer automatically writes all input time steps, or 
  //just the timestep that is currently on the input. 
  //Default is OFF.
  vtkSetMacro(WriteAllTimeSteps, int);
  vtkGetMacro(WriteAllTimeSteps, int);
  vtkBooleanMacro(WriteAllTimeSteps, int);

    // Description:
  // Called in parallel runs to identify the portion this process is responsible for
  // TODO: respect this
  vtkSetMacro(Piece, int);
  vtkSetMacro(NumberOfPieces, int);

  //TODO: control choice of heavy data format (xml, hdf5, sql, raw)

  //TODO: These controls are available in vtkXdmfWriter, but are not used here.
  //GridsOnly
  //Append to Domain

protected:
  vtkXdmfWriter2();
  ~vtkXdmfWriter2();

  //Choose composite executive by default for time.
  virtual vtkExecutive* CreateDefaultExecutive();

  //Can take any one data object
  virtual int FillInputPortInformation(int port, vtkInformation *info);

  //Overridden to ...
  virtual int RequestInformation(vtkInformation*, 
                                 vtkInformationVector**, 
                                 vtkInformationVector*);
  //Overridden to ...
  virtual int RequestUpdateExtent(vtkInformation*, 
                                  vtkInformationVector**, 
                                  vtkInformationVector*);
  //Overridden to ...
  virtual int RequestData(vtkInformation*, 
                          vtkInformationVector**, 
                          vtkInformationVector*);
  
  //These do the work: recursively parse down input's structure all the way to arrays, 
  //use XDMF lib to dump everything to file.
  virtual void WriteDataSet(vtkDataObject *dobj, XdmfGrid *grid);
  virtual void WriteCompositeDataSet(vtkCompositeDataSet *dobj, XdmfGrid *grid);
  virtual void WriteAtomicDataSet(vtkDataObject *dobj, XdmfGrid *grid);
  virtual void WriteArrays(vtkFieldData* dsa, XdmfGrid *grid, int association );
  virtual void ConvertVToXArray(vtkDataArray *vda, XdmfArray *xda);

  char *FileName;

  int LightDataLimit;

  int WriteAllTimeSteps;
  int NumberOfTimeSteps;
  int CurrentTimeIndex;

  int Piece;
  int NumberOfPieces;


  XdmfDOM *DOM;
  XdmfDomain *Domain;
  XdmfGrid *TopTemporalGrid;

private:
  vtkXdmfWriter2(const vtkXdmfWriter2&); // Not implemented
  void operator=(const vtkXdmfWriter2&); // Not implemented
};

#endif /* _vtkXdmfWriter2_h */
