#include "XdmfVisitor.hpp"

#include "XdmfTestDataGenerator.hpp"

// Make a new XdmfVisitor that simply counts number of values
class XdmfVisitorValueCounter : public XdmfVisitor {
public:

	XdmfNewMacro(XdmfVisitorValueCounter);

	int getCount()
	{
		return mCount;
	}

	virtual void visit(const XdmfArray * const array, boost::shared_ptr<XdmfVisitor> visitor)
	{
		mCount += array->getSize();
	}

protected:

	XdmfVisitorValueCounter() :
		mCount(0)
	{
	}

	~XdmfVisitorValueCounter()
	{
	};

private:

	int mCount;
};

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfVisitorValueCounter> visitor = XdmfVisitorValueCounter::New();
	boost::shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

	grid->write(visitor);

	assert(visitor->getCount() == 66);

	return 0;
}
