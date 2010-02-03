/*
 * XdmfGeometry.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFGEOMETRY_HPP_
#define XDMFGEOMETRY_HPP_

#include "XdmfObject.hpp"

class XdmfGeometry : public XdmfObject {

public:

	XdmfNewMacro(XdmfGeometry);
	virtual std::string printSelf() const;

protected:

	XdmfGeometry();
	virtual ~XdmfGeometry();

private:

  class Delete
  {
  public:
      void operator()(XdmfGeometry * p) { delete p; }
  };

  XdmfGeometry(const XdmfGeometry&);  // Not implemented.
  void operator=(const XdmfGeometry&);  // Not implemented.

};

#endif /* XDMFGEOMETRY_HPP_ */
