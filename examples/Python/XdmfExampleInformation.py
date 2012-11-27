from Xdmf import *

if __name__ == "__main__":
	infoExample = XdmfInformation.New()
	#Then the key and value must be set seperately
	infoExample.setKey("Your Key String")
	infoExample.setValue("Your Value String")

	infoExample = XdmfInformation.New("Your Key String", "Your Value String")
	#This code creates an information with the key "Your Key String" and the value "Your Value String"

	storedKey = infoExample.getKey()
	#"Your Key String" is now stored in the variable storedKey

	storedValue = infoExample.getValue()
	#"Your Value String" is now stored in the variable storedValue

	infoExample.setKey("Your New Key")
	#"Your New Key" is now the key for infoExample

	infoExample.setValue("Your New Value")
	#"Your New Value" is now the value for infoExample
