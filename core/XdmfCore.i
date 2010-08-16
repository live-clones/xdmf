/*
XdmfCorePython.cpp:
swig -v -c++ -python -o XdmfCorePython.cpp XdmfCore.i
*/

%module XdmfCore
%{
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
%}

%include boost_shared_ptr.i
%include std_string.i
%include std_vector.i
%include loki/Visitor.h

// Shared Pointer Templates
%shared_ptr(XdmfArray)
%shared_ptr(XdmfArrayType)
%shared_ptr(XdmfBaseVisitor)
%shared_ptr(XdmfCoreItemFactory)
%shared_ptr(XdmfCoreReader)
%shared_ptr(XdmfHDF5Controller)
%shared_ptr(XdmfHDF5Writer)
%shared_ptr(XdmfInformation)
%shared_ptr(XdmfItem)
%shared_ptr(XdmfItemProperty)
%shared_ptr(XdmfVisitor)
%shared_ptr(XdmfWriter)

// Abstract Base Classes
%template() Loki::BaseVisitable<void>;
%template() Loki::Visitor<XdmfArray>;
%template() Loki::Visitor<XdmfItem>;

#ifdef SWIGJAVA
%ignore XdmfArray::ItemTag;
%ignore XdmfInformation::ItemTag;
%ignore XdmfDataItem::getArray() const;
%ignore XdmfArray::getHDF5Controller() const;
%ignore XdmfArray::getValuesPointer() const;
%ignore XdmfWriter::getHDF5Writer() const;

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("XdmfCoreJava");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load for XdmfCoreJava\n" + e);
      System.exit(1);
    }
  }
%}
#endif

%include XdmfItem.hpp
%include XdmfItemProperty.hpp
%include XdmfVisitor.hpp

%include XdmfCoreItemFactory.hpp
%include XdmfCoreReader.hpp
%include XdmfInformation.hpp
%include XdmfHDF5Controller.hpp
%include XdmfHDF5Writer.hpp
%include XdmfWriter.hpp

%include XdmfArray.hpp
%include XdmfArrayType.hpp

%template(getValueAsInt8) XdmfArray::getValue<char>;
%template(getValueAsInt16) XdmfArray::getValue<short>;
%template(getValueAsInt32) XdmfArray::getValue<int>;
%template(getValueAsInt64) XdmfArray::getValue<long>;
%template(getValueAsFloat32) XdmfArray::getValue<float>;
%template(getValueAsFloat64) XdmfArray::getValue<double>;
%template(getValueAsUInt8) XdmfArray::getValue<unsigned char>;
%template(getValueAsUInt16) XdmfArray::getValue<unsigned short>;
%template(getValueAsUInt32) XdmfArray::getValue<unsigned int>;

%template(pushBackAsInt8) XdmfArray::pushBack<char>;
%template(pushBackAsInt16) XdmfArray::pushBack<short>;
%template(pushBackAsInt32) XdmfArray::pushBack<int>;
%template(pushBackAsInt64) XdmfArray::pushBack<long>;
%template(pushBackAsFloat32) XdmfArray::pushBack<float>;
%template(pushBackAsFloat64) XdmfArray::pushBack<double>;
%template(pushBackAsUInt8) XdmfArray::pushBack<unsigned char>;
%template(pushBackAsUInt16) XdmfArray::pushBack<unsigned short>;
%template(pushBackAsUInt32) XdmfArray::pushBack<unsigned int>;

%template(resizeAsInt8) XdmfArray::resize<char>;
%template(resizeAsInt16) XdmfArray::resize<short>;
%template(resizeAsInt32) XdmfArray::resize<int>;
%template(resizeAsInt64) XdmfArray::resize<long>;
%template(resizeAsFloat32) XdmfArray::resize<float>;
%template(resizeAsFloat64) XdmfArray::resize<double>;
%template(resizeAsUInt8) XdmfArray::resize<unsigned char>;
%template(resizeAsUInt16) XdmfArray::resize<unsigned short>;
%template(resizeAsUInt32) XdmfArray::resize<unsigned int>;

#ifndef SWIGJAVA
// Provide accessors from python lists to XdmfArrays
%extend XdmfArray {
	void copyValueAsInt8(int index, char value) {
		$self->insert(index, &value);
	}

	void copyValueAsInt16(int index, short value) {
		$self->insert(index, &value);
	}

	void copyValueAsInt32(int index, int value) {
		$self->insert(index, &value);
	}

	void copyValueAsInt64(int index, long value) {
		$self->insert(index, &value);
	}

	void copyValueAsFloat32(int index, float value) {
		$self->insert(index, &value);
	}

	void copyValueAsFloat64(int index, double value) {
		$self->insert(index, &value);
	}

	void copyValueAsUInt8(int index, unsigned char value) {
		$self->insert(index, &value);
	}

	void copyValueAsUInt16(int index, unsigned short value) {
		$self->insert(index, &value);
	}

	void copyValueAsUInt32(int index, unsigned int value) {
		$self->insert(index, &value);
	}
};

%extend XdmfArray {
	%pythoncode {
		def insertAsInt8(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsInt8(i+startIndex, values[i])

		def insertAsInt16(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsInt16(i+startIndex, values[i])

		def insertAsInt32(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsInt32(i+startIndex, values[i])

		def insertAsInt64(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsInt64(i+startIndex, values[i])

		def insertAsFloat32(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsFloat32(i+startIndex, values[i])

		def insertAsFloat64(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsFloat64(i+startIndex, values[i])

		def insertAsUInt8(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsUInt8(i+startIndex, values[i])

		def insertAsUInt16(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsUInt16(i+startIndex, values[i])

		def insertAsUInt32(self, startIndex, values):
			for i in range(0, len(values)):
				self.copyValueAsUInt32(i+startIndex, values[i])
	};
};
#endif /* SWIGJAVA */
