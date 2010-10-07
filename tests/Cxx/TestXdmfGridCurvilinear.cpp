#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCurvilinear.hpp"
#include "XdmfReader.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"
#include "XdmfWriter.hpp"

#include "XdmfTestCompareFiles.hpp"

int main(int, char *)
{
	boost::shared_ptr<XdmfGridCurvilinear> grid = XdmfGridCurvilinear::New(2, 2, 3);
	boost::shared_ptr<XdmfArray> dimensions = grid->getDimensions();
	assert(dimensions->getSize() == 3);
	assert(dimensions->getValue<unsigned int>(0) == 2);
	assert(dimensions->getValue<unsigned int>(1) == 2);
	assert(dimensions->getValue<unsigned int>(2) == 3);

	boost::shared_ptr<XdmfGeometry> geometry = grid->getGeometry();
	geometry->setType(XdmfGeometryType::XYZ());
	geometry->resize<int>(36);
	const int coordinates[36] = {0, 0, 0, 1, 0, 0, 4, 0, 0, 0, 0, 2, 1, 0, 2, 4, 0, 2, 0, 1, 0, 1, 1, 0, 4, 1, 0, 0, 1, 2, 1, 1, 2, 4, 1, 2};
	geometry->insert(0, &coordinates[0], 36);

	// Check values under the hood

	boost::shared_ptr<const XdmfTopology> topology = grid->getTopology();
	assert(topology->getNumberElements() == 2);
	boost::shared_ptr<const XdmfTopologyType> topologyType = topology->getType();
	assert(topologyType->getNodesPerElement() == 8);

	assert(geometry->getNumberPoints() == 12);
	boost::shared_ptr<const XdmfGeometryType> geometryType = geometry->getType();
	assert(geometryType->getDimensions() == 3);

	// Input / Output

	boost::shared_ptr<XdmfWriter> writer = XdmfWriter::New("TestXdmfGridCurvilinear1.xmf");
	grid->accept(writer);

	boost::shared_ptr<XdmfReader> reader = XdmfReader::New();
	boost::shared_ptr<XdmfGridCurvilinear> grid2 = boost::shared_dynamic_cast<XdmfGridCurvilinear>(reader->read("TestXdmfGridCurvilinear1.xmf"));

	boost::shared_ptr<XdmfWriter> writer2 = XdmfWriter::New("TestXdmfGridCurvilinear2.xmf");
	grid2->accept(writer2);

	assert(XdmfTestCompareFiles::compareFiles("TestXdmfGridCurvilinear1.xmf", "TestXdmfGridCurvilinear2.xmf"));

	return 0;
}
