#ifndef XDMFOBJECT_HPP_
#define XDMFOBJECT_HPP_

#include "boost/shared_ptr.hpp"
#include <iostream>
#include <ostream>

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

	virtual std::string printSelf() const = 0;

protected:

	XdmfObject();
	virtual ~XdmfObject();

};

std::ostream& operator<<(std::ostream& out, const XdmfObject& ob);

#endif /* XDMFOBJECT_HPP_ */
