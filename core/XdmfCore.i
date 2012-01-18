/*
XdmfCorePython.cpp:
swig -v -c++ -python -o XdmfCorePython.cpp XdmfCore.i
*/

%module XdmfCore
%{
    #include <XdmfArray.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCore.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfError.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
#ifdef XDMF_BUILD_DSM
    #include <XdmfHDF5ControllerDSM.hpp>
    #include <XdmfHDF5WriterDSM.hpp>
#endif
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVersion.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    #include <ProjectVersion.hpp>
%}

#ifdef SWIGJAVA

// Ignore const overloaded methods
%ignore XdmfArray::getHeavyDataController() const;
%ignore XdmfArray::getValuesInternal() const;
%ignore XdmfItem::getInformation(const unsigned int) const;
%ignore XdmfItem::getInformation(const std::string &) const;
%ignore XdmfWriter::getHeavyDataWriter() const;
%ignore XdmfInformation::getArray(unsigned int const) const;
%ignore XdmfInformation::getArray(std::string const &) const;

// Ignore ItemTags
%ignore XdmfArray::ItemTag;
%ignore XdmfInformation::ItemTag;

// Define equality operators
%extend XdmfItem {

    bool equals(boost::shared_ptr<XdmfItem> item) {
        if (item == NULL) {
            return false;
        }
        return self == item.get();
    }

    bool IsEqual(boost::shared_ptr<XdmfItem> item) {
        if (item == NULL) {
            return false;
        }
        return self == item.get();
    }
};

%extend XdmfItemProperty {

    bool equals(boost::shared_ptr<XdmfItemProperty> itemProperty) {
        if (itemProperty == NULL) {
            return false;
        }
        return self == itemProperty.get();
    }

    bool IsEqual(boost::shared_ptr<XdmfItemProperty> itemProperty) {
        if (itemProperty == NULL) {
            return false;
        }
        return self == itemProperty.get();
    }

};

%typemap(javacode) XdmfArray %{
    public void insertValuesAsInt8(int index, char[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsInt8(index+i, values[i]);
    }

    public void insertValuesAsInt16(int index, short[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsInt16(index+i, values[i]);
    }

    public void insertValuesAsInt32(int index, int[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsInt32(index+i, values[i]);
    }

    public void insertValuesAsFloat32(int index, float[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsFloat32(index+i, values[i]);
    }

    public void insertValuesAsFloat64(int index, double[] values) {
        for(int i = 0; i < values.length; i++)
            this.insertValueAsFloat64(index+i, values[i]);
    }
%}

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
    }
%}

#endif /* SWIGJAVA */

#ifdef SWIGPYTHON

// Provide accessors from python lists to XdmfArrays
%extend XdmfArray {

    PyObject * getBuffer() {
        void *vp = $self->getValuesInternal();
        Py_ssize_t sz =
            $self->getSize() * $self->getArrayType()->getElementSize();
        PyObject * c = PyBuffer_FromMemory(vp, sz);
        return(c);
    }

    %pythoncode {
        def getNumpyArray(self):
            h5ctl = self.getHDF5Controller()
            if h5ctl == None :
                try :
                    from numpy import frombuffer as ___frombuffer
                except :
                    return None
                buf = self.getBuffer()
                aType = self.getArrayType()
                if aType == XdmfArrayType.Int8() :
                    return(___frombuffer(buf, 'int8'))
                if aType == XdmfArrayType.Int16() :
                    return(___frombuffer(buf, 'int16'))
                if aType == XdmfArrayType.Int32() :
                    return(___frombuffer(buf, 'int32'))
                if aType == XdmfArrayType.Int64() :
                    return(___frombuffer(buf, 'int64'))
                if aType == XdmfArrayType.Float32() :
                    return(___frombuffer(buf, 'float32'))
                if aType == XdmfArrayType.Float64() :
                    return(___frombuffer(buf, 'float64'))
                if aType == XdmfArrayType.UInt8() :
                    return(___frombuffer(buf, 'uint8'))
                if aType == XdmfArrayType.UInt16() :
                    return(___frombuffer(buf, 'uint16'))
                if aType == XdmfArrayType.UInt32() :
                    return(___frombuffer(buf, 'uint32'))
                return None
            else :
                h5FileName = h5ctl.getFilePath()
                h5DataSetName = h5ctl.getDataSetPath()
                if (h5FileName == None) | (h5DataSetName == None) :
                    return None
                try :
                    from h5py import File as ___File
                    from numpy import array as ___array
                    f = ___File(h5FileName, 'r')
                    if h5DataSetName in f.keys() :
                        return(___array(f[h5DataSetName]))
                except :
                    pass
                return None
    };

    void insertAsInt8(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, (char)(PyLong_AsLong(PyList_GetItem(list, i))));
        }
    }

    void insertAsInt16(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, (short)(PyLong_AsLong(PyList_GetItem(list, i))));
        }
    }

    void insertAsInt32(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, (int)(PyLong_AsLong(PyList_GetItem(list, i))));
        }
    }

    void insertAsInt64(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, PyLong_AsLong(PyList_GetItem(list, i)));
        }
    }

    void insertAsFloat32(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, (float)PyFloat_AsDouble(PyList_GetItem(list, i)));
        }
    }

    void insertAsFloat64(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, PyFloat_AsDouble(PyList_GetItem(list, i)));
        }
    }

    void insertAsUInt8(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, (unsigned char)(PyLong_AsUnsignedLong(PyList_GetItem(list, i))));
        }
    }

    void insertAsUInt16(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, (unsigned short)(PyLong_AsUnsignedLong(PyList_GetItem(list, i))));
        }
    }

    void insertAsUInt32(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert(i+startIndex, (unsigned int)(PyLong_AsUnsignedLong(PyList_GetItem(list, i))));
        }
    }

};

