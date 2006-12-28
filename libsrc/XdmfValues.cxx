/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Values              */
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
#include "XdmfValues.h"
#include "XdmfDataStructure.h"
#include "XdmfArray.h"

XdmfValues::XdmfValues() {
    this->DataStructure = NULL;
}

XdmfValues::~XdmfValues() {
}

// Override this
XdmfArray *
XdmfValues::Read(XdmfArray *Array){
    return(NULL);
}

// Override this
XdmfInt32 
XdmfValues::Write(XdmfArray *Array){
    return(XDMF_FAIL);
}

