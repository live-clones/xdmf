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
	#include <XdmfHDF5Controller.hpp>
	#include <XdmfHDF5Writer.hpp>
	#include <XdmfInformation.hpp>
	#include <XdmfItem.hpp>
	#include <XdmfItemProperty.hpp>
	#include <XdmfSystemUtils.hpp>
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
	#include <XdmfMap.hpp>
	#include <XdmfReader.hpp>
	#include <XdmfSet.hpp>
	#include <XdmfSetType.hpp>
	#include <XdmfTime.hpp>
	#include <XdmfTopology.hpp>
	#include <XdmfTopologyType.hpp>
%}

%import XdmfCore.i

#ifdef SWIGJAVA
    %extend XdmfAttributeCenter {
        bool equals(boost::shared_ptr<XdmfAttributeCenter> attributeCenter) {
            return(self->IsEqual(attributeCenter));
        }
    };
    %extend XdmfAttributeType {
        bool equals(boost::shared_ptr<XdmfAttributeType> attributeType) {
            return(self->IsEqual(attributeType));
        }
    };
    %extend XdmfGeometryType {
        bool equals(boost::shared_ptr<XdmfGeometryType> geometryType) {
            return(self->IsEqual(geometryType));
        }
    };
    %extend XdmfGridCollectionType {
        bool equals(boost::shared_ptr<XdmfGridCollectionType> collectionType) {
            return(self->IsEqual(collectionType));
        }
    };
    %extend XdmfSetType {
        bool equals(boost::shared_ptr<XdmfSetType> setType) {
            return(self->IsEqual(setType));
        }
    };
    %extend XdmfTopologyType {
        bool equals(boost::shared_ptr<XdmfTopologyType> topologyType) {
            return(self->IsEqual(topologyType));
        }
    };
    %ignore XdmfTopologyType::operator==(const XdmfTopologyType & topologyType) const;
    %ignore XdmfTopologyType::operator!=(const XdmfTopologyType & topologyType) const;
    %ignore XdmfSetType::operator==(const XdmfSetType & setType) const;
    %ignore XdmfSetType::operator!=(const XdmfSetType & setType) const;
    %ignore XdmfGridCollectionType::operator==(const XdmfGridCollectionType & collectionType) const;
    %ignore XdmfGridCollectionType::operator!=(const XdmfGridCollectionType & collectionType) const;
    %ignore XdmfAttributeCenter::operator==(const XdmfAttributeCenter & attributeCenter) const;
    %ignore XdmfAttributeCenter::operator!=(const XdmfAttributeCenter & attributeCenter) const;
    %ignore XdmfGeometryType::operator==(const XdmfGeometryType & geometryType) const;
    %ignore XdmfGeometryType::operator!=(const XdmfGeometryType & geometryType) const;
    %ignore XdmfAttributeType::operator==(const XdmfAttributeType & attributeType) const;
    %ignore XdmfAttributeType::operator!=(const XdmfAttributeType & attributeType) const;

    %rename(getAttributeUIntConst) XdmfSet::getAttribute(unsigned int const) const;
    %rename(getAttributeStrConst) XdmfSet::getAttribute(std::string const &) const;
#endif


#ifdef SWIGPYTHON
%extend XdmfAttributeCenter {
  bool __eq__(boost::shared_ptr<XdmfAttributeCenter> attributeCenter) {
    return(self->IsEqual(attributeCenter));
  }
};
%extend XdmfAttributeType {
  bool __eq__(boost::shared_ptr<XdmfAttributeType> attributeType) {
    return(self->IsEqual(attributeType));
  }
};
%extend XdmfGeometryType {
  bool __eq__(boost::shared_ptr<XdmfGeometryType> geometryType) {
    return(self->IsEqual(geometryType));
  }
};
%extend XdmfGridCollectionType {
  bool __eq__(boost::shared_ptr<XdmfGridCollectionType> collectionType) {
    return(self->IsEqual(collectionType));
  }
};
%extend XdmfSetType {
  bool __eq__(boost::shared_ptr<XdmfSetType> setType) {
    return(self->IsEqual(setType));
  }
};
%extend XdmfTopologyType {
  bool __eq__(boost::shared_ptr<XdmfTopologyType> topologyType) {
    return(self->IsEqual(topologyType));
  }
};
#endif


#ifdef SWIGPYTHON
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
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfDomain>(domain)), SWIGTYPE_p_boost__shared_ptrT_XdmfDomain_t, SWIG_POINTER_OWN);
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
	else if(boost::shared_ptr<XdmfInformation> information = boost::shared_dynamic_cast<XdmfInformation>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfInformation>(information)), SWIGTYPE_p_boost__shared_ptrT_XdmfInformation_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfMap> map = boost::shared_dynamic_cast<XdmfMap>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfMap>(map)), SWIGTYPE_p_boost__shared_ptrT_XdmfMap_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfSet> set = boost::shared_dynamic_cast<XdmfSet>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfSet>(set)), SWIGTYPE_p_boost__shared_ptrT_XdmfSet_t, SWIG_POINTER_OWN);
	}
	else if(boost::shared_ptr<XdmfTime> time = boost::shared_dynamic_cast<XdmfTime>($1))
	{
		$result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfTime>(time)), SWIGTYPE_p_boost__shared_ptrT_XdmfTime_t, SWIG_POINTER_OWN);
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
#endif

