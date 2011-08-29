import mil.army.arl.xdmf.*;

public class TestXdmfEquals {
    public static void main (String argv[]) {
        System.out.println("Hello World");

	System.out.println("Checking XdmfAttributeCenter");	
	XdmfAttributeCenter xac1 = XdmfAttributeCenter.Grid();
	XdmfAttributeCenter xac2 = XdmfAttributeCenter.Grid();
	XdmfAttributeCenter xac3 = XdmfAttributeCenter.Cell();
	
	// Check IsEquals
	if(!(xac1.IsEqual(xac2))) /*True*/
		throw new SecurityException("Failed True Check");
	if((xac1.IsEqual(xac3))) /*False*/ 
    		throw new SecurityException("Failed False Check");	

	// Check equals
	if(!(xac1.equals(xac2))) /*True*/ 
		throw new SecurityException("Failed True Check (equals)");
	if(xac1.equals(xac3)) /*False*/ 
		throw new SecurityException("Failed False Check (equals)");

    	// Check ==
	if(xac1 == xac2) /*False*/ 
		throw new SecurityException("Failed True Check (==)");
	if(xac1 == xac3) /*False*/ 
		throw new SecurityException("Failed False Check (==)");

	System.out.println("Checking XdmfAttributeType");	
	XdmfAttributeType xat1 = XdmfAttributeType.Scalar();
	XdmfAttributeType xat2 = XdmfAttributeType.Scalar();
	XdmfAttributeType xat3 = XdmfAttributeType.Tensor();
	
	// Check IsEquals
	if(!(xat1.IsEqual(xat2))) /*True*/
		throw new SecurityException("Failed True Check");
	if((xat1.IsEqual(xat3))) /*False*/ 
    		throw new SecurityException("Failed False Check");	

	// Check equals
	if(!(xat1.equals(xat2))) /*True*/ 
		throw new SecurityException("Failed True Check (equals)");
	if(xat1.equals(xat3)) /*False*/ 
		throw new SecurityException("Failed False Check (equals)");

    	// Check ==
	if(xat1 == xat2) /*False*/ 
		throw new SecurityException("Failed True Check (==)");
	if(xat1 == xat3) /*False*/ 
		throw new SecurityException("Failed False Check (==)");

	System.out.println("Checking XdmfGeometryType");	
	XdmfGeometryType xgt1 = XdmfGeometryType.XYZ();
	XdmfGeometryType xgt2 = XdmfGeometryType.XYZ();
	XdmfGeometryType xgt3 = XdmfGeometryType.XY();
	
	// Check IsEquals
	if(!(xgt1.IsEqual(xgt2))) /*True*/
		throw new SecurityException("Failed True Check");
	if((xgt1.IsEqual(xgt3))) /*False*/ 
    		throw new SecurityException("Failed False Check");	

	// Check equals
	if(!(xgt1.equals(xgt2))) /*True*/ 
		throw new SecurityException("Failed True Check (equals)");
	if(xgt1.equals(xgt3)) /*False*/ 
		throw new SecurityException("Failed False Check (equals)");

    	// Check ==
	if(xgt1 == xgt2) /*False*/ 
		throw new SecurityException("Failed True Check (==)");
	if(xgt1 == xgt3) /*False*/ 
		throw new SecurityException("Failed False Check (==)");

	System.out.print("Checking XdmfGridCollection");
	XdmfGridCollectionType xgct1 = XdmfGridCollectionType.Spatial();
        XdmfGridCollectionType xgct2 = XdmfGridCollectionType.Spatial();
        XdmfGridCollectionType xgct3 = XdmfGridCollectionType.Temporal();

        // Check IsEquals
        if(!(xgct1.IsEqual(xgct2))) /*True*/
                throw new SecurityException("Failed True Check");
        if((xgct1.IsEqual(xgct3))) /*False*/
                throw new SecurityException("Failed False Check");

        // Check equals
        if(!(xgct1.equals(xgct2))) /*True*/
                throw new SecurityException("Failed True Check (equals)");
        if(xgct1.equals(xgct3)) /*False*/
                throw new SecurityException("Failed False Check (equals)");

        // Check ==
        if(xgct1 == xgct2) /*False*/
                throw new SecurityException("Failed True Check (==)");
        if(xgct1 == xgct3) /*False*/
                throw new SecurityException("Failed False Check (==)");
	
	System.out.println("Checking XdmfSetType");
	XdmfSetType xst1 = XdmfSetType.Node();
        XdmfSetType xst2 = XdmfSetType.Node();
        XdmfSetType xst3 = XdmfSetType.Face();

        // Check IsEquals
        if(!(xst1.IsEqual(xst2))) /*True*/
                throw new SecurityException("Failed True Check");
        if((xst1.IsEqual(xst3))) /*False*/
                throw new SecurityException("Failed False Check");

        // Check equals
        if(!(xst1.equals(xst2))) /*True*/
                throw new SecurityException("Failed True Check (equals)");
        if(xst1.equals(xst3)) /*False*/
                throw new SecurityException("Failed False Check (equals)");

        // Check ==
        if(xst1 == xst2) /*False*/
                throw new SecurityException("Failed True Check (==)");
        if(xst1 == xst3) /*False*/
                throw new SecurityException("Failed False Check (==)");


	System.out.println("Checking XdmfTopologyType");
 	XdmfTopologyType xtt1 = XdmfTopologyType.Polyvertex();
        XdmfTopologyType xtt2 = XdmfTopologyType.Polyvertex();
        XdmfTopologyType xtt3 = XdmfTopologyType.Polyline(0);

        // Check IsEquals
        if(!(xtt1.IsEqual(xtt2))) /*True*/
                throw new SecurityException("Failed True Check");
        if((xtt1.IsEqual(xtt3))) /*False*/
                throw new SecurityException("Failed False Check");

        // Check equals
        if(!(xtt1.equals(xtt2))) /*True*/
                throw new SecurityException("Failed True Check (equals)");
        if(xtt1.equals(xtt3)) /*False*/
                throw new SecurityException("Failed False Check (equals)");

        // Check ==
        if(xtt1 == xtt2) /*False*/
                throw new SecurityException("Failed True Check (==)");
        if(xtt1 == xtt3) /*False*/
                throw new SecurityException("Failed False Check (==)");

    }
}
