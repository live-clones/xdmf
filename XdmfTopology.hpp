/*
 * XdmfTopology.hpp
 *
 *  Created on: Jan 25, 2010
 *      Author: kleiter
 */

#ifndef XDMFTOPOLOGY_HPP_
#define XDMFTOPOLOGY_HPP_

#include "XdmfObject.hpp"

class XdmfTopology : public XdmfObject {

public:

	XdmfNewMacro(XdmfTopology);

	virtual std::string printSelf() const;

protected:

	XdmfTopology();
	virtual ~XdmfTopology();

private:

  XdmfTopology(const XdmfTopology&);  // Not implemented.
  void operator=(const XdmfTopology&);  // Not implemented.

};

#endif /* XDMFTOPOLOGY_HPP_ */
