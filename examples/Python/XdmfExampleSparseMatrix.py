from Xdmf import *

if __name__ == "__main__":
        exampleMatrix = XdmfSparseMatrix.New(3, 3)

        exampleMatrix.setName("TestMatrix")

        exampleName = exampleMatrix.getName()

        newRowPointer = XdmfArray.New()

        newRowPointer.insertAsUInt32(0, 1)
        newRowPointer.insertAsUInt32(1, 1)
        newRowPointer.insertAsUInt32(2, 2)
        newRowPointer.insertAsUInt32(3, 3)

        exampleMatrix.setRowPointer(newRowPointer)

        exampleRowPointer = exampleMatrix.getRowPointer()

        newColumnIndex = XdmfArray.New()

        newColumnIndex.pushBackAsUInt32(1)
        newColumnIndex.pushBackAsUInt32(2)
        newColumnIndex.pushBackAsUInt32(0)

        exampleMatrix.setColumnIndex(newColumnIndex)

        exampleColumnIndex = exampleMatrix.getColumnIndex()

        exampleNumRows = exampleMatrix.getNumberRows()

        exampleNumCols = exampleMatrix.getNumberColumns()

        newValues = XdmfArray.New()

        newValues.pushBackAsFloat64(5.0)
        newValues.pushBackAsFloat64(6.0)
        newValues.pushBackAsFloat64(-1.0)

        exampleMatrix.setValues(newValues)

        exampleValues = exampleMatrix.getValues()

        exampleValueString = exampleMatrix.getValuesString()
