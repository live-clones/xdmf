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
#ifndef __XdmfFormat_h
#define __XdmfFormat_h


#include "XdmfLightData.h"

class XdmfDataDesc;

//!  Wrapper Class for handeling DataStructure and DataTransform
/*!
This is the Base Object for Format Drivers.
If the Data if embedded in the XML, this object
is the driver. Otherwise the approrpiate
methods are overloaded.

An XML Format Node Looks like :

\verbatim
<DataStructure
  Name="XXX"
  Rank="2"
  Dimensions="2 4"
  Precision="4"
  DataType="Float">
  1.1 3.3 5.5 7.7 9.9 11 13.1 15
</DataStructure>
\endverbatim
*/


class XDMF_EXPORT XdmfFormat : public XdmfLightData {

public :

  XdmfFormat();
  ~XdmfFormat();

  XdmfConstString GetClassName() { return("XdmfFormat"); } ;

  void  SetFormat( XdmfString Format ) {
    strcpy( this->DataFormat, Format );
    };

  XdmfString  GetFormat( void ) { return( this->DataFormat ); };
//! Return a Data Desc from a DOM Element
  XdmfDataDesc  *ElementToDataDesc( XdmfXNode *Element );
//! Return Data Desc from Compound Element ( Used Internally )
  XdmfDataDesc  *ElementToCompoundDataDesc( XdmfXNode *Element );
//! Create/Modify a DOM Element based on Data Desc
  XdmfXNode    *DataDescToElement( XdmfDataDesc *Desc, XdmfXNode *Element = NULL );

protected :
  char  DataFormat[ XDMF_MAX_STRING_LENGTH ];
};

#endif
