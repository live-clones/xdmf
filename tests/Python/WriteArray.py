from Xdmf import *

if __name__ == "__main__":
	primaryDomain = XdmfDomain.New()
	testGrid = XdmfUnstructuredGrid.New()
	primaryDomain.insert(testGrid)
	testGeometry = XdmfGeometry.New()
	for i in range (0, 11):
		testGeometry.pushBackAsInt32(i);
	testGrid.setGeometry(testGeometry)
	testTopology = XdmfTopology.New()
	testGrid.setTopology(testTopology)

	arrayWriter = XdmfWriter.New("array.xmf")
	primaryDomain.accept(arrayWriter)
