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
#ifndef __XdmfDOM_h
#define __XdmfDOM_h

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <framework/XMLFormatter.hpp>
#include <util/TranscodingException.hpp>

#include <dom/DOM_DOMException.hpp>

#include <parsers/DOMParser.hpp>
#include <dom/DOM.hpp>
#include <framework/MemBufInputSource.hpp>
#include <framework/LocalFileInputSource.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <strstream.h>
#include <XdmfObject.h>

// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------
static const XMLCh  gEndElement[] = { chOpenAngle, chForwardSlash, chNull };
static const XMLCh  gEndPI[] = { chQuestion, chCloseAngle, chNull};
static const XMLCh  gStartPI[] = { chOpenAngle, chQuestion, chNull };
static const XMLCh  gXMLDecl1[] =
{
        chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l
    ,   chSpace, chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i
    ,   chLatin_o, chLatin_n, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl2[] =
{
        chDoubleQuote, chSpace, chLatin_e, chLatin_n, chLatin_c
    ,   chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual
    ,   chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl3[] =
{
        chDoubleQuote, chSpace, chLatin_s, chLatin_t, chLatin_a
    ,   chLatin_n, chLatin_d, chLatin_a, chLatin_l, chLatin_o
    ,   chLatin_n, chLatin_e, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl4[] =
{
        chDoubleQuote, chQuestion, chCloseAngle
    ,   chCR, chLF, chNull
};

static const XMLCh  gStartCDATA[] =
{ 
        chOpenAngle, chBang, chOpenSquare, chLatin_C, chLatin_D,
        chLatin_A, chLatin_T, chLatin_A, chOpenSquare, chNull
};

static const XMLCh  gEndCDATA[] =
{
    chCloseSquare, chCloseSquare, chCloseAngle, chNull
};
static const XMLCh  gStartComment[] =
{ 
    chOpenAngle, chBang, chDash, chDash, chNull
};

static const XMLCh  gEndComment[] =
{
    chDash, chDash, chCloseAngle, chNull
};

static const XMLCh  gStartDoctype[] =
{ 
    chOpenAngle, chBang, chLatin_D, chLatin_O, chLatin_C, chLatin_T,
    chLatin_Y, chLatin_P, chLatin_E, chSpace, chNull
};
static const XMLCh  gPublic[] =
{ 
    chLatin_P, chLatin_U, chLatin_B, chLatin_L, chLatin_I,
    chLatin_C, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gSystem[] =
{
    chLatin_S, chLatin_Y, chLatin_S, chLatin_T, chLatin_E,
    chLatin_M, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gStartEntity[] =
{
    chOpenAngle, chBang, chLatin_E, chLatin_N, chLatin_T, chLatin_I,
    chLatin_T, chLatin_Y, chSpace, chNull
};
static const XMLCh  gNotation[] =
{
    chLatin_N, chLatin_D, chLatin_A, chLatin_T, chLatin_A,
    chSpace, chDoubleQuote, chNull
};


// ---------------------------------------------------------------------------
// Begin XdmfDOM Class Definition
// ---------------------------------------------------------------------------

class XdmfDOM : public virtual XdmfObject, public virtual XMLFormatTarget { 

public :

  XdmfDOM();
  ~XdmfDOM();
  
  const char * GetClassName() { return("XdmfDOM"); } ;

//  XdmfInt32  SetXMLVersion( XdmfFloat32 value );
//  XdmfFloat32  GetXMLVersion( void );  
//  XdmfInt32  SetDocType( XdmfString value );
//  XdmfString  GetDocType( void );
//  XdmfInt32  SetSystem( XdmfString value );
//  XdmfString  GetSystem( void );
  
  XdmfInt32  IsChild(DOM_Node nodeToCheck);
  XdmfInt32  IsChild(DOM_Node nodeToCheck, DOM_Node startNode);

  DOMParser   *GetParser( void );

  DOM_Element  GetElement( XdmfString tagName );
  DOM_Element  GetElement( XdmfString tagName, XdmfInt32 index );
  DOM_Element  GetElement( XdmfString tagName, XdmfInt32 index, DOM_Element elementNode );

  XdmfInt32  GetNumberOfElements( );
  XdmfInt32  GetNumberOfElements( XdmfString tagName );
  XdmfInt32  GetNumberOfElements( DOM_Element elementNode );
  XdmfInt32  GetNumberOfElements( XdmfString tagName, DOM_Element elementNode );

  DOM_Element  GetProcessingInstructionData(DOM_ProcessingInstruction pi);

  DOM_ProcessingInstruction  GetProcessingInstruction( XdmfString tagName );
  DOM_ProcessingInstruction  GetProcessingInstruction( XdmfString tagName, XdmfInt32 index );
  DOM_ProcessingInstruction  GetProcessingInstruction( XdmfString tagName, XdmfInt32 index, DOM_Node node );

  XdmfInt32  GetNumberOfProcessingInstructions( );
  XdmfInt32  GetNumberOfProcessingInstructions( XdmfString tagName );
  XdmfInt32  GetNumberOfProcessingInstructions( DOM_Element elementNode );
  XdmfInt32  GetNumberOfProcessingInstructions( XdmfString tagName, DOM_Element elementNode );

  DOM_Element  GetParameter( XdmfString parameterName );
  DOM_Element  GetParameter( XdmfString parameterName, XdmfInt32 index );
  DOM_Element  GetParameter( XdmfString parameterName, XdmfInt32 index, DOM_Element elementNode );

  XdmfInt32  GetNumberOfParameters( );
  XdmfInt32  GetNumberOfParameters( XdmfString parameterName );
  XdmfInt32  GetNumberOfParameters( DOM_Element elementNode );
  XdmfInt32  GetNumberOfParameters( XdmfString parameterName, DOM_Element elementNode );

  XdmfInt32  Parse( XdmfString xml );
  XdmfString   Serialize( void );

protected :
  DOMParser       *parser;
  XMLFormatter::UnRepFlags   unRepFlags;
  ostrstream      xmlStream;
  XMLFormatter      *formatter;
  XMLCh        *encodingName;

  XdmfDOM&   operator<<( const DOMString& toWrite );
  XdmfDOM&   operator<<( DOM_Node& toWrite );
  XdmfDOM&   operator+( const DOMString& s );
  XdmfInt32  SearchDOM(DOM_NodeList nodeList, XdmfInt32 count, XdmfInt32 searchType);
  XdmfInt32  SearchDOM(DOM_NodeList nodeList, XdmfString tagName, XdmfInt32 count, XdmfInt32 searchType);
  DOM_Node  SearchDOM(DOM_NodeList nodeList, XdmfString tagName, XdmfInt32 index, XdmfInt32 searchType, XdmfInt32 resetFlag);
        void writeChars(const XMLByte* const toWrite)
        {
           this->xmlStream << (char *) toWrite;
        }
  
};

#endif
