# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _Xdmf

def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


SIZEOF_LONG_LONG = _Xdmf.SIZEOF_LONG_LONG
ICE_SYSTEM = _Xdmf.ICE_SYSTEM
ICE_MACHINE_TYPE = _Xdmf.ICE_MACHINE_TYPE
ICE_TRUE = _Xdmf.ICE_TRUE
ICE_FALSE = _Xdmf.ICE_FALSE
ICE_SUCCESS = _Xdmf.ICE_SUCCESS
ICE_FAIL = _Xdmf.ICE_FAIL
ICE_CHAR_TYPE = _Xdmf.ICE_CHAR_TYPE
ICE_8_INT_TYPE = _Xdmf.ICE_8_INT_TYPE
ICE_32_INT_TYPE = _Xdmf.ICE_32_INT_TYPE
ICE_64_INT_TYPE = _Xdmf.ICE_64_INT_TYPE
ICE_FLOAT_TYPE = _Xdmf.ICE_FLOAT_TYPE
ICE_DOUBLE_TYPE = _Xdmf.ICE_DOUBLE_TYPE
XDMF_SUCCESS = _Xdmf.XDMF_SUCCESS
XDMF_FAIL = _Xdmf.XDMF_FAIL
XDMF_TRUE = _Xdmf.XDMF_TRUE
XDMF_FALSE = _Xdmf.XDMF_FALSE
XDMF_MAX_DIMENSION = _Xdmf.XDMF_MAX_DIMENSION
XDMF_MAX_STRING_LENGTH = _Xdmf.XDMF_MAX_STRING_LENGTH
XDMF_DEFAULT_INDEX = _Xdmf.XDMF_DEFAULT_INDEX
XDMF_SELECT_SLAB = _Xdmf.XDMF_SELECT_SLAB
XDMF_SELECT_INDEX = _Xdmf.XDMF_SELECT_INDEX
XDMF_INT8_TYPE = _Xdmf.XDMF_INT8_TYPE
XDMF_INT16_TYPE = _Xdmf.XDMF_INT16_TYPE
XDMF_INT32_TYPE = _Xdmf.XDMF_INT32_TYPE
XDMF_INT64_TYPE = _Xdmf.XDMF_INT64_TYPE
XDMF_FLOAT32_TYPE = _Xdmf.XDMF_FLOAT32_TYPE
XDMF_FLOAT64_TYPE = _Xdmf.XDMF_FLOAT64_TYPE
XDMF_UINT8_TYPE = _Xdmf.XDMF_UINT8_TYPE
XDMF_UINT16_TYPE = _Xdmf.XDMF_UINT16_TYPE
XDMF_UINT32_TYPE = _Xdmf.XDMF_UINT32_TYPE
XDMF_COMPOUND_TYPE = _Xdmf.XDMF_COMPOUND_TYPE
class XdmfObject(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfObject, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfObject, name)
    def __repr__(self):
        return "<C XdmfObject instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfObject, 'this', _Xdmf.new_XdmfObject(*args))
        _swig_setattr(self, XdmfObject, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfObject_GetClassName(*args)
    def SetDebug(*args): return _Xdmf.XdmfObject_SetDebug(*args)
    def GetDebug(*args): return _Xdmf.XdmfObject_GetDebug(*args)
    def GetGlobalDebug(*args): return _Xdmf.XdmfObject_GetGlobalDebug(*args)
    def SetGlobalDebug(*args): return _Xdmf.XdmfObject_SetGlobalDebug(*args)
    def SetGlobalDebugOn(*args): return _Xdmf.XdmfObject_SetGlobalDebugOn(*args)
    def SetGlobalDebugOff(*args): return _Xdmf.XdmfObject_SetGlobalDebugOff(*args)

class XdmfObjectPtr(XdmfObject):
    def __init__(self, this):
        _swig_setattr(self, XdmfObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfObject, 'thisown', 0)
        _swig_setattr(self, XdmfObject,self.__class__,XdmfObject)
_Xdmf.XdmfObject_swigregister(XdmfObjectPtr)


SetGlobalDebugOn = _Xdmf.SetGlobalDebugOn

SetGlobalDebugOff = _Xdmf.SetGlobalDebugOff

GetGlobalDebug = _Xdmf.GetGlobalDebug

SetGlobalDebug = _Xdmf.SetGlobalDebug

GetUnique = _Xdmf.GetUnique

XdmfObjectToHandle = _Xdmf.XdmfObjectToHandle

HandleToXdmfObject = _Xdmf.HandleToXdmfObject

ICE_READ_STREAM64 = _Xdmf.ICE_READ_STREAM64

XdmfTypeToHDF5Type = _Xdmf.XdmfTypeToHDF5Type

HDF5TypeToXdmfType = _Xdmf.HDF5TypeToXdmfType

XdmfTypeToString = _Xdmf.XdmfTypeToString

StringToXdmfType = _Xdmf.StringToXdmfType

XdmfTypeToClassString = _Xdmf.XdmfTypeToClassString
XDMF_SELECTALL = _Xdmf.XDMF_SELECTALL
XDMF_HYPERSLAB = _Xdmf.XDMF_HYPERSLAB
XDMF_COORDINATES = _Xdmf.XDMF_COORDINATES
class XdmfDataDesc(XdmfObject):
    __swig_setmethods__ = {}
    for _s in [XdmfObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfDataDesc, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfDataDesc, name)
    def __repr__(self):
        return "<C XdmfDataDesc instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfDataDesc, 'this', _Xdmf.new_XdmfDataDesc(*args))
        _swig_setattr(self, XdmfDataDesc, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfDataDesc):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Print(*args): return _Xdmf.XdmfDataDesc_Print(*args)
    def SetNumberType(*args): return _Xdmf.XdmfDataDesc_SetNumberType(*args)
    def SetNumberTypeFromString(*args): return _Xdmf.XdmfDataDesc_SetNumberTypeFromString(*args)
    def GetNumberType(*args): return _Xdmf.XdmfDataDesc_GetNumberType(*args)
    def GetNumberTypeAsString(*args): return _Xdmf.XdmfDataDesc_GetNumberTypeAsString(*args)
    def SetShape(*args): return _Xdmf.XdmfDataDesc_SetShape(*args)
    def GetShape(*args): return _Xdmf.XdmfDataDesc_GetShape(*args)
    def GetShapeAsString(*args): return _Xdmf.XdmfDataDesc_GetShapeAsString(*args)
    def CopySelection(*args): return _Xdmf.XdmfDataDesc_CopySelection(*args)
    def CopyShape(*args): return _Xdmf.XdmfDataDesc_CopyShape(*args)
    def CopyType(*args): return _Xdmf.XdmfDataDesc_CopyType(*args)
    def SetNumberOfElements(*args): return _Xdmf.XdmfDataDesc_SetNumberOfElements(*args)
    def SetShapeFromString(*args): return _Xdmf.XdmfDataDesc_SetShapeFromString(*args)
    def SelectAll(*args): return _Xdmf.XdmfDataDesc_SelectAll(*args)
    def SelectHyperSlab(*args): return _Xdmf.XdmfDataDesc_SelectHyperSlab(*args)
    def SelectHyperSlabFromString(*args): return _Xdmf.XdmfDataDesc_SelectHyperSlabFromString(*args)
    def GetHyperSlab(*args): return _Xdmf.XdmfDataDesc_GetHyperSlab(*args)
    def GetHyperSlabAsString(*args): return _Xdmf.XdmfDataDesc_GetHyperSlabAsString(*args)
    def SelectCoordinates(*args): return _Xdmf.XdmfDataDesc_SelectCoordinates(*args)
    def SelectCoordinatesFromString(*args): return _Xdmf.XdmfDataDesc_SelectCoordinatesFromString(*args)
    def GetCoordinates(*args): return _Xdmf.XdmfDataDesc_GetCoordinates(*args)
    def GetCoordinatesAsString(*args): return _Xdmf.XdmfDataDesc_GetCoordinatesAsString(*args)
    def GetSelectionSize(*args): return _Xdmf.XdmfDataDesc_GetSelectionSize(*args)
    def GetNumberOfElements(*args): return _Xdmf.XdmfDataDesc_GetNumberOfElements(*args)
    def GetRank(*args): return _Xdmf.XdmfDataDesc_GetRank(*args)
    def GetDimension(*args): return _Xdmf.XdmfDataDesc_GetDimension(*args)
    def GetStart(*args): return _Xdmf.XdmfDataDesc_GetStart(*args)
    def GetStride(*args): return _Xdmf.XdmfDataDesc_GetStride(*args)
    def GetCount(*args): return _Xdmf.XdmfDataDesc_GetCount(*args)
    def GetSelectionType(*args): return _Xdmf.XdmfDataDesc_GetSelectionType(*args)
    def GetSelectionTypeAsString(*args): return _Xdmf.XdmfDataDesc_GetSelectionTypeAsString(*args)
    def GetDataType(*args): return _Xdmf.XdmfDataDesc_GetDataType(*args)
    def GetDataSpace(*args): return _Xdmf.XdmfDataDesc_GetDataSpace(*args)
    def GetElementSize(*args): return _Xdmf.XdmfDataDesc_GetElementSize(*args)
    def AddCompoundMemberFromString(*args): return _Xdmf.XdmfDataDesc_AddCompoundMemberFromString(*args)
    def GetNumberOfMembers(*args): return _Xdmf.XdmfDataDesc_GetNumberOfMembers(*args)
    def GetMemberName(*args): return _Xdmf.XdmfDataDesc_GetMemberName(*args)
    def GetMemberLength(*args): return _Xdmf.XdmfDataDesc_GetMemberLength(*args)
    def GetMemberSize(*args): return _Xdmf.XdmfDataDesc_GetMemberSize(*args)
    def GetMemberShape(*args): return _Xdmf.XdmfDataDesc_GetMemberShape(*args)
    def GetMemberShapeAsString(*args): return _Xdmf.XdmfDataDesc_GetMemberShapeAsString(*args)
    def GetMemberType(*args): return _Xdmf.XdmfDataDesc_GetMemberType(*args)
    def GetMemberTypeAsString(*args): return _Xdmf.XdmfDataDesc_GetMemberTypeAsString(*args)
    def GetMemberOffset(*args): return _Xdmf.XdmfDataDesc_GetMemberOffset(*args)

class XdmfDataDescPtr(XdmfDataDesc):
    def __init__(self, this):
        _swig_setattr(self, XdmfDataDesc, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfDataDesc, 'thisown', 0)
        _swig_setattr(self, XdmfDataDesc,self.__class__,XdmfDataDesc)
_Xdmf.XdmfDataDesc_swigregister(XdmfDataDescPtr)

XDMF_ARRAY_TAG_LENGTH = _Xdmf.XDMF_ARRAY_TAG_LENGTH
class XdmfArray(XdmfDataDesc):
    __swig_setmethods__ = {}
    for _s in [XdmfDataDesc]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfArray, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfDataDesc]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfArray, name)
    def __repr__(self):
        return "<C XdmfArray instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfArray, 'this', _Xdmf.new_XdmfArray(*args))
        _swig_setattr(self, XdmfArray, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfArray_GetClassName(*args)
    def GetTagName(*args): return _Xdmf.XdmfArray_GetTagName(*args)
    def GetCoreLength(*args): return _Xdmf.XdmfArray_GetCoreLength(*args)
    def Allocate(*args): return _Xdmf.XdmfArray_Allocate(*args)
    def SetShape(*args): return _Xdmf.XdmfArray_SetShape(*args)
    def SetShapeFromString(*args): return _Xdmf.XdmfArray_SetShapeFromString(*args)
    def SetNumberOfElements(*args): return _Xdmf.XdmfArray_SetNumberOfElements(*args)
    def ReformFromString(*args): return _Xdmf.XdmfArray_ReformFromString(*args)
    def CopyShape(*args): return _Xdmf.XdmfArray_CopyShape(*args)
    def GetDataPointer(*args): return _Xdmf.XdmfArray_GetDataPointer(*args)
    def SetDataPointer(*args): return _Xdmf.XdmfArray_SetDataPointer(*args)
    def SetValueFromFloat64(*args): return _Xdmf.XdmfArray_SetValueFromFloat64(*args)
    def SetValueFromInt64(*args): return _Xdmf.XdmfArray_SetValueFromInt64(*args)
    def SetValues(*args): return _Xdmf.XdmfArray_SetValues(*args)
    def GetValuesAsInt8(*args): return _Xdmf.XdmfArray_GetValuesAsInt8(*args)
    def SetValuesFromInt8(*args): return _Xdmf.XdmfArray_SetValuesFromInt8(*args)
    def GetValuesAsInt32(*args): return _Xdmf.XdmfArray_GetValuesAsInt32(*args)
    def SetValuesFromInt32(*args): return _Xdmf.XdmfArray_SetValuesFromInt32(*args)
    def GetValuesAsInt64(*args): return _Xdmf.XdmfArray_GetValuesAsInt64(*args)
    def SetValuesFromInt64(*args): return _Xdmf.XdmfArray_SetValuesFromInt64(*args)
    def GetValuesAsFloat32(*args): return _Xdmf.XdmfArray_GetValuesAsFloat32(*args)
    def SetValuesFromFloat32(*args): return _Xdmf.XdmfArray_SetValuesFromFloat32(*args)
    def GetValuesAsFloat64(*args): return _Xdmf.XdmfArray_GetValuesAsFloat64(*args)
    def SetValuesFromFloat64(*args): return _Xdmf.XdmfArray_SetValuesFromFloat64(*args)
    def GetValueAsFloat64(*args): return _Xdmf.XdmfArray_GetValueAsFloat64(*args)
    def GetValueAsFloat32(*args): return _Xdmf.XdmfArray_GetValueAsFloat32(*args)
    def GetValueAsInt64(*args): return _Xdmf.XdmfArray_GetValueAsInt64(*args)
    def GetValueAsInt32(*args): return _Xdmf.XdmfArray_GetValueAsInt32(*args)
    def GetValueAsInt16(*args): return _Xdmf.XdmfArray_GetValueAsInt16(*args)
    def GetValueAsInt8(*args): return _Xdmf.XdmfArray_GetValueAsInt8(*args)
    def GetValues(*args): return _Xdmf.XdmfArray_GetValues(*args)
    def Generate(*args): return _Xdmf.XdmfArray_Generate(*args)
    def Clone(*args): return _Xdmf.XdmfArray_Clone(*args)
    def Reference(*args): return _Xdmf.XdmfArray_Reference(*args)
    def GetMaxAsFloat64(*args): return _Xdmf.XdmfArray_GetMaxAsFloat64(*args)
    def GetMinAsFloat64(*args): return _Xdmf.XdmfArray_GetMinAsFloat64(*args)
    def GetMaxAsInt64(*args): return _Xdmf.XdmfArray_GetMaxAsInt64(*args)
    def GetMinAsInt64(*args): return _Xdmf.XdmfArray_GetMinAsInt64(*args)
    def GetMean(*args): return _Xdmf.XdmfArray_GetMean(*args)
    def SetDataFromChars(*args): return _Xdmf.XdmfArray_SetDataFromChars(*args)
    def GetDataPointerAsCharPointer(*args): return _Xdmf.XdmfArray_GetDataPointerAsCharPointer(*args)

class XdmfArrayPtr(XdmfArray):
    def __init__(self, this):
        _swig_setattr(self, XdmfArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfArray, 'thisown', 0)
        _swig_setattr(self, XdmfArray,self.__class__,XdmfArray)
_Xdmf.XdmfArray_swigregister(XdmfArrayPtr)

class XdmfArrayList(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfArrayList, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfArrayList, name)
    def __repr__(self):
        return "<C XdmfArrayList instance at %s>" % (self.this,)
    __swig_setmethods__["name"] = _Xdmf.XdmfArrayList_name_set
    __swig_getmethods__["name"] = _Xdmf.XdmfArrayList_name_get
    if _newclass:name = property(_Xdmf.XdmfArrayList_name_get, _Xdmf.XdmfArrayList_name_set)
    __swig_setmethods__["timecntr"] = _Xdmf.XdmfArrayList_timecntr_set
    __swig_getmethods__["timecntr"] = _Xdmf.XdmfArrayList_timecntr_get
    if _newclass:timecntr = property(_Xdmf.XdmfArrayList_timecntr_get, _Xdmf.XdmfArrayList_timecntr_set)
    __swig_setmethods__["Array"] = _Xdmf.XdmfArrayList_Array_set
    __swig_getmethods__["Array"] = _Xdmf.XdmfArrayList_Array_get
    if _newclass:Array = property(_Xdmf.XdmfArrayList_Array_get, _Xdmf.XdmfArrayList_Array_set)
    def __init__(self, *args):
        _swig_setattr(self, XdmfArrayList, 'this', _Xdmf.new_XdmfArrayList(*args))
        _swig_setattr(self, XdmfArrayList, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfArrayList):
        try:
            if self.thisown: destroy(self)
        except: pass

class XdmfArrayListPtr(XdmfArrayList):
    def __init__(self, this):
        _swig_setattr(self, XdmfArrayList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfArrayList, 'thisown', 0)
        _swig_setattr(self, XdmfArrayList,self.__class__,XdmfArrayList)
_Xdmf.XdmfArrayList_swigregister(XdmfArrayListPtr)


TagNameToArray = _Xdmf.TagNameToArray

PrintAllXdmfArrays = _Xdmf.PrintAllXdmfArrays

GetNextOlderArray = _Xdmf.GetNextOlderArray

GetCurrentArrayTime = _Xdmf.GetCurrentArrayTime
class XdmfLightData(XdmfObject):
    __swig_setmethods__ = {}
    for _s in [XdmfObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfLightData, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfLightData, name)
    def __repr__(self):
        return "<C XdmfLightData instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfLightData, 'this', _Xdmf.new_XdmfLightData(*args))
        _swig_setattr(self, XdmfLightData, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfLightData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfLightData_GetClassName(*args)
    def GetNdgmHost(*args): return _Xdmf.XdmfLightData_GetNdgmHost(*args)
    def SetNdgmHost(*args): return _Xdmf.XdmfLightData_SetNdgmHost(*args)
    def GetName(*args): return _Xdmf.XdmfLightData_GetName(*args)
    def SetName(*args): return _Xdmf.XdmfLightData_SetName(*args)
    def GetFileName(*args): return _Xdmf.XdmfLightData_GetFileName(*args)
    def SetFileName(*args): return _Xdmf.XdmfLightData_SetFileName(*args)
    def GetWorkingDirectory(*args): return _Xdmf.XdmfLightData_GetWorkingDirectory(*args)
    def SetWorkingDirectory(*args): return _Xdmf.XdmfLightData_SetWorkingDirectory(*args)
    def SetDOM(*args): return _Xdmf.XdmfLightData_SetDOM(*args)
    def GetDOM(*args): return _Xdmf.XdmfLightData_GetDOM(*args)
    def SetCurrentElement(*args): return _Xdmf.XdmfLightData_SetCurrentElement(*args)
    def GetCurrentElement(*args): return _Xdmf.XdmfLightData_GetCurrentElement(*args)
    def HasBeenInited(*args): return _Xdmf.XdmfLightData_HasBeenInited(*args)

class XdmfLightDataPtr(XdmfLightData):
    def __init__(self, this):
        _swig_setattr(self, XdmfLightData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfLightData, 'thisown', 0)
        _swig_setattr(self, XdmfLightData,self.__class__,XdmfLightData)
_Xdmf.XdmfLightData_swigregister(XdmfLightDataPtr)

XDMF_ATTRIBUTE_TYPE_NONE = _Xdmf.XDMF_ATTRIBUTE_TYPE_NONE
XDMF_ATTRIBUTE_TYPE_SCALAR = _Xdmf.XDMF_ATTRIBUTE_TYPE_SCALAR
XDMF_ATTRIBUTE_TYPE_VECTOR = _Xdmf.XDMF_ATTRIBUTE_TYPE_VECTOR
XDMF_ATTRIBUTE_TYPE_TENSOR = _Xdmf.XDMF_ATTRIBUTE_TYPE_TENSOR
XDMF_ATTRIBUTE_TYPE_MATRIX = _Xdmf.XDMF_ATTRIBUTE_TYPE_MATRIX
XDMF_ATTRIBUTE_CENTER_GRID = _Xdmf.XDMF_ATTRIBUTE_CENTER_GRID
XDMF_ATTRIBUTE_CENTER_CELL = _Xdmf.XDMF_ATTRIBUTE_CENTER_CELL
XDMF_ATTRIBUTE_CENTER_FACE = _Xdmf.XDMF_ATTRIBUTE_CENTER_FACE
XDMF_ATTRIBUTE_CENTER_EDGE = _Xdmf.XDMF_ATTRIBUTE_CENTER_EDGE
XDMF_ATTRIBUTE_CENTER_NODE = _Xdmf.XDMF_ATTRIBUTE_CENTER_NODE
class XdmfAttribute(XdmfLightData):
    __swig_setmethods__ = {}
    for _s in [XdmfLightData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfAttribute, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfLightData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfAttribute, name)
    def __repr__(self):
        return "<C XdmfAttribute instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfAttribute, 'this', _Xdmf.new_XdmfAttribute(*args))
        _swig_setattr(self, XdmfAttribute, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfAttribute):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfAttribute_GetClassName(*args)
    def SetAttributeType(*args): return _Xdmf.XdmfAttribute_SetAttributeType(*args)
    def GetAttributeType(*args): return _Xdmf.XdmfAttribute_GetAttributeType(*args)
    def SetAttributeTypeFromString(*args): return _Xdmf.XdmfAttribute_SetAttributeTypeFromString(*args)
    def GetAttributeTypeAsString(*args): return _Xdmf.XdmfAttribute_GetAttributeTypeAsString(*args)
    def SetAttributeCenterFromString(*args): return _Xdmf.XdmfAttribute_SetAttributeCenterFromString(*args)
    def GetAttributeCenterAsString(*args): return _Xdmf.XdmfAttribute_GetAttributeCenterAsString(*args)
    def SetAttributeCenter(*args): return _Xdmf.XdmfAttribute_SetAttributeCenter(*args)
    def GetAttributeCenter(*args): return _Xdmf.XdmfAttribute_GetAttributeCenter(*args)
    def GetShapeDesc(*args): return _Xdmf.XdmfAttribute_GetShapeDesc(*args)
    def SetValues(*args): return _Xdmf.XdmfAttribute_SetValues(*args)
    def GetValues(*args): return _Xdmf.XdmfAttribute_GetValues(*args)
    def InitAttributeFromElement(*args): return _Xdmf.XdmfAttribute_InitAttributeFromElement(*args)
    def SetAttributeFromElement(*args): return _Xdmf.XdmfAttribute_SetAttributeFromElement(*args)
    def Update(*args): return _Xdmf.XdmfAttribute_Update(*args)

class XdmfAttributePtr(XdmfAttribute):
    def __init__(self, this):
        _swig_setattr(self, XdmfAttribute, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfAttribute, 'thisown', 0)
        _swig_setattr(self, XdmfAttribute,self.__class__,XdmfAttribute)
_Xdmf.XdmfAttribute_swigregister(XdmfAttributePtr)

class XdmfCharArray(XdmfArray):
    __swig_setmethods__ = {}
    for _s in [XdmfArray]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfCharArray, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfArray]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfCharArray, name)
    def __repr__(self):
        return "<C XdmfCharArray instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfCharArray, 'this', _Xdmf.new_XdmfCharArray(*args))
        _swig_setattr(self, XdmfCharArray, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfCharArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfCharArray_GetClassName(*args)
    def GetString(*args): return _Xdmf.XdmfCharArray_GetString(*args)
    def SetString(*args): return _Xdmf.XdmfCharArray_SetString(*args)
    def SetFromFile(*args): return _Xdmf.XdmfCharArray_SetFromFile(*args)

class XdmfCharArrayPtr(XdmfCharArray):
    def __init__(self, this):
        _swig_setattr(self, XdmfCharArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfCharArray, 'thisown', 0)
        _swig_setattr(self, XdmfCharArray,self.__class__,XdmfCharArray)
_Xdmf.XdmfCharArray_swigregister(XdmfCharArrayPtr)

class XdmfXNode(XdmfObject):
    __swig_setmethods__ = {}
    for _s in [XdmfObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfXNode, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfXNode, name)
    def __repr__(self):
        return "<C XdmfXNode instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfXNode, 'this', _Xdmf.new_XdmfXNode(*args))
        _swig_setattr(self, XdmfXNode, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfXNode):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Set(*args): return _Xdmf.XdmfXNode_Set(*args)
    def Get(*args): return _Xdmf.XdmfXNode_Get(*args)
    def Print(*args): return _Xdmf.XdmfXNode_Print(*args)
    def GetSize(*args): return _Xdmf.XdmfXNode_GetSize(*args)
    def GetNameByIndex(*args): return _Xdmf.XdmfXNode_GetNameByIndex(*args)
    def GetDataByIndex(*args): return _Xdmf.XdmfXNode_GetDataByIndex(*args)
    def GetClientData(*args): return _Xdmf.XdmfXNode_GetClientData(*args)
    def SetClientData(*args): return _Xdmf.XdmfXNode_SetClientData(*args)
    def ManualDelete(*args): return _Xdmf.XdmfXNode_ManualDelete(*args)

class XdmfXNodePtr(XdmfXNode):
    def __init__(self, this):
        _swig_setattr(self, XdmfXNode, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfXNode, 'thisown', 0)
        _swig_setattr(self, XdmfXNode,self.__class__,XdmfXNode)
_Xdmf.XdmfXNode_swigregister(XdmfXNodePtr)

class XdmfDOM(XdmfObject):
    __swig_setmethods__ = {}
    for _s in [XdmfObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfDOM, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfDOM, name)
    def __repr__(self):
        return "<C XdmfDOM instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfDOM, 'this', _Xdmf.new_XdmfDOM(*args))
        _swig_setattr(self, XdmfDOM, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfDOM):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfDOM_GetClassName(*args)
    def SetXMLVersion(*args): return _Xdmf.XdmfDOM_SetXMLVersion(*args)
    def GetXMLVersion(*args): return _Xdmf.XdmfDOM_GetXMLVersion(*args)
    def SetDocType(*args): return _Xdmf.XdmfDOM_SetDocType(*args)
    def GetDocType(*args): return _Xdmf.XdmfDOM_GetDocType(*args)
    def SetSystem(*args): return _Xdmf.XdmfDOM_SetSystem(*args)
    def GetSystem(*args): return _Xdmf.XdmfDOM_GetSystem(*args)
    def SetInputFileName(*args): return _Xdmf.XdmfDOM_SetInputFileName(*args)
    def SetOutputFileName(*args): return _Xdmf.XdmfDOM_SetOutputFileName(*args)
    def GetInputFileName(*args): return _Xdmf.XdmfDOM_GetInputFileName(*args)
    def GetOutputFileName(*args): return _Xdmf.XdmfDOM_GetOutputFileName(*args)
    def GetOutput(*args): return _Xdmf.XdmfDOM_GetOutput(*args)
    def SetOutput(*args): return _Xdmf.XdmfDOM_SetOutput(*args)
    def GetInput(*args): return _Xdmf.XdmfDOM_GetInput(*args)
    def SetInput(*args): return _Xdmf.XdmfDOM_SetInput(*args)
    def GenerateHead(*args): return _Xdmf.XdmfDOM_GenerateHead(*args)
    def Puts(*args): return _Xdmf.XdmfDOM_Puts(*args)
    def GenerateTail(*args): return _Xdmf.XdmfDOM_GenerateTail(*args)
    def Gets(*args): return _Xdmf.XdmfDOM_Gets(*args)
    def GetTree(*args): return _Xdmf.XdmfDOM_GetTree(*args)
    def __Parse(*args): return _Xdmf.XdmfDOM___Parse(*args)
    def Parse(*args): return _Xdmf.XdmfDOM_Parse(*args)
    def GetRoot(*args): return _Xdmf.XdmfDOM_GetRoot(*args)
    def GetNumberOfChildren(*args): return _Xdmf.XdmfDOM_GetNumberOfChildren(*args)
    def GetChild(*args): return _Xdmf.XdmfDOM_GetChild(*args)
    def GetNumberOfAttributes(*args): return _Xdmf.XdmfDOM_GetNumberOfAttributes(*args)
    def GetAttribute(*args): return _Xdmf.XdmfDOM_GetAttribute(*args)
    def IsChild(*args): return _Xdmf.XdmfDOM_IsChild(*args)
    def Serialize(*args): return _Xdmf.XdmfDOM_Serialize(*args)
    def Insert(*args): return _Xdmf.XdmfDOM_Insert(*args)
    def InsertFromString(*args): return _Xdmf.XdmfDOM_InsertFromString(*args)
    def DeleteNode(*args): return _Xdmf.XdmfDOM_DeleteNode(*args)
    def FindElement(*args): return _Xdmf.XdmfDOM_FindElement(*args)
    def FindElementByAttribute(*args): return _Xdmf.XdmfDOM_FindElementByAttribute(*args)
    def FindNumberOfElements(*args): return _Xdmf.XdmfDOM_FindNumberOfElements(*args)
    def FindNumberOfElementsByAttribute(*args): return _Xdmf.XdmfDOM_FindNumberOfElementsByAttribute(*args)
    def FindProcessingInstruction(*args): return _Xdmf.XdmfDOM_FindProcessingInstruction(*args)
    def FindNumberOfProcessingInstructions(*args): return _Xdmf.XdmfDOM_FindNumberOfProcessingInstructions(*args)
    def GetNdgmHost(*args): return _Xdmf.XdmfDOM_GetNdgmHost(*args)
    def SetNdgmHost(*args): return _Xdmf.XdmfDOM_SetNdgmHost(*args)
    def GetWorkingDirectory(*args): return _Xdmf.XdmfDOM_GetWorkingDirectory(*args)
    def SetWorkingDirectory(*args): return _Xdmf.XdmfDOM_SetWorkingDirectory(*args)
    def Get(*args): return _Xdmf.XdmfDOM_Get(*args)
    def SetUserData(*args): return _Xdmf.XdmfDOM_SetUserData(*args)
    def GetUserData(*args): return _Xdmf.XdmfDOM_GetUserData(*args)
    def Set(*args): return _Xdmf.XdmfDOM_Set(*args)
    def GetNumberType(*args): return _Xdmf.XdmfDOM_GetNumberType(*args)
    def FindNumberOfParameters(*args): return _Xdmf.XdmfDOM_FindNumberOfParameters(*args)
    def FindParameter(*args): return _Xdmf.XdmfDOM_FindParameter(*args)
    def GetParameter(*args): return _Xdmf.XdmfDOM_GetParameter(*args)

class XdmfDOMPtr(XdmfDOM):
    def __init__(self, this):
        _swig_setattr(self, XdmfDOM, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfDOM, 'thisown', 0)
        _swig_setattr(self, XdmfDOM,self.__class__,XdmfDOM)
_Xdmf.XdmfDOM_swigregister(XdmfDOMPtr)


GetXNodeSize = _Xdmf.GetXNodeSize

GetXNodeName = _Xdmf.GetXNodeName

GetXNodeData = _Xdmf.GetXNodeData

HandleToXdmfDOM = _Xdmf.HandleToXdmfDOM

XdmfArrayExpr = _Xdmf.XdmfArrayExpr

XdmfScalarExpr = _Xdmf.XdmfScalarExpr

XdmfExpr = _Xdmf.XdmfExpr
class XdmfFormat(XdmfLightData):
    __swig_setmethods__ = {}
    for _s in [XdmfLightData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfFormat, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfLightData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfFormat, name)
    def __repr__(self):
        return "<C XdmfFormat instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfFormat, 'this', _Xdmf.new_XdmfFormat(*args))
        _swig_setattr(self, XdmfFormat, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfFormat):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfFormat_GetClassName(*args)
    def SetFormat(*args): return _Xdmf.XdmfFormat_SetFormat(*args)
    def GetFormat(*args): return _Xdmf.XdmfFormat_GetFormat(*args)
    def ElementToDataDesc(*args): return _Xdmf.XdmfFormat_ElementToDataDesc(*args)
    def ElementToCompoundDataDesc(*args): return _Xdmf.XdmfFormat_ElementToCompoundDataDesc(*args)
    def DataDescToElement(*args): return _Xdmf.XdmfFormat_DataDescToElement(*args)

class XdmfFormatPtr(XdmfFormat):
    def __init__(self, this):
        _swig_setattr(self, XdmfFormat, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfFormat, 'thisown', 0)
        _swig_setattr(self, XdmfFormat,self.__class__,XdmfFormat)
_Xdmf.XdmfFormat_swigregister(XdmfFormatPtr)

class XdmfFormatHDF(XdmfFormat):
    __swig_setmethods__ = {}
    for _s in [XdmfFormat]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfFormatHDF, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfFormat]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfFormatHDF, name)
    def __repr__(self):
        return "<C XdmfFormatHDF instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfFormatHDF, 'this', _Xdmf.new_XdmfFormatHDF(*args))
        _swig_setattr(self, XdmfFormatHDF, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfFormatHDF):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfFormatHDF_GetClassName(*args)
    def ElementToArray(*args): return _Xdmf.XdmfFormatHDF_ElementToArray(*args)
    def ArrayToElement(*args): return _Xdmf.XdmfFormatHDF_ArrayToElement(*args)

