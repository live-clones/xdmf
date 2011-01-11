/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Kenneth Leiter                                              */
/*     kenneth.leiter@arl.army.mil                                 */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2010 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/

class XdmfGrid;

#include "Xdmf.h"

/*!
 * @brief XdmfPartitioner partitions an XdmfGrid into a number of XdmfGrids 
 * using the metis library.  A pointer to an XdmfGrid spatial collection 
 * is returned containing the partitioned grids.
 */

class XdmfPartitioner
{
  public:

    enum MetisScheme {
      NodalGraph = 1,
      DualGraph
    };

    /*!
     * Constructor.
     */
    XdmfPartitioner();

    /*!
     * Destructor.
     */
    ~XdmfPartitioner();

    void DoNotSplit(XdmfSet * set);

    /*!
     * 
     * Partitions an XdmfGrid into a number of partitions using the 
     * metis library.  Currently supported topology types are:
     *
     * XDMF_TRI, XDMF_TRI_6, 
     * XDMF_QUAD, XDMF_QUAD_8, XDMF_QUAD_9,
     * XDMF_TET, XDMF_TET_10, 
     * XDMF_HEX, XDMF_HEX_20, XDMF_HEX_24, XDMF_HEX_27, XDMF_HEX_64, 
     * XDMF_HEX_125
     *
     * The routine splits the XdmfGrid and also splits all attributes and 
     * sets into their proper partitions.  An attribute with name 
     * "GlobalNodeId" is added to each partitioned grid to map node ids to 
     * their original global id in the unpartitioned grid.
     *
     * @param grid a XdmfGrid to partition.
     * @param numPartitions the number of partitions to split the grid.
     * @param parentElement the parent XdmfElement to insert the created 
     * spatial collection into.
     * @param metisScheme the metis partitioning scheme, default = NodalGraph.
     *
     * @return XdmfGrid a spatial collection containing partitioned grids.
     */
    XdmfGrid * Partition(XdmfGrid *    grid, 
			 int           numPartitions, 
			 XdmfElement * parentElement,
			 MetisScheme   metisScheme = NodalGraph);

  private:

    class XdmfPartitionerImpl;
    XdmfPartitionerImpl * mImpl;
};
