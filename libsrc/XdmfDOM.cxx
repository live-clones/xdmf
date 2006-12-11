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
#include "XdmfDOM.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#define xmlNodePtrCAST (xmlNode *)

XdmfDOM *HandleToXdmfDOM( XdmfConstString Source ){
  XdmfObject  *TempObj;
  XdmfDOM   *DOM;

  TempObj = HandleToXdmfObject( Source );
  DOM = (XdmfDOM *)TempObj;
  return( DOM );
  }

static xmlNode *
XdmfGetNextElement(xmlNode *Node){

xmlNode *NextElement = Node->next;
while(NextElement && (NextElement->type != XML_ELEMENT_NODE)){
    NextElement = NextElement->next;
}
return(NextElement);
}


XdmfDOM::XdmfDOM(){
  this->WorkingDirectory = 0;
  this->NdgmHost = 0;
  this->LastDOMGet = new char[256];
  this->tree = NULL;
  this->xml = NULL;
  this->Output = &cout;
  this->Input = &cin;
  this->Doc = NULL;

  this->OutputFileName = 0;
  this->InputFileName = 0;
  XDMF_STRING_DUPLICATE(this->OutputFileName, "stdout");
  XDMF_STRING_DUPLICATE(this->InputFileName, "stdin");
  this->SetNdgmHost( "" );
  this->SetWorkingDirectory( "" );
}

XdmfDOM::~XdmfDOM(){
  if (this->xml != NULL) free(this->xml);
  if( ( this->Output != &cout ) && ( this->Output != &cerr ) ) {
    ofstream *OldOutput = ( ofstream *)this->Output;
    OldOutput->close();
  }
  if( this->Input != &cin ) {
    ifstream *OldInput = ( ifstream *)this->Input;
    OldInput->close();
    delete this->Input;
    this->Input = &cin;
  }
  if ( this->LastDOMGet )
    {
    delete [] this->LastDOMGet;
    }
  this->SetWorkingDirectory(0);
  this->SetNdgmHost(0);
  if ( this->InputFileName )
    {
    delete [] this->InputFileName;
    }
  if ( this->OutputFileName )
    {
    delete [] this->OutputFileName;
    }
}

XdmfInt32
XdmfDOM::GetNumberOfAttributes( XdmfXmlNode Node ){
XdmfInt32  NumberOfAttributes = 0;
xmlAttr *Attr;

Attr = (xmlNodePtrCAST Node)->properties;
while(Attr){
    Attr = Attr->next;
    NumberOfAttributes++;
}
return( NumberOfAttributes );
}

XdmfConstString
XdmfDOM::GetAttribute( XdmfXmlNode Node, XdmfInt32 Index ){
xmlAttr *Attr;
XdmfConstString  AttributeValue = NULL;
XdmfInt32  EIndex = 0;

Attr = (xmlNodePtrCAST Node)->properties;
while( Attr && (EIndex < Index)){
    Attr = Attr->next;
    EIndex++;
}
if (Attr) {
    AttributeValue = (XdmfConstString)xmlGetProp(xmlNodePtrCAST Node, Attr->name);
}
return(AttributeValue);
}

XdmfInt32
XdmfDOM::IsChild( XdmfXmlNode ChildToCheck, XdmfXmlNode Start ) {
XdmfXmlNode Node;

Node = (xmlNodePtrCAST Start)->xmlChildrenNode;
// Check All Children
for(Node=(xmlNodePtrCAST Start)->xmlChildrenNode ; Node ; Node=(xmlNodePtrCAST Node)->next){
    if((xmlNodePtrCAST Node)->type = XML_ELEMENT_NODE) {
        // Is this it?
        if(Node == ChildToCheck) {
            return(XDMF_SUCCESS);
        }
        // Check Its children
        if(this->IsChild(ChildToCheck, Node) == XDMF_SUCCESS){
            return(XDMF_SUCCESS);
        }
    }
}

return(XDMF_FAIL);
}

XdmfInt32
XdmfDOM::SetOutputFileName( XdmfConstString Filename ){

  if( ( this->Output != &cout ) && ( this->Output != &cerr ) ) {
          ofstream *OldOutput = ( ofstream *)this->Output;
          OldOutput->close();
        }
  if( XDMF_WORD_CMP( Filename, "stdin" ) ) {
          this->Output = &cout;
  } else if( XDMF_WORD_CMP( Filename, "stderr" ) ) {
          this->Output = &cerr;
  } else {
          ofstream        *NewOutput = new ofstream( Filename );
          if( !NewOutput ) {
                  XdmfErrorMessage("Can't Open Output File " << Filename );
                  return( XDMF_FAIL );
                }
          this->Output = NewOutput;
        }
  if ( this->OutputFileName )
    {
    delete [] this->OutputFileName;
    }
  XDMF_STRING_DUPLICATE(this->InputFileName, Filename);
  return( XDMF_SUCCESS );

}