class XdmfFormatHDFPtr(XdmfFormatHDF):
    def __init__(self, this):
        _swig_setattr(self, XdmfFormatHDF, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfFormatHDF, 'thisown', 0)
        _swig_setattr(self, XdmfFormatHDF,self.__class__,XdmfFormatHDF)
_Xdmf.XdmfFormatHDF_swigregister(XdmfFormatHDFPtr)

class XdmfFormatMulti(XdmfFormat):
    __swig_setmethods__ = {}
    for _s in [XdmfFormat]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfFormatMulti, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfFormat]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfFormatMulti, name)
    def __repr__(self):
        return "<C XdmfFormatMulti instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfFormatMulti, 'this', _Xdmf.new_XdmfFormatMulti(*args))
        _swig_setattr(self, XdmfFormatMulti, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfFormatMulti):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfFormatMulti_GetClassName(*args)
    def SetFormatFromElement(*args): return _Xdmf.XdmfFormatMulti_SetFormatFromElement(*args)
    def ElementToArray(*args): return _Xdmf.XdmfFormatMulti_ElementToArray(*args)
    def ArrayToElement(*args): return _Xdmf.XdmfFormatMulti_ArrayToElement(*args)

class XdmfFormatMultiPtr(XdmfFormatMulti):
    def __init__(self, this):
        _swig_setattr(self, XdmfFormatMulti, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfFormatMulti, 'thisown', 0)
        _swig_setattr(self, XdmfFormatMulti,self.__class__,XdmfFormatMulti)
