#ifndef XDMFPARTITIONER_HPP_
#define XDMFPARTITIONER_HPP_

// Forward Declarations
class XdmfGrid;
class XdmfGridCollection;
class XdmfHDF5Writer;

// Includes
#include "XdmfObject.hpp"

/*!
 * @brief XdmfPartitioner partitions an XdmfGrid using the metis library.
 *
 * XdmfPartitioner uses the metis library to partition XdmfGrids.
 */
class XdmfPartitioner : public XdmfObject {

public:

	XdmfNewMacro(XdmfPartitioner);
	virtual ~XdmfPartitioner();

	/**
	 * Partitions an XdmfGrid using the metis library.  Currently supported topology types are:
	 *
	 * XdmfTopologyType::Triangle
	 * XdmfTopologyType::Triangle_6
	 * XdmfTopologyType::Quadrilateral
	 * XdmfTopologyType::Quadrilateral_8
	 * XdmfTopologyType::Tetrahedron
	 * XdmfTopologyType::Tetrahedron_10
	 * XdmfTopologyType::Hexahedron
	 * XdmfTopologyType::Hexahedron_20
	 * XdmfTopologyType::Hexahedron_24
	 * XdmfTopologyType::Hexahedron_27
	 * XdmfTopologyType::Hexahedron_64
	 *
	 * The partitioner splits the XdmfGrid and all attached XdmfAttributes and XdmfSets into their proper partition.
	 * An XdmfAttribute named "GlobalNodeId" is added to each partitioned grid to map partitioned node ids to their
	 * original unpartitioned id.  This could allow grids to be recombined.
	 *
	 * @param gridToPartition an XdmfGrid to partition.
	 * @param numberOfPartitions the number of pieces to partition the grid into.
	 * @param heavyDataWriter an XdmfHDF5Writer to write the partitioned mesh to.  If no heavyDataWriter is specified, all partitioned data will remain in memory.
	 *
	 * @return a spatial collection containing partitioned grids.
	 */
	boost::shared_ptr<XdmfGridCollection> partition(const boost::shared_ptr<XdmfGrid> gridToPartition, const unsigned int numberOfPartitions,
			boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter);

protected:

	XdmfPartitioner();

private:

	XdmfPartitioner(const XdmfPartitioner & partitioner);  // Not implemented.
	void operator=(const XdmfPartitioner & partitioner);  // Not implemented.
};

#endif /* XDMFPARTITIONER_HPP_ */
