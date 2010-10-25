#ifndef XDMFHEAVYDATAWRITER_HPP_
#define XDMFHEAVYDATAWRITER_HPP_

// Forward Declarations
class XdmfArray;

// Includes
#include <string>
#include "XdmfVisitor.hpp"

/**
 * @brief Traverses the Xdmf graph and writes heavy data stored in XdmfArrays to heavy data files on disk.
 * This is an abstract base class to support the writing of different heavy data formats.
 *
 * XdmfHeavyDataWriter traverses an Xdmf graph structure and writes data stored in XdmfArrays to heavy data files
 * on disk. Writing begins by calling the accept() operation on any XdmfItem and supplying this writer
 * as the parameter.  The writer will write all XdmfArrays under the XdmfItem to a heavy data file on
 * disk.  It will also attach a XdmfHeavyDataController to all XdmfArrays that it writes to disk.
 *
 * There are three modes of operation for this writer:
 * 	Default - All initialized XdmfArrays are written to new heavy datasets regardless of whether they
 * 		are attached to another heavy dataset on disk via an XdmfHeavyDataController.
 * 	Overwrite - If an initialized XdmfArray is attached to an heavy dataset via an XdmfHeavyDataController the writer
 * 		will write values to that location, overwriting all previous written values.
 *  Append - If an initialized XdmfArray is attached to an heavy dataset via an XdmfHeavyDataController the writer
 *  	will append the values to the end of the dataset on disk.
 */
class XdmfHeavyDataWriter : public XdmfVisitor,
	public Loki::Visitor<XdmfArray> {

public:

	enum Mode {
		Default, Overwrite, Append
	};

	virtual ~XdmfHeavyDataWriter() = 0;

	/**
	 * Get the path to the heavy data file on disk this writer is writing to.
	 *
	 * @return a std::string containing the path to the heavy file on disk this writer is writing to.
	 */
	std::string getFilePath() const;

	/**
	 * Get the Mode of operation for this writer.
	 *
	 * @return the Mode of operation for this writer.
	 */
	Mode getMode() const;

	/**
	 * Set the mode of operation for this writer.
	 *
	 * @param mode the Mode of operation for this writer.
	 */
	void setMode(const Mode mode);

	/**
	 * Write an XdmfArray to heavy data file on disk.
	 *
	 * @param array an XdmfArray to write to heavy data.
	 * @param visitor a smart pointer to this visitor --- aids in grid traversal.
	 */
	virtual void visit(XdmfArray & array, const boost::shared_ptr<XdmfBaseVisitor> visitor) = 0;

protected:

	XdmfHeavyDataWriter(const std::string & filePath);

	std::string mFilePath;
	Mode mMode;

private:

	XdmfHeavyDataWriter(const XdmfHeavyDataWriter & heavyDataWriter);  // Not implemented.
	void operator=(const XdmfHeavyDataWriter & heavyDataWriter);  // Not implemented.

};

#endif /* XDMFHEAVYDATAWRITER_HPP_ */
