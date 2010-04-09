from Xdmf import *

if __name__ == "__main__":

	values1 = [0, 1, 2, 3]
	array1 = XdmfArray.New()
	array1.copyValues(IntVector(values1))
	print array1.getValues()
	print array1.getType()

	array1.copyValues(IntVector(values1[0:2]))
	print array1.getValues()
	print array1.getType()
