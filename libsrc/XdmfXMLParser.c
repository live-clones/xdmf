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

#include "XdmfObject.h"
#include "XdmfXMLParser.h"
#include "XdmfXNode.h"

#include "stdlib.h"
#include "string.h"
#include "math.h"


/* startElement Routines */

int ValidElement = 0;

#define XdmfXML_MAX_ATTRIBUTES 100

extern char *C_GetXMLFromFile( char *FileName );

int
ExternalEntity(  XML_Parser parser,
    const XML_Char *context,
    const XML_Char *base,
    const XML_Char *systemId,
    const XML_Char *publicId ) {

XML_Parser   NewParser;
XML_Char   *encoding = NULL;
XMLUserData   *data;
void     *userData;
char    Name[1024], *Namep;
char    *ExternalXML = NULL;
FILE    *fp;

(void)publicId;

Name[0] = '\0';
userData = XML_GetUserData( parser );
/*
fprintf(stderr, "Parsing External Entity" );
*/
if( userData ) {
        data = (XMLUserData *)userData;
  /*
  fprintf(stderr, " ExternalEntity Depth = %d\n", data->Depth);
  */
  if( data->Depth <= 0 ) {
    /* This is our dtd ... ignore for now */
    /* return( 1 ); */
    }
  if( base ){
    strcpy( Name, base );
    Namep = &Name[ strlen( Name ) - 1 ];
    while( Namep > &Name[0] ) {
      if ( ( *Namep == '/' )  || 
        ( *Namep == '\\' ) ) {
        Namep++;
        break;
      }
      Namep--;
    }
    *Namep = '\0';
  } else {
    Namep = Name;
  }
  if( systemId ){
    strcat( Name, systemId );
    }
  /*
  fprintf(stderr, "Using External Name = %s\n", Name );
  */
  /* Only Handle URI on disk file for now, http later */
  if( (fp = fopen( Name, "r" )) != NULL ) {
    fclose(fp);
    ExternalXML = C_GetXMLFromFile( Name );
  }
  
} else {
  fprintf(stderr, " ExternalEntity userData = NULL\n" );
  return( 1 );
}

/*
fprintf(stderr, "ExternalEntity : \n");
fprintf(stderr, "publicId = %s\n", publicId );
fprintf(stderr, "systemId = %s\n", systemId );
fprintf(stderr, "base = %s\n", base );
fprintf(stderr, "context = %s\n", context );
fprintf(stderr, "Name = %s\n", Name );
fflush(stderr);
*/
if ( ExternalXML ){

/* fprintf( stderr, "Parsing XML = %s\n", ExternalXML ); */
NewParser = XML_ExternalEntityParserCreate(parser,
    context,
    encoding);

if (!XML_Parse(NewParser, ExternalXML, strlen(ExternalXML), 1)) {
  fprintf(stderr, "Parse Error at XML line %d : %s\n",
    XML_GetCurrentLineNumber(NewParser),
    XML_ErrorString(XML_GetErrorCode(NewParser)));
    XML_ParserFree(NewParser);
    return ( 0 );
}
XML_ParserFree(NewParser);
free( ExternalXML );
}

return( 1 );
}

void ProcessingElement( void *userData,
    const char *target,
    const char *data ){

const char  *Attributes[XdmfXML_MAX_ATTRIBUTES];
const char  *Name, *Value;
char *NodeData = NULL;
int  i, attr_index = 0;
char  *cp;

NodeData = strdup( data );
for( i = 0 ; i < XdmfXML_MAX_ATTRIBUTES ; i++ ){
  Attributes[i] = NULL;
  }
Attributes[attr_index++] = "Target";
Attributes[attr_index++] = target;
cp = NodeData;
while( *cp > '\0' ){
  /* Swallow White Space */
  while( ( *cp > '\0' ) && ( *cp <= ' ' )) {
    cp++;
    }
  if( *cp <= '\0' ) break;
  /* Name */
  Name = cp;
  cp++;
  /* Look for = */
  while( ( *cp > '\0' ) && ( *cp != '=' )) {
    cp++;
    }
  if( *cp <= '\0' ) break;
  *cp = '\0';
  cp++;
  /* Look for first " */
  while( ( *cp > '\0' ) && ( *cp != '"' )) {
    cp++;
    }
  if( *cp <= '\0' ) break;
  cp++;
  /* Value */
  Value = cp;
  /* Look for Second " */
  while( ( *cp > '\0' ) && ( *cp != '"' )) {
    cp++;
    }
  if( *cp <= '\0' ) break;
  *cp = '\0';
  Attributes[attr_index++] = Name;
  Attributes[attr_index++] = Value;
  cp++;
  }

StartElement( userData, "ProcessingInstruction", Attributes );
if( NodeData ) GetData(userData, NodeData, strlen(NodeData) );
EndElement( userData, "ProcessingInstruction" );
if( NodeData ) free( NodeData );
}

void
StartElement(void *userData, const char *type, const char **atts) {

        XMLUserData *data;
        void *node;
        int i = 0;
  char depth[10];
        node = C__NewXdmfXNode();
        C__XdmfXNodeSet(node, "NodeType", type);
        while((atts[i] != NULL)&&(atts[i+1] != NULL)){
                C__XdmfXNodeSet(node, atts[i], atts[i+1]);
                i += 2;
        }
        C__XdmfXNodeSet(node, "CData", "\0");

        data = (XMLUserData *)userData;
  sprintf(depth, "%d", data->Depth);
        C__XdmfXNodeSet(node, "NodeDepth", depth);

        data->Node = XdmfTree_add(data->Parent, node );

        data->Parent = data->Node;
        data->Depth += 1;
        data->NumElements += 1;
        ValidElement = 1;
}

