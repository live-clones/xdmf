/*
 * XdmfObject.hpp
 *
 *  Created on: Feb 1, 2010
 *      Author: kleiter
 */

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

#define XdmfGetValueMacro(var, type) \
type get##var () \
{ \
  return ( this->m##var ); \
}

#define XdmfSetValueMacro(var,type) \
void set##var (const type _arg) \
{ \
  this->m##var = _arg; \
}

class XdmfObject {

public:

	virtual const std::string printSelf() const = 0;

protected:

	XdmfObject();
	virtual ~XdmfObject();

};

std::ostream& operator<<(std::ostream& out, const XdmfObject& ob);

#endif /* XDMFOBJECT_HPP_ */
