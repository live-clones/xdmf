/*
XdmfPython.cpp:
swig -v -c++ -python -o XdmfPython.cpp Xdmf.i
*/

%include std_string.i
%include std_vector.i

namespace boost {
  template<class T> class shared_ptr
  {
  public:
    T * operator-> () const;
  };
}

%template(XdmfAttributePtr) boost::shared_ptr<XdmfAttribute>;
%template(XdmfArrayPtr) boost::shared_ptr<XdmfArray>;
%template(XdmfDataItemPtr) boost::shared_ptr<XdmfDataItem>;
%template(XdmfDomainPtr) boost::shared_ptr<XdmfDomain>;
%template(XdmfGeometryPtr) boost::shared_ptr<XdmfGeometry>;
%template(XdmfGridPtr) boost::shared_ptr<XdmfGrid>;
%template(XdmfItemPtr) boost::shared_ptr<XdmfItem>;
%template(XdmfObjPtr) boost::shared_ptr<XdmfObject>;
%template(XdmfTopologyPtr) boost::shared_ptr<XdmfTopology>;
%template(XdmfVisitorPtr) boost::shared_ptr<XdmfVisitor>;

%module Xdmf
%{
  #include <XdmfArray.hpp>
  #include <XdmfAttribute.hpp>
  #include <XdmfAttributeCenter.hpp>
  #include <XdmfAttributeType.hpp>
  #include <XdmfDataItem.hpp>
  #include <XdmfDomain.hpp>
  #include <XdmfGeometry.hpp>
  #include <XdmfGeometryType.hpp>
  #include <XdmfGrid.hpp>
  #include <XdmfItem.hpp>
  #include <XdmfObject.hpp>
  #include <XdmfTopology.hpp>
  #include <XdmfTopologyType.hpp>
  #include <XdmfVisitor.hpp>
%}

%include XdmfObject.hpp

%include XdmfItem.hpp
%include XdmfVisitor.hpp

%include XdmfDataItem.hpp

%include XdmfAttribute.hpp
%include XdmfAttributeCenter.hpp
%include XdmfAttributeType.hpp
%include XdmfArray.hpp
%include XdmfDomain.hpp
%include XdmfGeometry.hpp
%include XdmfGeometryType.hpp
%include XdmfGrid.hpp
%include XdmfTopology.hpp
%include XdmfTopologyType.hpp

%template(CharVector) std::vector<char>;
%template(ShortVector) std::vector<short>;
%template(IntVector) std::vector<int>;
%template(LongVector) std::vector<long>;
%template(FloatVector) std::vector<float>;
%template(DoubleVector) std::vector<double>;
%template(UCharVector) std::vector<unsigned char>;
%template(UShortVector) std::vector<unsigned short>;
%template(UIntVector) std::vector<unsigned int>;

%extend XdmfArray {
  %template(copyValues) copyValues<char>;
  %template(copyValues) copyValues<short>;
  %template(copyValues) copyValues<int>;
  %template(copyValues) copyValues<long>;
  %template(copyValues) copyValues<float>;
  %template(copyValues) copyValues<double>;
  %template(copyValues) copyValues<unsigned char>;
  %template(copyValues) copyValues<unsigned short>;
  %template(copyValues) copyValues<unsigned int>;
};
