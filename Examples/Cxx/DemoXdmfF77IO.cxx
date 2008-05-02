#include <Xdmf.h>

using std::setw;
using std::setfill;

#define XdmfWrite   xdmfwrite_

char *
DemoConvertFortranString( char *FtnName ) {

static char Name[80];
char *np;

memcpy(Name, FtnName, 79 );
Name[79] = '\0';
np = &Name[78];
while( ( np > Name ) && ( *np <= ' ') ) {
	np--;
	}
*np = '\0';
// cerr << "Name = " << Name << "\n";
return( Name );
}

extern "C" {

void
XdmfWrite( char *FtnName, int *Iteration,
    int *NumberOfPoints, int *NumberOfHex, XdmfFloat64 *Points,
    XdmfInt32 *Conns, XdmfFloat64 *NodeData,
    XdmfFloat64 *CellData){
char 		*Name;
char		FullName[80];
ostrstream  DataName(FullName, 80);
XdmfDOM         dom;
XdmfRoot        root;
XdmfDomain      domain;
XdmfGrid        grid;
XdmfTime        time;
XdmfTopology    *topology;
XdmfGeometry    *geometry;
XdmfAttribute   nodedata;
XdmfAttribute   celldata;
XdmfArray       *array;

Name = DemoConvertFortranString( FtnName );
// DataName << Name << "_" << setw(5) << setfill('0') << *Iteration << ".h5" << ends;

root.SetDOM(&dom);
root.SetVersion(2.0);
root.Build();

// Domain
root.Insert(&domain);
// Grid
grid.SetName("Demonstration Grid");
domain.Insert(&grid);
time.SetTimeType(XDMF_TIME_SINGLE);
time.SetValue(0.001 * *Iteration);
grid.Insert(&time);
// Topology
topology = grid.GetTopology();
topology->SetTopologyType(XDMF_HEX);
topology->SetNumberOfElements(*NumberOfHex);
// Fortran is 1 based while c++ is 0 based so
// Either subtract 1 from all connections or specify a BaseOffset
topology->SetBaseOffset(1);
array = topology->GetConnectivity();
array->SetNumberOfElements(*NumberOfHex * 8);
array->SetValues(0, Conns, *NumberOfHex * 8);
DataName.seekp(0);
DataName << Name << "_" << setw(5) << setfill('0') << *Iteration << ".h5:/Conns" << ends;
array->SetHeavyDataSetName(FullName);
// Geometry
geometry = grid.GetGeometry();
geometry->SetGeometryType(XDMF_GEOMETRY_XYZ);
geometry->SetNumberOfPoints(*NumberOfPoints);
array = geometry->GetPoints();
array->SetValues(0, Points, *NumberOfPoints * 3);
DataName.seekp(0);
DataName << Name << "_" << setw(5) << setfill('0') << *Iteration << ".h5:/Points" << ends;
array->SetHeavyDataSetName(FullName);
// Node Data
nodedata.SetName("Node Scalar");
nodedata.SetAttributeCenter(XDMF_ATTRIBUTE_CENTER_NODE);
nodedata.SetAttributeType(XDMF_ATTRIBUTE_TYPE_SCALAR);
array = nodedata.GetValues();
array->SetNumberOfElements(*NumberOfPoints);
array->SetValues(0, NodeData, *NumberOfPoints);
DataName.seekp(0);
DataName << Name << "_" << setw(5) << setfill('0') << *Iteration << ".h5:/NodeData" << ends;
array->SetHeavyDataSetName(FullName);
// Cell Data
celldata.SetName("Cell Scalar");
celldata.SetAttributeCenter(XDMF_ATTRIBUTE_CENTER_CELL);
celldata.SetAttributeType(XDMF_ATTRIBUTE_TYPE_SCALAR);
array = celldata.GetValues();
array->SetNumberOfElements(*NumberOfHex);
array->SetValues(0, CellData, *NumberOfHex);
DataName.seekp(0);
DataName << Name << "_" << setw(5) << setfill('0') << *Iteration << ".h5:/CellData" << ends;
array->SetHeavyDataSetName(FullName);
// Attach and Write
grid.Insert(&nodedata);
grid.Insert(&celldata);
root.Build();
DataName.seekp(0);
DataName << Name << "_" << setw(5) << setfill('0') << *Iteration << ".xmf" << ends;
dom.Write(FullName);
}

}
