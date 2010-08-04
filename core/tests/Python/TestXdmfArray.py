from XdmfCore import *

if __name__ == "__main__":

	values1 = [0, 1, 2, 3]
	array1 = XdmfArray.New()
	array1.copyValuesAsInt8(0, values1)
	print array1.getValuesString()
	assert array1.getArrayType() == XdmfArrayType.Int8()

	array2 = XdmfArray.New()
	array2.copyValuesAsInt16(0, values1)
	print array2.getValuesString()
	assert array2.getArrayType() == XdmfArrayType.Int16()

	array3 = XdmfArray.New()
	array3.copyValuesAsInt32(0, values1[2:4])
	print array3.getValuesString()
	assert array3.getArrayType() == XdmfArrayType.Int32()

	# Stride
	array4 = XdmfArray.New()
	array4.copyValuesAsInt64(0, values1[0:4:2])
	print array4.getValuesString()
	assert array4.getArrayType() == XdmfArrayType.Int64()

	# Reverse
	array5 = XdmfArray.New()
	array5.copyValuesAsFloat32(0, values1[::-1])
	print array5.getValuesString()
	assert array5.getArrayType() == XdmfArrayType.Float32()

	array6 = XdmfArray.New()
	array6.copyValuesAsFloat64(0, values1)
	print array6.getValuesString()
	assert array6.getArrayType() == XdmfArrayType.Float64()
	
	array7 = XdmfArray.New()
	array7.copyValuesAsUInt8(0, values1)
	print array7.getValuesString()
	assert array7.getArrayType() == XdmfArrayType.UInt8()
	
	array8 = XdmfArray.New()
	array8.copyValuesAsUInt16(0, values1)
	print array8.getValuesString()
	assert array8.getArrayType() == XdmfArrayType.UInt16()

	array9 = XdmfArray.New()
	array9.copyValuesAsUInt32(0, values1)
	print array9.getValuesString()
	assert array9.getArrayType() == XdmfArrayType.UInt32()
