import mil.army.arl.xdmf.*;

public class TestXdmfJava {
    static {
        System.loadLibrary("XdmfJava");
        System.loadLibrary("XdmfCoreJava");
    }

    public static void main (String argv[]) {
        System.out.println("Hello World");
        XdmfWriter writer = XdmfWriter.New("outputJavaTest.xmf");
        writer.setLightDataLimit((long)10);
        
        XdmfGridUnstructured grid = XdmfGridUnstructured.New();
        grid.setName("test");
        grid.getGeometry().setType(XdmfGeometryType.XYZ());

        XdmfDomain domain = XdmfDomain.New();
        domain.insert(grid);
        domain.accept(writer);
    }
}
