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
#ifndef __XdmfFormatXML_h
#define __XdmfFormatXML_h


#include "XdmfFormat.h"

class XdmfArray;

class XDMF_EXPORT XdmfFormatXML : public XdmfFormat {

public :

  XdmfFormatXML();
  ~XdmfFormatXML();

  const char * GetClassName() { return("XdmfFormatXML"); } ;

//! Return an Array from the DOM Element
  XdmfArray  *ElementToArray( XdmfXNode *Element,
            XdmfDataDesc  *Desc = NULL,
            XdmfArray *Array = NULL );
//! Write an Array. The Shape of the Array is overridden by the Optional Desc.
  XdmfXNode    *ArrayToElement( XdmfArray *Array,
        XdmfString HeavyDataset = NULL,
        XdmfXNode *Element = NULL,
        XdmfDataDesc *Desc = NULL );

};

#endif
