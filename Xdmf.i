/*
XdmfPython.cpp:
swig -v -c++ -python -o XdmfPython.cpp Xdmf.i
*/

%module Xdmf
%{
	// XdmfCore Includes
	#include <XdmfArray.hpp>
	#include <XdmfArrayType.hpp>
	#include <XdmfCoreItemFactory.hpp>
	#include <XdmfCoreReader.hpp>
	#include <XdmfDataItem.hpp>
	#include <XdmfHDF5Controller.hpp>
	#include <XdmfHDF5Writer.hpp>
	#include <XdmfItem.hpp>
	#include <XdmfItemProperty.hpp>
	#include <XdmfObject.hpp>
	#include <XdmfVisitor.hpp>
	#include <XdmfWriter.hpp>

	// Xdmf Includes
	#include <XdmfAttribute.hpp>
	#include <XdmfAttributeCenter.hpp>
	#include <XdmfAttributeType.hpp>
	#include <XdmfDomain.hpp>
	#include <XdmfGeometry.hpp>
	#include <XdmfGeometryType.hpp>
	#include <XdmfGrid.hpp>
	#include <XdmfGridCollection.hpp>
	#include <XdmfGridCollectionType.hpp>
	#include <XdmfItemFactory.hpp>
	#include <XdmfReader.hpp>
	#include <XdmfSet.hpp>
	#include <XdmfSetType.hpp>
	#include <XdmfTopology.hpp>
	#include <XdmfTopologyType.hpp>
%}

%import XdmfCore.i

%pythoncode {
	from XdmfCore import *
}

%typemap(out) boost::shared_ptr<XdmfItem> {
	if(boost::shared_ptr<XdmfAttribute> attribute = boost::shared_dynamic_cast<XdmfAttribute>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfAttribute>(attribute)), SWIGTYPE_p_boost__shared_ptrT_XdmfAttribute_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfDomain> domain = boost::shared_dynamic_cast<XdmfDomain>($1))
	{
		boost::shared_ptr<XdmfDomain> * newDomain = new boost::shared_ptr<XdmfDomain>(domain);
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(newDomain), SWIGTYPE_p_boost__shared_ptrT_XdmfDomain_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfGeometry> geometry = boost::shared_dynamic_cast<XdmfGeometry>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfGeometry>(geometry)), SWIGTYPE_p_boost__shared_ptrT_XdmfGeometry_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfGrid> grid = boost::shared_dynamic_cast<XdmfGrid>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfGrid>(grid)), SWIGTYPE_p_boost__shared_ptrT_XdmfGrid_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfGridCollection> gridCollection = boost::shared_dynamic_cast<XdmfGridCollection>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfGridCollection>(gridCollection)), SWIGTYPE_p_boost__shared_ptrT_XdmfGridCollection_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfSet> set = boost::shared_dynamic_cast<XdmfSet>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfSet>(set)), SWIGTYPE_p_boost__shared_ptrT_XdmfSet_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfTopology> topology = boost::shared_dynamic_cast<XdmfTopology>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfTopology>(topology)), SWIGTYPE_p_boost__shared_ptrT_XdmfTopology_t, SWIG_POINTER_OWN);
	}
	else
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfItem>($1)), SWIGTYPE_p_boost__shared_ptrT_XdmfItem_t, SWIG_POINTER_OWN);
	}
}

// Shared Pointer Templates
%shared_ptr(XdmfAttribute)
%shared_ptr(XdmfAttributeCenter)
%shared_ptr(XdmfAttributeType)
%shared_ptr(XdmfDomain)
%shared_ptr(XdmfGeometry)
%shared_ptr(XdmfGeometryType)
%shared_ptr(XdmfGrid)
%shared_ptr(XdmfGridCollection)
%shared_ptr(XdmfGridCollectionType)
%shared_ptr(XdmfItemFactory)
%shared_ptr(XdmfReader)
%shared_ptr(XdmfSet)
%shared_ptr(XdmfSetType)
%shared_ptr(XdmfTopology)
%shared_ptr(XdmfTopologyType)

%include XdmfAttribute.hpp
%include XdmfAttributeCenter.hpp
%include XdmfAttributeType.hpp
%include XdmfDomain.hpp
%include XdmfGeometry.hpp
%include XdmfGeometryType.hpp
%include XdmfGrid.hpp
%include XdmfGridCollection.hpp
%include XdmfGridCollectionType.hpp
%include XdmfItemFactory.hpp
%include XdmfReader.hpp
%include XdmfSet.hpp
%include XdmfSetType.hpp
%include XdmfTopology.hpp
%include XdmfTopologyType.hpp