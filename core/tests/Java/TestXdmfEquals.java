import mil.army.arl.xdmf.*;

public class TestXdmfEquals {
    static {
        System.loadLibrary("XdmfCoreJava");
    }

    public static void main (String argv[]) {
        System.out.println("Hello World");

		XdmfArrayType type1a = XdmfArrayType.Int16();
		XdmfArrayType type1b = XdmfArrayType.Int16();
		XdmfArrayType type2 = XdmfArrayType.Int32();	
		// Check IsEquals
		if(!(type1a.IsEqual(type1b))) /*True*/ {
			throw new SecurityException("Failed True Check");
		}
		if((type1a.IsEqual(type2))) /*False*/ {
	    		throw new SecurityException("Failed False Check");	
		}
	
		// Check equals
		if(!(type1a.equals(type1b))) /*True*/ {
			throw new SecurityException("Failed True Check (equals)");
		}
		if(type1a.equals(type2)) /*False*/ {
			throw new SecurityException("Failed False Check (equals)");
		}
	
	    	// Check ==
		if(type1a == type1b) /*False*/ {
			throw new SecurityException("Failed True Check (==)");
		}
		if(type1a == type2) /*False*/ {
			throw new SecurityException("Failed False Check (==)");
		}
		
    }
}
