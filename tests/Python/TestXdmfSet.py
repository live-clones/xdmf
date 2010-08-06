from Xdmf import *

if __name__ == "__main__":

	set = XdmfSet.New()
	assert(set.getName() == "")
	set.setName("foo")
	assert(set.getName() == "foo")
	assert(set.getType() == XdmfSetType.NoSetType())
	set.setType(XdmfSetType.Node())
	assert(set.getType() == XdmfSetType.Node())
	assert(set.size() == 0)
	set.insert(1)
	#assert(set.size() == 1)
	#print set.find("1")

