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

#include <Xdmf.h>
#include <vector>

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
     * Computes points dividing a line joined by two points point1 and point2
     * in the ratio 1/3 and 2/3 respectively.
     */
    inline void ComputeInteriorPoints(std::vector<XdmfFloat64> & midpoint1, std::vector<XdmfFloat64> & midpoint2, std::vector<XdmfFloat64> & point1, std::vector<XdmfFloat64> & point2);

    /*!
     * Computes points dividing a line joined by two points point1 and point2
     * in the ratio 1/3.
     */
    inline void ComputeOneThirdPoint(std::vector<XdmfFloat64> & midpoint1, std::vector<XdmfFloat64> & point1, std::vector<XdmfFloat64> & point2);

    /*!
     * Computes points dividing a line joined by two points point1 and point2
     * in the ratio 2/3.
     */
    inline void ComputeTwoThirdPoint(std::vector<XdmfFloat64> & midpoint1, std::vector<XdmfFloat64> & point1, std::vector<XdmfFloat64> & point2);
 
    /*!
     * Take an XdmfGrid containing linear hexahedron elements and convert to
     * 64-noded tri-cubic hexahedron elements.
     *
     * @param grid an XdmfGrid to convert.
     *
     * @return a pointer to the converted XdmfGrid
     */
    XdmfGrid * Generate(XdmfGrid * grid);

private:

    void GenerateNewPoints(XdmfGeometry * geometry);

};
