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
#include "XdmfCharArray.h"
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

XdmfCharArray::XdmfCharArray() {
this->SetNumberType( XDMF_INT8_TYPE );
}

XdmfCharArray::~XdmfCharArray() {
}

XdmfInt32
XdmfCharArray::SetFromFile(  XdmfString FileName ) {

char  *cp;
int  ch;
FILE  *fp;
struct stat FileStatus;

this->SetNumberType( XDMF_INT8_TYPE );
if ( stat( FileName, &FileStatus ) < 0 ) {
  XdmfErrorMessage("Can't stat() " << FileName );
  return( XDMF_FAIL );
  }
XdmfDebug("File " << FileName << " is " << FileStatus.st_size << " bytes long");
this->SetNumberOfElements( FileStatus.st_size + 1 );
cp = (char *)this->GetDataPointer();
if( (fp = fopen( FileName, "r" )) ) {
  while( ( ch = getc( fp ) ) != EOF ){
    *cp++ = ch;  
    }
  fclose( fp );
  *cp = '\0';
} else {
  XdmfErrorMessage("Can't open file " << FileName );
  return( XDMF_FAIL );
}
return( FileStatus.st_size );
}

