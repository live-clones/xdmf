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
#ifndef __vtkXdmfReader_h
#define __vtkXdmfReader_h

// #include "vtkDataSetSource.h"
// #include "vtkSource.h"
#include "vtkDataReader.h"
#include "vtkXdmfDataArray.h"
#include "Xdmf.h"


class vtkDataArrayCollection;
class vtkIdListCollection;

// class VTK_EXPORT vtkXdmfReader : public vtkDataSetSource
// class VTK_EXPORT vtkXdmfReader : public vtkSource
class VTK_EXPORT vtkXdmfReader : public vtkDataReader
{
public :
  static vtkXdmfReader *New();
  // vtkTypeRevisionMacro(vtkXdmfReader,vtkDataSetSource);
  vtkTypeRevisionMacro(vtkXdmfReader,vtkDataReader);
  void PrintSelf(ostream& os, vtkIndent indent);

  void Update();
  void SetInputFileName( const char *fileName );
  char *GetInputFileName( ) {
    return(this->InputFileName);
    }
  int Parse();
  int GetNumberOfAttributes() {
    return( this->Grid->GetNumberOfAttributes() );
    }
  vtkDataObject *GetOutput();
  vtkDataObject *GetOutput(int idx);
  int GetAttributeStatus( int Index ) {
    if( Index >= this->NumberOfAttributes ) {
      return( -1 );
      }
    return( this->AttributeStatus[ Index ] );
    }
  void SetAttributeStatusOn( int Index );
  void SetAttributeStatusOff( int Index );
  void SetAttributeStatusOn( char *Name );
  void SetAttributeStatusOff( char *Name );
  void SetAllAttributeStatusOn();
  void SetAllAttributeStatusOff();
  int  Initialize();
  char *GetXdmfDOMHandle();
  char *GetXdmfGridHandle();
  int GetGridIndex() {
    return( this->GridIndex );
    }
  void SetGridIndex( int Index ) {
    this->GridIndex = Index;
    }
  char *GetGridName() {
    return( this->GridName );
    }
  void SetGridName( char *Name ) {
    if( this->GridName ) {
      delete [] this->GridName ;
      }
    this->GridName = new char[ strlen(Name) + 1 ];
    strcpy( this->GridName, Name );
    }

  
protected:
  vtkXdmfReader();
  ~vtkXdmfReader();

  int    SetBaseTopology();
  int    SetBaseGeometry();
  int    SetBaseAttributes();
  int    NumberOfAttributes;
  int    *AttributeStatus;

  vtkDataObject  *CurrentOutput;
  vtkXdmfDataArray *ArrayConverter;
  XdmfDOM    *DOM;
  XdmfGrid  *Grid;
  void    Execute();
  char    *InputFileName;
  int    GridIndex;
  int    Initialized;
  char    *GridName;

private :
  vtkXdmfReader( const vtkXdmfReader&); // Not implemented.
  void operator=(const vtkXdmfReader&); // Not implemented.
};

#endif // __vtkXdmfReader_h
