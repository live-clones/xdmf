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

%template(XdmfGeometryPtr) boost::shared_ptr<XdmfGeometry>;
%template(XdmfGridPtr) boost::shared_ptr<XdmfGrid>;
%template(XdmfItemPtr) boost::shared_ptr<XdmfItem>;
%template(XdmfObjPtr) boost::shared_ptr<XdmfObject>;
%template(XdmfTopologyPtr) boost::shared_ptr<XdmfTopology>;
%template(XdmfVisitorPtr) boost::shared_ptr<XdmfVisitor>;

%module Xdmf
%{
  #include <XdmfGeometry.hpp>
  #include <XdmfGrid.hpp>
  #include <XdmfItem.hpp>
  #include <XdmfObject.hpp>
  #include <XdmfTopology.hpp>
  #include <XdmfVisitor.hpp>
%}

%include XdmfObject.hpp

%include XdmfItem.hpp
%include XdmfVisitor.hpp

%include XdmfGeometry.hpp
%include XdmfGrid.hpp
%include XdmfTopology.hpp
