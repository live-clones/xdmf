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
#ifndef __XdmfDataStructure_h
#define __XdmfDataStructure_h

#include "XdmfElement.h"

class XdmfDataDesc;
class XdmfArray;
class XdmfValues;

#define XDMF_FORMAT_XML 0
#define XDMF_FORMAT_HDF 1


/*! XdmfDataStructure represents an XdmfArray in XML. 
    DataStructures have an optional name. Rank is also optional since it can be determined from the dimensions.
    Dimensions are listed with the slowest varying dimension first. (i.e. KDim JDim IDim). Type is 
    "Char | Float | Int | Compound" with the default being Float. Precision is BytesPerElement and defaults to
    4 for Ints and Floats. Format is any supported XDMF format but usually XML | HDF.
    Examples :
\verbatim
    <!-- Fully Qualified -->
    <DataStructure Name="MyDataStructure"
        Rank="3" Dimensions="2 3 4"
        Type="Float" Precision="8"
        Format="XML>
        0 1 2 3
        4 5 6 7
        8 9 10 11

        0 1 2 3
        4 5 6 7
        8 9 10 11
    </DataStructure>
    <!-- Minimalist -->
    <DataStructure Dimensions="3">
    1 2 3
    </DataStructure>
\endverbatim
*/

class XDMF_EXPORT XdmfDataStructure : public XdmfElement {

public:
  XdmfDataStructure();
  ~XdmfDataStructure();

  XdmfConstString GetClassName() { return ( "XdmfDataStructure" ) ; };

//! Get the data values access object
    XdmfGetValueMacro(Values, XdmfValues *);

//! Get the format of the data. Usually XML | HDF
    XdmfGetValueMacro(Format, XdmfInt32);
//! Set the format of the data. Usually XML | HDF. Default is XML.
    XdmfSetValueMacro(Format, XdmfInt32);

//! Update Structure From XML (INPUT)
    XdmfInt32 UpdateInformation();

//! Update Structre and Values potentially reading Heavy Data (INPUT)
    XdmfInt32 Update();

//! Update the DOM (OUTPUT)
    XdmfInt32 UpdateDOM();

    //! Get the Internal XdmfDataDesc
    XdmfGetValueMacro(DataDesc, XdmfDataDesc *);

    //! Set the XdmfDataDesc.
    XdmfSetValueMacro(DataDesc, XdmfDataDesc *);

    //! Get the Internal Array
    XdmfGetValueMacro(Array, XdmfArray *);

    //! Set the Array
    XdmfSetValueMacro(Array, XdmfArray *);

    //! Convenience Function to access Array
    /*! The more robust access is via :
        \verbatim
        array = XdmfDataStructure->GetArray();
        array->GetValues(....)
        \endverbatim
    */
    XdmfString  GetDataValues( XdmfInt64 Index = 0,
                    XdmfInt64 NumberOfValues = 0,
                    XdmfInt64 ArrayStride = 1);

    //! Convenience Function to access Array
    /*! The more robust access is via :
        \verbatim
        array = XdmfDataStructure->GetArray();
        array->SetValues(....)
        \endverbatim
    */
    XdmfInt32  SetDataValues( XdmfInt64 Index, XdmfConstString Values,
                    XdmfInt64 ArrayStride = 1,
                    XdmfInt64 ValuesStride = 1 );

    //! Get Rank of the Dimensions
    XdmfInt32 GetRank();

    //! Set the Shape (Rank and Dimensions)
    XdmfInt32 SetShape(XdmfInt32 Rank, XdmfInt64 *Dimensions);
    //! Returns Rank and Fills in the Dimensions
    XdmfInt32 GetShape(XdmfInt64 *Dimensions);
    //! Returns Shape as String
    XdmfConstString GetShapeAsString();

    //! Convenience Function
    XdmfConstString GetDimensions(){this->GetShapeAsString();};
    //! Convenience Function
    XdmfInt32 SetDimensions(XdmfInt32 Rank, XdmfInt64 *Dimensions){this->SetShape(Rank, Dimensions);};
    //! Convenience Function
    XdmfInt32 SetDimensionsFromString(XdmfConstString Dimensions);

protected:
    XdmfInt32       Format;
    XdmfDataDesc    *DataDesc;
    XdmfArray       *Array;
    XdmfValues      *Values;
};

#endif // __XdmfDataStructure_h
