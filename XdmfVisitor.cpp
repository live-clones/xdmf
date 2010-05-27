// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfItem.hpp"
#include "XdmfVisitor.hpp"

XdmfVisitor::XdmfVisitor()
{
}

XdmfVisitor::~XdmfVisitor()
{
}

void XdmfVisitor::visit(XdmfItem & item, boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	std::cout << "Writing in base" << std::endl;
	item.traverse(visitor);
}