XdmfInt32
XdmfDOM::SetInputFileName( XdmfConstString Filename ){

  if( this->Input != &cin ) {
    ifstream *OldInput = ( ifstream *)this->Input;
    OldInput->close();
    delete this->Input;
    this->Input = &cin;
  }
  if( XDMF_WORD_CMP( Filename, "stdin" ) ) {
    this->Input = &cin;
  } else {
    ifstream        *NewInput = new ifstream( Filename );
    if( !NewInput ) {
      XdmfErrorMessage("Can't Open Input File " << Filename );
      return( XDMF_FAIL );
    }
    this->Input = NewInput;
  }
  if ( this->InputFileName )
    {
    delete [] this->InputFileName;
    }
  XDMF_STRING_DUPLICATE(this->InputFileName, Filename);
  return( XDMF_SUCCESS );
}

XdmfInt32
XdmfDOM::GenerateHead() {
  *this->Output << "<?xml version=\"1.0\" ?>" << endl 
        << "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\">" << endl 
        << "<Xdmf>" << endl;
  this->Output->flush();
  return( XDMF_SUCCESS );
}

XdmfInt32
XdmfDOM::Puts( XdmfConstString String ){
  *this->Output << String;
  this->Output->flush();
  return( XDMF_SUCCESS );
}

XdmfConstString
XdmfDOM::Gets( void ) {
}

XdmfInt32
XdmfDOM::GenerateTail() {
  *this->Output << "</Xdmf>";
  this->Output->flush();
  return( XDMF_SUCCESS );
}

XdmfConstString
XdmfDOM::Serialize(XdmfXmlNode Node) {
XdmfConstString XML = NULL;
xmlBufferPtr bufp;
xmlNode *node;

node = (xmlNode *)Node;
if(!node) node = (xmlNode *)this->tree;
if(!node) return(NULL);
bufp = xmlBufferCreate();
if( xmlNodeDump(bufp, (xmlDoc *)this->Doc, node, 0, 0) > 0 ){
    XML = (XdmfConstString)bufp->content;
}
return(XML);
}

XdmfXmlNode 
XdmfDOM::__Parse( XdmfConstString inxml) {

xmlNode *Root = NULL;

if(inxml) {
    this->Doc = xmlReadMemory(inxml, strlen(inxml), NULL, NULL, 0);
}else{
    this->Doc = xmlReadFile(this->GetInputFileName(), NULL, 0);
}
if(this->Doc){
    Root = xmlDocGetRootElement((xmlDoc *)this->Doc);
}

return(Root);
}

XdmfInt32
XdmfDOM::Parse(XdmfConstString inxml) {

XdmfXmlNode Root;
XdmfXmlNode Node;
XdmfConstString  Attribute;

// Remove Previous Data
// if (this->tree != NULL) XdmfTree_remove(this->tree,C__XdmfXmlNodeDelete);
this->tree = NULL;
// if (this->xml != NULL) free(this->xml);
// this->xml = NULL;

Root = this->__Parse(inxml);
if (Root) {
  this->tree = Root;
} else {
  return(XDMF_FAIL);
  }

Node = this->FindElement( "Xdmf", 0, NULL );
if( Node != NULL ){
  Attribute = this->Get( Node, "NdgmHost" );
  if( Attribute != NULL ){
    XdmfDebug("NdgmHost = " << Attribute );
    this->SetNdgmHost( Attribute );
    }
  Attribute = this->Get( Node, "WorkingDirectory" );
  if( Attribute != NULL ){
    XdmfDebug("WorkingDirectory = " << Attribute );
    this->SetWorkingDirectory( Attribute );
    }
  }
return( XDMF_SUCCESS );
}


