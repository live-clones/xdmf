from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        exampleID = XdmfTopologyType.Triangle().getID()
        createdTopology = XdmfTopology.New()
        createdTopology.setType(XdmfTopologyType.New(exampleID))

        #//initialization end

        #//getCellType begin

        exampleType = XdmfTopologyType.Linear
        if exampleType == exampleTopology.getCellType:
                '''
                Do whatever is to be done if the cell type is linear
                '''

        #//getCellType end

        #//getEdgesPerElement begin

        numEdges = XdmfTopologyType.Triangle().getEdgesPerElement()

        #//getEdgesPerElement end

        #//getFacesPerElement begin

        numFaces = XdmfTopologyType.Triangle().getFacesPerElement()

        #//getFacesPerElement end

        #//getId begin

        holdID = XdmfTopologyType::Triangle().getID()

        #//getId end

        #//getName begin

        exampleName = XdmfTopologyType.Triangle().getName()

        #//getName end

        #//getNodesPerElement begin

        numNodes = XdmfTopologyType.Triangle().getNodesPerElement()

        #//getNodesPerElement end

        #//getType begin

        testType = createdTopology.getType()

        if testType == XdmfTopologyType.Triangle:
                #do whatever is to be done if the type is a triangle

        #//getType end
