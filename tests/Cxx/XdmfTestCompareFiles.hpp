#include <fstream>
#include <sstream>

class XdmfTestCompareFiles {
public:

	/**
	 * Compares two files on disk for equality.  If they are equal it returns true.
	 */
	static bool compareFiles(const std::string & firstFileName, const std::string & secondFileName)
	{
		// Compare two files for equality
		std::ifstream firstFile(firstFileName.c_str());
		std::ifstream secondFile(secondFileName.c_str());

		std::stringstream firstBuffer;
		std::stringstream secondBuffer;

		firstBuffer << firstFile.rdbuf();
		secondBuffer << secondFile.rdbuf();

		std::string firstContents(firstBuffer.str());
		std::string secondContents(secondBuffer.str());

		return(firstContents.compare(secondContents) == 0);
	}

};

