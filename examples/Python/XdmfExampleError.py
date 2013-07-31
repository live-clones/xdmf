from Xdmf import *

if __name__ == "__main__":
	#//getLevelLimit begin

	exampleLevel = XdmfError.getLevelLimit()
	#these are considered integers in Python

	#//getLevelLimit end

	#//setLevelLimit begin

	XdmfError.setLevelLimit(XdmfError.FATAL)

	#//setLevelLimit end

	#//message begin

	try:
		XdmfError::message(XdmfError::FATAL, "this is an example error")
	except RuntimeError as exampleError:
		print exampleError

	#//message end

	#//what begin

	try:
		raise RuntimeError(XdmfError(XdmfError.FATAL, "testError"))
	except RuntimeError as exampleError:
		errorString = e.args[0].what()

	#//what end

	#//getLevel begin

	try:
		raise RuntimeError(XdmfError(XdmfError.FATAL, "testError"))
	except RuntimeError as exampleError:
		errorLevel = e.args[0].getLevel()

	#//getLevel end

	#//setLevel begin

	try:
		raise RuntimeError(XdmfError(XdmfError.FATAL, "testError"))
	except RuntimeError as exampleError:
		e.args[0].setLevel(XdmfError.FATAL)

	#//setLevel end

	#//initialization begin

	try:
		raise RuntimeError(XdmfError(XdmfError.FATAL, "testError"))
	except RuntimeError as exampleError:
		print exampleError

	#//initialization end
