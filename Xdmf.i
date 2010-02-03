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
%template(XdmfObjPtr) boost::shared_ptr<XdmfObject>;
%template(XdmfTopologyPtr) boost::shared_ptr<XdmfTopology>;

%module Xdmf
%{
        #include <XdmfGeometry.hpp>
        #include <XdmfGrid.hpp>
        #include <XdmfObject.hpp>
        #include <XdmfTopology.hpp>
%}

%include XdmfObject.hpp

%include XdmfGeometry.hpp
%include XdmfGrid.hpp
%include XdmfTopology.hpp
