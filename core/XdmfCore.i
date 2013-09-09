/*
XdmfCorePython.cpp:
swig -v -c++ -python -o XdmfCorePython.cpp XdmfCore.i
*/


%module XdmfCore
%{
    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCore.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSubset.hpp>
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

/*This converts XdmfErrors to Python RuntimeErrors*/
%exception
{
        try
        {
                $action
        }
        catch (XdmfError e)
        {
                PyErr_SetString(PyExc_RuntimeError, e.what());
                return NULL;
        }
}

/*This causes it to avoid throwing a warning about overloaded functions.
  We are doing this intentionally so suppressing the warning makes sense.*/
#pragma SWIG nowarn=509

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
            h5ctl = self.getHeavyDataController()
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

    void insertAsString(int startIndex, PyObject * list) {
        Py_ssize_t size = PyList_Size(list);
        for(Py_ssize_t i = 0; i < size; ++i) {
            $self->insert<std::string>(i+startIndex, PyString_AsString(PyList_GetItem(list, i)));
        }
    }

    static shared_ptr<XdmfArray> XdmfArrayPtr(PyObject * obj)
    {
      void * resultPointer = 0;
      swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
      SWIG_ConvertPtr(obj, &resultPointer, returnType, 0);
      shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
      shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
      return returnArray;
    }
};

%extend XdmfArrayType {
    bool __eq__(const XdmfArrayType * arrayType) {
   	return $self == arrayType;
    }
};

/*This causes it to avoid throwing a warning for redefining fuctions that are defined for XdmfArray.
  I do this because doing so was intentional.*/
#pragma SWIG nowarn=302
/*Warnint 325 is due to having nested classes in XdmfFunction that are not accessible when wrapped.
  As of right now, this is acceptable behavior. So, the warning is suppressed*/
#pragma SWIG nowarn=325

