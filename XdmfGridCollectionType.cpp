/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfGridCollectionType.cpp                                          */
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

#include "XdmfGridCollectionType.hpp"

// Supported XdmfGridCollectionTypes
boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollectionType::NoCollectionType()
{
	static boost::shared_ptr<const XdmfGridCollectionType> p(new XdmfGridCollectionType("None"));
	return p;
}

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollectionType::Spatial()
{
	static boost::shared_ptr<const XdmfGridCollectionType> p(new XdmfGridCollectionType("Spatial"));
	return p;
}

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollectionType::Temporal()
{
	static boost::shared_ptr<const XdmfGridCollectionType> p(new XdmfGridCollectionType("Temporal"));
	return p;
}

XdmfGridCollectionType::XdmfGridCollectionType(const std::string & name) :
	mName(name)
{
}

XdmfGridCollectionType::~XdmfGridCollectionType()
{
}

boost::shared_ptr<const XdmfGridCollectionType> XdmfGridCollectionType::New(const std::map<std::string, std::string> & itemProperties)
{
	std::map<std::string, std::string>::const_iterator type = itemProperties.find("CollectionType");
	if(type != itemProperties.end())
	{
		const std::string typeVal = type->second;
		if(typeVal.compare("None") == 0)
		{
			return NoCollectionType();
		}
		else if(typeVal.compare("Spatial") == 0)
		{
			return Spatial();
		}
		else if(typeVal.compare("Temporal") == 0)
		{
			return Temporal();
		}
		else
		{
			assert(false);
		}
	}
	assert(false);
    return boost::shared_ptr<const XdmfGridCollectionType>();
}

void XdmfGridCollectionType::getProperties(std::map<std::string, std::string> & collectedProperties) const
{
	collectedProperties["CollectionType"] = mName;
}
