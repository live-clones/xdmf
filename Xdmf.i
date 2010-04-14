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

// Provide accessors from python lists to XdmfArrays
%extend XdmfArray {
  void copyValueAsChar(int index, char value) {
    $self->copyValues(index, &value);
  }

  void copyValueAsShort(int index, short value) {
    $self->copyValues(index, &value);
  }

  void copyValueAsInt(int index, int value) {
    $self->copyValues(index, &value);
  }

  void copyValueAsLong(int index, long value) {
    $self->copyValues(index, &value);
  }

  void copyValueAsFloat(int index, float value) {
    $self->copyValues(index, &value);
  }
  
  void copyValueAsDouble(int index, double value) {
    $self->copyValues(index, &value);
  }

  void copyValueAsUChar(int index, unsigned char value) {
    $self->copyValues(index, &value);
  }

  void copyValueAsUShort(int index, unsigned short value) {
    $self->copyValues(index, &value);
  }

  void copyValueAsUInt(int index, unsigned int value) {
    $self->copyValues(index, &value);
  }
};

%extend boost::shared_ptr<XdmfArray> {
  %pythoncode {
    def copyValuesAsChar(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsChar(i+startIndex, values[i])

    def copyValuesAsShort(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsShort(i+startIndex, values[i])

    def copyValuesAsInt(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsInt(i+startIndex, values[i])

    def copyValuesAsLong(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsLong(i+startIndex, values[i])

    def copyValuesAsFloat(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsFloat(i+startIndex, values[i])

    def copyValuesAsDouble(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsDouble(i+startIndex, values[i])

    def copyValuesAsUChar(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsUChar(i+startIndex, values[i])

    def copyValuesAsUShort(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsUShort(i+startIndex, values[i])

    def copyValuesAsUInt(self, startIndex, values):
      for i in range(0, len(values)):
        self.copyValueAsUInt(i+startIndex, values[i])
  };
};
