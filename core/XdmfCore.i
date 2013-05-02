/*
XdmfCorePython.cpp:
swig -v -c++ -python -o XdmfCorePython.cpp XdmfCore.i
*/


#ifdef XDMF_BUILD_DSM

%module XdmfCore
%{

    #include <mpi.h>

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
    #include <XdmfHDF5ControllerDSM.hpp>
    #include <XdmfHDF5WriterDSM.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVersion.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    #include <ProjectVersion.hpp>
%}


#else

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
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVersion.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    #include <ProjectVersion.hpp>
%}

#endif

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

#ifdef XDMF_BUILD_DSM

%include mpi4py/mpi4py.i

%mpi4py_typemap(Comm, MPI_Comm);

#endif

/*This causes it to avoid throwing a warning for redefining fuctions that are defined for XdmfArray.
  I this case doing so was intentional.*/
#pragma SWIG nowarn=302

// Provide accessors from python lists to XdmfArrays
%extend XdmfArray {

%{
  #include <stack>;
/*These can't be accessed from python so don't worry about security issues.*/
  static std::map<std::string, PyObject *> pythonFunctions;
  static std::map<char, PyObject *> pythonOperations;
  static int pythonOperationPriority [4];
  static std::string pythonSupportedOperations = "";
%}

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


/*Note, the things in this block are defined twice for a reason.
  If they are not defined here they can not be referenced by other functions in swig.*/
%{
    /*trying to transfer python funcitons*/
    /*note, accessing private members is impossible from swig.*/

    /*to generate new static functions mark them as static here.*/
    static std::vector<std::string> getSupportedFunctions()
    {
      std::vector<std::string> returnVector = XdmfArray::getSupportedFunctions();
      for (std::map<std::string, PyObject *>::iterator functionWalker = pythonFunctions.begin(); functionWalker != pythonFunctions.end(); functionWalker++) {
        returnVector.push_back(functionWalker->first);
      }
      return returnVector;
    }

    static shared_ptr<XdmfArray> evaluateFunction(std::vector<shared_ptr<XdmfArray> > functValues, std::string functName)
    {
      if (pythonFunctions.find(functName)!= pythonFunctions.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_std__vectorT_boost__shared_ptrT_XdmfArray_t_std__allocatorT_boost__shared_ptrT_XdmfArray_t_t_t");
        PyObject * pyVector = SWIG_NewPointerObj(static_cast<void*>(& functValues), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pyVector);/*In this case you could also cast a pointer to the vector into the PyObject * type, but that doesn't work for all types*/
        PyObject * resultObject = PyObject_CallObject(pythonFunctions[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfArray::evaluateFunction(functValues, functName);
      }
    }

    static std::string getSupportedOperations()
    {
      std::string returnVector = XdmfArray::getSupportedOperations();
      returnVector += pythonSupportedOperations;
      return returnVector;
    }

    static int getOperationPriority(char operation)
    {
      size_t operationLocation = pythonSupportedOperations.find(operation);
      if (operationLocation != std::string::npos) {
        return pythonOperationPriority[operationLocation];
      }
      else {
        return XdmfArray::getOperationPriority(operation);
      }
    }

    static shared_ptr<XdmfArray> evaluateOperation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char functName)
    {
      if (pythonOperations.find(functName)!= pythonOperations.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        PyObject * pyVal1 = SWIG_NewPointerObj(static_cast<void*>(& val1), paramType, SWIG_POINTER_NEW);
        PyObject * pyVal2 = SWIG_NewPointerObj(static_cast<void*>(& val2), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, pyVal1);
        PyTuple_SetItem(args, 1, pyVal2);
        PyObject * resultObject = PyObject_CallObject(pythonOperations[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfArray::evaluateOperation(val1, val2, functName);
      }
    }

    static shared_ptr<XdmfArray> evaluateExpression(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables)
    {
      std::stack<shared_ptr<XdmfArray> > valueStack;
      std::stack<char> operationStack;

      /*string is parsed left to right
        elements of the same priority are evaluated right to left*/
      for (int i = 0; i < expression.size(); i++) {
        if (XdmfArray::getValidDigitChars().find(expression[i]) != std::string::npos) {/*found to be a digit*/
          /*progress until a non-digit is found*/
          int valueStart = i;
          while (XdmfArray::getValidDigitChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*push back to the value stack*/
          shared_ptr<XdmfArray> valueArray = XdmfArray::New();
          valueArray->insert(0, atof(expression.substr(valueStart, i + 1 - valueStart).c_str()));/*use this to convert to double*/
          valueStack.push(valueArray);
        }
        else if (XdmfArray::getValidVariableChars().find(expression[i]) != std::string::npos) {/*found to be a variable*/
          int valueStart = i;
          /*progress until a nonvariable value is found*/
          while (XdmfArray::getValidVariableChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*convert to equivalent*/
          if (variables.find(expression.substr(valueStart, i + 1 - valueStart)) == variables.end()) {
            std::vector<std::string> functionList = XdmfArray::getSupportedFunctions();
            bool functionExists = false;
            for (int j = 0; j < functionList.size() && !functionExists; j++) {
              if (functionList[j] == expression.substr(valueStart, i + 1 - valueStart)) {
                functionExists = true;
              }
            }
            if (functionExists) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Invalid Variable in evaluateExpression " + expression.substr(valueStart, i + 1 - valueStart));
            }
            else {
              std::string currentFunction = expression.substr(valueStart, i + 1 - valueStart);
              /*check if next character is an open parenthesis*/
              if (expression[i+1] != '(') {
                XdmfError::message(XdmfError::FATAL,
                                   "Error: No values supplied to function " + expression.substr(valueStart, i + 1 - valueStart));
              }
              /*if it is grab the string between paranthesis*/
              i = i + 2;
              valueStart = i;
              int numOpenParenthesis = 0;
              while ((expression[i] != ')' || numOpenParenthesis) && i < expression.size()) {
                if (expression[i] == '(') {
                  numOpenParenthesis++;
                }
                else if (expression[i] == ')') {
                  numOpenParenthesis--;
                }
                i++;
              }
              std::string functionParameters = expression.substr(valueStart, i - valueStart);
              std::vector<shared_ptr<XdmfArray> > parameterVector;
              /*split that string at commas*/
              size_t parameterSplit = 0;
              while (parameterSplit != std::string::npos) {
                parameterSplit = 0;
                parameterSplit = functionParameters.find_first_of(",", parameterSplit);
                /*feed the substrings to the parse function*/
                if (parameterSplit == std::string::npos) {
                  parameterVector.push_back(evaluateExpression(functionParameters, variables));
                }
                else {
                  parameterVector.push_back(evaluateExpression(functionParameters.substr(0, parameterSplit), variables));
                  functionParameters = functionParameters.substr(parameterSplit+1);
                }
              }
              valueStack.push(evaluateFunction(parameterVector, currentFunction));
            }
          }
          else {
            /*push equivalent to value stack*/
            valueStack.push(variables.find(expression.substr(valueStart, i + 1 - valueStart))->second);
          }
        }
        else if (XdmfArray::getSupportedOperations().find(expression[i]) != std::string::npos) {/*found to be an operation*/
          /*pop operations off the stack until one of a lower or equal importance is found*/
          if (operationStack.size() > 0) {
            if (expression[i] == ')') {
              /*to close a parenthesis pop off all operations until another parentheis is found*/
              while (operationStack.size() > 0 && operationStack.top() != '(') {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                }
              }
              operationStack.pop();
            }
            else if (expression[i] == '(') {
              /*just add it if it's a start parenthesis
                nothing happens here in that case
                addition happens after the if statement block*/
            }
            else {
              int operationLocation = XdmfArray::getOperationPriority(expression[i]);
              int topOperationLocation = XdmfArray::getOperationPriority(operationStack.top());
              /*see order of operations to determine importance*/
              while (operationStack.size() > 0 && operationLocation < topOperationLocation) {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                  if (operationStack.size() == 0) {
                    break;
                  }
                  topOperationLocation = XdmfArray::getOperationPriority(operationStack.top());
                }
              }
            }
          }
          if (expression[i] != ')') {
            /*add the operation to the operation stack*/
            operationStack.push(expression[i]);
          }
        }
        /*if not a value or operation the character is ignored*/
      }


      /*empty what's left in the stacks before finishing*/
      while (valueStack.size() > 1 && operationStack.size() > 0) {
        if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
          XdmfError::message(XdmfError::FATAL,
                             "Error: Not Enough Values in evaluateExpression");
        }
        else {
          if(operationStack.top() == '(') {
            XdmfError::message(XdmfError::WARNING,
                               "Warning: Unpaired Parenthesis");
          }
          else {
            shared_ptr<XdmfArray> val2 = valueStack.top();
            valueStack.pop();
            shared_ptr<XdmfArray> val1 = valueStack.top();
            valueStack.pop();
            if (operationStack.size() == 0) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Not Enough Operators in evaluateExpression");
            }
            else {
              valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
              operationStack.pop();
            }
          }
        }
      }

      /*throw error if there's extra operations*/
      if (operationStack.size() > 0) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Operators in evaluateExpression");
      }

      if (valueStack.size() > 1) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Values in evaluateExpression");
      }

      return valueStack.top();
    }

