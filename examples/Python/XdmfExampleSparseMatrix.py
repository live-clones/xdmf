from Xdmf import *

if __name__ == "__main__":
	#//initialization begin

        exampleMatrix = XdmfSparseMatrix.New(3, 3)

	#//initialization end

	#//setName begin

        exampleMatrix.setName("TestMatrix")

	#//setName end

	#//getName begin

        exampleName = exampleMatrix.getName()

	#//getName end

	#//setRowPointer begin

        newRowPointer = XdmfArray.New()

        newRowPointer.insertAsUInt32(0, 1)
        newRowPointer.insertAsUInt32(1, 1)
        newRowPointer.insertAsUInt32(2, 2)
        newRowPointer.insertAsUInt32(3, 3)

        exampleMatrix.setRowPointer(newRowPointer)

	#//setRowPointer end

	#//getRowPointer begin

        exampleRowPointer = exampleMatrix.getRowPointer()

	#//getRowPointer end

	#//setColumnIndex begin

        newColumnIndex = XdmfArray.New()

        newColumnIndex.pushBackAsUInt32(1)
        newColumnIndex.pushBackAsUInt32(2)
        newColumnIndex.pushBackAsUInt32(0)

        exampleMatrix.setColumnIndex(newColumnIndex)

	#//setColumnIndex end

	#//getColumnIndex begin

        exampleColumnIndex = exampleMatrix.getColumnIndex()

	#//getColumnIndex end

	#//getNumberRows begin

        exampleNumRows = exampleMatrix.getNumberRows()

	#//getNumberRows end

	#//getNumberColumns begin

        exampleNumCols = exampleMatrix.getNumberColumns()

	#//getNumberColumns end

	#//setValues begin

        newValues = XdmfArray.New()

        newValues.pushBackAsFloat64(5.0)
        newValues.pushBackAsFloat64(6.0)
        newValues.pushBackAsFloat64(-1.0)

        exampleMatrix.setValues(newValues)

	#//setValues end

	#//getValues begin

        exampleValues = exampleMatrix.getValues()

	#//getValues end

	#//getValuesString begin

        exampleValueString = exampleMatrix.getValuesString()

	#//getValuesString end
