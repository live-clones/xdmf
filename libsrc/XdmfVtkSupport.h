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
#ifndef __XdmfVtkSupport_h
#define __XdmfVtkSupport_h



#ifndef SWIG
#include "vtkVersion.h"
#include "vtkUnstructuredGrid.h"
#include "vtkStructuredGrid.h"
#include "vtkRectilinearGrid.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkCellData.h"

#include "vtkUnsignedCharArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkDoubleArray.h"



/*
#include <strstream.h>
#include <string.h>
*/

#ifdef MAIN_SWIG_TCL_XDMF
extern int vtkUnstructuredGridCommand(ClientData cd, Tcl_Interp *interp,
      int argc, char *argv[]);
extern int vtkRectilinearGridCommand(ClientData cd, Tcl_Interp *interp,
      int argc, char *argv[]);
extern int vtkStructuredGridCommand(ClientData cd, Tcl_Interp *interp,
      int argc, char *argv[]);
extern int vtkScalarsCommand(ClientData cd, Tcl_Interp *interp,
      int argc, char *argv[]);
extern int vtkPointsCommand(ClientData cd, Tcl_Interp *interp,
      int argc, char *argv[]);
extern int vtkPolyDataCommand(ClientData cd, Tcl_Interp *interp,
      int argc, char *argv[]);
extern void vtkTclGetObjectFromPointer(Tcl_Interp *interp, void *temp,
      int command(ClientData, Tcl_Interp *,int, char *[]));

#endif


#include "XdmfArray.h"

#ifndef NO_VTK
#include "vtkObject.h"
#endif

typedef XdmfPointer  XdmfScriptVariablePointer;
typedef XdmfPointer  VtkScriptObject;

extern VtkScriptObject XdmfVtkAddressToCommand( XdmfPointer VtkObject);
extern XdmfInt32 XdmfArrayCopyToVtkDataArray( XdmfArray *Array, XdmfPointer VtkArray, XdmfInt64 Start, XdmfInt64 Length );
extern XdmfInt32 XdmfArrayCopyFromVtkDataArray( XdmfArray *Array, XdmfPointer VtkArray , XdmfInt64 Start, XdmfInt64 Length);

#else // SWIG



typedef XdmfPointer  XdmfScriptVariablePointer;
typedef XdmfPointer  VtkScriptObject;


%typemap(tcl8, in) VtkScriptObject {
  static XdmfPointer  Pointer;
  char      *CommandName;
  Tcl_CmdInfo    info;

  CommandName = Tcl_GetStringFromObj( $source, NULL );
  if( CommandName == NULL ){
    fprintf(stderr, "CommandName is NULL\n");
    return( TCL_ERROR );
    }
  if( Tcl_GetCommandInfo( interp,
      CommandName,
      &info ) <= 0 ){
    return(TCL_ERROR);
    }
  Pointer = ( XdmfPointer )info.clientData;
  $target = Pointer;
  }

%typemap(tcl8, out) VtkScriptObject {
  const char    *CommandName;
  vtkObject    *obj;

  obj = ( vtkObject *)$source;
  CommandName = obj->GetClassName();
  if( !strcmp( CommandName, "vtkUnstructuredGrid" ) ) {
    vtkTclGetObjectFromPointer(interp,(void *)obj,vtkUnstructuredGridCommand);
  } else if( !strcmp( CommandName, "vtkStructuredGrid" ) ) {
    vtkTclGetObjectFromPointer(interp,(void *)obj,vtkStructuredGridCommand);
  } else if( !strcmp( CommandName, "vtkRectilinearGrid" ) ) {
    vtkTclGetObjectFromPointer(interp,(void *)obj,vtkRectilinearGridCommand);
  } else if( !strcmp( CommandName, "vtkDataArray" ) ) {
    vtkTclGetObjectFromPointer(interp,(void *)obj,vtkScalarsCommand);
  } else if( !strcmp( CommandName, "vtkPolyData" ) ) {
    vtkTclGetObjectFromPointer(interp,(void *)obj,vtkPolyDataCommand);
  } else if( !strcmp( CommandName, "vtkPoints" ) ) {
    vtkTclGetObjectFromPointer(interp,(void *)obj,vtkPointsCommand);
  } else {
    fprintf(stderr, "Don't know how to create a %s\n", CommandName );
    return(TCL_ERROR);  
  }
  }

%typemap(java, out) VtkScriptObject {
  const char  *CommandName;
  vtkObject  *obj;

  obj = ( vtkObject *)$source;
  CommandName = obj->GetClassName();

  cerr << "VtkScriptObject == " << CommandName << endl;
  $target = (jlong)obj;
  }

%typemap(tcl8, in) XdmfScriptVariablePointer {
  static XdmfPointer  Pointer;

  Pointer = ( XdmfPointer )Tcl_GetByteArrayFromObj( $source, NULL );
  $target = Pointer;
  }

extern VtkScriptObject XdmfVtkAddressToCommand( XdmfPointer VtkObject);
#endif // SWIG
// XdmfInt32 XdmfWriteDataSet( XdmfString FileName, vtkUnstructuredGrid *DataSet );
#endif // __XdmfVtkSupport_h
