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
/*     Copyright @ 2007 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef __XdmfDsmBuffer_h
#define __XdmfDsmBuffer_h

#include "XdmfDsm.h"


//! Base comm object for Distributed Shared Memory implementation
/*!
*/


class XDMF_EXPORT XdmfDsmBuffer : public XdmfDsm {

public:
  XdmfDsmBuffer();
  ~XdmfDsmBuffer();

  XdmfConstString GetClassName() { return ( "XdmfDsmBuffer" ) ; };

    XdmfInt32   Put(XdmfInt64 Address, XdmfInt64 Length, void *Data);

protected:
};

#endif // __XdmfDsmBuffer_h
