#include "XdmfTestDataGenerator.hpp"
#include "XdmfVisitor.hpp"
#include "XdmfGrid.hpp"

// Make a new XdmfVisitor that simply counts number of values
class XdmfVisitorValueCounter : public XdmfVisitor,
                                public Loki::Visitor<XdmfArray> {

public:

  /**
   * Create a new XdmfVisitorValueCounter.
   *
   * @return constructed XdmfVisitorValueCounter.
   */
  static shared_ptr<XdmfVisitorValueCounter>
  New()
  {
    shared_ptr<XdmfVisitorValueCounter> p(new XdmfVisitorValueCounter());
    return p;
  };

  ~XdmfVisitorValueCounter()
  {
  };

  int
  getCount()
  {
    return mCount;
  }

  void
  visit(XdmfArray & array, 
        const shared_ptr<XdmfBaseVisitor>)
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

int main(int, char **)
{
  shared_ptr<XdmfVisitorValueCounter> visitor = XdmfVisitorValueCounter::New();
  shared_ptr<XdmfGrid> grid = XdmfTestDataGenerator::createHexahedron();

  grid->accept(visitor);

  assert(visitor->getCount() == 71);

  return 0;
}
