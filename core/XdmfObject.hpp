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

	/**
	 * Converts a filesystem path to an absolute real path (absolute path with no symlinks)
	 *
	 * @param path a string containing the path to convert.
	 * @return the equivalent real path.
	 */
	static std::string getRealPath(const std::string & path);

protected:

	XdmfObject();

private:

	XdmfObject(const XdmfObject & object);  // Not implemented.
	void operator=(const XdmfObject & object);  // Not implemented.

};

#endif /* XDMFOBJECT_HPP_ */
