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
#include <stdlib.h>
#include <Xdmf.h>

extern "C" {
#include <netcdf.h>
#include <exodusII.h>
}


#define True 1
#define False 0
#define MAXVAR 200
#define MAXBLK          400
#define MAX_LINE_LENGTH 80L

typedef struct {
  char  title[256];
  char  h5file[256];
  char  exofile[256];
  int  id;
  int  blockid[MAXBLK];
  int  num_nodes;
  int  num_elements;
  int  num_dims;
  int  num_elemets_blocks;
  int  num_node_sets;
  int  num_side_sets;
  int  num_node_vars;
  int  num_cell_vars;
  int  num_times;
  int  datum_to_size;
  int  dataum_from_size;
  float  version;
  } DiceExodus;

void
FixName( char *Name ){
int i;
int length = MIN( MAX_STR_LENGTH, strlen( Name ) );

Name[ length ] = '\0';
for( i = 0 ; i < length ; i++){
  if( Name[i] == ' ' ){
    Name[i] = '\0';
    }
  }
}

ostrstream *
FixXML( ostrstream *XML ){

XdmfDOM    Current, Desc;
XdmfInt32  i, j, k, NGrid, NVect, NComp, ComponentExists;
XdmfXNode  *Element, *Component, *CurrentComponent;
XdmfXNode  *GridElement;
XdmfXNode  *ComponentNodes[12];
ostrstream  *Output;
char    *Attribute;
char    XMLPath[ 256 ];

Attribute = getenv("ICE_RELEASE_ROOT");
if( Attribute ) {
  strcpy( XMLPath, Attribute );
} else {
  cerr << "ICE_RELEASE_ROOT needs to be set" << endl;
  exit(1);
}
strcat( XMLPath, "/lib/FromExoII.xml" );
Desc.SetInputFileName(XMLPath);
Desc.Parse();
Current.Parse( XML->str() );
NGrid = Current.FindNumberOfElements("Grid");
// Find Vectors
NVect = Desc.FindNumberOfElements("Vector");
for( i = 0 ; i < NVect ; i++ ){
  cout << "Checking Vector #" << i + 1 << endl;
  Element = Desc.FindElement( "Vector", i, NULL);
  NComp = Desc.FindNumberOfElements("Component", Element );
  for( k = 0 ; k < NGrid ; k++){
    ComponentExists = 1;
    GridElement = Current.FindElement("Grid", k, NULL);
    for( j = 0 ; j < NComp ; j++ ){
      Component =  Desc.FindElement("Component", j, Element );
      Attribute =  Desc.Get( Component, "Name" );
      // cout << "Component = " << Attribute << endl;
      CurrentComponent = Current.FindElementByAttribute( "Name", Attribute, 0, GridElement );
      if( CurrentComponent != NULL ){
        // cout << "Component Exists in Grid" << endl;
        ComponentNodes[j] = CurrentComponent;
      } else {
        cout << "Component " << Attribute << " Does Not Exists in Grid" << endl;
        ComponentExists = 0;
        break;
        }
      }
      if( ComponentExists ){
        XdmfXNode *Vector = new XdmfXNode();
        XdmfXNode *Transform = new XdmfXNode();
        XdmfXNode *Parent;
        ostrstream Dimensions;
    
        Parent = Current.FindElement("Parameter", 0, GridElement );
        if( Parent == NULL ) {
          Parent = GridElement;
          }
        Current.Insert( Parent, Vector );
        Current.Insert( Vector, Transform );
        Current.Set( Vector, "NodeType", "Attribute");
        Current.Set( Vector, "Name", Desc.Get( Element, "Name" ) );
        Current.Set( Vector, "Type", "Vector" );
        Current.Set( Vector, "Center", Current.Get(ComponentNodes[0], "Center"));
        Current.Set( Transform, "NodeType", "DataTransform" );
        Current.Set( Transform, "Type", "Function" );
        Current.Set( Transform, "Format", "XML" );
        Current.Set( Transform, "Function", "join($0 , $1 ,  $2)" );
        for( j = 0 ; j < NComp ; j++ ){
          XdmfXNode *DataStructure = Current.GetChild( 0, ComponentNodes[j]);
          if( j == 0 ){
            Dimensions << Current.Get( DataStructure, "Dimensions" ) <<
              " 3";
            Current.Set( Transform, "Dimensions", Dimensions.str() );
            }
          Current.InsertFromString(Transform,
            Current.Serialize( DataStructure ));
          }
      }
    }
  }

// cout << "XML = " << Current.Serialize() << endl;
Output = new ostrstream();
*Output << Current.Serialize();
return( Output );
}

