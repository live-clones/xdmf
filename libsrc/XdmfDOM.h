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


#include "XdmfObject.h"
#include "XdmfXNode.h"

#ifndef SWIG
#  include "XdmfXMLParser.h"
#endif

class XdmfParameter;

/*!
This is the Base XML Parsing Object. A XdmfDOM will 
read the XML and build an internal tree structure. The
tree can then be walked and queried. Any node can be 
"serialized". This generates an XML string that implements
the node and all of its' children.
*/
class XDMF_EXPORT XdmfDOM : public XdmfObject {

public :

  XdmfDOM();
  ~XdmfDOM();

  XdmfConstString GetClassName() { return("XdmfDOM"); } ;

  XdmfSetValueMacro(XMLVersion, float);
  XdmfGetValueMacro(XMLVersion, float);

  XdmfSetStringMacro(DocType);
  XdmfGetStringMacro(DocType);

  XdmfSetStringMacro(System);
  XdmfGetStringMacro(System);

  //! Set the FileName of the XML Description : stdin or Filename
        XdmfInt32 SetInputFileName( XdmfConstString Filename );

  //! Set the FileName of the XML Description : stderr or Filename
        XdmfInt32 SetOutputFileName( XdmfConstString Filename );

  //! Get the FileName of the XML Description
         XdmfGetStringMacro( InputFileName );

  //! Get the FileName of the XML Description
         XdmfGetStringMacro( OutputFileName );

  //! Get the XML destination
        XdmfGetValueMacro( Output, ostream *);
  
  //! Set the XML destination
        XdmfSetValueMacro( Output, ostream *);

  //! Get the XML destination
        XdmfGetValueMacro( Input, istream *);
  
  //! Set the XML destination
        XdmfSetValueMacro( Input, istream *);

  XdmfInt32 GenerateHead( void );
  XdmfInt32 Puts( XdmfConstString String );
  XdmfInt32 GenerateTail( void );

  XdmfConstString Gets( void );

  XDMF_TREE_NODE *GetTree( void );

  //! Parse XML without re-initializing entire DOM
  XDMF_TREE_NODE *__Parse( XdmfConstString xml );

  //! Re-Initialize and Parse 
  XdmfInt32 Parse(XdmfConstString xml = NULL );


  XdmfXNode *GetRoot( void );

