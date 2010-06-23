#ifndef XDMFOBJECT_HPP_
#define XDMFOBJECT_HPP_

// Includes
#include <iostream>
#include <boost/shared_ptr.hpp>

#define XdmfNewMacro(type) \
static boost::shared_ptr<type> New() \
{ \
	boost::shared_ptr<type> p(new type()); \
	return p; \
}

/**
 * @brief Root Xdmf Class.
 *
 * XdmfObject is the root Xdmf class which all Xdmf classes are derived from.
 */
class XdmfObject {

public:

	virtual ~XdmfObject();

protected:

	XdmfObject();

};

#endif /* XDMFOBJECT_HPP_ */
