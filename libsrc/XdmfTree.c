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
#include <XdmfDice.h>


XDMF_TREE_NODE *
XdmfTree_add(XDMF_TREE_NODE *parent, ICE_PTR data)
{
XDMF_TREE_NODE  *this_node;

this_node = (XDMF_TREE_NODE *)calloc(1, sizeof(XDMF_TREE_NODE));
if(this_node == NULL){
  ICE_PERROR
  perror("XdmfTree_add : calloc:");
  return(NULL);
  }

this_node->nchild = 0;
this_node->data = data;
this_node->children = (XDMF_TREE_NODE **)calloc(1, sizeof(XDMF_TREE_NODE));
if(this_node->children == NULL){
  ICE_PERROR
  perror("XdmfTree_add : calloc");
  return(NULL);
  }
this_node->parent = parent;

if(parent != NULL){
  parent->nchild++;
  parent->children = (XDMF_TREE_NODE **)realloc(parent->children,
      (parent->nchild + 1) *  sizeof(XDMF_TREE_NODE));
  if(parent->children == NULL){
    ICE_PERROR
    perror("XdmfTree_add : realloc");
    return(NULL);
    }
  parent->children[parent->nchild - 1] = this_node;
  parent->children[parent->nchild] = NULL;
  }

return(this_node);
}

XDMF_TREE_NODE *
XdmfTree_add_branch(XDMF_TREE_NODE *parent, XDMF_TREE_NODE *branch)
{

if(parent != NULL){
  branch->parent = parent;
  parent->nchild++;
  parent->children = (XDMF_TREE_NODE **)realloc(parent->children,
      (parent->nchild + 1) *  sizeof(XDMF_TREE_NODE));
  if(parent->children == NULL){
    ICE_PERROR
    perror("XdmfTree_add : realloc");
    return(NULL);
    }
  parent->children[parent->nchild - 1] = branch;
  parent->children[parent->nchild] = NULL;
  }

return(parent);
}

ICE_VOID
XdmfTree_remove(XDMF_TREE_NODE *this_node,
  ICE_VOID (*client_data_free)(ICE_PTR))
{
ICE_32_INT  i;
XDMF_TREE_NODE *child;

while(this_node->nchild > 0){
  child = this_node->children[this_node->nchild - 1];
  XdmfTree_remove(child, client_data_free);
  }

if(this_node->parent != NULL){
  for(i = 0 ; i < this_node->parent->nchild ; i++){
    if(this_node->parent->children[i] == this_node){
      memmove(&this_node->parent->children[i],
        &this_node->parent->children[i + 1],
    (this_node->parent->nchild - i) * sizeof(XDMF_TREE_NODE));
      break;
      }
    }
  this_node->parent->nchild--;
  }

if(client_data_free != NULL){
  client_data_free(this_node->data);
  }

   free(this_node->children);
   free(this_node);
}

ICE_32_INT
XdmfTree_walk(XDMF_TREE_NODE *this_node,
  ICE_32_INT (*node_list_call)(XDMF_TREE_NODE *, void *),
  void *clientdata)
{
ICE_32_INT  i;

if(node_list_call != NULL){
  if(node_list_call(this_node, clientdata) <= ICE_FALSE){
    /* End of Walk */
    return(ICE_FALSE);
    }
  }

for(i = 0 ; i < this_node->nchild ; i++){
   if(XdmfTree_walk(this_node->children[i], node_list_call, clientdata) <= ICE_FALSE){
  return(ICE_FALSE);
  }
   }

return(ICE_TRUE);
}
