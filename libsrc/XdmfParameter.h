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

#include "XdmfArray.h"
#include "XdmfXNode.h"

/*!
This is the basic "looping" mechanism inside the XdmfDom.
It is used to enclose a group of elements, particularly
for timesteps, so that data names with patterns can be easily
defined.
*/
class XDMF_EXPORT XdmfParameter : public XdmfObject {

public:
  XdmfParameter();
  ~XdmfParameter();
  const char * GetClassName() { return ( "XdmfParameter" ) ; };

//! Get the type of Parameter
  XdmfGetValueMacro(ParameterType, XdmfInt32);
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

//! Get the Name of the Parameter
  XdmfGetValueMacro(ParameterName, XdmfString);
//! Set the Name of the Parameter
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
  XdmfInt64 GetNumberOfElements( void );
//! Get a handle to the internal array
  XdmfArray *GetArray( void ) { return ( this->ParameterArray ); };
//! Get the current value of the parameter
  XdmfString GetParameterValue( void ) { return( this->ParameterValue ) ; };

//! Force a Update
  XdmfInt32 Update( XdmfInt32 CurrentIndex = -1 );

//! Perform a Substitution
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