_Xdmf.XdmfFormatMulti_swigregister(XdmfFormatMultiPtr)

class XdmfFormatXML(XdmfFormat):
    __swig_setmethods__ = {}
    for _s in [XdmfFormat]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfFormatXML, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfFormat]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfFormatXML, name)
    def __repr__(self):
        return "<C XdmfFormatXML instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfFormatXML, 'this', _Xdmf.new_XdmfFormatXML(*args))
        _swig_setattr(self, XdmfFormatXML, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfFormatXML):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfFormatXML_GetClassName(*args)
    def ElementToArray(*args): return _Xdmf.XdmfFormatXML_ElementToArray(*args)
    def ArrayToElement(*args): return _Xdmf.XdmfFormatXML_ArrayToElement(*args)

class XdmfFormatXMLPtr(XdmfFormatXML):
    def __init__(self, this):
        _swig_setattr(self, XdmfFormatXML, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfFormatXML, 'thisown', 0)
        _swig_setattr(self, XdmfFormatXML,self.__class__,XdmfFormatXML)
_Xdmf.XdmfFormatXML_swigregister(XdmfFormatXMLPtr)

XDMF_MAX_ORDER = _Xdmf.XDMF_MAX_ORDER
XDMF_STRUCTURED = _Xdmf.XDMF_STRUCTURED
XDMF_UNSTRUCTURED = _Xdmf.XDMF_UNSTRUCTURED
XDMF_NOTOPOLOGY = _Xdmf.XDMF_NOTOPOLOGY
XDMF_POLYVERTEX = _Xdmf.XDMF_POLYVERTEX
XDMF_POLYLINE = _Xdmf.XDMF_POLYLINE
XDMF_POLYGON = _Xdmf.XDMF_POLYGON
XDMF_TRI = _Xdmf.XDMF_TRI
XDMF_QUAD = _Xdmf.XDMF_QUAD
XDMF_TET = _Xdmf.XDMF_TET
XDMF_PYRAMID = _Xdmf.XDMF_PYRAMID
XDMF_WEDGE = _Xdmf.XDMF_WEDGE
XDMF_HEX = _Xdmf.XDMF_HEX
XDMF_2DSMESH = _Xdmf.XDMF_2DSMESH
XDMF_2DRECTMESH = _Xdmf.XDMF_2DRECTMESH
XDMF_2DCORECTMESH = _Xdmf.XDMF_2DCORECTMESH
XDMF_3DSMESH = _Xdmf.XDMF_3DSMESH
XDMF_3DRECTMESH = _Xdmf.XDMF_3DRECTMESH
XDMF_3DCORECTMESH = _Xdmf.XDMF_3DCORECTMESH
class XdmfTopology(XdmfLightData):
    __swig_setmethods__ = {}
    for _s in [XdmfLightData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfTopology, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfLightData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfTopology, name)
    def __repr__(self):
        return "<C XdmfTopology instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfTopology, 'this', _Xdmf.new_XdmfTopology(*args))
        _swig_setattr(self, XdmfTopology, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfTopology):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfTopology_GetClassName(*args)
    def GetClass(*args): return _Xdmf.XdmfTopology_GetClass(*args)
    def GetClassAsString(*args): return _Xdmf.XdmfTopology_GetClassAsString(*args)
    def SetTopologyType(*args): return _Xdmf.XdmfTopology_SetTopologyType(*args)
    def SetTopologyTypeFromString(*args): return _Xdmf.XdmfTopology_SetTopologyTypeFromString(*args)
    def GetTopologyType(*args): return _Xdmf.XdmfTopology_GetTopologyType(*args)
    def GetTopologyTypeAsString(*args): return _Xdmf.XdmfTopology_GetTopologyTypeAsString(*args)
    def GetOrder(*args): return _Xdmf.XdmfTopology_GetOrder(*args)
    def GetOrderAsString(*args): return _Xdmf.XdmfTopology_GetOrderAsString(*args)
    def SetOrderFromString(*args): return _Xdmf.XdmfTopology_SetOrderFromString(*args)
    def SetOrder(*args): return _Xdmf.XdmfTopology_SetOrder(*args)
    def SetNodesPerElement(*args): return _Xdmf.XdmfTopology_SetNodesPerElement(*args)
    def GetNodesPerElement(*args): return _Xdmf.XdmfTopology_GetNodesPerElement(*args)
    def SetBaseOffset(*args): return _Xdmf.XdmfTopology_SetBaseOffset(*args)
    def GetBaseOffset(*args): return _Xdmf.XdmfTopology_GetBaseOffset(*args)
    def GetShapeDesc(*args): return _Xdmf.XdmfTopology_GetShapeDesc(*args)
    def GetNumberOfElements(*args): return _Xdmf.XdmfTopology_GetNumberOfElements(*args)
    def InitTopologyFromElement(*args): return _Xdmf.XdmfTopology_InitTopologyFromElement(*args)
    def SetTopologyFromElement(*args): return _Xdmf.XdmfTopology_SetTopologyFromElement(*args)
    def GetConnectivity(*args): return _Xdmf.XdmfTopology_GetConnectivity(*args)
    def SetConnectivity(*args): return _Xdmf.XdmfTopology_SetConnectivity(*args)
    def Update(*args): return _Xdmf.XdmfTopology_Update(*args)

