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

#include <libxml/globals.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>

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
  this->StaticBuffer = (XdmfPointer)xmlBufferCreateSize(1024);
  this->Tree = NULL;
  this->Output = &cout;
  this->Input = &cin;
  this->Doc = NULL;

  this->OutputFileName = 0;
  this->InputFileName = 0;
  XDMF_STRING_DUPLICATE(this->OutputFileName, "stdout");
  XDMF_STRING_DUPLICATE(this->InputFileName, "stdin");
  this->SetNdgmHost( "" );
  this->SetWorkingDirectory( "" );
  // Allow Indenting on Serialization
  xmlIndentTreeOutput = 1;
  xmlKeepBlanksDefault(0);
  // Set Default Options
  this->ParserOptions = XML_PARSE_NOENT | XML_PARSE_XINCLUDE | XML_PARSE_NONET;
}

XdmfDOM::~XdmfDOM(){
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
  if ( this->StaticBuffer) {
    xmlBufferFree((xmlBuffer *)this->StaticBuffer);
    }
  this->SetWorkingDirectory(0);
  this->SetNdgmHost(0);
  if ( this->InputFileName ) {
    delete [] this->InputFileName;
    }
  if ( this->OutputFileName ) {
    delete [] this->OutputFileName;
    }
    if(this->Doc) xmlFreeDoc((xmlDoc *)this->Doc);
}

XdmfInt32
XdmfDOM::GetNumberOfAttributes( XdmfXmlNode Node ){
XdmfInt32  NumberOfAttributes = 0;
xmlAttr *attr;
xmlNode *node;

node = xmlNodePtrCAST Node;
attr = node->properties;
while(attr){
    attr = attr->next;
    NumberOfAttributes++;
}
return( NumberOfAttributes );
}

XdmfConstString
XdmfDOM::GetAttributeName( XdmfXmlNode Node, XdmfInt32 Index ){
XdmfInt32  EIndex = 0;
xmlAttr *attr;
xmlNode *node;

node = xmlNodePtrCAST Node;

attr = node->properties;
while( attr && (EIndex < Index)){
    attr = attr->next;
    EIndex++;
}
if (attr) {
    return((XdmfConstString)attr->name);
}
return(NULL);
}

