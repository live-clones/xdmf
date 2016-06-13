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

#ifdef __cplusplus

// Forward Declarations
class XdmfGraph;
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
  void ignore(const XdmfSet *);

  /**
   * Partitions an XdmfGraph using the metis library. An attribute
   * named "Partition" is added to the XdmfGraph that contains
   * partition numbers for each graph vertex.
   *
   * @param graphToPartition an XdmfGraph to partition.
   * @param numberOfPartitions the number of pieces to partition the
   * graph into.
   */
  void
  partition(const shared_ptr<XdmfGraph> graphToPartition,
            const unsigned int numberOfPartitions) const;

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

  XdmfPartitioner(const XdmfPartitioner & partitioner);

protected:

  XdmfPartitioner();

private:

  void operator=(const XdmfPartitioner & partitioner);  // Not implemented.

  std::set<const XdmfSet * > mIgnoredSets;

};

#endif

#ifdef __cplusplus
extern "C" {
#endif

#define XDMF_PARTITIONER_SCHEME_DUAL_GRAPH  500
#define XDMF_PARTITIONER_SCHEME_NODAL_GRAPH 501

// C wrappers go here

struct XDMFPARTITIONER; // Simply as a typedef to ensure correct typing
typedef struct XDMFPARTITIONER XDMFPARTITIONER;

XDMFUTILS_EXPORT XDMFPARTITIONER * XdmfPartitionerNew();

XDMFUTILS_EXPORT void XdmfPartitionerIgnore(XDMFPARTITIONER * partitioner,
                                            XDMFSET * set);

XDMFUTILS_EXPORT void XdmfPartitionerPartitionGraph(XDMFPARTITIONER * partitioner,
                                                    XDMFGRAPH * graphToPartition,
                                                    unsigned int numberOfPartitions);

XDMFUTILS_EXPORT XDMFGRIDCOLLECTION * XdmfPartitionerPartitionUnstructuredGrid(XDMFPARTITIONER * partitioner,
                                                                               XDMFUNSTRUCTUREDGRID * gridToPartition,
                                                                               unsigned int numberOfPartitions,
                                                                               int metisScheme,
                                                                               XDMFHEAVYDATAWRITER * heavyDataWriter);

XDMFUTILS_EXPORT XDMFUNSTRUCTUREDGRID * XdmfPartitionerUnpartition(XDMFPARTITIONER * partitioner,
                                                                   XDMFGRIDCOLLECTION * gridToUnPartition);

XDMFUTILS_EXPORT void XdmfPartitionerFree(XDMFPARTITIONER * partitioner);

#ifdef __cplusplus
}
#endif


#endif /* XDMFPARTITIONER_HPP_ */