ostrstream  *
InitXML( ostrstream *XML, DiceExodus *info ){
if( XML == NULL ){
  XML = new ostrstream();
  *XML << "<?xml version=\"1.0\" ?>" << endl;
  *XML << "<!DOCTYPE Xdmf SYSTEM \"xdmf.dtd\" [" << endl;
  *XML << "<!ENTITY Iteration \"Iteration/\" >" << endl;
  *XML << "<!ENTITY HeavyData \"" << info->h5file << "\" >" << endl;
  *XML << "]>" << endl;
  *XML << "<Xdmf>" << endl;
  *XML << " <Domain>" << endl << endl;
  *XML << "<!-- Raw Information from ExodusII -->" << endl;
  *XML << "<?ExodusII Title=\"" << info->title << "\"" << endl;
  *XML << "\tExodus File=\"" << info->exofile << "\"" << endl;
  *XML << "\tNodes=\"" << info->num_nodes << "\"" << endl;
  *XML << "\tIterations=\"" << info->num_times << "\"" << endl;
  *XML << "\tNodeSets=\"" << info->num_node_sets << "\"" << endl;
  *XML << "\tSideSets=\"" << info->num_side_sets << "\"" << endl;
  *XML << "\t?>" << endl << endl;
  }
return( XML );
}

ostrstream  *
WriteGridXML( ostrstream *XML, DiceExodus *info,
  int BlockIndex,
  char *ElementType,
  int NumberOfElements,
  int NodesPerElement,
  int IterStart, int IterCount ){

if( XML == NULL ){
  XML = InitXML( NULL, info );
  }
*XML << "\t<Grid Name=\"Block " << BlockIndex << "\">" << endl;
if( info->num_times > 0 ){
  *XML << "\t<Parameter Name=\"Iteration\" Type=\"Range\"" << endl;
  *XML << "\t\tFormat=\"Iteration %d\" Values=\"" << IterStart <<
    " 1 " << IterCount << "\"" << endl;
  *XML << "\t\tCurrentIndex=\"0\">" << endl;
}
*XML << "\t<Topology" << endl;
if( strncasecmp( ElementType, "SHE", 3 ) == 0 ) {
  *XML << "\t\tType=\"Quadrilateral\"" << endl;
}
if( strncasecmp( ElementType, "HEX", 3 ) == 0 ) {
  *XML << "\t\tType=\"Hexahedron\"" << endl;
}
if( strncasecmp( ElementType, "TET", 3 ) == 0 ) {
  *XML << "\t\tType=\"Tetrahedron\"" << endl;
}
*XML << "\t\tNumberOfElements=\"" << NumberOfElements << "\"" << endl;
*XML << "\t\tBaseOffset=\"1\"" << endl;
*XML << "\t\t>" << endl;
*XML << "\t\t<DataStructure" << endl;
*XML << "\t\t\tFormat=\"HDF\"" << endl;
*XML << "\t\t\tDimensions=\"" << NumberOfElements << " " << NodesPerElement << "\""<< endl;
*XML << "\t\t\tDataType=\"Int\">" << endl;
*XML << "\t\t\t&HeavyData;:/Block " << BlockIndex << "/Connections" << endl;
*XML << "\t\t</DataStructure>" << endl;
*XML << "\t</Topology>" << endl;
*XML << "\t<Geometry Type=\"XYZ\">" << endl;
*XML << "\t\t<DataStructure" << endl;
*XML << "\t\t\tFormat=\"HDF\"" << endl;
*XML << "\t\t\tDimensions=\"" << info->num_nodes << " 3\"" << endl;
*XML << "\t\t\tDataType=\"Float\">" << endl;
*XML << "\t\t&HeavyData;:/Geometry" << endl;
*XML << "\t\t</DataStructure>" << endl;
*XML << "\t</Geometry>" << endl;
return( XML );
}

