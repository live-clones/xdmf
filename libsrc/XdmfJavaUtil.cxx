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
#include "XdmfJavaUtil.h"
#include "XdmfVtkSupport.h"
#include "vtkSystemIncludes.h"
#include "vtkDataSet.h"
#include "vtkJavaUtil.h"

extern "C" JNIEXPORT void* vtkDataSet_Typecast(void *me,char *dType);
extern "C" JNIEXPORT void* vtkDataArray_Typecast(void *me,char *dType);

extern "C"
JNIEXPORT void JNICALL
Java_xdmf_XdmfVtkDataSet_JNIRegister( JNIEnv *env,
        jobject      obj,
        jlong      _InVtkObject ) {

vtkDataSet *aNewOne = *(vtkDataSet **)&_InVtkObject;
int id= vtkJavaRegisterNewObject(env,obj,(void *)aNewOne);
vtkJavaRegisterCastFunction(env,obj,id,(void *)vtkDataSet_Typecast);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_xdmf_XdmfVtkDataSet_GetPointer( JNIEnv *env, jobject obj ) {

vtkDataSet *Vptr;
jlong      Ptr;

Vptr = ( vtkDataSet *)vtkJavaGetPointerFromObject(env,obj,(char *)"vtkDataSet");
Ptr = *( jlong *)&Vptr;
return( Ptr );
}

extern "C"
JNIEXPORT jint JNICALL
Java_xdmf_XdmfVtkData_CopyFrom( JNIEnv *env, jobject obj, jobject vobj, jlong ArrayPtr, jlong Start, jlong Length ) {

vtkDataArray *Vptr;
XdmfArray    *Array;
jint    Status;

Vptr = ( vtkDataArray *)vtkJavaGetPointerFromObject(env,vobj,(char *)"vtkDataArray");
Array = *( XdmfArray **)&ArrayPtr;
Status = XdmfArrayCopyToVtkDataArray( Array, Vptr, Start, Length );
return( Status );
}

extern "C"
JNIEXPORT jint JNICALL
Java_xdmf_XdmfVtkData_CopyTo( JNIEnv *env, jobject obj, jobject vobj, jlong ArrayPtr, jlong Start, jlong Length ) {

vtkDataArray *Vptr;
XdmfArray    *Array;
jint    Status;

Vptr = ( vtkDataArray *)vtkJavaGetPointerFromObject(env,vobj,(char *)"vtkDataArray");
Array = *( XdmfArray **)&ArrayPtr;
Status = XdmfArrayCopyFromVtkDataArray( Array, Vptr, Start, Length );
return( Status );
}

