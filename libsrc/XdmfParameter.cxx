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
#include "XdmfParameter.h"

#include "XdmfArray.h"
#include "XdmfXNode.h"

XdmfParameter::XdmfParameter() {
  this->CurrentIndex = 0;
  this->ParameterType = XDMF_PARAMETER_LIST_TYPE;
  this->ParameterName[0] = '\0';
  this->ParameterValue[0] = '\0';
  this->ParameterFormat[0] = '\0';
  this->ParameterArray = NULL;
  this->ParameterNode = NULL;
  }

XdmfParameter::~XdmfParameter() {
  if( this->ParameterArray ) delete this->ParameterArray;
  }


XdmfInt32
XdmfParameter::SetCurrentIndex( XdmfInt64 Value ) {

this->CurrentIndex = Value;
if( this->ParameterNode ) {
  XdmfInt32  IValue = Value;
  char  CurrentIndexString[ 80 ];
  sprintf( CurrentIndexString, "%d", IValue );
  this->ParameterNode->Set("CurrentIndex" , CurrentIndexString );
  }
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfParameter::SetParameterNode( XdmfXNode *Node ) {

XdmfString Attribute;

if( !Node ) return( XDMF_FAIL );

this->ParameterNode = Node;
this->ParameterType = XDMF_PARAMETER_LIST_TYPE;
this->CurrentIndex = 0;
Attribute = Node->Get("Name");
if( Attribute ) strcpy( this->ParameterName, Attribute );
Attribute = Node->Get("Type");
if( XDMF_WORD_CMP( Attribute, "Range" ) ) {
  this->ParameterType = XDMF_PARAMETER_RANGE_TYPE;
  }
Attribute = Node->Get("Values");
if( Attribute ){
  XdmfArray  *Values = new XdmfArray;

  Values->SetNumberType( XDMF_FLOAT64_TYPE );
  Values->SetValues( 0, Attribute );
  this->ParameterArray = Values;
  }
Attribute = Node->Get("Format");
if( Attribute ){
  strcpy( this->ParameterFormat, Attribute );
} else {
  strcpy( this->ParameterFormat, "%s");
  }
Attribute = Node->Get("CurrentIndex");
if( Attribute ){
  this->CurrentIndex = strtol( Attribute, ( XdmfString *)NULL, 0 );
  }
return( XDMF_SUCCESS );
}

XdmfInt64
XdmfParameter::GetNumberOfElements( void ) {

if( !this->ParameterArray ) return( 0 );
if( this->ParameterType == XDMF_PARAMETER_RANGE_TYPE ) {
  // Start, Stride, Count
  return( this->ParameterArray->GetValueAsInt64( 2 ) );
}
return( this->ParameterArray->GetNumberOfElements() );
}

XdmfInt32
XdmfParameter::Update( XdmfInt32 currentIndex ){

XdmfFloat64  Value;
XdmfInt64  IValue;
int    LastFormatCharIndex;

if( currentIndex < 0 ) currentIndex = this->CurrentIndex;
XdmfDebug( "Setting Value from CurrentIndex = " << currentIndex );
if( !this->ParameterArray ){
  Value = 0.0;
} else {
  if( this->ParameterType == XDMF_PARAMETER_RANGE_TYPE ) {
    Value = this->ParameterArray->GetValueAsFloat64( 0 );
    Value += this->ParameterArray->GetValueAsFloat64( 1 ) *
      MIN( this->ParameterArray->GetValueAsInt64( 2 ) - 1 , currentIndex );
  } else {
    Value = this->ParameterArray->GetValueAsFloat64( MIN( currentIndex,
      this->ParameterArray->GetNumberOfElements() - 1 ));
  }
}
IValue = static_cast<XdmfInt64>(Value);
LastFormatCharIndex = strlen( this->ParameterFormat ) - 1;
if ( LastFormatCharIndex >= 0 ) {
        switch ( this->ParameterFormat[ LastFormatCharIndex ] ) {
                case 'g' :
                case 'G' :
                case 'f' :
                case 'e' :
                case 'E' :
      sprintf( this->ParameterValue,
        this->ParameterFormat,
        Value );
                        break;
                default :
      sprintf( this->ParameterValue,
        this->ParameterFormat,
        IValue );
                        break;
        }
}
 

XdmfDebug("Format = " << this->ParameterFormat <<
  " Value = " << Value <<
  " Result is " << this->ParameterValue );

return( XDMF_SUCCESS );
}

XdmfString
XdmfParameter::Substitute( XdmfString OriginalString ){

XdmfString NewString;
XdmfString Where;
XdmfString Name;
XdmfString NewName = NULL;

this->Update();

NewString = new char[ strlen( this->ParameterValue ) + 
    strlen( OriginalString ) + 2 ];

// look for "&Parameter;" First
NewName = new char[ strlen( this->ParameterName ) + 3 ];
strcpy( NewName, "&" );
strcat( NewName, this->ParameterName );
strcat( NewName, ";" );
Where = strstr( OriginalString, NewName );
Name = NewName;
if( !Where ) {
  // look for "Parameter" Next
  Name = this->ParameterName;
  Where = strstr( OriginalString, Name );
  }
if( Where ) {
  // Do the Substitution
  int  Length = strlen( Name );

  NewString[0] = '\0';
  strncat( NewString, OriginalString, Where - OriginalString );
  strcat( NewString, this->ParameterValue );
  Where += Length;
  strcat( NewString, Where );
} else {
  // Just Copy Original
  strcpy( NewString, OriginalString );
}

if( NewName ) delete [] NewName;
return( NewString );
}
