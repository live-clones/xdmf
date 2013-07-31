from Xdmf import *

if __name__ == "__main__":
	#//createItem begin

	#using XdmfItemFactory because XdmfCoreItemFactory is abstract
	exampleFactory = XdmfItemFactory.New()
	newProperties = StringMap()
	newChildren = ItemVector()
	exampleItem = exampleFactory.createItem(XdmfAttribute.ItemTag, newProperties, newChildren)
	#Same usage as the individual constructors
	#childItems and itemProperties are not added to the item when created this way
	#the collections are used to determine type

	#//createItem end
