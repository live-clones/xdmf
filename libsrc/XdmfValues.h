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
#ifndef __XdmfValues_h
#define __XdmfValues_h


#include "XdmfDataStructure.h"

class XdmfArray;

//!  Parent Class for handeling I/O of actual data for an XdmfDataStructure
/*!
This is the base class for access of values. By default, the
Values are in XML and handled XdmfValuesXML. Otherwise they're 
handled by XdmfValuesXXX (where XXX is the format).

An XdmfDataStructure Node Looks like :

\verbatim
<DataStructure
  Rank="2"
  Dimensions="2 4"
  Precision="4"
  DataType="Float">
  1.1 3.3 5.5 7.7 9.9 11 13.1 15
</DataStructure>
     OR
<DataStructure
  Rank="2"
  Dimensions="2 4"
  Precision="4"
  DataType="Float"
  Format="HDF">
    MyData.h5:/AllValues/ThisArray
</DataStructure>
\endverbatim

XdmfValues is used to access the "1.1 3.3 5.5 7.7 9.9 11 13.1 15" part wheather it's in the
XML or in a file described in the XML.
This class is overreidden for various formats supported by Xdmf (i.e. XML, HDF5, etc.)
*/


class XDMF_EXPORT XdmfValues : public XdmfDataStructure{

public :

  XdmfValues();
  ~XdmfValues();

  XdmfConstString GetClassName() { return("XdmfValues"); } ;
  //! Set DOM and Element from another XdmfDataStructure
  XdmfInt32 Inherit(XdmfDataStructure *DataStructure);
  //! Read the Array from the External Representation
  virtual XdmfArray *Read(XdmfArray *Array=NULL);
  //! Write the Array to the External Representation
  virtual XdmfInt32 Write(XdmfArray *Array);

protected :
};

#endif