void 
EndElement(void *userData, const char *type) {

        XMLUserData *data;
        void *node;

        (void)type;

        data = (XMLUserData *)userData;
        data->Depth -= 1;
        data->Parent = XDMF_TREE_PARENT(data->Parent);

        node = XDMF_TREE_CLIENT_DATA(data->Node);
/*        tmp1 = C__XdmfXNodeGet(node, "CData");*/
        ValidElement = 0;
}

void
GetDefaultData(void *userData, const XML_Char *NodeData, int newlen){
  (void)userData;
  (void)NodeData;
  (void)newlen;
/*
char data[256], *cpt;
cpt = ( char *)NodeData;
strncpy( data, cpt, newlen );
data[newlen] = '\0';
printf("default ... %s\n", data );
*/
}

void
GetData(void *userData, const XML_Char *NodeData, int newlen){
        XMLUserData *data;
        void *node;
        char *s;
        char *tmp1;
        int   oldlen;

        if (!ValidElement) return;

        fflush(stdout);
  data = (XMLUserData *)userData;
        node = XDMF_TREE_CLIENT_DATA(data->Node);
        tmp1 = C__XdmfXNodeGet(node, "CData");
        oldlen = strlen(tmp1);
  if (newlen > 1) {
          s = (char *)calloc((oldlen + newlen + 1), sizeof(char));
    strcpy(s, tmp1);
          strncat(s, NodeData, newlen);
          C__XdmfXNodeSet(node, "CData", s);
    free( s );
  }
}


/* Xdmf Tree Walk Routines */

int
PrintTree(XDMF_TREE_NODE *node, void *clientdata){

void *data;

(void)clientdata;
printf("Printit : Node has %d children\n", XDMF_TREE_NCHILD(node));
data = XDMF_TREE_CLIENT_DATA(node);
C__XdmfXNodePrint(data);

if(XDMF_TREE_PARENT(node) != NULL){
        data = XDMF_TREE_CLIENT_DATA(XDMF_TREE_PARENT(node));
  puts("Parent:");
  C__XdmfXNodePrint(data);
}


return(XDMF_TRUE);

}

int
C_FindXMLNode(XDMF_TREE_NODE *node, void *clientdata){

void *data;
FindNodeData *fndata;
char *type;

data = XDMF_TREE_CLIENT_DATA(node);
type = C__XdmfXNodeGet(data, "NodeType");
fndata = (FindNodeData*)clientdata;
fndata->node = NULL;
if (fndata->occurance <= 0) return(XDMF_FALSE);
if ((type != NULL) && (strcmp(type, fndata->tag) == 0)) {
  fndata->occurance--;
  if (fndata->occurance == 0) {
    fndata->node = node;
    return(XDMF_FALSE);
  } else {
    return(XDMF_TRUE);
  }
} else {
      return(XDMF_TRUE);
}

}

int
C_FindXMLNodeByAttribute(XDMF_TREE_NODE *node, void *clientdata){

void *data;
FindNodeData *fndata;
char *attribute;

data = XDMF_TREE_CLIENT_DATA(node);
fndata = (FindNodeData*)clientdata;
attribute = C__XdmfXNodeGet(data, fndata->attribute);
fndata->node = NULL;
if (fndata->occurance <= 0) return(XDMF_FALSE);
if ((attribute != NULL) && (strcmp(attribute, fndata->value) == 0)) {
  fndata->occurance--;
  if (fndata->occurance == 0) {
    fndata->node = node;
    return(XDMF_FALSE);
  } else {
    return(XDMF_TRUE);
  }
} else {
      return(XDMF_TRUE);
}

}

int
C_FindXMLNumberOfNodes(XDMF_TREE_NODE *node, void *clientdata){

void *data;
FindNodeData *fndata;
char *type;

data = XDMF_TREE_CLIENT_DATA(node);
type = C__XdmfXNodeGet(data, "NodeType");
fndata = (FindNodeData*)clientdata;
fndata->node = NULL;
if ((type != NULL) && (strcmp(type, fndata->tag) == 0))  fndata->occurance++;
return(XDMF_TRUE);


}

int
C_FindXMLNumberOfNodesByAttribute(XDMF_TREE_NODE *node, void *clientdata){

void *data;
FindNodeData *fndata;
char *attribute;

data = XDMF_TREE_CLIENT_DATA(node);
fndata = (FindNodeData*)clientdata;
attribute = C__XdmfXNodeGet(data, fndata->attribute );
fndata->node = NULL;
if ((attribute != NULL) && (strcmp(attribute, fndata->value ) == 0))  fndata->occurance++;
return(XDMF_TRUE);


}

int
C_SetDepth(XDMF_TREE_NODE *node, void *clientdata){

char *nodedepth = NULL;
int depth;
char tmp[10];
XDMF_TREE_NODE *parent;
void *data, *pdata;

(void)clientdata;
data = XDMF_TREE_CLIENT_DATA(node);
parent = XDMF_TREE_PARENT(node);
pdata = XDMF_TREE_CLIENT_DATA(parent);
if( pdata ) nodedepth = C__XdmfXNodeGet(pdata, "NodeDepth");
if( nodedepth ) {
  depth = atoi(nodedepth);
} else {
  depth = 0;
}
depth++;
sprintf(tmp, "%d", depth);
C__XdmfXNodeSet(data, "NodeDepth", tmp);

return(XDMF_FALSE);


}

