import mil.army.arl.xdmf.*;

public class TestXdmfJava {
    public static void main (String argv[]) {
        System.out.println("Hello World");
        XdmfWriter writer = XdmfWriter.New("outputJavaTest.xmf");
        writer.setLightDataLimit((long)10);
        
        XdmfUnstructuredGrid grid = XdmfUnstructuredGrid.New();
        grid.setName("test");
        grid.getGeometry().setType(XdmfGeometryType.XYZ());

        XdmfDomain domain = XdmfDomain.New();
        domain.insert(grid);
        domain.accept(writer);
    }
}
