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

void XdmfVisitor::visit(const XdmfItem * const item)
{
	xmlData << item->printSelf();
}

const std::string XdmfVisitor::printSelf() const
{
	return "XdmfVisitor:\n\tContents: " + xmlData.str();
}
