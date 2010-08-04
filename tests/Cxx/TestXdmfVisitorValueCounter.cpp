#include "XdmfTestDataGenerator.hpp"
#include "XdmfVisitor.hpp"
#include "XdmfGrid.hpp"

// Make a new XdmfVisitor that simply counts number of values
class XdmfVisitorValueCounter : public XdmfVisitor,
	public Loki::Visitor<XdmfArray>,
	public Loki::Visitor<XdmfSet> {

public:

	/**
	 * Create a new XdmfVisitorValueCounter.
	 *
	 * @return constructed XdmfVisitorValueCounter.
	 */
	static boost::shared_ptr<XdmfVisitorValueCounter> New()
	{
		boost::shared_ptr<XdmfVisitorValueCounter> p(new XdmfVisitorValueCounter());
		return p;
	};

	~XdmfVisitorValueCounter()
	{
	};

	int getCount()
	{
		return mCount;
	}

	void visit(XdmfArray & array, boost::shared_ptr<Loki::BaseVisitor> visitor)
	{
		mCount += array.size();
	}

	void visit(XdmfSet & set, boost::shared_ptr<Loki::BaseVisitor> visitor)
	{
		mCount += set.size();
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
