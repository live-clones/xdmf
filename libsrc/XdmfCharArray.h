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
#ifndef __XdmfCharArray_h
#define __XdmfCharArray_h

#include "XdmfArray.h"

/*!
This is a special XdmfArray designed to
handle character strings.
*/
class XDMF_EXPORT XdmfCharArray : public XdmfArray {

public:
  XdmfCharArray();
  ~XdmfCharArray();

  const char * GetClassName() { return ( "XdmfCharArray" ) ; };

//! Get the data as if is were a char *
  char *GetString( void ) {
    return( this->GetDataPointerAsCharPointer() );
    };
//! Set the data from a char *
  XdmfInt32 SetString( XdmfString String ) {
    this->SetDataFromChars( String );
    return( strlen( String ) );
    };
//! Set the Data from a Disk File
  XdmfInt32 SetFromFile( XdmfString FileName );
};

#endif // __XdmfCharArray_h
