/*
XdmfPython.cpp:
swig -v -c++ -python -o XdmfPython.cpp Xdmf.i
*/

%module Xdmf
%{

    // XdmfCore Includes
    #include <Xdmf.hpp>
    #include <XdmfArray.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
#ifdef XDMF_BUILD_DSM
    #include <XdmfHDF5ControllerDSM.hpp>
    #include <XdmfHDF5WriterDSM.hpp>
#endif
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
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
    #include <XdmfCurvilinearGrid.hpp>
    #include <XdmfDomain.hpp>
    #include <XdmfGeometry.hpp>
    #include <XdmfGeometryType.hpp>
    #include <XdmfGrid.hpp>
    #include <XdmfGridCollection.hpp>
    #include <XdmfGridCollectionType.hpp>
    #include <XdmfItemFactory.hpp>
    #include <XdmfMap.hpp>
    #include <XdmfReader.hpp>
    #include <XdmfRectilinearGrid.hpp>
    #include <XdmfRegularGrid.hpp>
    #include <XdmfSet.hpp>
    #include <XdmfSetType.hpp>
    #include <XdmfTime.hpp>
    #include <XdmfTopology.hpp>
    #include <XdmfTopologyType.hpp>
    #include <XdmfUnstructuredGrid.hpp>
%}

%import XdmfCore.i

#ifdef SWIGJAVA

// Typemaps that work for Java

%typemap(out)  boost::shared_ptr<XdmfItem> {
    if(boost::shared_ptr<XdmfAttribute> value = boost::shared_dynamic_cast<XdmfAttribute>($1)) {
        *(boost::shared_ptr< XdmfAttribute > **)&($result) = value ? new boost::shared_ptr< XdmfAttribute >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfCurvilinearGrid> value = boost::shared_dynamic_cast<XdmfCurvilinearGrid>($1)) {
        *(boost::shared_ptr< XdmfCurvilinearGrid > **)&($result) = value ? new boost::shared_ptr< XdmfCurvilinearGrid >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfDomain> value = boost::shared_dynamic_cast<XdmfDomain>($1)) {
        *(boost::shared_ptr< XdmfDomain > **)&($result) = value ? new boost::shared_ptr< XdmfDomain >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfGeometry> value = boost::shared_dynamic_cast<XdmfGeometry>($1)) {
        *(boost::shared_ptr< XdmfGeometry > **)&($result) = value ? new boost::shared_ptr< XdmfGeometry >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfGridCollection> value = boost::shared_dynamic_cast<XdmfGridCollection>($1)) {
        *(boost::shared_ptr< XdmfGridCollection > **)&($result) = value ? new boost::shared_ptr< XdmfGridCollection >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfInformation> value = boost::shared_dynamic_cast<XdmfInformation>($1)) {
        *(boost::shared_ptr< XdmfInformation > **)&($result) = value ? new boost::shared_ptr< XdmfInformation >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfRectilinearGrid> value = boost::shared_dynamic_cast<XdmfRectilinearGrid>($1)) {
        *(boost::shared_ptr< XdmfRectilinearGrid > **)&($result) = value ? new boost::shared_ptr< XdmfRectilinearGrid >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfRegularGrid> value = boost::shared_dynamic_cast<XdmfRegularGrid>($1)) {
        *(boost::shared_ptr< XdmfRegularGrid > **)&($result) = value ? new boost::shared_ptr< XdmfRegularGrid >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfSet> value = boost::shared_dynamic_cast<XdmfSet>($1)) {
        *(boost::shared_ptr< XdmfSet > **)&($result) = value ? new boost::shared_ptr< XdmfSet >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfTime> value = boost::shared_dynamic_cast<XdmfTime>($1)) {
        *(boost::shared_ptr< XdmfTime > **)&($result) = value ? new boost::shared_ptr< XdmfTime >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfTopology> value = boost::shared_dynamic_cast<XdmfTopology>($1)) {
        *(boost::shared_ptr< XdmfTopology > **)&($result) = value ? new boost::shared_ptr< XdmfTopology >(value) : 0;
    }
    else if(boost::shared_ptr<XdmfUnstructuredGrid> value = boost::shared_dynamic_cast<XdmfUnstructuredGrid>($1)) {
        *(boost::shared_ptr< XdmfUnstructuredGrid > **)&($result) = value ? new boost::shared_ptr< XdmfUnstructuredGrid >(value) : 0;
    }
    else {
        *(boost::shared_ptr< XdmfItem > **)&($result) = &($1);
    }
}

