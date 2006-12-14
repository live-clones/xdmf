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
#include "XdmfInformation.h"

XdmfInformation::XdmfInformation() {
}

XdmfInformation::~XdmfInformation() {
}

XdmfInt32 XdmfInformation::UpdateInformation(){
    const char *n;
    XdmfElement::UpdateInformation();
    n = this->Get("Name");
    cout << "Name = " << n << endl;
    this->SetName(n);
}

XdmfConstString XdmfInformation::GetValue() {
    XdmfConstString Value;

    Value = this->Get("Value");
    if(!Value) Value = this->Get("CDATA");
    return(Value);
}
