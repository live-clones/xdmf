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
class XdmfSet;
class XdmfUnstructuredGrid;

// Includes
#include <set>
#include "XdmfUtils.hpp"
#include "XdmfSharedPtr.hpp"

/**
 * @brief XdmfPartitioner partitions an XdmfGrid using the metis
 * library.
 *
 * XdmfPartitioner uses the metis library to partition XdmfGrids.
 */
class XDMFUTILS_EXPORT XdmfPartitioner {

public:

  enum MetisScheme {
    DUAL_GRAPH = 0,
    NODAL_GRAPH = 1
  };

  /**
   * Create a new XdmfPartitioner.
   *
   * @return constructed XdmfPartitioner.
   */
  static shared_ptr<XdmfPartitioner> New();

  virtual ~XdmfPartitioner();

  /**
   * Ignore set when partitioning. Set is not partitioned or added to
   * resulting grid.
   *
   * @param set the set to ignore when partitioning.
   */
  void ignore(const shared_ptr<const XdmfSet> set);

  /**
   * Partitions an XdmfUnstructuredGrid using the metis library.
   *
   * The partitioner splits the XdmfGridUnstructured and all attached
   * XdmfAttributes and XdmfSets into their proper partition. An
   * XdmfAttribute named "GlobalNodeId" is added to each partitioned
   * grid to map partitioned node ids to their original unpartitioned
   * id. An XdmfMap is added to each partitioned grid mapping shared
   * nodes to other processors. All arrays attached to the passed
   * gridToPartition are read from disk if not initialized.
   *
   * @param gridToPartition an XdmfGridUnstructured to partition.
   * @param numberOfPartitions the number of pieces to partition the grid into.
   * @param metisScheme which metis partitioning scheme to use.
   * @param heavyDataWriter an XdmfHDF5Writer to write the partitioned mesh to.
   * If no heavyDataWriter is specified, all partitioned data will remain in
   * memory.
   *
   * @return a spatial collection containing partitioned grids.
   */
  shared_ptr<XdmfGridCollection>
  partition(const shared_ptr<XdmfUnstructuredGrid> gridToPartition,
            const unsigned int numberOfPartitions,
            const MetisScheme metisScheme = DUAL_GRAPH,
            const shared_ptr<XdmfHeavyDataWriter> heavyDataWriter = shared_ptr<XdmfHeavyDataWriter>()) const;

  /**
   * Unpartitions an XdmfGridCollection of unstructured grids into a single
   * XdmfUnstructuredGrid.
   *
   * @param gridToPartition an XdmfGridUnstructured to partition.
   *
   * @return a unstructured grid containing the unpartitioned grid.
   */
  shared_ptr<XdmfUnstructuredGrid>
  unpartition(const shared_ptr<XdmfGridCollection> gridToUnPartition) const;

protected:

  XdmfPartitioner();

private:

  XdmfPartitioner(const XdmfPartitioner & partitioner);  // Not implemented.
  void operator=(const XdmfPartitioner & partitioner);  // Not implemented.

  std::set<shared_ptr<const XdmfSet> > mIgnoredSets;

};

#endif /* XDMFPARTITIONER_HPP_ */
