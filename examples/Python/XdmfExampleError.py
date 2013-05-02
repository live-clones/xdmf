from Xdmf import *

if __name__ == "__main__":
	exampleLevel = XdmfError.getLevelLimit()#these are considered integers in Python

	XdmfError.setLevelLimit(XdmfError.FATAL)

	try:
		XdmfError::message(XdmfError::FATAL, "this is an example error")
	except RuntimeError as exampleError:
		print exampleError

	try:
		raise RuntimeError(XdmfError(XdmfError.FATAL, "testError"))
	except RuntimeError as exampleError:
		errorString = e.args[0].what()
		errorLevel = e.args[0].getLevel()
		e.args[0].setLevel(XdmfError.FATAL)