XdmfInt32
XdmfDOM::DeleteNode( XdmfXmlNode Node ) {
return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDOM::InsertFromString(XdmfXmlNode Parent, XdmfConstString inxml) {

XdmfXmlNode NewNode;

NewNode = (XdmfXmlNode)xmlNewDocText((xmlDoc *)this->Doc, (const xmlChar *)inxml);
if(NewNode){
    return(this->Insert(Parent, NewNode));
}
return(XDMF_FAIL);
}

XdmfInt32
XdmfDOM::Insert(XdmfXmlNode Parent, XdmfXmlNode Child) {

if(xmlAddChild(xmlNodePtrCAST Parent, xmlNodePtrCAST Child)){
    return(XDMF_SUCCESS);
}
return(XDMF_FAIL);
}

XdmfXmlNode
XdmfDOM::GetChild( XdmfInt64 Index, XdmfXmlNode Node ){

xmlNode *Child = (xmlNodePtrCAST Node)->children;
while(Child && Index){
    Child = XdmfGetNextElement(Child);
    Index--;
}
return(Child);
}


XdmfInt64
XdmfDOM::GetNumberOfChildren( XdmfXmlNode Node ){
XdmfInt64 Index = 0;
xmlNode *Child;

if(!Node){
    Node = this->tree;
}

if(!Node) return(0);
Child = (xmlNodePtrCAST Node)->children;
while(Child){
    Child = XdmfGetNextElement(Child);
    Index++;
}
return(Index);
}

XdmfXmlNode  
XdmfDOM::GetRoot( void ) {
return(this->tree);
}

XdmfXmlNode  
XdmfDOM::FindElement(XdmfConstString TagName, XdmfInt32 Index, XdmfXmlNode Node ) {

XdmfString type = (XdmfString )TagName;
XdmfXmlNode Start;
xmlNode *Child;

// XdmfDebug( " IN FindElement , type = " << type << " Node = " << Node << " #  " << occurance);
Start = Node;
if(!Start) {
    Start = this->tree;
    Child = (xmlNode *)this->tree;
    if( !Start ) return( NULL );
}else{
    Child = (xmlNodePtrCAST Start)->children;
}
if ( type ) {
  if( STRNCASECMP( type, "NULL", 4 ) == 0 ) type = NULL;
}
if ( !type ) {
    return(this->GetChild(Index, Start));
} else {
    while(Child){
        cout << "Checking " << type << " against " << (xmlNodePtrCAST Child)->name << endl;
        if(XDMF_WORD_CMP((const char *)type, (const char *)(xmlNodePtrCAST Child)->name)){
            if(Index <= 0){
                return(Child);
            }
            Index--;
        }
        Child = XdmfGetNextElement(Child);
    }
}
return(NULL);
}

XdmfXmlNode  
XdmfDOM::FindElementByAttribute(XdmfConstString Attribute,
    XdmfConstString Value, XdmfInt32 Index, XdmfXmlNode Node ) {
XdmfXmlNode Start;
xmlNode *Child = (xmlNodePtrCAST Node)->children;

Start = Node;
if( !Start) {
  Start = this->tree;
}
if( !Start ) return( NULL );
while(Child){
    if(XDMF_WORD_CMP((const char *)xmlGetProp(Child, (xmlChar *)Attribute), (const char *)Value)){
        if(Index <= 0){
            return(Child);
        }
        Index--;
    }
Child = XdmfGetNextElement(Child);
}
return(NULL);
}


XdmfInt32
XdmfDOM::FindNumberOfElements(XdmfConstString TagName, XdmfXmlNode Node ) {
xmlNode *node, *child;
XdmfInt32 Index = 0;

if( !Node ) {
    if(!this->tree) return(XDMF_FAIL);
    Node = this->tree;
}
node = (xmlNode *)Node;
child = node->children;
if(!child) return(0);
while(child){
    if(XDMF_WORD_CMP(TagName, (const char *)child->name)){
        Index++;
    }
    child = XdmfGetNextElement(child);
}
return(Index);
}

XdmfInt32
XdmfDOM::FindNumberOfElementsByAttribute(XdmfConstString Attribute, 
    XdmfConstString Value, XdmfXmlNode Node ) {
}

XdmfConstString
XdmfDOM::Get( XdmfXmlNode Node, XdmfConstString Attribute ) {

xmlNode *node;

if( !Node ) {
    if(!this->tree) return(NULL);
    Node = this->tree;
}
node = (xmlNode *)Node;
if( STRNCASECMP( Attribute, "CDATA", 5 ) == 0 ){
    char    *txt;
    delete [] this->LastDOMGet;
    txt = (char *)xmlNodeListGetString((xmlDoc *)this->Doc, node->xmlChildrenNode, 1);
    this->LastDOMGet = new char[ strlen(txt) + 2];
    strcpy(this->LastDOMGet, txt);
    xmlFree(txt);
    return((XdmfConstString)this->LastDOMGet);
}
return((XdmfConstString)xmlGetProp(node, (xmlChar *)Attribute));
}

void
XdmfDOM::Set( XdmfXmlNode Node, XdmfConstString Attribute, XdmfConstString Value ){
    // What About CDATA ??
    xmlSetProp(xmlNodePtrCAST Node, (xmlChar *)Attribute, (xmlChar *)Value);
}

