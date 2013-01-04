from Xdmf import *

if __name__ == "__main__":
	exampleGrid = XdmfUnstructuredGrid.New()

	#Using an unstructured grid since XdmfGrid is an abstract class
	#Assumming that exampleGrid is a shared pointer to an XdmfUnstructuredGrid object

	newName = "New Name"
	exampleGrid.setName(newName)

	newTime = XdmfTime.New(20.0)
	exampleGrid.setTime(newTime)

	exampleTime = exampleGrid.getTime()

	exampleName = exampleGrid.getName()

	exampleGeometry = exampleGrid.getGeometry()

	exampleTopology = exampleGrid.getTopology()
