from Xdmf import *

if __name__ == "__main__":

	values1 = [0, 1, 2, 3]
	array1 = XdmfArray.New()
	array1.copyValuesAsChar(0, values1)
	print array1.getValuesString()
	print array1.getType() + " " + str(array1.getPrecision())
	assert array1.getType() == "Char"
	assert array1.getPrecision() == 1

	array2 = XdmfArray.New()
	array2.copyValuesAsShort(0, values1)
	print array2.getValuesString()
	print array2.getType() + " " + str(array2.getPrecision())
	assert array2.getType() == "Short"
	assert array2.getPrecision() == 2

	array3 = XdmfArray.New()
	array3.copyValuesAsInt(0, values1[2:4])
	print array3.getValuesString()
	print array3.getType() + " " + str(array3.getPrecision())
	assert array3.getType() == "Int"
	assert array3.getPrecision() == 4

	# Stride
	array4 = XdmfArray.New()
	array4.copyValuesAsLong(0, values1[0:4:2])
	print array4.getValuesString()
	print array4.getType() + " " + str(array4.getPrecision())
	assert array4.getType() == "Int"
	assert array4.getPrecision() == 8

	# Reverse
	array5 = XdmfArray.New()
	array5.copyValuesAsFloat(0, values1[::-1])
	print array5.getValuesString()
	print array5.getType() + " " + str(array5.getPrecision())
	assert array5.getType() == "Float"
	assert array5.getPrecision() == 4

	array6 = XdmfArray.New()
	array6.copyValuesAsDouble(0, values1)
	print array6.getValuesString()
	print array6.getType() + " " + str(array6.getPrecision())
	assert array6.getType() == "Float"
	assert array6.getPrecision() == 8
	
	array7 = XdmfArray.New()
	array7.copyValuesAsUChar(0, values1)
	print array7.getValuesString()
	print array7.getType() + " " + str(array7.getPrecision())
	assert array7.getType() == "UChar"
	assert array7.getPrecision() == 1
	
	array8 = XdmfArray.New()
	array8.copyValuesAsUShort(0, values1)
	print array8.getValuesString()
	print array8.getType() + " " + str(array8.getPrecision())
	assert array8.getType() == "UShort"
	assert array8.getPrecision() == 2

	array9 = XdmfArray.New()
	array9.copyValuesAsUInt(0, values1)
	print array9.getValuesString()
	print array9.getType() + " " + str(array9.getPrecision())
	assert array9.getType() == "UInt"
	assert array9.getPrecision() == 4
