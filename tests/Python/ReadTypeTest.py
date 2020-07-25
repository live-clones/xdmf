from Xdmf import *

if __name__ == "__main__":

    testDomain = XdmfDomain.New()
    testCollection1 = XdmfGridCollection.New()
    testCollection2 = XdmfGridCollection.New()
    testCollection3 = XdmfGridCollection.New()

    testCollection2.insert(testCollection3)
    testCollection1.insert(testCollection2)
    testDomain.insert(testCollection1)

    testWriter = XdmfWriter.New("typeTest.xmf")

    testDomain.accept(testWriter)

    readXPath = "/Xdmf/Domain/Grid"
    testReader = XdmfReader.New()

    readVector = testReader.read("typeTest.xmf", readXPath)

    print(type(readVector))
    print(type(readVector[0]))

    print(str(readVector[0]) + " ?= " + str(XdmfGridCollection))

    assert type(readVector[0]) == XdmfGridCollection