class XdmfTopologyPtr(XdmfTopology):
    def __init__(self, this):
        _swig_setattr(self, XdmfTopology, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfTopology, 'thisown', 0)
        _swig_setattr(self, XdmfTopology,self.__class__,XdmfTopology)
_Xdmf.XdmfTopology_swigregister(XdmfTopologyPtr)


HandleToXdmfTopology = _Xdmf.HandleToXdmfTopology
XDMF_GEOMETRY_NONE = _Xdmf.XDMF_GEOMETRY_NONE
XDMF_GEOMETRY_XYZ = _Xdmf.XDMF_GEOMETRY_XYZ
XDMF_GEOMETRY_XY = _Xdmf.XDMF_GEOMETRY_XY
XDMF_GEOMETRY_X_Y_Z = _Xdmf.XDMF_GEOMETRY_X_Y_Z
XDMF_GEOMETRY_VXVYVZ = _Xdmf.XDMF_GEOMETRY_VXVYVZ
XDMF_GEOMETRY_ORIGIN_DXDYDZ = _Xdmf.XDMF_GEOMETRY_ORIGIN_DXDYDZ
class XdmfGeometry(XdmfLightData):
    __swig_setmethods__ = {}
    for _s in [XdmfLightData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfGeometry, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfLightData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfGeometry, name)
    def __repr__(self):
        return "<C XdmfGeometry instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfGeometry, 'this', _Xdmf.new_XdmfGeometry(*args))
        _swig_setattr(self, XdmfGeometry, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfGeometry):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfGeometry_GetClassName(*args)
    def SetNumberOfPoints(*args): return _Xdmf.XdmfGeometry_SetNumberOfPoints(*args)
    def GetNumberOfPoints(*args): return _Xdmf.XdmfGeometry_GetNumberOfPoints(*args)
    def SetGeometryType(*args): return _Xdmf.XdmfGeometry_SetGeometryType(*args)
    def GetGeometryType(*args): return _Xdmf.XdmfGeometry_GetGeometryType(*args)
    def GetGeometryTypeAsString(*args): return _Xdmf.XdmfGeometry_GetGeometryTypeAsString(*args)
    def SetGeometryTypeFromString(*args): return _Xdmf.XdmfGeometry_SetGeometryTypeFromString(*args)
    def InitGeometryFromElement(*args): return _Xdmf.XdmfGeometry_InitGeometryFromElement(*args)
    def SetGeometryFromElement(*args): return _Xdmf.XdmfGeometry_SetGeometryFromElement(*args)
    def SetPoints(*args): return _Xdmf.XdmfGeometry_SetPoints(*args)
    def GetPoints(*args): return _Xdmf.XdmfGeometry_GetPoints(*args)
    def GetOrigin(*args): return _Xdmf.XdmfGeometry_GetOrigin(*args)
    def GetOriginX(*args): return _Xdmf.XdmfGeometry_GetOriginX(*args)
    def GetOriginY(*args): return _Xdmf.XdmfGeometry_GetOriginY(*args)
    def GetOriginZ(*args): return _Xdmf.XdmfGeometry_GetOriginZ(*args)
    def SetOrigin(*args): return _Xdmf.XdmfGeometry_SetOrigin(*args)
    def SetDxDyDz(*args): return _Xdmf.XdmfGeometry_SetDxDyDz(*args)
    def GetDx(*args): return _Xdmf.XdmfGeometry_GetDx(*args)
    def GetDy(*args): return _Xdmf.XdmfGeometry_GetDy(*args)
    def GetDz(*args): return _Xdmf.XdmfGeometry_GetDz(*args)
    def GetDxDyDz(*args): return _Xdmf.XdmfGeometry_GetDxDyDz(*args)
    def GetVectorX(*args): return _Xdmf.XdmfGeometry_GetVectorX(*args)
    def GetVectorY(*args): return _Xdmf.XdmfGeometry_GetVectorY(*args)
    def GetVectorZ(*args): return _Xdmf.XdmfGeometry_GetVectorZ(*args)
    def SetVectorX(*args): return _Xdmf.XdmfGeometry_SetVectorX(*args)
    def SetVectorY(*args): return _Xdmf.XdmfGeometry_SetVectorY(*args)
    def SetVectorZ(*args): return _Xdmf.XdmfGeometry_SetVectorZ(*args)
    def HasData(*args): return _Xdmf.XdmfGeometry_HasData(*args)
    def Update(*args): return _Xdmf.XdmfGeometry_Update(*args)