ostrstream  *
FinishGridXML( ostrstream *XML, DiceExodus *info){
if( info->num_times > 0 ){
  *XML << "\t</Parameter>" << endl;
  }
*XML << "</Grid>" << endl;
*XML << "" << endl;
return( XML );
}

ostrstream  *
WriteAttribute( ostrstream *XML, DiceExodus *info, int BlockIndex,
    char *Name, char *Center, char *Type, XdmfArray *Data ) {

*XML << "\t<Attribute Name=\"" << Name << "\"" << endl;
*XML << "\t\tCenter=\"" << Center << "\"" << endl;
*XML << "\t\tType=\"" << Type << "\">" << endl;
*XML << "\t\t<DataStructure" << endl;
*XML << "\t\t\tFormat=\"HDF\"" << endl;
*XML << "\t\t\tDimensions=\"" << Data->GetNumberOfElements() << "\"" << endl;
*XML << "\t\t\tDataType=\""<< XdmfTypeToClassString( Data->GetNumberType() ) << "\" >" << endl;
*XML << "\t\t&HeavyData;:/Block " << BlockIndex << "/&Iteration;" << Name << endl;
*XML << "\t\t</DataStructure>" << endl;
*XML << "\t</Attribute>" << endl;


return( XML );
}

void
GenerateFileNames( char *Original, char **H5File, char **XMLFile ) {
char *dot;
*H5File = ( char *)calloc( strlen( Original ) + 5, sizeof( char ) );
*XMLFile = ( char *)calloc( strlen( Original ) + 5, sizeof( char ) );
strcpy( *H5File, Original );
strcpy( *XMLFile, Original );
dot = strrchr( *H5File, '.' );
if( dot ) {
  *dot = '\0';
  }
strcat( *H5File, ".h5" );
dot = strrchr( *XMLFile, '.' );
if( dot ) {
  *dot = '\0';
  }
strcat( *XMLFile, ".xml" );
}

