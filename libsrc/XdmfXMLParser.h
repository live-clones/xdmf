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
#ifndef __XdmfXMLParser_h 
#define __XdmfXMLParser_h

#include "XdmfDice.h"

#if defined(__cplusplus)
extern "C" {
#endif 

#include "expat.h"

typedef struct {
        XDMF_TREE_NODE *Root;
        XDMF_TREE_NODE *Node;
        XDMF_TREE_NODE *Parent;
        int Depth;
        int NumElements;
} XMLUserData;

typedef struct {
  const char * tag;
  const char * attribute;
  const char * value;
  int occurance;
  XDMF_TREE_NODE *node;
} FindNodeData;

extern int ExternalEntity(XML_Parser parser,
                const XML_Char *context,
                const XML_Char *base,
                const XML_Char *systemId,
                const XML_Char *publicId );
extern void ProcessingElement( void *userData,
  const char *target, const char *data );
extern void StartElement(void *userData, const char *name, const char **atts);
extern void EndElement(void *userData, const char *name);
extern void GetDefaultData(void *userData, const XML_Char *data, int len);
extern void GetData(void *userData, const XML_Char *data, int len);
extern int PrintTree(XDMF_TREE_NODE *tree, void *clientData);
extern int C_SetDepth(XDMF_TREE_NODE *tree, void *clientData);
extern int C_FindXMLNode(XDMF_TREE_NODE *tree, void *clientData);
extern int C_FindXMLNodeByAttribute(XDMF_TREE_NODE *tree, void *clientData);
extern int C_FindXMLNumberOfNodes(XDMF_TREE_NODE *tree, void *clientData);
extern int C_FindXMLNumberOfNodesByAttribute(XDMF_TREE_NODE *tree, void *clientData);

#if defined(__cplusplus)
}
#endif

#endif 

