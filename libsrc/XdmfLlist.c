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
#ifdef _MSC_VER
#pragma warning (disable: 4115)
#endif

#include "XdmfDice.h"

static  XdmfLlist_Anchor  *xdmf_master_llist = NULL;
static  XdmfLlist    XdmfLlist_dummy;
static  unsigned long    prev_ptr_plus_pad;
static  XDMF_LIST_KEY    next_unique_key = XDMF_LIST_NDGM_LAST_KEY;

 XDMF_LIST_KEY
XdmfLlist_new_key(void)
{
return(next_unique_key++);
}

void
XdmfLlist_remove_anchor(XDMF_LIST_KEY key)
{
XdmfLlist_Anchor  *item = xdmf_master_llist;
XdmfLlist_Anchor  *previous = xdmf_master_llist;

while(item != NULL){
  if(item->key == key){
    break;
    }
  previous = item;
  item = item->next;
  }

if ( item->key != key )
  {
  return;
  }
if ( previous == item )
  {
  xdmf_master_llist = item->next;
  }
else
  {
  previous->next = item->next;
  }
free(item);
}

XdmfLlist_Anchor *
XdmfLlist_anchor_init(XDMF_LIST_KEY key)
{
char      *ptr1, *ptr2;
XdmfLlist_Anchor  *new, *item;

new = (XdmfLlist_Anchor *)calloc(1, sizeof(XdmfLlist_Anchor));
new->key = key;

if(xdmf_master_llist == NULL){
  xdmf_master_llist = new;
  ptr1 = (char *)&XdmfLlist_dummy.next;
  ptr2 = (char *)&XdmfLlist_dummy.data;
  prev_ptr_plus_pad = (unsigned long)(ptr2 - ptr1);
}else{
  item = xdmf_master_llist;
  while(item->next != NULL){
    item = item->next;
    }
  item->next = new;
  }
return(new);
}

XdmfLlist_Anchor *
XdmfLlist_find_anchor(XDMF_LIST_KEY key)
{
XdmfLlist_Anchor  *item = xdmf_master_llist;

while(item != NULL){
  if(item->key == key){
    return(item);
    }
  item = item->next;
  }
/* None Exists, Make a new one */
item = XdmfLlist_anchor_init(key);
return(item);
}

 int
XdmfLlist_init(XDMF_LIST_KEY key)
{
XdmfLlist_Anchor  *anchor;

anchor = XdmfLlist_find_anchor(key);
if(anchor == NULL){
  return(-1);
}

return(1);
}

 void *
XdmfLlist_add_to_head(XDMF_LIST_KEY key, XDMF_LIST_KEY body_size)
{
XdmfLlist     *new;
XdmfLlist_Anchor  *anchor;

new = (XdmfLlist *)calloc(1, sizeof(XdmfLlist) + body_size);
anchor = XdmfLlist_find_anchor(key);

new->next = anchor->member;
anchor->member = new;
return(&new->data);
}

 void *
XdmfLlist_add_to_tail(XDMF_LIST_KEY key, XDMF_LIST_KEY body_size)
{
XdmfLlist     *new, *item;
XdmfLlist_Anchor  *anchor;

new = (XdmfLlist *)calloc(1, sizeof(XdmfLlist) + body_size);
anchor = XdmfLlist_find_anchor(key);

item = anchor->member;
if(item == NULL){
  anchor->member = new;
}else{
  while(item->next != NULL){
    item = item->next;
  }
  item->next = new;
  new->prev = item;
}
return(&new->data);
}

 void *
XdmfLlist_add_before(XDMF_LIST_KEY key, void *item_after, XDMF_LIST_KEY body_size)
{
char      *ptr, *dptr;
XdmfLlist     *new, *item;
XdmfLlist_Anchor  *anchor;

new = (XdmfLlist *)calloc(1, sizeof(XdmfLlist) + body_size);
dptr = (char *)item_after;
anchor = XdmfLlist_find_anchor(key);
item = anchor->member;

if(item == NULL){
  anchor->member = new;
  return(&new->data);
}

while(item != NULL){
  ptr = (char *)&item->data;
  if(ptr == dptr){
    if(item->prev != NULL){
      new->prev = item->prev;
      new->next = item;
      item->prev->next = new;
      item->prev = new;
      }
    if(anchor->member == item){
      anchor->member = new;
      new->next = item;
      item->prev = new;
      }
    return(&new->data);
    }
  item = item->next;
  }

/* Not found put it in front */
item = anchor->member;
item->prev = new;
new->next = item;
anchor->member = new;
return(&new->data);
}

 void *
XdmfLlist_add_after(XDMF_LIST_KEY key, void *item_before, XDMF_LIST_KEY body_size)
{
char      *ptr, *dptr;
XdmfLlist     *new, *item, *last = 0;
XdmfLlist_Anchor  *anchor;

new = (XdmfLlist *)calloc(1, sizeof(XdmfLlist) + body_size);
dptr = (char *)item_before;
anchor = XdmfLlist_find_anchor(key);
item = anchor->member;

if(item == NULL){
  anchor->member = new;
  return(&new->data);
}

while(item != NULL){
  ptr = (char *)&item->data;
  if(ptr == dptr){
    if(item->next != NULL){
      item->next->prev = new;
      new->next = item->next;
      }
    new->prev = item;
    item->next = new;
    return(&new->data);
    }
  last = item;
  item = item->next;
  }

/* Not found put it at end */
if ( last )
  {
  last->next = new;
  }
new->prev = new;
return(&new->data);
}

 void
XdmfLlist_delete_item(XDMF_LIST_KEY key, void *item_to_delete)
{
char      *ptr, *dptr;
XdmfLlist_Anchor  *anchor;
XdmfLlist    *item;


dptr = (char *)item_to_delete;
anchor = XdmfLlist_find_anchor(key);
item = anchor->member;

while(item != NULL){
  ptr = (char *)&item->data;
  if(ptr == dptr){
    if(item->prev != NULL){
      item->prev->next = item->next;
      }
    if(item->next != NULL){
      item->next->prev = item->prev;
      }
    if(anchor->member == item){
      anchor->member = item->next;
      }
    free(item);
    return;
    }
  item = item->next;
  }
}

 void *
XdmfLlist_first_member(XDMF_LIST_KEY key)
{
XdmfLlist_Anchor  *anchor;

anchor = XdmfLlist_find_anchor(key);
if(anchor->member == NULL){
  return(NULL);
}else{
  return(&anchor->member->data);
  }
}

 void *
XdmfLlist_next_member(void *member)
{
char     *next_ptr;
XdmfLlist  *next;

next_ptr = (char *)member;
next_ptr -= prev_ptr_plus_pad;
next = *(XdmfLlist **)next_ptr;

if(next == NULL){
  return(NULL);
}
return(&next->data);
}