void
main( int argc, char **argv)
{
char    *filename;
char    *hdfname, *xmlname;
char    name[MAX_STR_LENGTH + 1];
char    cdummy;
int    IterStart = 1, IterEnd = -1, IterCount;
int    i, j, status;
int    *ElementTable;
float    fdummy;
DiceExodus  info_struct,  *info = &info_struct;

XdmfArray  *Geometry = new XdmfArray();
XdmfArray  *FinalGeometry;
XdmfArray  *Times = NULL;
XdmfHDF    *Hdf = new XdmfHDF();
ofstream    *XMLFile;
ostrstream   *XML = NULL;

filename = argv[1];
if( argc > 2 ){
  IterStart = atoi( argv[2] );  
  if( IterStart <= 0 ) IterStart = 1;
  }
if( argc > 3 ){
  IterEnd = atoi( argv[3] );  
  if( IterEnd < IterStart ) IterEnd = -1;
  }
GenerateFileNames( filename, &hdfname, &xmlname );
cout << "Input:" << filename <<
  ", Hdf5:" << hdfname <<
  ", XML:" << xmlname << 
  ", Iterations:" << IterStart << " to " << IterEnd <<
  endl;
memset( info, 0, sizeof( DiceExodus ));
strcpy( info->h5file, hdfname );
strcpy( info->exofile, filename );
info->dataum_from_size = 0;
// info->datum_to_size = sizeof(DICE_FLOAT);
info->datum_to_size = 0;
info->id = ex_open( filename, EX_READ,
    &info->datum_to_size,
    &info->dataum_from_size,
    &info->version);

if ( info->id < 0 ){
  XdmfErrorMessage("Can't Open " << filename );
  exit(1);  
  }

printf("%s Version %f\n", filename, info->version);
cout << filename << endl <<
  " Exo Version " << info->version << endl <<
  " From Size " << info->dataum_from_size << endl <<
  " to Size " << info->datum_to_size << endl;
status = ex_get_init( info->id,
      info->title,
      &info->num_dims,
      &info->num_nodes,
      &info->num_elements,
      &info->num_elemets_blocks,
      &info->num_node_sets,
      &info->num_side_sets);
status = ex_get_var_param( info->id,
    "e",
    &info->num_cell_vars);
status = ex_get_var_param( info->id,
    "n",
    &info->num_node_vars);
cout << info->num_nodes << " Nodes " << endl <<
  info->num_elements << " Elements " << endl <<
  info->num_cell_vars << " Cell Vars " << endl <<
  info->num_node_vars << " Node Vars" << endl <<
  info->num_node_sets << " Node Sets" << endl <<
  info->num_side_sets << " Side Sets" << endl <<
  " Title = " << info->title << endl;

if( info->datum_to_size == 8 ){
  Geometry->SetNumberType( XDMF_FLOAT64_TYPE );
} else {
  Geometry->SetNumberType( XDMF_FLOAT32_TYPE );
}
Geometry->SetNumberOfElements( info->num_nodes * 3 );
status = ex_get_coord( info->id,
    Geometry->GetDataPointer(0),
    Geometry->GetDataPointer(info->num_nodes),
    Geometry->GetDataPointer(info->num_nodes * 2));

// Transpose into X,Y,Z 
FinalGeometry = Geometry->Clone();
// Copy X
{
  XdmfInt64  Start, Stride, Count;
  Start = 0;
  Stride = 1;
  Count = info->num_nodes;
  Geometry->SelectHyperSlab( &Start, &Stride, &Count );
  Start = 0;
  Stride = 3;
  Count = info->num_nodes;
  FinalGeometry->SelectHyperSlab( &Start, &Stride, &Count );
  CopyArray( Geometry, FinalGeometry );
}
// Copy Y
{
  XdmfInt64  Start, Stride, Count;
  Start = info->num_nodes;
  Stride = 1;
  Count = info->num_nodes;
  Geometry->SelectHyperSlab( &Start, &Stride, &Count );
  Start = 1;
  Stride = 3;
  Count = info->num_nodes;
  FinalGeometry->SelectHyperSlab( &Start, &Stride, &Count );
  CopyArray( Geometry, FinalGeometry );
}
// Copy Z
{
  XdmfInt64  Start, Stride, Count;
  Start = info->num_nodes * 2;
  Stride = 1;
  Count = info->num_nodes;
  Geometry->SelectHyperSlab( &Start, &Stride, &Count );
  Start = 2;
  Stride = 3;
  Count = info->num_nodes;
  FinalGeometry->SelectHyperSlab( &Start, &Stride, &Count );
  CopyArray( Geometry, FinalGeometry );
}

{
XdmfInt64 Dimensions[2];
char    FinalName[256];

strcpy( FinalName, hdfname );
strcat( FinalName, ":/Geometry");
Dimensions[0] = info->num_nodes;
Dimensions[1] = 3;
Hdf->CopyType( FinalGeometry );
Hdf->SetShape( 2, Dimensions );
Hdf->Open( FinalName, "rw" );
Hdf->CreateDataset( FinalName );
FinalGeometry->SelectAll();
Hdf->Write( FinalGeometry );
Hdf->Close();
}

ex_get_elem_blk_ids( info->id, info->blockid );
status = ex_inquire( info->id,
      EX_INQ_TIME,
      &info->num_times,
      &fdummy, &cdummy);
cout << "Found " << info->num_times << " Iterations" << endl;
if( info->num_times > 0 ){
  Times = new XdmfArray();
  Times->SetNumberType( XDMF_FLOAT32_TYPE );
  Times->SetNumberOfElements( info->num_times );  
  status = ex_get_all_times( info->id, Times->GetDataPointer() );
}
if( info->num_cell_vars ){
  ElementTable = (int *) calloc(( info->num_elemets_blocks * info->num_cell_vars ), sizeof(int));
  ex_get_elem_var_tab( info->id, info->num_elemets_blocks, info->num_cell_vars, ElementTable );
}

XML = InitXML( XML, info );
*XML << "<ExodusII>" << endl;
/* Read Node Sets */
if (info->num_node_sets){
  int node_set;
  int node_set_id;
  int num_nodes_in_set, num_df_in_set;
  char    FinalName[256];

  XdmfArray *NodeSetIds = new XdmfArray();
  XdmfArray *NodeSetNodes = new XdmfArray();
  XdmfArray *NodeSetDFs = new XdmfArray();

  NodeSetIds->SetNumberType( XDMF_INT32_TYPE );
  NodeSetNodes->SetNumberType( XDMF_INT32_TYPE );
  NodeSetDFs->SetNumberType( XDMF_FLOAT32_TYPE );

  
  NodeSetIds->SetNumberOfElements( info->num_node_sets );
  ex_get_node_set_ids (info->id, (int *)NodeSetIds->GetDataPointer() );
  for( node_set = 0 ; node_set < info->num_node_sets ; node_set++ ){
    node_set_id = NodeSetIds->GetValueAsInt32( node_set );
    cout << "Reading Node Set #" << node_set << " Id = " << node_set_id << endl;
    ex_get_node_set_param(info->id, node_set_id,
          &num_nodes_in_set, &num_df_in_set);
    cout << "Node Set " << node_set << " has " << num_nodes_in_set << " nodes" << endl;
    cout << "Node Set " << node_set << " has " << num_df_in_set << " distribution factors" << endl;
    NodeSetNodes->SetNumberOfElements( num_nodes_in_set );
    ex_get_node_set (info->id, node_set_id, (int *)NodeSetNodes->GetDataPointer());

    *XML << "\t<NodeSet Name=\"" << node_set_id << "\" >" << endl;
    *XML << "\t<Nodes>" << endl;
    *XML << "\t\t<DataStructure" << endl;
    *XML << "\t\t\tFormat=\"HDF\"" << endl;
    *XML << "\t\t\tDimensions=\"" << num_nodes_in_set << "\""<< endl;
    *XML << "\t\t\tDataType=\"Int\">" << endl;
    *XML << "\t\t\t&HeavyData;:/NodeSets/" << node_set_id << "/Node" << endl;
    *XML << "\t\t</DataStructure>" << endl;
    *XML << "\t</Nodes>" << endl;
    sprintf( FinalName, "%s:/NodeSets/%d/Node", hdfname, node_set_id );
    Hdf->CopyType( NodeSetNodes );
    Hdf->CopyShape( NodeSetNodes );
    Hdf->Open( FinalName, "rw" );
    Hdf->CreateDataset( FinalName );
    NodeSetNodes->SelectAll();
    Hdf->Write( NodeSetNodes);
    Hdf->Close();
    if( num_df_in_set ){
      NodeSetDFs->SetNumberOfElements( num_df_in_set );
      ex_get_node_set_dist_fact(info->id, node_set_id, (float *)NodeSetDFs->GetDataPointer());
      *XML << "\t<DistributionFactor>" << endl;
      *XML << "\t\t<DataStructure" << endl;
      *XML << "\t\t\tFormat=\"HDF\"" << endl;
      *XML << "\t\t\tDimensions=\"" << num_df_in_set << "\""<< endl;
      *XML << "\t\t\tDataType=\"Float\">" << endl;
      *XML << "\t\t\t&HeavyData;:/NodeSets/" << node_set_id << "/DistributionFactor" << endl;
      *XML << "\t\t</DataStructure>" << endl;
      *XML << "\t</DistributionFactor>" << endl;
      sprintf( FinalName, "%s:/NodeSets/%d/DistributionFactor", hdfname, node_set_id );
      Hdf->CopyType( NodeSetDFs );
      Hdf->CopyShape( NodeSetDFs );
      Hdf->Open( FinalName, "rw" );
      Hdf->CreateDataset( FinalName );
      NodeSetDFs->SelectAll();
      Hdf->Write( NodeSetDFs );
      Hdf->Close();
    }
    *XML << "\t</NodeSet>" << endl;
  }
}

/* Read Side Sets */
if(info->num_side_sets){
  int side_set;
  int side_set_id;
  int num_sides_in_set, num_df_in_set;
  char    FinalName[256];

  XdmfArray *SideSetIds = new XdmfArray();
  XdmfArray *SideSetNodes = new XdmfArray();
  XdmfArray *SideSetNodeCounts = new XdmfArray();
  XdmfArray *SideSetSides = new XdmfArray();
  XdmfArray *SideSetElements = new XdmfArray();

  XdmfArray *SideSetDFs = new XdmfArray();

  SideSetIds->SetNumberType( XDMF_INT32_TYPE );
  SideSetNodes->SetNumberType( XDMF_INT32_TYPE );
  SideSetNodeCounts->SetNumberType( XDMF_INT32_TYPE );
  SideSetSides->SetNumberType( XDMF_INT32_TYPE );
  SideSetElements->SetNumberType( XDMF_INT32_TYPE );

  SideSetDFs->SetNumberType( XDMF_FLOAT32_TYPE );

  
  SideSetIds->SetNumberOfElements( info->num_side_sets );
  ex_get_side_set_ids (info->id, (int *)SideSetIds->GetDataPointer() );
  for( side_set = 0 ; side_set < info->num_side_sets ; side_set++ ){
    side_set_id = SideSetIds->GetValueAsInt32( side_set );
    cout << "Reading Side Set #" << side_set << " Id = " << side_set_id << endl;
    ex_get_side_set_param(info->id, side_set_id,
          &num_sides_in_set, &num_df_in_set);
    cout << "Side Set " << side_set << " has " << num_sides_in_set << " sides " << endl;
    cout << "Side Set " << side_set << " has " << num_df_in_set << " distribution factors" << endl;
    // Note: The # of elements is same as # of sides!
    // Why ? I have no idea !!
    SideSetNodeCounts->SetNumberOfElements( num_sides_in_set );
    SideSetSides->SetNumberOfElements( num_sides_in_set );
    SideSetElements->SetNumberOfElements( num_sides_in_set );
    // Why * 21 ? I have no idea !!
    SideSetNodes->SetNumberOfElements( num_sides_in_set * 21 );
    ex_get_side_set (info->id, side_set_id,
      (int *)SideSetElements->GetDataPointer(),
      (int *)SideSetSides->GetDataPointer());
    ex_get_side_set_node_list(info->id, side_set_id,
      (int *)SideSetNodeCounts->GetDataPointer(),
      (int *)SideSetNodes->GetDataPointer());

    *XML << "\t<SideSet Name=\"" << side_set_id << "\" >" << endl;
    *XML << "\t<Node>" << endl;
    *XML << "\t\t<DataStructure" << endl;
    *XML << "\t\t\tFormat=\"HDF\"" << endl;
    *XML << "\t\t\tDimensions=\"" << SideSetNodes->GetNumberOfElements() << "\""<< endl;
    *XML << "\t\t\tDataType=\"Int\">" << endl;
    *XML << "\t\t\t&HeavyData;:/SideSets/" << side_set_id << "/Node" << endl;
    *XML << "\t\t</DataStructure>" << endl;
    *XML << "\t</Node>" << endl;
    sprintf( FinalName, "%s:/SideSets/%d/Node", hdfname, side_set_id );
    Hdf->CopyType( SideSetNodes );
    Hdf->CopyShape( SideSetNodes );
    Hdf->Open( FinalName, "rw" );
    Hdf->CreateDataset( FinalName );
    SideSetNodes->SelectAll();
    Hdf->Write( SideSetNodes);
    Hdf->Close();
    *XML << "\t<Side>" << endl;
    *XML << "\t\t<DataStructure" << endl;
    *XML << "\t\t\tFormat=\"HDF\"" << endl;
    *XML << "\t\t\tDimensions=\"" << SideSetSides->GetNumberOfElements() << "\""<< endl;
    *XML << "\t\t\tDataType=\"Int\">" << endl;
    *XML << "\t\t\t&HeavyData;:/SideSets/" << side_set_id << "/Side" << endl;
    *XML << "\t\t</DataStructure>" << endl;
    *XML << "\t</Side>" << endl;
    sprintf( FinalName, "%s:/SideSets/%d/Side", hdfname, side_set_id );
    Hdf->CopyType( SideSetSides );
    Hdf->CopyShape( SideSetSides );
    Hdf->Open( FinalName, "rw" );
    Hdf->CreateDataset( FinalName );
    SideSetSides->SelectAll();
    Hdf->Write( SideSetSides);
    Hdf->Close();
    *XML << "\t<Element>" << endl;
    *XML << "\t\t<DataStructure" << endl;
    *XML << "\t\t\tFormat=\"HDF\"" << endl;
    *XML << "\t\t\tDimensions=\"" << SideSetElements->GetNumberOfElements() << "\""<< endl;
    *XML << "\t\t\tDataType=\"Int\">" << endl;
    *XML << "\t\t\t&HeavyData;:/SideSets/" << side_set_id << "/Element" << endl;
    *XML << "\t\t</DataStructure>" << endl;
    *XML << "\t</Element>" << endl;
    sprintf( FinalName, "%s:/SideSets/%d/Element", hdfname, side_set_id );
    Hdf->CopyType( SideSetElements );
    Hdf->CopyShape( SideSetElements );
    Hdf->Open( FinalName, "rw" );
    Hdf->CreateDataset( FinalName );
    SideSetElements->SelectAll();
    Hdf->Write( SideSetElements);
    Hdf->Close();
    *XML << "\t<NodeCount>" << endl;
    *XML << "\t\t<DataStructure" << endl;
    *XML << "\t\t\tFormat=\"HDF\"" << endl;
    *XML << "\t\t\tDimensions=\"" << SideSetNodeCounts->GetNumberOfElements() << "\""<< endl;
    *XML << "\t\t\tDataType=\"Int\">" << endl;
    *XML << "\t\t\t&HeavyData;:/SideSets/" << side_set_id << "/NodeCount" << endl;
    *XML << "\t\t</DataStructure>" << endl;
    *XML << "\t</NodeCount>" << endl;
    sprintf( FinalName, "%s:/SideSets/%d/NodeCount", hdfname, side_set_id );
    Hdf->CopyType( SideSetNodeCounts );
    Hdf->CopyShape( SideSetNodeCounts );
    Hdf->Open( FinalName, "rw" );
    Hdf->CreateDataset( FinalName );
    SideSetNodeCounts->SelectAll();
    Hdf->Write( SideSetNodeCounts);
    Hdf->Close();
    if( num_df_in_set ){
      SideSetDFs->SetNumberOfElements( num_df_in_set );
      ex_get_side_set_dist_fact(info->id, side_set_id, (float *)SideSetDFs->GetDataPointer());
      *XML << "\t<DistributionFactor>" << endl;
      *XML << "\t\t<DataStructure" << endl;
      *XML << "\t\t\tFormat=\"HDF\"" << endl;
      *XML << "\t\t\tDimensions=\"" << SideSetDFs->GetNumberOfElements() << "\""<< endl;
      *XML << "\t\t\tDataType=\"Float\">" << endl;
      *XML << "\t\t\t&HeavyData;:/SideSets/" << side_set_id << "/DistributionFactor" << endl;
      *XML << "\t\t</DataStructure>" << endl;
      *XML << "\t</DistributionFactor>" << endl;
      sprintf( FinalName, "%s:/SideSets/%d/DistributionFactor", hdfname, side_set_id );
      Hdf->CopyType( SideSetDFs );
      Hdf->CopyShape( SideSetDFs );
      Hdf->Open( FinalName, "rw" );
      Hdf->CreateDataset( FinalName );
      SideSetDFs->SelectAll();
      Hdf->Write( SideSetDFs );
      Hdf->Close();
    }
    *XML << "\t</SideSet>" << endl;
  }
}

*XML << "</ExodusII>" << endl;


for ( i = 0 ; i < info->num_elemets_blocks ; i++ ){
  char  ElementType[256];
  int  Iteration;
  int  ElementsInBlock;
  int  NodesPerElement;
  int  AttributesPerElement;
  int  ElementBlockIndex;
  XdmfArray *Connections = new XdmfArray();
  XdmfInt64 Dimensions[2];
  char    FinalName[256];


  ElementBlockIndex = i + 1;
  printf("Reading Connectivity\n");
  status = ex_get_elem_block( info->id,
      info->blockid[i],
      ElementType,
      &ElementsInBlock,
      &NodesPerElement,
      &AttributesPerElement );

  cout << "Block " << i + 1 <<
    " ElementType <" << ElementType << ">" << 
    " ElementsInBlock " << ElementsInBlock <<
    " NodesPerElement " << NodesPerElement <<
    " AttributesPerElement " << AttributesPerElement <<
    endl;

  if( IterEnd < IterStart ){
    IterEnd = info->num_times - IterStart;
    }
  IterCount  = IterEnd - IterStart + 1;
  XML = WriteGridXML( XML, info, ElementBlockIndex,
    ElementType, ElementsInBlock, NodesPerElement,
    IterStart, IterCount );
  sprintf( FinalName, "%s:/Block %d/Connections", hdfname, ElementBlockIndex);
  Dimensions[0] = ElementsInBlock;
  Dimensions[1] = NodesPerElement;
  Connections->SetNumberType( XDMF_INT32_TYPE );
  Connections->SetShape( 2, Dimensions );
  status = ex_get_elem_conn( info->id, info->blockid[i],( int *)Connections->GetDataPointer() );
  Hdf->Open( FinalName, "rw" );
  Hdf->Write( Connections );
  Hdf->Close();

  Iteration = IterStart;
  while( Iteration <= IterEnd ){
    XdmfArray *ScalarArray = new XdmfArray();

    cout << "Reading Scalars for Iteration #" << Iteration << "( t = " <<
      Times->GetValueAsFloat32( Iteration ) << " ) " << endl;
    cout << "  Element Variables" << endl;
    // Element Variables
    ScalarArray->SetNumberType( XDMF_FLOAT32_TYPE );
    ScalarArray->SetNumberOfElements( ElementsInBlock );
    for( j = 0 ; j < info->num_cell_vars ; j++){
      ex_get_var_name( info->id, "Element", j, name );
      FixName( name );
      cout << "\tProcessing Cell Variable " << j + 1 << " = " << name << endl;
      if( ElementTable[ ( i * info->num_cell_vars ) + j ] > 0 ){
        status = ex_get_elem_var( info->id,
          Iteration,
          j + 1,
          info->blockid[i],
          ElementsInBlock,
          ScalarArray->GetDataPointer() );
        sprintf( FinalName, "%s:/Block %d/Iteration %d/%s",
            hdfname, ElementBlockIndex, Iteration, name);
        if( Iteration == IterStart ){
          WriteAttribute( XML, info, ElementBlockIndex, name, "Cell", "Scalar", ScalarArray );
        }
        Hdf->Open( FinalName, "rw" );
        Hdf->Write( ScalarArray );
        Hdf->Close();
      } else {
        cout << "\t ... Variable " << name << " not in ExodusII Database" << endl;
        }
      }
    // Nodal Variables
    cout << "  Node Variables" << endl;
    ScalarArray->SetNumberOfElements( info->num_nodes );
    for( j = 0 ; j < info->num_node_vars ; j++){
      ex_get_var_name( info->id, "Node", j, name );
      FixName( name );  
      cout << "\tProcessing Node Variable " << j + 1 << " = " << name << endl;
      status = ex_get_nodal_var( info->id,
          Iteration,
          j + 1,
          info->num_nodes,
          ScalarArray->GetDataPointer());
      sprintf( FinalName, "%s:/Block %d/Iteration %d/%s",
        hdfname, i, Iteration, name);
      if( Iteration == IterStart ){
        WriteAttribute( XML, info, i, name, "Node", "Scalar", ScalarArray );
      }
      Hdf->Open( FinalName, "rw" );
      Hdf->Write( ScalarArray );
      Hdf->Close();
      }
    Iteration++;
    }
  XML = FinishGridXML( XML, info );
  }
ex_close( info->id );
*XML << " </Domain>" << endl<< "</Xdmf>" << endl;
XML = FixXML( XML );
XMLFile = new ofstream( xmlname );
*XMLFile << "<?xml version=\"1.0\" ?>" << endl;
*XMLFile << "<!DOCTYPE Xdmf SYSTEM \"xdmf.dtd\" [" << endl;
*XMLFile << "<!ENTITY Iteration \"Iteration/\" >" << endl;
*XMLFile << "<!ENTITY HeavyData \"" << info->h5file << "\" >" << endl;
*XMLFile << "]>" << endl;
*XMLFile << XML->str() << endl;
XMLFile->close();
cout.flush();
exit(0);
}