%extend XdmfFunction {

%{
    /*trying to transfer python functions*/
    /*note, accessing private members is impossible from swig.*/


    /*Class to wrap python functions to be compatible with the XdmfFunction code.
      This version has an execute that takes a vector of XdmfArrays as parameters,
      so it is used for functions.*/
    class PythonFunction : public XdmfFunction::XdmfFunctionInternal {
      public:
        static shared_ptr<PythonFunction>
        New(PyObject * functionref)
        {
          shared_ptr<PythonFunction> p (new PythonFunction(functionref));
          return p;
        }

        ~PythonFunction()
        {
        }

        virtual shared_ptr<XdmfArray> execute(std::vector<shared_ptr<XdmfArray> > valueVector)
        {
          swig_type_info * paramType = SWIG_TypeQuery("_p_std__vectorT_boost__shared_ptrT_XdmfArray_t_std__allocatorT_boost__shared_ptrT_XdmfArray_t_t_t");
          PyObject * pyVector = SWIG_NewPointerObj(static_cast<void*>(& valueVector), paramType, SWIG_POINTER_NEW);
          PyObject * args = PyTuple_New(1);
          /* In this case you could also cast a pointer to the vector
             into the PyObject * type, but that doesn't work for all types*/
          PyTuple_SetItem(args, 0, pyVector);
          PyObject * resultObject = PyObject_CallObject(mInternalFunction, args);
          void * resultPointer = 0;
          swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
          SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
          shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
          shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
          return returnArray;
        }
      private:
        PythonFunction(PyObject * functionref)
        {
          if (PyCallable_Check(functionref) == 1) {
            mInternalFunction = functionref;
          }
          else {
            XdmfError::message(XdmfError::FATAL,
                               "Error: Function is not callable");
          }
        }

        PyObject * mInternalFunction;
    };

    /*Class to wrap python functions to be compatible with the XdmfFunction Operation code.
      This version has an execute that takes two XdmfArrays as parameters,
      so it is used for binary operators.*/
    class PythonOperation : public XdmfFunction::XdmfOperationInternal {
      public:
        static shared_ptr<PythonOperation>
        New(PyObject * operationref)
        {
          shared_ptr<PythonOperation> p (new PythonOperation(operationref));
          return p;
        }

        ~PythonOperation()
        {
        }

        virtual shared_ptr<XdmfArray> execute(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
        {
          swig_type_info * paramType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
          PyObject * pyVal1 = SWIG_NewPointerObj(static_cast<void*>(& val1), paramType, SWIG_POINTER_NEW);
          PyObject * pyVal2 = SWIG_NewPointerObj(static_cast<void*>(& val2), paramType, SWIG_POINTER_NEW);
          PyObject * args = PyTuple_New(2);
          PyTuple_SetItem(args, 0, pyVal1);
          PyTuple_SetItem(args, 1, pyVal2);
          PyObject * resultObject = PyObject_CallObject(mInternalOperation, args);
          void * resultPointer = 0;
          swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
          SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
          shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
          shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
          return returnArray;
        }
      private:
        PythonOperation(PyObject * operationref)
        {
          if (PyCallable_Check(operationref) == 1) {
            mInternalOperation = operationref;
          }
          else {
            XdmfError::message(XdmfError::FATAL,
                               "Error: Operation is not callable");
          }
        }

        PyObject * mInternalOperation;
    };
%}

    static int addFunction(std::string newName, PyObject * functionref)
    {
      shared_ptr<PythonFunction> newFunction = PythonFunction::New(functionref);
      return XdmfFunction::addFunction(newName, newFunction);
    }

    static int addOperation(char newName, PyObject * calcref, int priority)
    {
      shared_ptr<PythonOperation> newOperation = PythonOperation::New(calcref);
      return XdmfFunction::addOperation(newName, newOperation, priority);
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
%include std_map.i

%shared_ptr(Loki::BaseVisitor)
%shared_ptr(Loki::BaseVisitable<void>)
%shared_ptr(Loki::Visitor<XdmfItem>)
%shared_ptr(Loki::Visitor<XdmfArray>)

%include loki/Visitor.h

// Shared Pointer Templates
%shared_ptr(XdmfArray)
%shared_ptr(XdmfArrayReference)
%shared_ptr(XdmfArrayType)
%shared_ptr(XdmfCoreItemFactory)
%shared_ptr(XdmfCoreReader)
%shared_ptr(XdmfFunction)
%shared_ptr(XdmfHDF5Controller)
%shared_ptr(XdmfHDF5Writer)
%shared_ptr(XdmfHeavyDataController)
%shared_ptr(XdmfHeavyDataWriter)
%shared_ptr(XdmfInformation)
%shared_ptr(XdmfItem)
%shared_ptr(XdmfItemProperty)
%shared_ptr(XdmfSparseMatrix)
%shared_ptr(XdmfSubset)
%shared_ptr(XdmfVisitor)
%shared_ptr(XdmfWriter)

%shared_ptr(std::vector<int>)

// Abstract Base Classes
%template(BaseVisitable) Loki::BaseVisitable<void>;
%template() Loki::Visitor<XdmfArray>;
%template() Loki::Visitor<XdmfItem>;

%include XdmfCore.hpp
%include XdmfError.hpp
%include XdmfItem.hpp
%include XdmfItemProperty.hpp
%include XdmfSparseMatrix.hpp
%include XdmfSystemUtils.hpp
%include XdmfVisitor.hpp
%include XdmfArrayReference.hpp
%include XdmfFunction.hpp
%include XdmfHeavyDataController.hpp
%include XdmfHeavyDataWriter.hpp
%include XdmfSubset.hpp

%include XdmfCoreItemFactory.hpp
%include XdmfCoreReader.hpp
%include XdmfInformation.hpp
%include XdmfHDF5Controller.hpp
%include XdmfHDF5Writer.hpp
%include XdmfWriter.hpp

%include CMake/VersionSuite/ProjectVersion.hpp
%include XdmfVersion.hpp

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
%template(getValueAsString) XdmfArray::getValue<std::string>;

%template(initializeAsInt8) XdmfArray::initialize<char>;
%template(initializeAsInt16) XdmfArray::initialize<short>;
%template(initializeAsInt32) XdmfArray::initialize<int>;
%template(initializeAsInt64) XdmfArray::initialize<long>;
%template(initializeAsFloat32) XdmfArray::initialize<float>;
%template(initializeAsFloat64) XdmfArray::initialize<double>;
%template(initializeAsUInt8) XdmfArray::initialize<unsigned char>;
%template(initializeAsUInt16) XdmfArray::initialize<unsigned short>;
%template(initializeAsUInt32) XdmfArray::initialize<unsigned int>;
%template(initializeAsString) XdmfArray::initialize<std::string>;

%template(insertValueAsInt8) XdmfArray::insert<char>;
%template(insertValueAsInt16) XdmfArray::insert<short>;
%template(insertValueAsInt32) XdmfArray::insert<int>;
%template(insertValueAsInt64) XdmfArray::insert<long>;
%template(insertValueAsFloat32) XdmfArray::insert<float>;
%template(insertValueAsFloat64) XdmfArray::insert<double>;
%template(insertValueAsUInt8) XdmfArray::insert<unsigned char>;
%template(insertValueAsUInt16) XdmfArray::insert<unsigned short>;
%template(insertValueAsUInt32) XdmfArray::insert<unsigned int>;
%template(insertValueAsString) XdmfArray::insert<std::string>;

%template(pushBackAsInt8) XdmfArray::pushBack<char>;
%template(pushBackAsInt16) XdmfArray::pushBack<short>;
%template(pushBackAsInt32) XdmfArray::pushBack<int>;
%template(pushBackAsInt64) XdmfArray::pushBack<long>;
%template(pushBackAsFloat32) XdmfArray::pushBack<float>;
%template(pushBackAsFloat64) XdmfArray::pushBack<double>;
%template(pushBackAsUInt8) XdmfArray::pushBack<unsigned char>;
%template(pushBackAsUInt16) XdmfArray::pushBack<unsigned short>;
%template(pushBackAsUInt32) XdmfArray::pushBack<unsigned int>;
%template(pushBackAsString) XdmfArray::pushBack<std::string>;

%template(resizeAsInt8) XdmfArray::resize<char>;
%template(resizeAsInt16) XdmfArray::resize<short>;
%template(resizeAsInt32) XdmfArray::resize<int>;
%template(resizeAsInt64) XdmfArray::resize<long>;
%template(resizeAsFloat32) XdmfArray::resize<float>;
%template(resizeAsFloat64) XdmfArray::resize<double>;
%template(resizeAsUInt8) XdmfArray::resize<unsigned char>;
%template(resizeAsUInt16) XdmfArray::resize<unsigned short>;
%template(resizeAsUInt32) XdmfArray::resize<unsigned int>;
%template(resizeAsString) XdmfArray::resize<std::string>;

%template(UInt8Vector) std::vector<unsigned char>;
%template(UInt16Vector) std::vector<unsigned short>;
%template(UInt32Vector) std::vector<unsigned int>;
%template(Int8Vector) std::vector<char>;
%template(Int16Vector) std::vector<short>;
%template(Int32Vector) std::vector<int>;
%template(Int64Vector) std::vector<long>;
%template(Float32Vector) std::vector<float>;
%template(Float64Vector) std::vector<double>;
%template(StringVector) std::vector<std::string>;
%template(ItemVector) std::vector<boost::shared_ptr<XdmfItem> >;
%template(ArrayMap) std::map<std::string, boost::shared_ptr<XdmfArray> >;
%template(StringMap) std::map<std::string, std::string>;
