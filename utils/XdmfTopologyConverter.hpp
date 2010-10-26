#ifndef XDMFTOPOLOGYCONVERTER_HPP_
#define XDMFTOPOLOGYCONVERTER_HPP_

// Forward Declarations
class XdmfGridUnstructured;
class XdmfHDF5Writer;
class XdmfTopologyType;

// Includes
#include <boost/shared_ptr.hpp>

/*!
 * @brief XdmfTopologyConverter converts an XdmfGridUnstructured to different topology types.  XdmfAttributes and XdmfSets attached to the XdmfGrid
 * are adjusted to remain valid for the new topology type.
 *
 * When converting from a lower order topology to a higher order topology type (e.g. Hexahedron to Hexahedron_64) additional points are added to the mesh, no additional elements are added.
 * When converting from a higher order topology to a lower order topology type (e.h. Hexahedron_64 to Hexahedron) elements are tesselated to form the new topology, no additional points are added.
 *
 * Currently supported conversions:
 * 	Hexahedron to Hexahedron_64
 * 	Hexahedron to Hexahedron_64_Spectral
 * 	Hexahedron to Hexahedron_125
 * 	Hexahedron to Hexahedron_125_Spectral
 * 	Hexahedron_64 to Hexahedron
 * 	Hexahedron_125 to Hexahedron
 */
class XdmfTopologyConverter {

public:

	/**
	 * Create a new XdmfTopologyConverter.
	 *
	 * @return constructed XdmfTopologyConverter.
	 */
	static boost::shared_ptr<XdmfTopologyConverter> New();

	virtual ~XdmfTopologyConverter();

	/**
	 * Converts an XdmfGridUnstructured to a different topology type
	 *
	 * @param gridToConvert the XdmfGridUnstructured to convert to a different topology
	 * @param topologyType the XdmfTopologyType to convert to.
	 * @param heavyDataWriter an XdmfHDF5Writer to write the converted mesh to.  If no heavyDataWriter is specified, all mesh data will remain in memory.
	 *
	 * @return the converted XdmfGridUnstructured.
	 */
	boost::shared_ptr<XdmfGridUnstructured> convert(const boost::shared_ptr<XdmfGridUnstructured> gridToConvert, const boost::shared_ptr<const XdmfTopologyType> topologyType,
			const boost::shared_ptr<XdmfHDF5Writer> heavyDataWriter = boost::shared_ptr<XdmfHDF5Writer>()) const;

protected:

	XdmfTopologyConverter();

private:

	/**
	 * PIMPL
	 */
	class XdmfTopologyConverterImpl;

	XdmfTopologyConverter(const XdmfTopologyConverter & converter);  // Not implemented.
	void operator=(const XdmfTopologyConverter & converter);  // Not implemented.
};

#endif /* XDMFPARTITIONER_HPP_ */
