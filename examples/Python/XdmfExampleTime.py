from Xdmf import *

if __name__ == "__main__":
	#Assumming that exampleTime is a shared pointer to an XdmfTime object

	exampleTime = XdmfTime.New()
	#The Default case sets the time to 0.0
	#You can also specify a time when creating the XdmfTime object

	newTime = 5.0
	exampleTime2 = XdmfTime.New(newTime)

	exampleTime.setValue(newTime)

	readTime = exampleTime.getTime()
