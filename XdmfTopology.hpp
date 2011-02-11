/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfTopology.hpp                                                    */
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

#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

// Forward Declarations
class XdmfTopologyType;

// Includes
#include "Xdmf.hpp"
#include "XdmfArray.hpp"

/**
 * @brief Handles the connectivity information in an XdmfGrid.
 *
 * XdmfTopology is a required part of an XdmfGrid.  It stores the connectivity information
 * between all points contained in an XdmfGrid.  XdmfTopology contains an XdmfTopologyType property
 * which should be set that specifies the element type stored.
 */
class XDMF_EXPORT XdmfTopology : public XdmfArray {

public:

	/**
	 * Create a new XdmfTopology.
	 *
	 * @return constructed XdmfTopology.
	 */
	static boost::shared_ptr<XdmfTopology> New();

	virtual ~XdmfTopology();

	LOKI_DEFINE_VISITABLE(XdmfTopology, XdmfArray)
	static const std::string ItemTag;

	std::map<std::string, std::string> getItemProperties() const;

	std::string getItemTag() const;

	/**
	 * Get the number of elements this Topology contains.
	 *
	 * @return int of number elements in the Topology.
	 */
	virtual unsigned int getNumberElements() const;

	/**
	 * Get the XdmfTopologyType associated with this topology.
	 *
	 * @return XdmfTopologyType of the topology.
	 */
	boost::shared_ptr<const XdmfTopologyType> getType() const;

	/**
	 * Set the XdmfTopologyType associated with this topology.
	 *
	 * @param type the XdmfTopologyType to set.
	 */
	void setType(const boost::shared_ptr<const XdmfTopologyType> type);

protected:

	XdmfTopology();
	virtual void populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader);

private:

	XdmfTopology(const XdmfTopology & topology);  // Not implemented.
	void operator=(const XdmfTopology & topology);  // Not implemented.

	boost::shared_ptr<const XdmfTopologyType> mType;
};

#ifdef _WIN32
    XDMF_TEMPLATE template class XDMF_EXPORT boost::shared_ptr<const XdmfTopologyType>;
#endif

#endif /* XDMFTOPOLOGY_HPP_ */
