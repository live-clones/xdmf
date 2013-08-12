from Xdmf import *

if __name__ == "__main__":
	#//initialization begin

	infoExample = XdmfInformation.New()
	#Then the key and value must be set seperately
	infoExample.setKey("Your Key String")
	infoExample.setValue("Your Value String")

	#//initialization end

	#//initializationfull begin

	infoExample = XdmfInformation.New("Your Key String", "Your Value String")
	#This code creates an information with the key "Your Key String" and the value "Your Value String"

	#//initializationfull end

	#//getKey begin

	storedKey = infoExample.getKey()
	#"Your Key String" is now stored in the variable storedKey

	#//getKey end

	#//getValue begin

	storedValue = infoExample.getValue()
	#"Your Value String" is now stored in the variable storedValue

	#//getValue end

	#//setKey begin

	infoExample.setKey("Your New Key")
	#"Your New Key" is now the key for infoExample

	#//setKey end

	#//setValue begin

	infoExample.setValue("Your New Value")
	#"Your New Value" is now the value for infoExample

	#//setValue end
