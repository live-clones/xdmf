from Xdmf import *
import string
import sys

Dom = XdmfDOM()
Dom.SetInputFileName(sys.argv[1])
Dom.Parse()
# Find the DataStructure that we know is there
AttrNode = Dom.FindElement('Attribute')
DataNode = Dom.FindElement('DataStructure', 0, AttrNode )

PortionReader = XdmfFormatMulti()
PortionReader.SetDOM( Dom )
DataDesc = PortionReader.ElementToDataDesc( DataNode )
ShapeString = DataDesc.GetShapeAsString()
print 'Shape is ' + DataDesc.GetShapeAsString()
Dims = ShapeString.split()
# Only Read one 'K' Plane
SelectionString = '1 ' + string.join(Dims[1:])
print 'Selecting ' + SelectionString
DataDesc.SelectHyperSlabFromString('0 0 0', '1 1 1', SelectionString ) 
Values = PortionReader.ElementToArray( DataNode, DataDesc )
print 'I read %d Elements' % Values.GetNumberOfElements()
del DataDesc
