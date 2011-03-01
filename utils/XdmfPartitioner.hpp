/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfPartitioner.hpp                                                 */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFPARTITIONER_HPP_
#define XDMFPARTITIONER_HPP_

// Forward Declarations
class XdmfGridCollection;
class XdmfHeavyDataWriter;
class XdmfUnstructuredGrid;

// Includes
#include <boost/shared_ptr.hpp>

/**
 * @brief XdmfPartitioner partitions an XdmfGrid using the metis
 * library.
 *
 * XdmfPartitioner uses the metis library to partition XdmfGrids.
 */
class XdmfPartitioner {

public:

  /**
   * Create a new XdmfPartitioner.
   *
   * @return constructed XdmfPartitioner.
   */
  static boost::shared_ptr<XdmfPartitioner> New();

  virtual ~XdmfPartitioner();

  /**
   * Partitions an XdmfGrid using the metis library.
   * Currently supported topology types are:
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
   * The partitioner splits the XdmfGridUnstructured and all attached
   * XdmfAttributes and XdmfSets into their proper partition. An
   * XdmfAttribute named "GlobalNodeId" is added to each partitioned
   * grid to map partitioned node ids to their original unpartitioned
   * id. All arrays attached to the passed gridToPartition are read
   * from disk if not initialized.
   *
   * @param gridToPartition an XdmfGridUnstructured to partition.
   * @param numberOfPartitions the number of pieces to partition the grid into.
   * @param heavyDataWriter an XdmfHDF5Writer to write the partitioned mesh to.
   * If no heavyDataWriter is specified, all partitioned data will remain in
   * memory.
   *
   * @return a spatial collection containing partitioned grids.
   */
  boost::shared_ptr<XdmfGridCollection>
  partition(const boost::shared_ptr<XdmfUnstructuredGrid> gridToPartition,
            const unsigned int numberOfPartitions,
            const boost::shared_ptr<XdmfHeavyDataWriter> heavyDataWriter = boost::shared_ptr<XdmfHeavyDataWriter>()) const;

protected:

  XdmfPartitioner();

private:

  XdmfPartitioner(const XdmfPartitioner & partitioner);  // Not implemented.
  void operator=(const XdmfPartitioner & partitioner);  // Not implemented.
};

#endif /* XDMFPARTITIONER_HPP_ */