class XdmfGeometryPtr(XdmfGeometry):
    def __init__(self, this):
        _swig_setattr(self, XdmfGeometry, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfGeometry, 'thisown', 0)
        _swig_setattr(self, XdmfGeometry,self.__class__,XdmfGeometry)
_Xdmf.XdmfGeometry_swigregister(XdmfGeometryPtr)


GetXdmfGeometryHandle = _Xdmf.GetXdmfGeometryHandle
class XdmfGrid(XdmfTopology):
    __swig_setmethods__ = {}
    for _s in [XdmfTopology]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfGrid, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfTopology]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfGrid, name)
    def __repr__(self):
        return "<C XdmfGrid instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfGrid, 'this', _Xdmf.new_XdmfGrid(*args))
        _swig_setattr(self, XdmfGrid, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfGrid):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfGrid_GetClassName(*args)
    def SetGeometry(*args): return _Xdmf.XdmfGrid_SetGeometry(*args)
    def GetGeometry(*args): return _Xdmf.XdmfGrid_GetGeometry(*args)
    def GetNumberOfAttributes(*args): return _Xdmf.XdmfGrid_GetNumberOfAttributes(*args)
    def GetAttribute(*args): return _Xdmf.XdmfGrid_GetAttribute(*args)
    def AssignAttribute(*args): return _Xdmf.XdmfGrid_AssignAttribute(*args)
    def AssignAttributeByIndex(*args): return _Xdmf.XdmfGrid_AssignAttributeByIndex(*args)
    def AssignAttributeByName(*args): return _Xdmf.XdmfGrid_AssignAttributeByName(*args)
    def GetAssignedAttribute(*args): return _Xdmf.XdmfGrid_GetAssignedAttribute(*args)
    def GetAssignedAttributeIndex(*args): return _Xdmf.XdmfGrid_GetAssignedAttributeIndex(*args)
    def InitGridFromElement(*args): return _Xdmf.XdmfGrid_InitGridFromElement(*args)
    def SetGridFromElement(*args): return _Xdmf.XdmfGrid_SetGridFromElement(*args)
    def Update(*args): return _Xdmf.XdmfGrid_Update(*args)

