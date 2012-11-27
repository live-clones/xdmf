from Xdmf import *

if __name__ == "__main__":
	exampleTime = XdmfTime.New()

	newTime = 5.0
	exampleTime2 = XdmfTime.New(newTime)

	exampleTime.setValue(newTime)

	readTime = exampleTime.getTime()
