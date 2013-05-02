#include "XdmfError.hpp"
#include <iostream>

int main(int, char **)
{
	XdmfError testError = XdmfError(XdmfError:DEBUG, "This is a debug error");

	XdmfError::Level exampleLevel = XdmfError::getLevelLimit();

	XdmfError::setLevelLimit(XdmfError::FATAL);

	XdmfError::setBuffer(std::cout.rdbuf());//the default buffer is cout



	try
	{
		XdmfError::message(XdmfError::FATAL, "this is an example error");
	}
	catch (XdmfError exampleError)
	{
		XdmfError::Level exampleError.getLevel();

		exampleError.setLevel(XdmfError::WARNING);

		char * errorOutput = exampleError.what();
	}

	return 0;
}
