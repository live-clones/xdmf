from Xdmf import *

if __name__ == "__main__":

    set = XdmfSet.New()
    assert(set.getName() == "")
    set.setName("foo")
    assert(set.getName() == "foo")
    assert(set.getType() == XdmfSetType.NoSetType())
    set.setType(XdmfSetType.Node())
    assert(set.getType() == XdmfSetType.Node())
    assert(set.getSize() == 0)
    set.copyValueAsInt32(0, 1)
    assert(set.getSize() == 1)

