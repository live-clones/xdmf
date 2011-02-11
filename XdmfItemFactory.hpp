/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfItemFactory.hpp                                                 */
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

#ifndef XDMFITEMFACTORY_HPP_
#define XDMFITEMFACTORY_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include "Xdmf.hpp"
#include "XdmfCoreItemFactory.hpp"

/**
 * @brief Factory for constructing XdmfItems from their ItemTag and ItemProperties
 */
class XDMF_EXPORT XdmfItemFactory : public XdmfCoreItemFactory {

public:

	/**
	 * Create a new XdmfItemFactory.
	 *
	 * @return constructed XdmfItemFactory.
	 */
	static boost::shared_ptr<XdmfItemFactory> New();

	virtual ~XdmfItemFactory();

	virtual boost::shared_ptr<XdmfItem> createItem(const std::string & itemTag, const std::map<std::string, std::string> & itemProperties, const std::vector<boost::shared_ptr<XdmfItem> > & childItems) const;

protected:

	XdmfItemFactory();

private:

	XdmfItemFactory(const XdmfItemFactory & itemFactory);  // Not implemented.
	void operator=(const XdmfItemFactory & itemFactory);  // Not implemented.

};

#ifdef _WIN32
    XDMF_TEMPLATE template class XDMF_EXPORT boost::shared_ptr<XdmfItemFactory>;
#endif

#endif /* XDMFITEMFACTORY_HPP_ */
