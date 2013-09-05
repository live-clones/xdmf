from Xdmf import *

if __name__ == "__main__":
        #//initialization begin

        exampleTime = XdmfTime.New()
        #The Default case sets the time to 0.0
        #You can also specify a time when creating the XdmfTime object

        newTime = 5.0
        exampleTime2 = XdmfTime.New(newTime)

        #//initialization end

        #//setValue begin

        exampleTime.setValue(newTime)

        #//setValue end

        #//getValue begin

        readTime = exampleTime.getValue()

        #//getValue end
