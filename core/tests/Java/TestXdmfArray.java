import mil.army.arl.xdmf.*;

public class TestXdmfArray {
    static {
        System.loadLibrary("XdmfCoreJava");
    }

    public static void main (String argv[]) {
        System.out.println("Hello World");
        
        XdmfArray array = XdmfArray.New();
        array.initializeAsFloat64();
        array.pushBackAsFloat64(1.0);
        array.pushBackAsFloat64(2.0);
        array.pushBackAsFloat64(3.0);
		if(array.getSize() != 3) {
			throw new SecurityException("XdmfArray: pushBack failed");
		}
    
        double[] da = {1.0,2.0,3.0};
        XdmfArray array2 = XdmfArray.New();
        array2.initializeAsFloat64();
        array2.insertValuesAsFloat64(0, da);
        System.out.println(array2.getSize());
        if(array2.getSize() != 3) {
            throw new SecurityException("XdmfArray: insert failed");
        }

        
		
    }
}