%}

    /*trying to transfer python funcitons*/
    /*note, accessing private members is impossible from swig.*/
    static int addFunction(std::string newName, PyObject * functionref)
    {
      /*check if the object is callable*/
      if (PyCallable_Check(functionref) == 1) {
        pythonFunctions[newName] = functionref;
        return pythonFunctions.size();
      }
      else {
        XdmfError::message(XdmfError::FATAL,
          "Error: Function is not callable");
        return -1;
      }
    }

    /*to generate new static functions mark them as static here.*/
    static std::vector<std::string> getSupportedFunctions()
    {
      std::vector<std::string> returnVector = XdmfArray::getSupportedFunctions();
      for (std::map<std::string, PyObject *>::iterator functionWalker = pythonFunctions.begin(); functionWalker != pythonFunctions.end(); functionWalker++) {
        returnVector.push_back(functionWalker->first);
      }
      return returnVector;
    }

    static shared_ptr<XdmfArray> evaluateFunction(std::vector<shared_ptr<XdmfArray> > functValues, std::string functName)
    {
      if (pythonFunctions.find(functName)!= pythonFunctions.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_std__vectorT_boost__shared_ptrT_XdmfArray_t_std__allocatorT_boost__shared_ptrT_XdmfArray_t_t_t");
        PyObject * pyVector = SWIG_NewPointerObj(static_cast<void*>(& functValues), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pyVector);/*In this case you could also cast a pointer to the vector into the PyObject * type, but that doesn't work for all types*/
        PyObject * resultObject = PyObject_CallObject(pythonFunctions[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfArray::evaluateFunction(functValues, functName);
      }
    }

    static int addOperation(char newName, PyObject * calcref, int priority)
    {
      if (newName == '(' || newName == ')') {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Parenthesis can not be redefined");
      }
      /*check if the object is callable*/
      if (PyCallable_Check(calcref) == 1) {
        if (pythonOperations.find(newName) != pythonOperations.end() || XdmfArray::getSupportedOperations().find(newName) != std::string::npos) {
          XdmfError::message(XdmfError::WARNING,
                             "Warning: Operation Redefined");/*It's a good idea to warn users when they're doing this*/
          size_t operationLocation = pythonSupportedOperations.find(newName);
          /*if not an already defined python function*/
          if (pythonOperations.find(newName) == pythonOperations.end()) {
            pythonSupportedOperations.push_back(newName);
            int priorityArraySize = sizeof(pythonOperationPriority)/sizeof(int);
            if (pythonSupportedOperations.size()-1 > priorityArraySize) {
              int newArray [priorityArraySize*2];
              std::copy(pythonOperationPriority, pythonOperationPriority+(priorityArraySize-1), newArray);
              delete pythonOperationPriority;
              *pythonOperationPriority = *newArray;
            }
          }
          pythonOperationPriority[operationLocation] = priority;
          /*if defined on the c++ side same as adding a new one*/
        }
        else {
          pythonSupportedOperations += newName;
          if (XdmfArray::getValidVariableChars().find(newName) != std::string::npos || XdmfArray::getValidDigitChars().find(newName) != std::string::npos) {
            XdmfError::message(XdmfError::FATAL,
              "Error: Operation Overlaps with Variables");
          }
          else {
            pythonSupportedOperations.push_back(newName);
            int priorityArraySize = sizeof(pythonOperationPriority)/sizeof(int);
            if (pythonSupportedOperations.size()-1 > priorityArraySize)
            {
              int newArray [priorityArraySize*2];
              std::copy(pythonOperationPriority, pythonOperationPriority+(priorityArraySize-1), newArray);
              delete pythonOperationPriority;
              *pythonOperationPriority = *newArray;
            }
          }
        }
        pythonOperations[newName] = calcref;
        return pythonOperations.size();
      }
      else {
        XdmfError::message(XdmfError::FATAL,
          "Error: Operation is not callable");
        return -1;
      }
    }

    static std::string getSupportedOperations()
    {
      std::string returnVector = XdmfArray::getSupportedOperations();
      for (std::map<char, PyObject *>::iterator functionWalker = pythonOperations.begin(); functionWalker != pythonOperations.end(); functionWalker++) {
        returnVector += functionWalker->first;
      }
      return returnVector;
    }

    static shared_ptr<XdmfArray> evaluateOperation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char functName)
    {
      if (pythonOperations.find(functName)!= pythonOperations.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        PyObject * pyVal1 = SWIG_NewPointerObj(static_cast<void*>(& val1), paramType, SWIG_POINTER_NEW);
        PyObject * pyVal2 = SWIG_NewPointerObj(static_cast<void*>(& val2), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, pyVal1);
        PyTuple_SetItem(args, 1, pyVal2);
        PyObject * resultObject = PyObject_CallObject(pythonOperations[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfArray::evaluateOperation(val1, val2, functName);
      }
    }

    static shared_ptr<XdmfArray> evaluateExpression(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables)
    {
      std::stack<shared_ptr<XdmfArray> > valueStack;
      std::stack<char> operationStack;

      /*string is parsed left to right
        elements of the same priority are evaluated right to left*/
      for (int i = 0; i < expression.size(); i++) {
        if (XdmfArray::getValidDigitChars().find(expression[i]) != std::string::npos) { /*found to be a digit*/
          /*progress until a non-digit is found*/
          int valueStart = i;
          while (XdmfArray::getValidDigitChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*push back to the value stack*/
          shared_ptr<XdmfArray> valueArray = XdmfArray::New();
          valueArray->insert(0, atof(expression.substr(valueStart, i + 1 - valueStart).c_str()));/*use this to convert to double*/
          valueStack.push(valueArray);
        }
        else if (XdmfArray::getValidVariableChars().find(expression[i]) != std::string::npos) {/*found to be a variable*/
          int valueStart = i;
          /*progress until a nonvariable value is found*/
          while (XdmfArray::getValidVariableChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*convert to equivalent*/
          if (variables.find(expression.substr(valueStart, i + 1 - valueStart)) == variables.end()) {
            std::vector<std::string> functionList = XdmfArray::getSupportedFunctions();
            bool functionExists = false;
            for (int j = 0; j < functionList.size() && !functionExists; j++) {
              if (functionList[j] == expression.substr(valueStart, i + 1 - valueStart)) {
                functionExists = true;
              }
            }
            if (functionExists) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Invalid Variable in evaluateExpression " + expression.substr(valueStart, i + 1 - valueStart));
            }
            else {
              std::string currentFunction = expression.substr(valueStart, i + 1 - valueStart);
              /*check if next character is an open parenthesis*/
              if (expression[i+1] != '(') {
                XdmfError::message(XdmfError::FATAL,
                                   "Error: No values supplied to function " + expression.substr(valueStart, i + 1 - valueStart));
              }
              /*if it is grab the string between paranthesis*/
              i = i + 2;
              valueStart = i;
              int numOpenParenthesis = 0;
              while ((expression[i] != ')' || numOpenParenthesis) && i < expression.size()) {
                if (expression[i] == '(') {
                  numOpenParenthesis++;
                }
                else if (expression[i] == ')') {
                  numOpenParenthesis--;
                }
                i++;
              }
              std::string functionParameters = expression.substr(valueStart, i - valueStart);
              std::vector<shared_ptr<XdmfArray> > parameterVector;
              /*split that string at commas*/
              size_t parameterSplit = 0;
              while (parameterSplit != std::string::npos) {
                parameterSplit = 0;
                parameterSplit = functionParameters.find_first_of(",", parameterSplit);
                /*feed the substrings to the parse function*/
                if (parameterSplit == std::string::npos) {
                  parameterVector.push_back(evaluateExpression(functionParameters, variables));
                }
                else {
                  parameterVector.push_back(evaluateExpression(functionParameters.substr(0, parameterSplit), variables));
                  functionParameters = functionParameters.substr(parameterSplit+1);
                }
              }
              valueStack.push(evaluateFunction(parameterVector, currentFunction));
            }
          }
          else {
            /*push equivalent to value stack*/
            valueStack.push(variables.find(expression.substr(valueStart, i + 1 - valueStart))->second);
          }
        }
        else if (XdmfArray::getSupportedOperations().find(expression[i]) != std::string::npos) {/*found to be an operation*/
          /*pop operations off the stack until one of a lower or equal importance is found*/
          if (operationStack.size() > 0) {
            if (expression[i] == ')') {
              /*to close a parenthesis pop off all operations until another parentheis is found*/
              while (operationStack.size() > 0 && operationStack.top() != '(') {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                }
              }
              operationStack.pop();
            }
            else if (expression[i] == '(') {
              /*just add it if it's a start parenthesis
                nothing happens here in that case
                addition happens after the if statement*/
            }
            else {
              int operationLocation = XdmfArray::getOperationPriority(expression[i]);
              int topOperationLocation = XdmfArray::getOperationPriority(operationStack.top());
              /*see order of operations to determine importance*/
              while (operationStack.size() > 0 && operationLocation < topOperationLocation) {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                  if (operationStack.size() == 0) {
                    break;
                  }
                  topOperationLocation = XdmfArray::getOperationPriority(operationStack.top());
                }
              }
            }
          }
          if (expression[i] != ')') {
            /*add the operation to the operation stack*/
            operationStack.push(expression[i]);
          }
        }
        /*if not a value or operation the character is ignored*/
      }


      /*empty what's left in the stacks before finishing*/
      while (valueStack.size() > 1 && operationStack.size() > 0) {
        if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
          XdmfError::message(XdmfError::FATAL,
                             "Error: Not Enough Values in evaluateExpression");
        }
        else {
          if(operationStack.top() == '(') {
            XdmfError::message(XdmfError::WARNING,
                               "Warning: Unpaired Parenthesis");
          }
          else {
            shared_ptr<XdmfArray> val2 = valueStack.top();
            valueStack.pop();
            shared_ptr<XdmfArray> val1 = valueStack.top();
            valueStack.pop();
            if (operationStack.size() == 0) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Not Enough Operators in evaluateExpression");
            }
            else {
              valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
              operationStack.pop();
            }
          }
        }
      }

      /*throw error if there's extra operations*/
      if (operationStack.size() > 0) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Operators in evaluateExpression");
      }

      if (valueStack.size() > 1) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Values in evaluateExpression");
      }

      return valueStack.top();
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
%shared_ptr(XdmfSparseMatrix)
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
