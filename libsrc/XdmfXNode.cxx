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
#include "XdmfXNode.h"

void
C__XdmfXNodeDelete(void *obj){
        XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
  if ( ObjectHandle ) {
          ObjectHandle->ManualDelete(ObjectHandle);
    }
        }

XdmfXNodeNode::XdmfXNodeNode() {
  this->Name = NULL;
  this->Data = NULL;
}

XdmfXNodeNode::~XdmfXNodeNode() {

if (this->Name != NULL) free(this->Name);
if (this->Data != NULL) free(this->Data);

}

void
XdmfXNodeNode::SetName(const char *name){
  if(this->Name != NULL)
    {
    free(this->Name);
    this->Name = 0;
    }
  if ( name )
    {
    this->Name = strdup(name);
    }
}

char *
XdmfXNodeNode::GetName(){
  return(this->Name);
}

void
XdmfXNodeNode::SetData(const char *data){
  if(this->Data != NULL) 
    {
    free(this->Data);
    this->Data = 0;
    }
  if ( data )
    {
    this->Data = strdup(data);  
    }
}

char *
XdmfXNodeNode::GetData(){
  return(this->Data);
}


XdmfXNode::XdmfXNode(){

this->key = XdmfLlist_new_key();
if(XdmfLlist_init(this->key) <= 0){
        XdmfErrorMessage("Can't Initialize List\n");
        exit(1);
}
this->size = 0;
this->ClientData = NULL;
this->UserData = NULL;

}

XdmfXNode::~XdmfXNode() {


XdmfXNodeNode *node;
node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
while(node != NULL) {

  node->SetData(0);
  node->SetName(0);
  XdmfLlist_delete_item(this->key, node);
//  This was not created with "new XdmfXNodeNode" so
//  it's part of the LinkedList that is freed with 
//   XdmfLlist_delete_item(this->key, node);
//  delete node;
  node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
}
//  Don't do this - it may be a treee node which
//  will result in it getting freed twice .... bad !!
// if (this->ClientData != NULL) free(this->ClientData);
XdmfLlist_remove_anchor(this->key);
}

void 
XdmfXNode::Set(const char *name, const char *data){

XdmfXNodeNode *node;
node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
while(node != NULL) {
   if (strcmp(node->GetName(), name) == 0) break;
  node = (XdmfXNodeNode *)XdmfLlist_next_member(node);
}
if (node == NULL) {
// Note : ( Jerry )
//  This seems to work but maked me nurvous
//  we really shoule use "new XdmfXNodeNode" then
//  just store the pointer in the Linked List.
  XdmfXNodeNode *newnode;
  newnode = (XdmfXNodeNode *)XdmfLlist_add_to_tail(this->key, sizeof(XdmfXNodeNode));
  newnode->SetName(name);
  newnode->SetData(data);
} else {
  if( data != NULL ){
    node->SetData(data);
  } else {
    XdmfLlist_delete_item(this->key, node);
  }
}
}

char *
XdmfXNode::Get(const char *name){
XdmfXNodeNode *node;
node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
while(node != NULL) {
   if (strcmp(node->GetName(), name) == 0) break;
  node = (XdmfXNodeNode *)XdmfLlist_next_member(node);
}
if (node != NULL) {
  return(node->GetData());
} else {
  // XdmfDebug("Element <" << name << ">does not exist\n");
  return(NULL);
}

}

int
XdmfXNode::GetSize(){

int size = 0;
XdmfXNodeNode *node;
node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
while(node != NULL) {
        size++;
        node = (XdmfXNodeNode *)XdmfLlist_next_member(node);
}
return(size);

}

char *
XdmfXNode::GetNameByIndex(int index){

int i;
XdmfXNodeNode *node;
node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
for(i = 0; i < index; i++) {
        node = (XdmfXNodeNode *)XdmfLlist_next_member(node);
}
if(node != NULL) {
        return(node->GetName());
} else {
        XdmfErrorMessage("Invalid Index\n");
        return(NULL);
}
}

char *
XdmfXNode::GetDataByIndex(int index){

int i;
XdmfXNodeNode *node;
node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
for(i = 0; i < index; i++) {
        node = (XdmfXNodeNode *)XdmfLlist_next_member(node);
}
if(node != NULL) {
        return(node->GetData());
} else {
        XdmfErrorMessage("Invalid Index\n");
        return(NULL);
}
}

void
XdmfXNode::Print(){

XdmfXNodeNode *node;
node = (XdmfXNodeNode *)XdmfLlist_first_member(this->key);
while(node != NULL) {
  cout << node->GetName() << " = " << node->GetData() << endl;
   node = (XdmfXNodeNode *)XdmfLlist_next_member(node);
}
cout.flush();

}

void 
XdmfXNode::ManualDelete(XdmfXNode *obj){
  delete obj;
}

extern "C" void *C__NewXdmfXNode(void) {
  XdmfXNode *NewObject = new XdmfXNode();
  return( NewObject );
  };

extern "C" void C__XdmfXNodeSet(void *obj,  const char *name, const char *data){
  XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
  ObjectHandle->Set(name, data);
  };
extern "C" char *C__XdmfXNodeGet(void *obj,  const char *name){
  XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
  return( (char *)ObjectHandle->Get(name) );
  };
extern "C" void C__XdmfXNodePrint(void *obj){
  XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
  ObjectHandle->Print();
  };
extern "C" int C__XdmfXNodeGetSize(void *obj){
  XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
  return( (int)ObjectHandle->GetSize() );
  };
extern "C" char *C__XdmfXNodeGetNameByIndex(void *obj, int index){
  XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
  return( (char *)ObjectHandle->GetNameByIndex(index) );
  };
extern "C" char *C__XdmfXNodeGetDataByIndex(void *obj, int index){
  XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
  return( (char *)ObjectHandle->GetDataByIndex(index) );
  };