// Ignore const overloaded methods
%ignore XdmfCurvilinearGrid::getDimensions() const;
%ignore XdmfDomain::getCurvilinearGrid(const unsigned int) const;
%ignore XdmfDomain::getCurvilinearGrid(const std::string &) const;
%ignore XdmfDomain::getGridCollection(const unsigned int) const;
%ignore XdmfDomain::getGridCollection(const std::string &) const;
%ignore XdmfDomain::getRectilinearGrid(const unsigned int) const;
%ignore XdmfDomain::getRectilinearGrid(const std::string &) const;
%ignore XdmfDomain::getRegularGrid(const unsigned int) const;
%ignore XdmfDomain::getRegularGrid(const std::string &) const;
%ignore XdmfDomain::getUnstructuredGrid(const unsigned int) const;
%ignore XdmfDomain::getUnstructuredGrid(const std::string &) const;
%ignore XdmfGrid::getAttribute(const unsigned int) const;
%ignore XdmfGrid::getAttribute(const std::string &) const;
%ignore XdmfGrid::getMap() const;
%ignore XdmfGrid::getSet(const unsigned int) const;
%ignore XdmfGrid::getSet(const std::string &) const;
%ignore XdmfGrid::getTime() const;
%ignore XdmfRectilinearGrid::getCoordinates(const unsigned int) const;
%ignore XdmfRectilinearGrid::getCoordinates() const;
%ignore XdmfRectilinearGrid::getDimensions() const;
%ignore XdmfRegularGrid::getBrickSize() const;
%ignore XdmfRegularGrid::getDimensions() const;
%ignore XdmfRegularGrid::getOrigin() const;
%ignore XdmfSet::getAttribute(const unsigned int) const;
%ignore XdmfSet::getAttribute(const std::string &) const;

// Ignore ItemTags
%ignore XdmfAttribute::ItemTag;
%ignore XdmfCurvilinearGrid::ItemTag;
%ignore XdmfDomain::ItemTag;
%ignore XdmfGeometry::ItemTag;
%ignore XdmfGrid::ItemTag;
%ignore XdmfGridCollection::ItemTag;
%ignore XdmfMap::ItemTag;
%ignore XdmfRectilinearGrid::ItemTag;
%ignore XdmfRegularGrid::ItemTag;
%ignore XdmfSet::ItemTag;
%ignore XdmfTime::ItemTag;
%ignore XdmfTopology::ItemTag;
%ignore XdmfUnstructuredGrid::ItemTag;

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("XdmfCoreJava");
        }
        catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load for" +
                                "XdmfCoreJava\n" + e);
            System.exit(1);
        }
        try {
            System.loadLibrary("XdmfJava");
        }
        catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load for" +
                                "XdmfJava\n" + e);
            System.exit(1);
        }
    }
%}

#endif */ SWIGJAVA */

#ifdef SWIGPYTHON

%pythoncode {
    from XdmfCore import *
}

