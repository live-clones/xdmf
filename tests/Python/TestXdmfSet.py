from Xdmf import *

if __name__ == "__main__":

    set = XdmfSet.New()
    print(set.getName() + " ?= ")
    print("?! " + str(set.isInitialized()))
    assert(set.getName() == "")
    assert(not set.isInitialized())
    set.setName("foo")
    print(set.getName() + " ?= foo")
    print(str(set.getType()) + " ?= " + str(XdmfSetType.NoSetType()))
    assert(set.getName() == "foo")
    assert(set.getType() == XdmfSetType.NoSetType())
    set.setType(XdmfSetType.Node())
    print(str(set.getType()) + " ?= " + str(XdmfSetType.Node()))
    print(str(set.getSize()) + " ?= " + str(0))
    assert(set.getType() == XdmfSetType.Node())
    assert(set.getSize() == 0)
    print(type(1).__name__)
    set.insertValueAsInt32(0, 1)
    print(str(set.getSize()) + " ?= " + str(1))
    assert(set.getSize() == 1)

