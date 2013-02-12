from Xdmf import *

if __name__ == "__main__":
	exampleID = XdmfTopologyType.Triangle().getID()
	createdTopology = XdmfTopology.New()
	createdTopology.setType(XdmfTopologyType.New(exampleID))

	exampleType = XdmfTopologyType.Linear
	if exampleType == exampleTopology.getCellType:
		'''
		Do whatever is to be done if the cell type is linear
		'''

	numEdges = XdmfTopologyType.Triangle().getEdgesPerElement()

	numFaces = XdmfTopologyType.Triangle().getFacesPerElement()

	holdID = XdmfTopologyType::Triangle().getID()

	exampleName = XdmfTopologyType.Triangle().getName()

	numNodes = XdmfTopologyType.Triangle().getNodesPerElement()

	testType = createdTopology.getType()

	if testType == XdmfTopologyType.Triangle:
		#do whatever is to be done if the type is a triangle