%typemap(out) boost::shared_ptr<XdmfItem> {
    if(boost::shared_ptr<XdmfAttribute> attribute = boost::shared_dynamic_cast<XdmfAttribute>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfAttribute>(attribute)), SWIGTYPE_p_boost__shared_ptrT_XdmfAttribute_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfCurvilinearGrid> gridCurvilinear = boost::shared_dynamic_cast<XdmfCurvilinearGrid>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfCurvilinearGrid>(gridCurvilinear)), SWIGTYPE_p_boost__shared_ptrT_XdmfCurvilinearGrid_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfDomain> domain = boost::shared_dynamic_cast<XdmfDomain>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfDomain>(domain)), SWIGTYPE_p_boost__shared_ptrT_XdmfDomain_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfGeometry> geometry = boost::shared_dynamic_cast<XdmfGeometry>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfGeometry>(geometry)), SWIGTYPE_p_boost__shared_ptrT_XdmfGeometry_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfGridCollection> gridCollection = boost::shared_dynamic_cast<XdmfGridCollection>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfGridCollection>(gridCollection)), SWIGTYPE_p_boost__shared_ptrT_XdmfGridCollection_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfInformation> information = boost::shared_dynamic_cast<XdmfInformation>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfInformation>(information)), SWIGTYPE_p_boost__shared_ptrT_XdmfInformation_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfMap> map = boost::shared_dynamic_cast<XdmfMap>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfMap>(map)), SWIGTYPE_p_boost__shared_ptrT_XdmfMap_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfRectilinearGrid> gridRectilinear = boost::shared_dynamic_cast<XdmfRectilinearGrid>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfRectilinearGrid>(gridRectilinear)), SWIGTYPE_p_boost__shared_ptrT_XdmfRectilinearGrid_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfRegularGrid> gridRegular = boost::shared_dynamic_cast<XdmfRegularGrid>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfRegularGrid>(gridRegular)), SWIGTYPE_p_boost__shared_ptrT_XdmfRegularGrid_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfSet> set = boost::shared_dynamic_cast<XdmfSet>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfSet>(set)), SWIGTYPE_p_boost__shared_ptrT_XdmfSet_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfTime> time = boost::shared_dynamic_cast<XdmfTime>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfTime>(time)), SWIGTYPE_p_boost__shared_ptrT_XdmfTime_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfTopology> topology = boost::shared_dynamic_cast<XdmfTopology>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfTopology>(topology)), SWIGTYPE_p_boost__shared_ptrT_XdmfTopology_t, SWIG_POINTER_OWN);
    }
    else if(boost::shared_ptr<XdmfUnstructuredGrid> gridUnstructured = boost::shared_dynamic_cast<XdmfUnstructuredGrid>($1)) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfUnstructuredGrid>(gridUnstructured)), SWIGTYPE_p_boost__shared_ptrT_XdmfUnstructuredGrid_t, SWIG_POINTER_OWN);
    }
    else {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr(new boost::shared_ptr<XdmfItem>($1)), SWIGTYPE_p_boost__shared_ptrT_XdmfItem_t, SWIG_POINTER_OWN);
    }
}

#endif /* SWIGPYTHON */

// Shared Pointer Templates
%shared_ptr(XdmfAttribute)
%shared_ptr(XdmfAttributeCenter)
%shared_ptr(XdmfAttributeType)
%shared_ptr(XdmfCurvilinearGrid)
%shared_ptr(XdmfDomain)
%shared_ptr(XdmfGeometry)
%shared_ptr(XdmfGeometryType)
%shared_ptr(XdmfGrid)
%shared_ptr(XdmfGridCollection)
%shared_ptr(XdmfGridCollectionType)
%shared_ptr(XdmfItemFactory)
%shared_ptr(XdmfMap)
%shared_ptr(XdmfReader)
%shared_ptr(XdmfRectilinearGrid)
%shared_ptr(XdmfRegularGrid)
%shared_ptr(XdmfSet)
%shared_ptr(XdmfSetType)
%shared_ptr(XdmfTime)
%shared_ptr(XdmfTopology)
%shared_ptr(XdmfTopologyType)
%shared_ptr(XdmfUnstructuredGrid)

%include Xdmf.hpp
%include XdmfGrid.hpp

%include XdmfAttribute.hpp
%include XdmfAttributeCenter.hpp
%include XdmfAttributeType.hpp
%include XdmfCurvilinearGrid.hpp
%include XdmfDomain.hpp
%include XdmfGeometry.hpp
%include XdmfGeometryType.hpp
%include XdmfGridCollection.hpp
%include XdmfGridCollectionType.hpp
%include XdmfItemFactory.hpp
%include XdmfMap.hpp
%include XdmfReader.hpp
%include XdmfRectilinearGrid.hpp
%include XdmfRegularGrid.hpp
%include XdmfSet.hpp
%include XdmfSetType.hpp
%include XdmfTime.hpp
%include XdmfTopology.hpp
%include XdmfTopologyType.hpp
%include XdmfUnstructuredGrid.hpp
