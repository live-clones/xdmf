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
 * @brief XdmfHex125Generator generates a 125-noded tri-quartic hexahedron
 *        mesh from an XdmfGrid containing linear hexahedron elements.
 */

class XdmfHex125Generator
{
public:

    enum Type { Normal, Spectral };

    /*!
     * Constructor.
     */
    XdmfHex125Generator();

    /*!
     * Destructor.
     */
    ~XdmfHex125Generator();

    /*!
     * Take an XdmfGrid containing linear hexahedron elements and convert to
     * 125-noded tri-quartic hexahedron elements.
     *
     * @param grid an XdmfGrid containing XDMF_HEX topoology to convert.
     * @param parentElement the XdmfElement to insert the grid into.
     *
     * @return a pointer to the converted XdmfGrid.
     */
    XdmfGrid * Generate(XdmfGrid * grid, XdmfElement * parentElement, Type type = Normal);

    /*!
     * Take an XdmfGrid containing tri-quartic hexahedron elements and convert to
     * linear hexahedron grid (split each tri-quartic element into linear hexahedron elements).
     *
     * @param grid an XdmfGrid containing XDMF_HEX_125 topology to split.
     * @param parentElement the XdmfElement to insert the grid into.
     * @param splitAttributes whether to split attributes in the grid.
     *
     * @return a pointer to the split XdmfGrid.
     */
    XdmfGrid * Split(XdmfGrid * grid, 
                     XdmfElement * parentElement,
                     bool splitAttributes = true);

private:

    // Operations used to generate elements
    class Operations;
    class NormalOperations;
    class SpectralOperations;

};
