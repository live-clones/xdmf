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
#ifndef __XdmfTransform_h
#define __XdmfTransform_h


#include "XdmfLightData.h"

/*!
This is the Base Object for Transform Drivers.
If the Data if embedded in the XML, this object
is the driver. Otherwise the approrpiate
methods are overloaded.

An XML Transform Node Looks like :

<DataTransform
  Name="XXX"
  Type= HyperSlab
  Format = XML >
  <DataStructure ...
  0 0 0    2 2 2    10 20 30
  </DataStructure
  < DataStructure ....
    Points.xml
  </DataStructure>
</DataTransform>
*/

class XDMF_EXPORT XdmfTransform : public XdmfLightData {

public :

  XdmfTransform();
  ~XdmfTransform();

  const char * GetClassName() { return("XdmfTransform"); } ;

  void  SetTransform( XdmfString Transform ) {
    strcpy( this->DataTransform, Transform );
    };

  XdmfString  GetTransform( void ) { return( this->DataTransform ); };
//! Return a Data Desc from a DOM Element
  XdmfDataDesc  *ElementToDataDesc( XdmfXNode *Element );
  XdmfArray  *ElementToArray( XdmfXNode *Element );
//! Create/Modify a DOM Element based on Data Desc
  XdmfXNode    *DataDescToElement( XdmfDataDesc *Desc, XdmfString HeavyDataName,
            XdmfXNode *Element = NULL );

  XdmfXNode    *ArrayToElement( XdmfArray *Array, XdmfString HeavyDataName,
            XdmfString HeavyArrayName,
            XdmfXNode *Element = NULL );

protected :
  char  DataTransform[ XDMF_MAX_STRING_LENGTH ];
};

#endif
