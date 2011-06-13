import mil.army.arl.xdmf.*;

public class TestXdmfGC {
    public static void main (String argv[]) {
        System.out.println("Hello Java World (Garbage Collector)");

	for(int i = 0; i < 10; i++)        
	{
	    System.gc();
	    XdmfWriter w = XdmfWriter.New("outputXdmfGC"); 
	    XdmfDomain d = XdmfDomain.New();
	    d.accept(w);
	}
        
        System.out.println("Done");
    }	    
}
