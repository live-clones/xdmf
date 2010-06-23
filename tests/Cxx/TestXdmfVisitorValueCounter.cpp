#include "XdmfTestDataGenerator.hpp"
#include "XdmfVisitor.hpp"
#include "XdmfGrid.hpp"

// Make a new XdmfVisitor that simply counts number of values
class XdmfVisitorValueCounter : public XdmfVisitor,
	public Loki::Visitor<XdmfArray> {

public:

	XdmfNewMacro(XdmfVisitorValueCounter);
	~XdmfVisitorValueCounter()
	{
	};

	int getCount()
	{
		return mCount;
	}

	void visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
	{
		mCount += array.getSize();
	}

protected:

	XdmfVisitorValueCounter() :
		mCount(0)
	{
	}

private:

	int mCount;
};

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfVisitorValueCounter> visitor = XdmfVisitorValueCounter::New();
	boost::shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

	grid->accept(visitor);

	assert(visitor->getCount() == 69);

	return 0;
}
