import Xdmf
from Xdmf import *

if __name__ == '__main__':   
    array = Xdmf.XdmfArray()
    array.SetNumberType(Xdmf.XDMF_INT64_TYPE)
    assert(array.GetNumberType() == Xdmf.XDMF_INT64_TYPE)
    array.SetShapeFromString("3 3")
    assert(array.GetShapeAsString() == "3 3")
    assert(array.GetNumberOfElements() == 9)
    toWrite = [0,1,2,3,4,500,5000,500000,1000000000000]
    i = 0;
    for element in toWrite:
        array.SetValueFromInt64(i,element)
        i += 1
    
    i=0
    for element in toWrite:
        assert(array.GetValueAsInt64(i) == element)
        i += 1
    
    assert (array.GetMaxAsInt64() == 1000000000000)
    assert (array.GetMinAsInt64() == 0)
