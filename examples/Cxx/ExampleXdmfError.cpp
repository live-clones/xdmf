#include "XdmfError.hpp"
#include <iostream>

int main(int, char **)
{
	//#initialization begin

	XdmfError testError = XdmfError(XdmfError:DEBUG, "This is a debug error");

	//#initialization end

	//#getLevel begin

	XdmfError::Level testError.getLevel();

	//#getLevel end

	//#setLevel begin

	testError.setLevel(XdmfError::WARNING);

	//#setLevel end

	//#what begin

	char * errorOutput = testError.what();

	//#what end

	//#getLevelLimit begin

	XdmfError::Level exampleLevel = XdmfError::getLevelLimit();

	//#getLevelLimit end

	//#setLevelLimit begin

	XdmfError::setLevelLimit(XdmfError::FATAL);

	//#setLevelLimit end

	//#setBuffer begin

	XdmfError::setBuffer(std::cout.rdbuf());
	//the default buffer is cout

	//#setBuffer end

	//#message begin

	try
	{
		XdmfError::message(XdmfError::FATAL, "this is an example error");
	}
	catch (XdmfError exampleError)
	{
		char * errorOutput = exampleError.what();
	}

	//#message end

	return 0;
}