XdmfInt32
XdmfDOM::IsChild( XdmfXmlNode ChildToCheck, XdmfXmlNode Node) {
xmlNode *node, *child, *nodeToCheck;

node = xmlNodePtrCAST Node;
nodeToCheck = xmlNodePtrCAST ChildToCheck;
// Check All Children
for(child=node->xmlChildrenNode; child ; child=child->next){
    if(child->type = XML_ELEMENT_NODE) {
        // Is this it?
        if(child == nodeToCheck) {
            return(XDMF_SUCCESS);
        }
        // Check Its children
        if(this->IsChild((XdmfXmlNode)nodeToCheck, child) == XDMF_SUCCESS){
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

XdmfInt32
XdmfDOM::GenerateTail() {
  *this->Output << "</Xdmf>";
  this->Output->flush();
  return( XDMF_SUCCESS );
}

XdmfConstString
XdmfDOM::Serialize(XdmfXmlNode Node) {
int buflen;
XdmfConstString XML = NULL;
xmlBufferPtr bufp;
xmlNode *node;

node = (xmlNode *)Node;
if(!node) node = (xmlNode *)this->Tree;
if(!node) return(NULL);
bufp = xmlBufferCreate();
buflen = xmlNodeDump(bufp, (xmlDoc *)this->Doc, node, 0, 1);
if( buflen > 0 ){
    xmlBuffer *sbufp;
    sbufp = (xmlBuffer *)this->StaticBuffer;
    xmlBufferEmpty(sbufp);
    xmlBufferCat(sbufp, bufp->content);
    XML = (XdmfConstString)sbufp->content;
    xmlBufferFree(bufp);
}
return(XML);
}

XdmfXmlNode 
XdmfDOM::__Parse(XdmfConstString inxml) {

xmlNode *Root = NULL;
int parserOptions;

parserOptions = this->ParserOptions;
if(inxml) {
    // Is  this XML or a File Name
    if(inxml[0] == '<'){
        // It's XML
        this->Doc = xmlReadMemory(inxml, strlen(inxml), NULL, NULL, parserOptions);
    }else{
        // It's a File Name
        this->SetInputFileName(inxml);
        this->Doc = xmlReadFile(this->GetInputFileName(), NULL, parserOptions);
    }
}else{
    this->Doc = xmlReadFile(this->GetInputFileName(), NULL, parserOptions);
}
if(this->Doc){
    if(parserOptions & XML_PARSE_XINCLUDE){
        if (xmlXIncludeProcess((xmlDoc *)this->Doc) < 0) {
            xmlFreeDoc((xmlDoc *)this->Doc);
            this->Doc = NULL;
        }
    }
    Root = xmlDocGetRootElement((xmlDoc *)this->Doc);
}

return((XdmfXmlNode)Root);
}

XdmfInt32
XdmfDOM::Parse(XdmfConstString inxml) {

XdmfXmlNode Root;
XdmfXmlNode Node;
XdmfConstString  Attribute;

// Remove Previous Data
if(this->Doc) xmlFreeDoc((xmlDoc *)this->Doc);
this->Tree = NULL;

Root = this->__Parse(inxml);
if (Root) {
  this->Tree = Root;
} else {
  return(XDMF_FAIL);
  }

Node = this->FindElement("Xdmf", 0, NULL );
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
XdmfDOM::DeleteNode(XdmfXmlNode Node) {
xmlNode *node = xmlNodePtrCAST Node;
if(!node) return(XDMF_FAIL);
xmlUnlinkNode(node);
xmlFreeNode(node);
return(XDMF_SUCCESS);
}

XdmfInt32
XdmfDOM::InsertFromString(XdmfXmlNode Parent, XdmfConstString inxml) {

XdmfXmlNode NewNode = NULL;
xmlDoc *doc = NULL;
xmlNode *root = NULL;
int parserOptions = this->ParserOptions;

doc = xmlReadMemory(inxml, strlen(inxml), NULL, NULL, parserOptions);
if(doc){
    root = xmlDocGetRootElement(doc);
    NewNode = (XdmfPointer)root;
}
if(NewNode){
    XdmfInt32 Status;
    Status = this->Insert(Parent, NewNode);
    xmlFreeDoc(doc);
    return(Status);
}
return(XDMF_FAIL);
}

XdmfInt32
XdmfDOM::Insert(XdmfXmlNode Parent, XdmfXmlNode Child) {
xmlNode *parent, *child;

parent = xmlNodePtrCAST Parent;
child = xmlNodePtrCAST Child;
if(parent && child){
    if(xmlAddChildList(parent, xmlCopyNodeList(child))){
        return(XDMF_SUCCESS);
    }
}
return(XDMF_FAIL);
}

XdmfXmlNode
XdmfDOM::GetChild( XdmfInt64 Index, XdmfXmlNode Node ){
xmlNode *node, *child;

node = xmlNodePtrCAST Node;
child = node->children;
while(child && Index){
    child = XdmfGetNextElement(child);
    Index--;
}
return((XdmfXmlNode)child);
}


XdmfInt64
XdmfDOM::GetNumberOfChildren( XdmfXmlNode Node ){
XdmfInt64 Index = 0;
xmlNode *node, *child;

node = xmlNodePtrCAST Node;
if(!node){
    node = xmlNodePtrCAST this->Tree;
}

if(!node) return(0);
child = node->children;
while(child){
    if(child->type == XML_ELEMENT_NODE) Index++;
    child = XdmfGetNextElement(child);
}
return(Index);
}

XdmfXmlNode  
XdmfDOM::GetRoot( void ) {
return(this->Tree);
}

XdmfXmlNode  
XdmfDOM::FindElement(XdmfConstString TagName, XdmfInt32 Index, XdmfXmlNode Node ) {

XdmfString type = (XdmfString )TagName;
XdmfXmlNode Start;
xmlNode *child;

Start = Node;
if(!Start) {
    if(!this->Tree) return( NULL );
    Start = this->Tree;
}
child = (xmlNodePtrCAST Start)->children;
if(!child) return(NULL);
if ( type ) {
  if( STRNCASECMP( type, "NULL", 4 ) == 0 ) type = NULL;
}
if ( !type ) {
    return(this->GetChild(Index, Start));
} else {
    while(child){
        // cout << "Checking " << type << " against " << (xmlNodePtrCAST child)->name << endl;
        if(XDMF_WORD_CMP((const char *)type, (const char *)(child)->name)){
            if(Index <= 0){
                return(child);
            }
            Index--;
        }
        child = XdmfGetNextElement(child);
    }
}
return(NULL);
}

XdmfXmlNode  
XdmfDOM::FindElementByAttribute(XdmfConstString Attribute,
    XdmfConstString Value, XdmfInt32 Index, XdmfXmlNode Node ) {
XdmfXmlNode Start;
xmlNode *child;

Start = Node;
if( !Start) {
  Start = this->Tree;
}
if( !Start ) return( NULL );
child = (xmlNodePtrCAST Start)->children;
while(child){
    if(XDMF_WORD_CMP((const char *)xmlGetProp(child, (xmlChar *)Attribute), (const char *)Value)){
        if(Index <= 0){
            return(child);
        }
        Index--;
    }
    child = XdmfGetNextElement(child);
}
return(NULL);
}


XdmfInt32
XdmfDOM::FindNumberOfElements(XdmfConstString TagName, XdmfXmlNode Node ) {
xmlNode *node, *child;
XdmfInt32 Index = 0;

if( !Node ) {
    if(!this->Tree) return(XDMF_FAIL);
    Node = this->Tree;
}
node = xmlNodePtrCAST  Node;
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
XdmfInt32 NElements = 0;
XdmfXmlNode Start;
xmlNode *child;

Start = Node;
if( !Start) {
  Start = this->Tree;
}
if( !Start ) return(0);
child = (xmlNodePtrCAST Start)->children;
while(child){
    if(XDMF_WORD_CMP((const char *)xmlGetProp(child, (xmlChar *)Attribute), (const char *)Value)){
        NElements++;
    }
    child = XdmfGetNextElement(child);
}
return(0);
}

XdmfConstString
XdmfDOM::GetAttribute(XdmfXmlNode Node, XdmfConstString Attribute) {

xmlNode *node;
if( !Node ) {
    if(!this->Tree) return(NULL);
    Node = this->Tree;
}
node = xmlNodePtrCAST Node;
return((XdmfConstString)xmlGetProp(node, (xmlChar *)Attribute));
}

XdmfConstString
XdmfDOM::GetCData(XdmfXmlNode Node) {
xmlNode *node;
xmlBuffer *sbufp;
char    *txt;

if( !Node ) {
    if(!this->Tree) return(NULL);
    Node = this->Tree;
}
node = xmlNodePtrCAST Node;
txt = (char *)xmlNodeListGetString((xmlDoc *)this->Doc, node->xmlChildrenNode, 1);
if(!txt) return(NULL);
sbufp = (xmlBuffer *)this->StaticBuffer;
xmlBufferEmpty(sbufp);
xmlBufferCat(sbufp, (const xmlChar *)txt);
xmlFree(txt);
return((XdmfConstString)this->StaticBuffer);
}

XdmfConstString
XdmfDOM::Get(XdmfXmlNode Node, XdmfConstString Attribute) {

if( STRNCASECMP( Attribute, "CDATA", 5 ) == 0 ){
    return(this->GetCData(Node));
}
return(this->GetAttribute(Node, Attribute));
}

void
XdmfDOM::Set( XdmfXmlNode Node, XdmfConstString Attribute, XdmfConstString Value ){

if(!Node) return;
if( STRNCASECMP( Attribute, "CDATA", 5 ) == 0 ){
    xmlNode *node, *next, *text;

    // Delete Existing CData
    node = (xmlNodePtrCAST Node)->children;
    while(node){
        next = node->next;
        if ((node->type == XML_TEXT_NODE) ||
            (node->type == XML_CDATA_SECTION_NODE)) {
            xmlUnlinkNode(node);
            xmlFreeNode(node);
        }
        node = next;
    }
    text = xmlNewDocText((xmlDoc *)this->Doc, (const xmlChar *)Value);
    xmlAddChildList(xmlNodePtrCAST Node, text);
}else{
    xmlSetProp(xmlNodePtrCAST Node, (xmlChar *)Attribute, (xmlChar *)Value);
}
}

