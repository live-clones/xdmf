from XdmfCore import *

if __name__ == "__main__":

    type1 = XdmfArrayType.Int16()
    type2 = XdmfArrayType.Int16()
    type3 = XdmfArrayType.Int32()

    print("IsEqual True")
    assert type1 == type2
    print("IsEqual False")
    assert not type1 == type3

