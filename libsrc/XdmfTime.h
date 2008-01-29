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
#ifndef __XdmfTime_h
#define __XdmfTime_h

//! Define time for Grid
/*! XdmfTime represents a Time specification.
Time is a child of the <Grid> element :
\verbatim
<Time TypeType="Single* | List | HyperSlab | Range | Function" 
    NumberOfIterations="1* | N"
    Value="(no default)">
     <DataItem ....
</Time>
    TimeType can be :
        Single - A Single Time for the entire Grid
        List - a Time Series
        HyperSlab - Start Stride Count
        Range - Min Max
        Function - XdmfFloat64 *Function(GridIndex)
\endverbatim
*/

#include "XdmfElement.h"

class XdmfDataItem;
class XdmfArray;
class XdmfGrid;

#define XDMF_TIME_SINGLE    0x00
#define XDMF_TIME_LIST      0x01
#define XDMF_TIME_HYPERSLAB 0x02
#define XDMF_TIME_RANGE     0x03
#define XDMF_TIME_FUNCTION  0x04
#define XDMF_TIME_UNSET     0x0FF

class XDMF_EXPORT XdmfTime : public XdmfElement {

public:
  XdmfTime();
  virtual ~XdmfTime();

  XdmfConstString GetClassName() { return ( "XdmfTime" ) ; };

//! Insert an Element
  XdmfInt32 Insert (XdmfElement *Child);
//! Update From XML
    XdmfInt32 UpdateInformation();

/*! Set the internal value. This is not reflected in the DOM
    Until Build() is called.
*/
    XdmfSetValueMacro(Value, XdmfFloat64);

//! Update the DOM
    XdmfInt32 Build();

/*! Get the internal Value.
*/
    XdmfGetValueMacro(Value, XdmfFloat64);
    //! Get the Array
    XdmfGetValueMacro(Array, XdmfArray *);
    //! Set the Array
    XdmfSetValueMacro(Array, XdmfArray *);
    //! Get the Array
    XdmfGetValueMacro(DataItem, XdmfDataItem *);
    //! Set the DataItem
    XdmfSetValueMacro(DataItem, XdmfDataItem *);
    //! Get the Type
    XdmfGetValueMacro(TimeType, XdmfInt32);
    //! Get the Type as a String
    XdmfConstString GetTimeTypeAsString(void);
    //! Set the Type
    XdmfSetValueMacro(TimeType, XdmfInt32);
    //! Get the Function
    XdmfGetStringMacro(Function);
    //! Set the Function
    XdmfSetStringMacro(Function);
    //! Set Time From Parent Information
    XdmfInt32 SetTimeFromParent(XdmfTime *ParentTime, XdmfInt64 Index);
protected:
    XdmfInt32    TimeType;
    XdmfFloat64  Value;
    XdmfArray    *Array;
    XdmfDataItem *DataItem;
    XdmfString   Function;
};

#endif // __XdmfTime_h
