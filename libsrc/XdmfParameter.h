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
#ifndef __XdmfParameter_h
#define __XdmfParameter_h

#define XDMF_PARAMETER_RANGE_TYPE  1
#define XDMF_PARAMETER_LIST_TYPE  2

#include "XdmfObject.h"


class XdmfArray;
class XdmfXNode;

//! Basic Substitution Element for XML
/*!
This is the basic "looping" mechanism inside the XdmfDOM.
It is used to enclose a group of elements, particularly
for timesteps, so that data names with patterns can be easily
defined.

Parameters are used in conjunction with the DOM. The DOM is parsed,
then the XdmfParameter is retreived. The user uses the SetCurrentIndex()
method to set the Value for the parameter. Then any child node of \b Parameter
that is retreived from the DOM will automatically have the substituion performed.

There are two non-trival types of XdmfParameters: \b List and \b Range. 
\b List explicitly specifies all of it's possible values while \Range 
specifies a \b Start a \b Stride and a \b Count (Length). In
the following example we show a double substitution :

\verbatim
<?xml version="1.0" ?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" [
<!ENTITY HeavyData "Iteration">
]>
<Xdmf>
<Parameter Type="Range"
 Name="Iteration"
 Format="Data_%05d.h5:"
 Values="0 1 266"
 CurrentIndex="10" >
	.
	.
	.
	<DataStructure Type="HDF" ...
		&HeavyData;:/Global Scalars/Pressure
	</DataStructure>
	.
	.
	.
</Parameter>

\endverbatim

There are 266 HDF5 files named Data_00000.h5 thru Data_00265.h5. 
The \b Values attribute says that the substituion begins with 0,
strides by 1, and there are 266 of them. The \b CurrentIndex attribute
says that the default index is 10 ; the 10'th one : specifically (Start + (CurrentIndex * Stride) - 1).
So 0 + (10 * 1) - 1 = 9.

The XML uses an \b ENTITY to substitute the word \b Iteration everywhere it sees
the string \b &HeavyData; . This is convenient since we can override the
default behavior by changing \b &HeavyData; to a specific file name like
Data_12345.h5 by changing the XML in one location.

The XML \b ENTITY is never seen by the XdmfDOM; the substitution is accomplished
at lower layers. The XdmfParameter specifies that the word \b Iteration will be
replaced by the string resulting from substituting \b %05d with the \b CurrentIndex
of the range. So by default, the HDF dataset name is "Data_00009.h5:/Global Scalars/Pressure".

By using the SetCurrentIndex() method, this can be changed. XdmfParameter->SetCurrentIndex(20)
would result in "Data_00019.h5:/Global Scalars/Pressure".

\b List Parameters are similar except that the \b Values attribute specfically specifies
the Values :

\verbatim
<?xml version="1.0" ?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" [
<!ENTITY HeavyData "Iteration">
]>
<Xdmf>
<Parameter Type="List"
 Name="Iteration"
 Format="Data_%f.h5:"
 Values="0.1 1.1 22.3"
 CurrentIndex="1" >
	.
	.
	.
\endverbatim

Specifies Data_0.1.h5, Data_1.1.h5, and Data_22.3.h5 with Data_1.1.h5 being the default since
\b CurrentIndex is zero based.

XdmfParameters can also be used in much the same way as an XML \b ENTITY. The only difference
is that the XdmfDOM will perform the substitution instead of the lower level XML Parser.

*/

class XDMF_EXPORT XdmfParameter : public XdmfObject {

public:
  XdmfParameter();
  ~XdmfParameter();
  XdmfConstString GetClassName() { return ( "XdmfParameter" ) ; };

//! Get the type of Parameter : XDMF_PARAMETER_RANGE_TYPE | XDMF_PARAMETER_LIST_TYPE
  XdmfGetValueMacro(ParameterType, XdmfInt32);
//! Get the type of Parameter as a String
  XdmfString GetParameterTypeAsString( void ) {
    if( this->ParameterType == XDMF_PARAMETER_RANGE_TYPE ) {
      return("XDMF_PARAMETER_RANGE_TYPE");
    }
    return("XDMF_PARAMETER_LIST_TYPE");
    }
//! Set the Parameter to XDMF_PARAMETER_RANGE_TYPE or XDMF_PARAMETER_LIST_TYPE
  XdmfSetValueMacro(ParameterType, XdmfInt32);

//! Get the Value of the CurrentIndex
  XdmfGetValueMacro(CurrentIndex, XdmfInt64);
//! Set the Value of the CurrentIndex
  XdmfInt32 SetCurrentIndex( XdmfInt64 Value );

//! Get the Name of the Parameter i.e. The string that is to be substituted
  XdmfGetValueMacro(ParameterName, XdmfString);
//! Set the Name of the Parameter i.e. The string that is to be substituted
  XdmfInt32 SetParameterName( XdmfString Value ) {
    strcpy(this->ParameterName, Value );
    return( XDMF_SUCCESS );
    };

//! Get the Current XNode which came from a XdmfDom
  XdmfGetValueMacro(ParameterNode, XdmfXNode *);
//! Set the Current XNode which came from a XdmfDom
  XdmfInt32 SetParameterNode( XdmfXNode *Node);

//! Get the Format String
  XdmfString GetFormat( void ) { return( this->ParameterFormat ) ; };
//! Get the length. This is either the Count or the Length of the Array
/*!
	The \b NumberOfElements of a \b RANGE Parameter is the third number
	specified in the \b Values attribute (the \b COUNT). For a \b LIST
	Parameter, the \b NumberOfElements is the number of elements in the
	\b Values attribute. In other words, this means "how many possible values are there ?"
*/
  XdmfInt64 GetNumberOfElements( void );
//! Get a handle to the internal array
/*!
	For a \b RANGE Parameter this will be 3 Values. For a \b LIST 
	Parameter, this is all possible values.
*/
  XdmfArray *GetArray( void ) { return ( this->ParameterArray ); };
//! Get the current value of the parameter
/*!
	The substituion is performed and the resulting string is returned.
	The substitution is performed with the value in \b CurrentIndex.
*/
  XdmfString GetParameterValue( void ) { return( this->ParameterValue ) ; };

//! Force a Update
  XdmfInt32 Update( XdmfInt32 CurrentIndex = -1 );

//! Perform a Substitution on a user specified string
  XdmfString Substitute( XdmfString OriginalString );

protected:
  char    ParameterName[ XDMF_MAX_STRING_LENGTH ];
  char    ParameterValue[ XDMF_MAX_STRING_LENGTH ];
  char    ParameterFormat[ XDMF_MAX_STRING_LENGTH ];
  XdmfInt64  CurrentIndex;
  XdmfInt32  ParameterType;
  XdmfArray  *ParameterArray;
  XdmfXNode    *ParameterNode;
  
};


#endif // __XdmfParameter_h