%extend XdmfArrayType {
    bool __eq__(const XdmfArrayType * arrayType) {
   	return $self == arrayType;
    }
};

#endif /* SWIGPYTHON */

%include boost_shared_ptr.i

%inline
%{
    #include <boost/shared_ptr.hpp>
    using namespace boost;
%}

%include std_string.i
%include std_vector.i

%shared_ptr(Loki::BaseVisitor)
%shared_ptr(Loki::BaseVisitable<void>)
%shared_ptr(Loki::Visitor<XdmfItem>)
%shared_ptr(Loki::Visitor<XdmfArray>)

%include loki/Visitor.h

// Shared Pointer Templates
%shared_ptr(XdmfArray)
%shared_ptr(XdmfArrayType)
%shared_ptr(XdmfCoreItemFactory)
%shared_ptr(XdmfCoreReader)
%shared_ptr(XdmfHDF5Controller)
%shared_ptr(XdmfHDF5Writer)
#ifdef XDMF_BUILD_DSM
    %shared_ptr(XdmfHDF5ControllerDSM)
    %shared_ptr(XdmfHDF5WriterDSM)
#endif
%shared_ptr(XdmfHeavyDataController)
%shared_ptr(XdmfHeavyDataWriter)
%shared_ptr(XdmfInformation)
%shared_ptr(XdmfItem)
%shared_ptr(XdmfItemProperty)
%shared_ptr(XdmfVisitor)
%shared_ptr(XdmfWriter)

// Abstract Base Classes
%template(BaseVisitable) Loki::BaseVisitable<void>;
%template() Loki::Visitor<XdmfArray>;
%template() Loki::Visitor<XdmfItem>;

%include XdmfCore.hpp
%include XdmfError.hpp
%include XdmfItem.hpp
%include XdmfItemProperty.hpp
%include XdmfVisitor.hpp
%include XdmfHeavyDataController.hpp
%include XdmfHeavyDataWriter.hpp

%include XdmfCoreItemFactory.hpp
%include XdmfCoreReader.hpp
%include XdmfInformation.hpp
%include XdmfHDF5Controller.hpp
%include XdmfHDF5Writer.hpp
%include XdmfWriter.hpp

%include CMake/VersionSuite/ProjectVersion.hpp
%include XdmfVersion.hpp

#ifdef XDMF_BUILD_DSM
    %include XdmfHDF5ControllerDSM.hpp
    %include XdmfHDF5WriterDSM.hpp
#endif

%include XdmfArray.hpp
%include XdmfArrayType.hpp

#ifdef SWIGPYTHON

%pythoncode {
    XdmfVersion = _XdmfCore.cvar.XdmfVersion
};

#endif /* SWIGPYTHON */

%template(getValueAsInt8) XdmfArray::getValue<char>;
%template(getValueAsInt16) XdmfArray::getValue<short>;
%template(getValueAsInt32) XdmfArray::getValue<int>;
%template(getValueAsInt64) XdmfArray::getValue<long>;
%template(getValueAsFloat32) XdmfArray::getValue<float>;
%template(getValueAsFloat64) XdmfArray::getValue<double>;
%template(getValueAsUInt8) XdmfArray::getValue<unsigned char>;
%template(getValueAsUInt16) XdmfArray::getValue<unsigned short>;
%template(getValueAsUInt32) XdmfArray::getValue<unsigned int>;

%template(initializeAsInt8) XdmfArray::initialize<char>;
%template(initializeAsInt16) XdmfArray::initialize<short>;
%template(initializeAsInt32) XdmfArray::initialize<int>;
%template(initializeAsInt64) XdmfArray::initialize<long>;
%template(initializeAsFloat32) XdmfArray::initialize<float>;
%template(initializeAsFloat64) XdmfArray::initialize<double>;
%template(initializeAsUInt8) XdmfArray::initialize<unsigned char>;
%template(initializeAsUInt16) XdmfArray::initialize<unsigned short>;
%template(initializeAsUInt32) XdmfArray::initialize<unsigned int>;

%template(insertValueAsInt8) XdmfArray::insert<char>;
%template(insertValueAsInt16) XdmfArray::insert<short>;
%template(insertValueAsInt32) XdmfArray::insert<int>;
%template(insertValueAsInt64) XdmfArray::insert<long>;
%template(insertValueAsFloat32) XdmfArray::insert<float>;
%template(insertValueAsFloat64) XdmfArray::insert<double>;
%template(insertValueAsUInt8) XdmfArray::insert<unsigned char>;
%template(insertValueAsUInt16) XdmfArray::insert<unsigned short>;
%template(insertValueAsUInt32) XdmfArray::insert<unsigned int>;

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

%template(UIntVector) std::vector<unsigned int>;
%template(ItemVector) std::vector<boost::shared_ptr<XdmfItem> >;

