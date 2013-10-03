#include "XdmfTopologyType.hpp"
#include "XdmfTopology.hpp"

int main(int, char **)
{
        //#initialization begin

        unsigned int exampleID = XdmfTopologyType::Triangle()->getID();
        shared_ptr<XdmfTopology> createdTopology = XdmfTopology::New();
        createdTopology->setType(XdmfTopologyType::New(exampleID));

        //#initialization end

        //#getCellType begin

        XdmfTopologyType::CellType exampleType = XdmfTopologyType::Linear;
        //Assuming that exampleTopology is a shared pointer to a filled XdmfTopology object
        if (exampleType == exampleTopology->getCellType())
        {
                //Do whatever is to be done if the cell type is linear
        }

        //#getCellType end

        //#getEdgesPerElement begin

        unsigned int numEdges = XdmfTopologyType::Triangle()->getEdgesPerElement();

        //#getEdgesPerElement end

        //#getFacesPerElement begin

        unsigned int numFaces = XdmfTopologyType::Triangle()->getFacesPerElement();

        //#getFacesPerElement end

        //#getID begin

        unsigned int holdID = XdmfTopologyType::Triangle()->getID();

        //#getID end

        //#getName begin

        std::string exampleName = XdmfTopologyType::Triangle()->getName();

        //#getName end

        //#getFaceType begin

        shared_ptr<const XdmfTopologyType> exampleface = XdmfTopologyType::Tetrahedron()->getFaceType();

        //#getFaceType end

        //#getNodesPerElement begin

        unsigned int numNodes = XdmfTopologyType::Triangle()->getNodesPerElement();

        //#getNodesPerElement end

        //#getType begin

        if (createdTopology->getType() == XdmfTopologyType::Triangle())
        {
                //Do whatever is to be done if the type is Triangle
        }

        //#getType end

        return 0;
}
