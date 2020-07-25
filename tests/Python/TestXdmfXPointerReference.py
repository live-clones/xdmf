from Xdmf import *

if __name__ == "__main__":

        reader = XdmfReader.New()

        domain = XdmfDomain.New()

        collection1 = XdmfGridCollection.New()
        collection2 = XdmfGridCollection.New()

        unstructuredGrid = XdmfUnstructuredGrid.New()

        collection1.insert(unstructuredGrid)
        collection2.insert(unstructuredGrid)
        domain.insert(collection1)
        domain.insert(collection2)

        writer = XdmfWriter.New("duplicateXpointer.xmf")

        domain.accept(writer)

        readDomain = reader.read("duplicateXpointer.xmf")

        print(str(readDomain.getGridCollection(0).getUnstructuredGrid(0)) + "\n?=\n" + str(readDomain.getGridCollection(1).getUnstructuredGrid(0)))

        assert(readDomain.getGridCollection(0).getUnstructuredGrid(0) == readDomain.getGridCollection(1).getUnstructuredGrid(0))

        readDomain.getGridCollection(0).getUnstructuredGrid(0).setName("Test Grid")

        time = XdmfTime.New(5.5)

        readDomain.getGridCollection(1).getUnstructuredGrid(0).setTime(time)

        writer2 = XdmfWriter.New("duplicateXpointer2.xmf")

        readDomain.accept(writer2)

        readDomain2 = reader.read("duplicateXpointer2.xmf")

        print(readDomain2.getGridCollection(1).getUnstructuredGrid(0).getName() + " ?= Test Grid")

        print(str(readDomain2.getGridCollection(0).getUnstructuredGrid(0).getTime().getValue()) + " ?= " + str(5.5))

        assert(readDomain2.getGridCollection(1).getUnstructuredGrid(0).getName() =="Test Grid")

        assert(readDomain2.getGridCollection(0).getUnstructuredGrid(0).getTime().getValue() == 5.5)
