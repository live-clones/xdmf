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
#ifndef SWIG

#ifndef __XdmfXNode_h
#define __XdmfXNode_h

#ifdef __cplusplus

#include "XdmfObject.h"

#include "XdmfDice.h"

//! XDMF Runtime Expandable Structure.
/*!
XdmfXNode : XDMF Runtime Expandable Structure.
This is an attempt to implement Tcl Arrays in C++.
*/
#ifndef DOXYGEN_SKIP
class XDMF_EXPORT XdmfXNodeNode : public virtual XdmfObject {

public :
  XdmfXNodeNode();
  ~XdmfXNodeNode();
  void SetName(XdmfConstString name);
  XdmfString  GetName();
  void SetData(XdmfConstString data);
  XdmfString  GetData();

protected :

XdmfString Name;
XdmfString Data;

};
#endif /* DOXYGEN_SKIP */
class XDMF_EXPORT XdmfXNode : public virtual XdmfObject {

public :
  
  XdmfXNode();
  ~XdmfXNode();
  void Set(XdmfConstString name, XdmfConstString data);
  XdmfString  Get(XdmfConstString name);
  void Print();
        int GetSize();
        XdmfString GetNameByIndex(int index);
        XdmfString GetDataByIndex(int index);
  void *GetClientData( void ) {
      return( this->ClientData );
      }
  void SetClientData( void *clientData ) {
    this->ClientData = clientData;
    }
  void ManualDelete(XdmfXNode *obj);
  XdmfGetValueMacro( UserData, XdmfPointer );
  XdmfSetValueMacro( UserData, XdmfPointer );

protected :

void       *ClientData;
XdmfPointer  UserData;
XDMF_LIST_KEY  key;
int        size;

};


// void C__XdmfXNodeDelete(void *obj){
//       XdmfXNode *ObjectHandle = ( XdmfXNode *)obj;
//     ObjectHandle->ManualDelete(ObjectHandle);
//   }

extern void C__XdmfXNodeDelete(void *XdmfXNode);

#else 

extern void *C__NewXdmfXNode();
extern void C__XdmfXNodeSet(void *XdmfXNode, XdmfConstString name, XdmfConstString data);
extern XdmfString C__XdmfXNodeGet(void *XdmfXNode, XdmfConstString name);
extern int  C__XdmfXNodeGetSize(void *XdmfXNode);
extern XdmfString C__XdmfXNodeGetNameByIndex(void *XdmfXNode, int index);
extern XdmfString C__XdmfXNodeGetDataByIndex(void *XdmfXNode, int index);
extern void C__XdmfXNodePrint(void *XdmfXNode);
extern void C__XdmfXNodeDelete(void *XdmfXNode);

#endif /* ifdef __cplusplus */

#endif /* ifndef __XdmfXNode_h */

#else /* SWIG */

class XdmfXNode : public virtual XdmfObject {

public :
  
  XdmfXNode();
  ~XdmfXNode();
  void Set(XdmfConstString name, XdmfConstString data);
  XdmfString  Get(XdmfConstString name);
  void Print();
        int GetSize();
        XdmfString GetNameByIndex(int index);
        XdmfString GetDataByIndex(int index);
  void *GetClientData( void ) {
      return( this->ClientData );
      }
  void SetClientData( void *ClientData ) {
    this->ClientData = ClientData;
    }
  void ManualDelete(XdmfXNode *obj);
};

#endif