class XdmfGridPtr(XdmfGrid):
    def __init__(self, this):
        _swig_setattr(self, XdmfGrid, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfGrid, 'thisown', 0)
        _swig_setattr(self, XdmfGrid,self.__class__,XdmfGrid)
_Xdmf.XdmfGrid_swigregister(XdmfGridPtr)


HandleToXdmfGrid = _Xdmf.HandleToXdmfGrid
class XdmfHeavyData(XdmfDataDesc):
    __swig_setmethods__ = {}
    for _s in [XdmfDataDesc]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfHeavyData, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfDataDesc]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfHeavyData, name)
    def __repr__(self):
        return "<C XdmfHeavyData instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfHeavyData, 'this', _Xdmf.new_XdmfHeavyData(*args))
        _swig_setattr(self, XdmfHeavyData, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfHeavyData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfHeavyData_GetClassName(*args)
    def GetNdgmHost(*args): return _Xdmf.XdmfHeavyData_GetNdgmHost(*args)
    def SetNdgmHost(*args): return _Xdmf.XdmfHeavyData_SetNdgmHost(*args)
    def GetWorkingDirectory(*args): return _Xdmf.XdmfHeavyData_GetWorkingDirectory(*args)
    def SetWorkingDirectory(*args): return _Xdmf.XdmfHeavyData_SetWorkingDirectory(*args)
    def GetDomain(*args): return _Xdmf.XdmfHeavyData_GetDomain(*args)
    def SetDomain(*args): return _Xdmf.XdmfHeavyData_SetDomain(*args)
    def GetFileName(*args): return _Xdmf.XdmfHeavyData_GetFileName(*args)
    def SetFileName(*args): return _Xdmf.XdmfHeavyData_SetFileName(*args)
    def GetPath(*args): return _Xdmf.XdmfHeavyData_GetPath(*args)
    def SetPath(*args): return _Xdmf.XdmfHeavyData_SetPath(*args)
    def GetAccess(*args): return _Xdmf.XdmfHeavyData_GetAccess(*args)
    def SetAccess(*args): return _Xdmf.XdmfHeavyData_SetAccess(*args)

class XdmfHeavyDataPtr(XdmfHeavyData):
    def __init__(self, this):
        _swig_setattr(self, XdmfHeavyData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfHeavyData, 'thisown', 0)
        _swig_setattr(self, XdmfHeavyData,self.__class__,XdmfHeavyData)
_Xdmf.XdmfHeavyData_swigregister(XdmfHeavyDataPtr)

XDMF_H5_OTHER = _Xdmf.XDMF_H5_OTHER
class XdmfHDF(XdmfHeavyData):
    __swig_setmethods__ = {}
    for _s in [XdmfHeavyData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfHDF, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfHeavyData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfHDF, name)
    def __repr__(self):
        return "<C XdmfHDF instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfHDF, 'this', _Xdmf.new_XdmfHDF(*args))
        _swig_setattr(self, XdmfHDF, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfHDF):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfHDF_GetClassName(*args)
    def SetCompression(*args): return _Xdmf.XdmfHDF_SetCompression(*args)
    def GetCompression(*args): return _Xdmf.XdmfHDF_GetCompression(*args)
    def SetUseSerialFile(*args): return _Xdmf.XdmfHDF_SetUseSerialFile(*args)
    def GetUseSerialFile(*args): return _Xdmf.XdmfHDF_GetUseSerialFile(*args)
    def SetCwdName(*args): return _Xdmf.XdmfHDF_SetCwdName(*args)
    def GetCwdName(*args): return _Xdmf.XdmfHDF_GetCwdName(*args)
    def Cd(*args): return _Xdmf.XdmfHDF_Cd(*args)
    def Mkdir(*args): return _Xdmf.XdmfHDF_Mkdir(*args)
    def GetNumberOfChildren(*args): return _Xdmf.XdmfHDF_GetNumberOfChildren(*args)
    def GetChild(*args): return _Xdmf.XdmfHDF_GetChild(*args)
    def SetNextChild(*args): return _Xdmf.XdmfHDF_SetNextChild(*args)
    def Info(*args): return _Xdmf.XdmfHDF_Info(*args)
    def GetChildType(*args): return _Xdmf.XdmfHDF_GetChildType(*args)
    def GetChildTypeAsString(*args): return _Xdmf.XdmfHDF_GetChildTypeAsString(*args)
    def CreateDataset(*args): return _Xdmf.XdmfHDF_CreateDataset(*args)
    def OpenDataset(*args): return _Xdmf.XdmfHDF_OpenDataset(*args)
    def Open(*args): return _Xdmf.XdmfHDF_Open(*args)
    def Read(*args): return _Xdmf.XdmfHDF_Read(*args)
    def Write(*args): return _Xdmf.XdmfHDF_Write(*args)
    def Close(*args): return _Xdmf.XdmfHDF_Close(*args)

class XdmfHDFPtr(XdmfHDF):
    def __init__(self, this):
        _swig_setattr(self, XdmfHDF, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfHDF, 'thisown', 0)
        _swig_setattr(self, XdmfHDF,self.__class__,XdmfHDF)
_Xdmf.XdmfHDF_swigregister(XdmfHDFPtr)


CopyArray = _Xdmf.CopyArray
XDMF_PARAMETER_RANGE_TYPE = _Xdmf.XDMF_PARAMETER_RANGE_TYPE
XDMF_PARAMETER_LIST_TYPE = _Xdmf.XDMF_PARAMETER_LIST_TYPE
class XdmfParameter(XdmfObject):
    __swig_setmethods__ = {}
    for _s in [XdmfObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfParameter, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfParameter, name)
    def __repr__(self):
        return "<C XdmfParameter instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfParameter, 'this', _Xdmf.new_XdmfParameter(*args))
        _swig_setattr(self, XdmfParameter, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfParameter):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfParameter_GetClassName(*args)
    def GetParameterType(*args): return _Xdmf.XdmfParameter_GetParameterType(*args)
    def GetParameterTypeAsString(*args): return _Xdmf.XdmfParameter_GetParameterTypeAsString(*args)
    def SetParameterType(*args): return _Xdmf.XdmfParameter_SetParameterType(*args)
    def GetCurrentIndex(*args): return _Xdmf.XdmfParameter_GetCurrentIndex(*args)
    def SetCurrentIndex(*args): return _Xdmf.XdmfParameter_SetCurrentIndex(*args)
    def GetParameterName(*args): return _Xdmf.XdmfParameter_GetParameterName(*args)
    def SetParameterName(*args): return _Xdmf.XdmfParameter_SetParameterName(*args)
    def GetParameterNode(*args): return _Xdmf.XdmfParameter_GetParameterNode(*args)
    def SetParameterNode(*args): return _Xdmf.XdmfParameter_SetParameterNode(*args)
    def GetFormat(*args): return _Xdmf.XdmfParameter_GetFormat(*args)
    def GetNumberOfElements(*args): return _Xdmf.XdmfParameter_GetNumberOfElements(*args)
    def GetArray(*args): return _Xdmf.XdmfParameter_GetArray(*args)
    def GetParameterValue(*args): return _Xdmf.XdmfParameter_GetParameterValue(*args)
    def Update(*args): return _Xdmf.XdmfParameter_Update(*args)
    def Substitute(*args): return _Xdmf.XdmfParameter_Substitute(*args)

class XdmfParameterPtr(XdmfParameter):
    def __init__(self, this):
        _swig_setattr(self, XdmfParameter, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfParameter, 'thisown', 0)
        _swig_setattr(self, XdmfParameter,self.__class__,XdmfParameter)
_Xdmf.XdmfParameter_swigregister(XdmfParameterPtr)

class XdmfTransform(XdmfLightData):
    __swig_setmethods__ = {}
    for _s in [XdmfLightData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfTransform, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfLightData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfTransform, name)
    def __repr__(self):
        return "<C XdmfTransform instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfTransform, 'this', _Xdmf.new_XdmfTransform(*args))
        _swig_setattr(self, XdmfTransform, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfTransform):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfTransform_GetClassName(*args)
    def SetTransform(*args): return _Xdmf.XdmfTransform_SetTransform(*args)
    def GetTransform(*args): return _Xdmf.XdmfTransform_GetTransform(*args)
    def ElementToDataDesc(*args): return _Xdmf.XdmfTransform_ElementToDataDesc(*args)
    def ElementToArray(*args): return _Xdmf.XdmfTransform_ElementToArray(*args)
    def DataDescToElement(*args): return _Xdmf.XdmfTransform_DataDescToElement(*args)
    def ArrayToElement(*args): return _Xdmf.XdmfTransform_ArrayToElement(*args)

class XdmfTransformPtr(XdmfTransform):
    def __init__(self, this):
        _swig_setattr(self, XdmfTransform, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfTransform, 'thisown', 0)
        _swig_setattr(self, XdmfTransform,self.__class__,XdmfTransform)
_Xdmf.XdmfTransform_swigregister(XdmfTransformPtr)

XDMF_NDGM_MSG_SERVER = _Xdmf.XDMF_NDGM_MSG_SERVER
XDMF_NDGM_MSG_CLIENT = _Xdmf.XDMF_NDGM_MSG_CLIENT
XDMF_NDGM_CLIENT = _Xdmf.XDMF_NDGM_CLIENT
class XdmfNDGM(XdmfDataDesc):
    __swig_setmethods__ = {}
    for _s in [XdmfDataDesc]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, XdmfNDGM, name, value)
    __swig_getmethods__ = {}
    for _s in [XdmfDataDesc]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, XdmfNDGM, name)
    def __repr__(self):
        return "<C XdmfNDGM instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, XdmfNDGM, 'this', _Xdmf.new_XdmfNDGM(*args))
        _swig_setattr(self, XdmfNDGM, 'thisown', 1)
    def __del__(self, destroy=_Xdmf.delete_XdmfNDGM):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetClassName(*args): return _Xdmf.XdmfNDGM_GetClassName(*args)
    def GetNdgmHost(*args): return _Xdmf.XdmfNDGM_GetNdgmHost(*args)
    def SetNdgmHost(*args): return _Xdmf.XdmfNDGM_SetNdgmHost(*args)
    def GetNdgmDriver(*args): return _Xdmf.XdmfNDGM_GetNdgmDriver(*args)
    def SetNdgmDriver(*args): return _Xdmf.XdmfNDGM_SetNdgmDriver(*args)
    def GetNdgmUid(*args): return _Xdmf.XdmfNDGM_GetNdgmUid(*args)
    def SetNdgmUid(*args): return _Xdmf.XdmfNDGM_SetNdgmUid(*args)
    def GetNdgmMode(*args): return _Xdmf.XdmfNDGM_GetNdgmMode(*args)
    def SetNdgmMode(*args): return _Xdmf.XdmfNDGM_SetNdgmMode(*args)
    def SetModeToMsgServer(*args): return _Xdmf.XdmfNDGM_SetModeToMsgServer(*args)
    def SetModeToMsgClient(*args): return _Xdmf.XdmfNDGM_SetModeToMsgClient(*args)
    def SetModeToClient(*args): return _Xdmf.XdmfNDGM_SetModeToClient(*args)
    def GetNdgmNode(*args): return _Xdmf.XdmfNDGM_GetNdgmNode(*args)
    def SetNdgmNode(*args): return _Xdmf.XdmfNDGM_SetNdgmNode(*args)
    def GetNdgmClient(*args): return _Xdmf.XdmfNDGM_GetNdgmClient(*args)
    def SetNdgmClient(*args): return _Xdmf.XdmfNDGM_SetNdgmClient(*args)
    def GetTotalLength(*args): return _Xdmf.XdmfNDGM_GetTotalLength(*args)
    def GetLastAddress(*args): return _Xdmf.XdmfNDGM_GetLastAddress(*args)
    def Clear(*args): return _Xdmf.XdmfNDGM_Clear(*args)
    def Open(*args): return _Xdmf.XdmfNDGM_Open(*args)
    def Check(*args): return _Xdmf.XdmfNDGM_Check(*args)
    def Close(*args): return _Xdmf.XdmfNDGM_Close(*args)
    def Put(*args): return _Xdmf.XdmfNDGM_Put(*args)
    def Get(*args): return _Xdmf.XdmfNDGM_Get(*args)
    def Send(*args): return _Xdmf.XdmfNDGM_Send(*args)
    def Recv(*args): return _Xdmf.XdmfNDGM_Recv(*args)
    def BarrierInit(*args): return _Xdmf.XdmfNDGM_BarrierInit(*args)
    def BarrierWait(*args): return _Xdmf.XdmfNDGM_BarrierWait(*args)
    def BarrierAudit(*args): return _Xdmf.XdmfNDGM_BarrierAudit(*args)
    def BarrierPoll(*args): return _Xdmf.XdmfNDGM_BarrierPoll(*args)

class XdmfNDGMPtr(XdmfNDGM):
    def __init__(self, this):
        _swig_setattr(self, XdmfNDGM, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XdmfNDGM, 'thisown', 0)
        _swig_setattr(self, XdmfNDGM,self.__class__,XdmfNDGM)
_Xdmf.XdmfNDGM_swigregister(XdmfNDGMPtr)


XdmfGetNdgmEntries = _Xdmf.XdmfGetNdgmEntries

XdmfDeleteAllNdgmEntries = _Xdmf.XdmfDeleteAllNdgmEntries

XdmfAddNdgmEntry = _Xdmf.XdmfAddNdgmEntry

