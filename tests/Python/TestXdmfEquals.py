from XdmfCore import *
from Xdmf import *

if __name__ == "__main__":

    xac1 = XdmfAttributeCenter.Grid()
    xac2 = XdmfAttributeCenter.Grid()
    xac3 = XdmfAttributeCenter.Cell()

    print "XdmfAttributeCenter True Check"
    assert xac1 == xac2
    print "XdmfAttributeCenter False Check"
    assert not xac1 == xac3

    xat1 = XdmfAttributeType.Scalar()
    xat2 = XdmfAttributeType.Scalar()
    xat3 = XdmfAttributeType.Tensor()

    print "XdmfAttributeType True Check"
    assert xat1 == xat2
    print "XdmfAttributeType False Check"
    assert not xat1 == xat3

    xgt1 = XdmfGeometryType.XYZ()
    xgt2 = XdmfGeometryType.XYZ()
    xgt3 = XdmfGeometryType.XY()

    print "XdmfGeometryType True Check"
    assert xgt1 == xgt2
    print "XdmfGeometryType False Check"
    assert not xgt1 == xgt3

    xgct1 = XdmfGridCollectionType.Spatial()
    xgct2 = XdmfGridCollectionType.Spatial()
    xgct3 = XdmfGridCollectionType.Temporal()

    print "XdmfGridCollectionType True Check"
    assert xgct1 == xgct2
    print "XdmfGridCollectionType False Check"
    assert not xgct1 == xgct3

    xst1 = XdmfSetType.Node()
    xst2 = XdmfSetType.Node()
    xst3 = XdmfSetType.Face()

    print "XdmfSetType True Check"
    assert xst1 == xst2
    print "XdmfSetType False Check"
    assert not xst1 == xst3

    xtt1 = XdmfTopologyType.Polyvertex()
    xtt2 = XdmfTopologyType.Polyvertex()
    xtt3 = XdmfTopologyType.Polyline(0)

    print "XdmfTopologyType True Check"
    assert xtt1 == xtt2
    print "XdmfTopologyType False Check"
    assert not xtt1 == xtt3

