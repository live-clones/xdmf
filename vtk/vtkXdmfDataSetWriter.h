/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Jerry A. Clarke                                             */
/*     clarke@arl.army.mil                                         */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2002 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef _vtkXdmfDataSetWriter_h
#define _vtkXdmfDataSetWriter_h

#include <vtkUnstructuredGrid.h>
#include <vtkFloatArray.h>

class vtkXdmfDataSetWriterInternals;

class VTK_EXPORT vtkXdmfDataSetWriter : public vtkUnstructuredGrid
{
public:
  vtkDataSet  *Input;
  char    HeavyDataSetName[256];
  int    FastWrite;
  int    AllLight;
  vtkXdmfDataSetWriterInternals *Internals;
  

  vtkXdmfDataSetWriter();

  static vtkXdmfDataSetWriter *New();
  vtkTypeMacro(vtkXdmfDataSetWriter,vtkObject);
  void ResetXML( void );
  char *GetXML( void );
  void SetFastWrite( int OnOff ) {
  this->FastWrite = OnOff;
  }
  int GetFastWrite( void ) {
  return( this->FastWrite );
  }
  void SetAllLight( int OnOff ) {
  this->AllLight = OnOff;
  }
  int GetAllLight( void ) {
  return( this->AllLight );
  }
  void WriteAttributes( void );
  void SetInput(vtkDataSet *input) {
    this->Input = input;
    }
  void SetHeavyDataSetName( char *HeavyDataSetName ) {
    if( HeavyDataSetName ) strcpy( this->HeavyDataSetName, HeavyDataSetName );
    this->AllLight = 0;
    }
  void StartTopology( int Type, vtkCellArray *Cells);
  int WriteScalar( vtkDataArray *Scalars, char *Name, char *Center );
  int WriteVector( vtkDataArray *Vectors, char *Name, char *Center );
  virtual int WriteHead( void );
  virtual int WriteTail( void );
  virtual int WriteGrid( void );
  virtual int WriteCellArray( vtkCellArray *Cells );
  virtual int WritePoints( vtkPoints *Points );
};
#endif /* _vtkXdmfDataSetWriter_h */
