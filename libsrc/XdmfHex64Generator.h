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
/*     Copyright @ 2009 US Army Research Laboratory                */
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

#include "XdmfObject.h"

/*!
 * @brief XdmfHex64Generator generates a 64-noded tri-cubic hexahedron
 *        mesh from an XdmfGrid containing linear hexahedron elements.
 *        Currently, attributes and sets are ignored.
 */

class XdmfHex64Generator
{
public:
    /*!
     * Constructor.
     */
    XdmfHex64Generator();

    /*!
     * Destructor.
     */
    ~XdmfHex64Generator();

    /*!
     * Take an XdmfGrid containing linear hexahedron elements and convert to
     * 64-noded tri-cubic hexahedron elements.
     *
     * @param grid an XdmfGrid containing XDMF_HEX topoology to convert.
     * @param parentElement the XdmfElement to insert the grid into.
     *
     * @return a pointer to the converted XdmfGrid.
     */
    XdmfGrid * Generate(XdmfGrid * grid, XdmfElement * parentElement);

    /*!
     * Take an XdmfGrid containing tri-cubic hexahedron elements and convert to
     * linear hexahedron grid (split each tri-cubic element into 27 linear hexahedron elements).
     *
     * @param grid an XdmfGrid containing XDMF_HEX_64 topology to split.
     * @param parentElement the XdmfElement to insert the grid into.
     *
     * @return a pointer to the split XdmfGrid.
     */
    XdmfGrid * Split(XdmfGrid * grid, XdmfElement * parentElement);

};
