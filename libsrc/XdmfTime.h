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

#include "XdmfElement.h"


/*! XdmfTime represents a Time specification.
Time is a child of the <Grid> element :

<Time TypeType="Single* | Uniform | HyperSlab | Function" 
    NumberOfIterations="1* | N"
    Value="(no default)">
     <DataItem ....
</Time>
    TimeType can be :
        Single - A Single Time for the entire Grid
        Uniform - a Time Series
        HyperSlab - Start Stride Count
        Function - XdmfFloat64 *Function(GridIndex)
*/

class XdmfDataItem;
class XdmfArray;

#define XDMF_TIME_SINGLE    0x00
#define XDMF_TIME_UNIFORM   0x01
#define XDMF_TIME_HYPERSLAB 0x02
#define XDMF_TIME_FUNCTION  0x03

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
protected:
    XdmfInt32    TimeType;
    XdmfFloat64  Value;
    XdmfArray    *Array;
    XdmfDataItem *DataItem;
    XdmfString   Function;
};

#endif // __XdmfTime_h
