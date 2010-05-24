#ifndef XDMFOBJECT_HPP_
#define XDMFOBJECT_HPP_

// Includes
#include <iostream>
#include <boost/shared_ptr.hpp>

#define XdmfNewMacro(type) \
template <typename T> friend void boost::checked_delete(T * x); \
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

protected:

	XdmfObject();
	virtual ~XdmfObject();

};

#endif /* XDMFOBJECT_HPP_ */