  //! Get the Number of immediate Children
  XdmfInt64 GetNumberOfChildren( XdmfXNode *node = NULL);
  //! Get The N'th Child
  XdmfXNode *GetChild( XdmfInt64 Index , XdmfXNode *Node );
  //! Get Number of Attribute in a Node
  XdmfInt32 GetNumberOfAttributes( XdmfXNode *Node );
  //! Get Attribute Name by Index
  XdmfConstString GetAttribute( XdmfXNode *Node, XdmfInt32 Index );
  //! Is the XdmfXNode a child of "Start" in this DOM
  XdmfInt32  IsChild( XdmfXNode *ChildToCheck, XdmfXNode *Start = NULL );
  //! Convert DOM to XML String
  XdmfConstString Serialize(XdmfXNode *node = NULL);
  //! Insert a node into a DOM
  XdmfInt32 Insert(XdmfXNode *parent, XdmfXNode *node, XdmfInt32 Level = 0);
  //! Create a node from an XML string and insert it in the DOM
  XdmfInt32 InsertFromString(XdmfXNode *parent, XdmfConstString xml );
  //! Delete a node
  XdmfInt32 DeleteNode(XdmfXNode *node);
  //! Find the n'th occurance of a certain node type
/*!
Walk the tree and find the first
element that is of a certain type. 
Index ( 0 based ) can be used to find the n'th
node that satisfies the criteria. The search can also
tree starting at a particular node.
*/
  XdmfXNode *FindElement(XdmfConstString TagName,
      XdmfInt32 Index= 0,
      XdmfXNode *Node = NULL );
  XdmfXNode *FindElementByAttribute(XdmfConstString Attribute,
      XdmfConstString Value,
      XdmfInt32 Index= 0,
      XdmfXNode *Node = NULL );
  //! Find the number of nodes of a certain type
  XdmfInt32 FindNumberOfElements(XdmfConstString TagName,
      XdmfXNode *Node = NULL );
  XdmfInt32 FindNumberOfElementsByAttribute(XdmfConstString Attribute,
      XdmfConstString Value,
      XdmfXNode *Node = NULL );
  //! Find the n'th occurance of a certain type of PI
  XdmfXNode *FindProcessingInstruction( XdmfConstString Target = NULL,
      XdmfInt32 occurance = 0,
      XdmfXNode *Node = NULL );
  //! Find the number of PIs of a certain type
  XdmfInt32 FindNumberOfProcessingInstructions( XdmfConstString Target = NULL,
      XdmfXNode *Node = NULL );

//! Get the default NDGM Host to use for HDF5 files
  XdmfGetStringMacro( NdgmHost );
//! Set the default NDGM Host to use for HDF5 files
  void SetNdgmHost( XdmfConstString String ) { strcpy( this->NdgmHost, String ); }

//! Get the default Working directory to use for HDF5 files
  XdmfGetStringMacro( WorkingDirectory );
//! Set the default Working directory to use for HDF5 files
  void SetWorkingDirectory( XdmfConstString String ) { strcpy( this->WorkingDirectory, String ); }

/*!
Get the various attributes from a node. If the XML is :
  <Tag Name="Test" Type="Data">
  file.h5
  </Tag>
Dom->Get(Node, "Name")  will return "Test"
Dom->Get(Node, "Type")  will return "Data"
Dom->Get(Node, "Other")  will return NUll ; there is none
Dom->Get(Node, "CData")  will return "file.h5" ; the Character Data

The XdmfXNode can be parsed directly with :
Node->Get( "Name ); etc.

The reason to use this get is to confirm that this node
is in this DOM and to accomplish PARAMETER substiution.
*/
  XdmfConstString  Get( XdmfXNode *Node, XdmfConstString Attribute );

//! Set a Node's User Data
  void    SetUserData( XdmfXNode *Node, XdmfPointer UserData ){
        Node->SetUserData( UserData );
        }
//! Get a Node's User Data
  XdmfPointer  GetUserData( XdmfXNode *Node ){
        return( Node->GetUserData() );
        }
//! Set a new Attribute=Value in a Node
  void    Set( XdmfXNode *Node, XdmfConstString Attribute, XdmfConstString Value ) {
      if( !Node ) {
        Node = this->FindElement( NULL, 0, NULL );
        if( !Node ) {
          return;
          }
        }
      if( Value && ( STRCASECMP( Value, "NULL" ) == 0 )) {
        Value = NULL;
      }
      Node->Set( Attribute, Value );
      }

/*!
Return the number type of the node as set in the
XML by : NuberType="Integer" Precision="4"
*/
  XdmfInt32 GetNumberType( XdmfXNode *Node );

  XdmfInt32 FindNumberOfParameters( XdmfXNode *Node = NULL ) {
      return( this->FindNumberOfElements( "Parameter", Node ) );
      }

  XdmfParameter  *FindParameter( XdmfConstString ParameterName, XdmfXNode *Node = NULL );
  XdmfParameter  *GetParameter( XdmfInt32 Index = 0, XdmfXNode *Node = NULL );

protected :

char    NdgmHost[ XDMF_MAX_STRING_LENGTH ];
char    WorkingDirectory[ XDMF_MAX_STRING_LENGTH ];
char            InputFileName[ XDMF_MAX_STRING_LENGTH ];
char            OutputFileName[ XDMF_MAX_STRING_LENGTH ];
ostream         *Output;
istream         *Input;
float     XMLVersion;
XdmfString  DocType;
XdmfString  System;
XdmfString  xml;
XDMF_TREE_NODE  *tree;

void ExpandNode(XDMF_TREE_NODE *node, XdmfInt32 *size);
void ReNew(XdmfInt32 *size);

};

extern XDMF_EXPORT int GetXNodeSize( XdmfXNode *Node );
extern XDMF_EXPORT XdmfConstString GetXNodeName(XdmfXNode *Node, int index);
extern XDMF_EXPORT XdmfConstString GetXNodeData(XdmfXNode *Node, int index);
extern XDMF_EXPORT XdmfDOM *HandleToXdmfDOM( XdmfConstString Source );
#endif