#if 0
%include std_set.i
%template(std_set_uint) std::set<unsigned int>;

// Abstract Base Classes
%template() Loki::Visitor<XdmfSet>;
#endif

#ifdef SWIGJAVA

// Typemaps that work for Java

%typemap(out)  boost::shared_ptr<XdmfItem> {
        if(boost::shared_ptr<XdmfAttribute> value = boost::shared_dynamic_cast<XdmfAttribute>($1))
        {
                *(boost::shared_ptr< XdmfAttribute > **)&($result) = value ? new boost::shared_ptr< XdmfAttribute >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfDomain> value = boost::shared_dynamic_cast<XdmfDomain>($1))
        {
                *(boost::shared_ptr< XdmfDomain > **)&($result) = value ? new boost::shared_ptr< XdmfDomain >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfGeometry> value = boost::shared_dynamic_cast<XdmfGeometry>($1))
        {
                *(boost::shared_ptr< XdmfGeometry > **)&($result) = value ? new boost::shared_ptr< XdmfGeometry >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfGrid> value = boost::shared_dynamic_cast<XdmfGrid>($1))
        {
                *(boost::shared_ptr< XdmfGrid > **)&($result) = value ? new boost::shared_ptr< XdmfGrid >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfGridCollection> value = boost::shared_dynamic_cast<XdmfGridCollection>($1))
        {
                *(boost::shared_ptr< XdmfGridCollection > **)&($result) = value ? new boost::shared_ptr< XdmfGridCollection >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfInformation> value = boost::shared_dynamic_cast<XdmfInformation>($1))
        {
                *(boost::shared_ptr< XdmfInformation > **)&($result) = value ? new boost::shared_ptr< XdmfInformation >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfSet> value = boost::shared_dynamic_cast<XdmfSet>($1))
        {
                *(boost::shared_ptr< XdmfSet > **)&($result) = value ? new boost::shared_ptr< XdmfSet >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfTime> value = boost::shared_dynamic_cast<XdmfTime>($1))
        {
               *(boost::shared_ptr< XdmfTime > **)&($result) = value ? new boost::shared_ptr< XdmfTime >(value) : 0;
        }
        else if(boost::shared_ptr<XdmfTopology> value = boost::shared_dynamic_cast<XdmfTopology>($1))
        {
                *(boost::shared_ptr< XdmfTopology > **)&($result) = value ? new boost::shared_ptr< XdmfTopology >(value) : 0;
        }
        else
        {
                *(boost::shared_ptr< XdmfItem > **)&($result) = &($1);
        }
}

// Swig+Java will automatically create 'getter' functions for static
// variables in a class (in this case the ItemTag member variable).
// This happens to conflict with the getters that Xdmf implements to
// override the otherwise virtual functions of the parent class.  Here,
// we ask swig to ignore the static variable (thereby removing direct
// access to the variable, but leaving access to our getter function

%ignore XdmfAttribute::ItemTag;
%ignore XdmfDomain::ItemTag;
%ignore XdmfGeometry::ItemTag;
%ignore XdmfGrid::ItemTag;
%ignore XdmfGridCollection::ItemTag;
%ignore XdmfSet::ItemTag;
%ignore XdmfTime::ItemTag;
%ignore XdmfTopology::ItemTag;
%ignore XdmfMap::ItemTag;

// Swig+Java does not like 2 functions with the same prototype that
// simply return const/non-const versions of the same type.  We
// ask Swig to ignore one of the two getter functions.  We may
// have to change this to rename the function to a new name such
// that we preserve the ability to get a constant variable

%ignore XdmfDomain::getGrid(const unsigned int index) const;
%ignore XdmfDomain::getGridCollection(unsigned int const) const;
%ignore XdmfGrid::getAttribute(const unsigned int index) const;
%ignore XdmfGrid::getAttribute(const std::string & name) const;
%ignore XdmfGrid::getSet(const unsigned int index) const;
%ignore XdmfGrid::getSet(const std::string & name) const;
%ignore XdmfGrid::getGeometry() const;
%ignore XdmfGrid::getTime() const;
%ignore XdmfGrid::getTopology() const;
%ignore XdmfGridCollection::getGrid(const unsigned int index) const;
%ignore XdmfGridCollection::insert(const boost::shared_ptr<XdmfInformation> information);
%ignore XdmfGrid::getMap() const;
%ignore XdmfSet::getHDF5Controller() const;

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("XdmfCoreJava");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load for XdmfCoreJava\n" + e);
      System.exit(1);
    }
    try {
        System.loadLibrary("XdmfJava");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load for XdmfJava\n" + e);
      System.exit(1);
    }
  }
%}

#endif */ SWIGJAVA */

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
%shared_ptr(XdmfMap)
%shared_ptr(XdmfReader)
%shared_ptr(XdmfSet)
%shared_ptr(XdmfSetType)
%shared_ptr(XdmfTime)
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
%include XdmfMap.hpp
%include XdmfReader.hpp
%include XdmfSet.hpp
%include XdmfSetType.hpp
%include XdmfTime.hpp
%include XdmfTopology.hpp
%include XdmfTopologyType.hpp
