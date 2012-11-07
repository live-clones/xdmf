#include "XdmfDomain.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfInformation.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfMap.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfSet.hpp"
#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"

#include "XdmfTestDataGenerator.hpp"

int main(int, char **)
{
	shared_ptr<XdmfDomain> primaryDomain = XdmfDomain::New();
        shared_ptr<XdmfInformation> domaininfo = XdmfInformation::New("Domain", "This is the primary data structure in Xdmf");
        shared_ptr<XdmfInformation> domaininfoinfo = XdmfInformation::New("Information", "Information can have information");
        domaininfo->insert(domaininfoinfo);
        primaryDomain->insert(domaininfo);

        shared_ptr<XdmfGridCollection> gridHolder = XdmfGridCollection::New();
        gridHolder->setType(XdmfGridCollectionType::Spatial());
        shared_ptr<XdmfInformation> holderInfo = XdmfInformation::New("Grid Collection 1", "This is the main grid collection");
        gridHolder->insert(holderInfo);
        gridHolder->setName("GridCollection Example");


        shared_ptr<XdmfUnstructuredGrid> ungrid = XdmfUnstructuredGrid::New();
        ungrid->setName("Unstructured Grid Example");
        shared_ptr<XdmfTime> untime = XdmfTime::New(5.0);
        shared_ptr<XdmfInformation> untimeinfo = XdmfInformation::New("Time", "This is the time for the Unstructured Grid");
        untime->insert(untimeinfo);
        ungrid->setTime(untime);
        shared_ptr<XdmfAttribute> unglobalID = XdmfAttribute::New();
        unglobalID->setType(XdmfAttributeType::GlobalId());
        unglobalID->setCenter(XdmfAttributeCenter::Node());
        unglobalID->setName("Global Node Equivalencies");
        int task1globalnodes [] = {1, 4, 5, 7, 3, 6};
        unglobalID->insert(0, task1globalnodes, 6, 1, 1 );
        shared_ptr<XdmfInformation> unglobalIDinfo = XdmfInformation::New("Global Nodes", "This is the global nodes that accociate with the local nodes");
        ungrid->insert(unglobalID);
        shared_ptr<XdmfSet> unset = XdmfSet::New();
        shared_ptr<XdmfInformation> unsetinfo = XdmfInformation::New("Data Set", "This is a set of arbitrary data");
        unset->insert(unsetinfo);
        unset->setName("Unstructured Grid's Set");
        unset->setType(XdmfSetType::Node());
        shared_ptr<XdmfAttribute> unsetattribute = XdmfAttribute::New();
        unsetattribute->setType(XdmfAttributeType::Scalar());
        unsetattribute->setCenter(XdmfAttributeCenter::Node());
        unsetattribute->setName("The Set's attribute");
        double unsetattribdata [] = {1.9, 2.8, 3.7, 4.6, 5.5, 6.4, 7.3, 8.2, 9.1};
        unsetattribute->insert(0, unsetattribdata, 9, 1, 1);
        unset->insert(unsetattribute);
        double unsetdata [] = {5.1, 4.2, 3.3, 2.4, 1.5};
        unset->insert(0, unsetdata, 5, 1, 1);
        ungrid->insert(unset);
        shared_ptr<XdmfGeometry> ungeometry = XdmfGeometry::New();
        ungeometry->setType(XdmfGeometryType::XYZ());
        ungeometry->setName("Unstructured Geometry");
        double ungeopoints [] = {0.1, 0.1, 1.1, 1.1, 0.1, 1.1, 3.1, 0.1, 2.1, 0.1, 1.1, 1.1, 1.1,
                1.1, 1.1, 3.1, 2.1, 2.1, 0.1, 0.1, -1.1, 1.1, 0.1, -1.1, 3.1,
                0.1, -2.1, 0.1, 1.1, -1.1, 1.1, 1.1, -1.1, 3.1, 2.1, -2.1};
        ungeometry->insert(0, ungeopoints, 36, 1, 1);
        shared_ptr<XdmfInformation> ungeometryinfo = XdmfInformation::New("Geometry", "This is the geometry associated with the unstructured grid");
        ungeometry->insert(ungeometryinfo);
        ungrid->setGeometry(ungeometry);
        shared_ptr<XdmfTopology> untopology = XdmfTopology::New();
        untopology->setType(XdmfTopologyType::Hexahedron());
        untopology->setName("Unstructured Topology");
        int untopovalues [] = {0, 1, 7, 6, 3, 4, 10, 9, 1, 2, 8, 7, 4, 5, 11, 10};
        untopology->insert(0, untopovalues, 16, 1, 1);
        shared_ptr<XdmfInformation> untopologyinfo = XdmfInformation::New("Topology", "This is the topology associated with the unstructured grid");
        ungrid->setTopology(untopology);


        shared_ptr<XdmfArray> curvdimensions = XdmfArray::New();
        curvdimensions->pushBack(12);
        curvdimensions->pushBack(12);
        curvdimensions->pushBack(12);
        shared_ptr<XdmfCurvilinearGrid> curvgrid = XdmfCurvilinearGrid::New(curvdimensions);
        curvgrid->setName("Curvilinear Grid Example");
        shared_ptr<XdmfInformation> curvgridinfo = XdmfInformation::New("Curvilinear Grid", "This is an example curvilinear grid");
        curvgrid->insert(curvgridinfo);
        shared_ptr<XdmfTime>curvtime = XdmfTime::New(5.0);
        shared_ptr<XdmfInformation> curvtimeinfo = XdmfInformation::New("Time", "The Time of the Curvilinear Grid");
        curvtime->insert(curvtimeinfo);
        curvgrid->setTime(curvtime);
        shared_ptr<XdmfAttribute> curvglobalID = XdmfAttribute::New();
        curvglobalID->setType(XdmfAttributeType::GlobalId());
        curvglobalID->setCenter(XdmfAttributeCenter::Node());
        curvglobalID->setName("Global Node Equivalencies");
        int task2globalnodes [] = {7, 3, 8, 2, 5, 1};
        curvglobalID->insert(0, task1globalnodes, 6, 1, 1);
        shared_ptr<XdmfInformation> curvglobalIDinfo = XdmfInformation::New("Global Node Equivalencies", "These are the global nodes that accociate with the local nodes");
        curvglobalID->insert(curvglobalIDinfo);
        curvgrid->insert(curvglobalID);
        shared_ptr<XdmfGeometry> curvgeometry = XdmfGeometry::New();
        curvgeometry->setType(XdmfGeometryType::XYZ());
        curvgeometry->setName("Curvilinear Geometry");
        double curvgeopoints [] = {1.1, 1.1, 2.1, 2.1, 1.1, 2.1, 4.1, 1.1, 3.1, 1.1, 2.1, 2.1, 2.1,
                2.1, 2.1, 4.1, 3.1, 3.1, 1.1, 1.1, 0.1, 2.1, 1.1, 0.1, 4.1,
                1.1, -1.1, 1.1, 2.1, 0.1, 1.1, 2.1, -0.1, 4.1, 3.1, -1.1};
        curvgeometry->insert(0, curvgeopoints, 36, 1, 1);
        shared_ptr<XdmfInformation> curvgeometryinfo = XdmfInformation::New("Geometry", "The geometry of the curvilinear grid");
        curvgeometry->insert(curvgeometryinfo);
        curvgrid->setGeometry(curvgeometry);


        double rectXcoordinates [] = {1.1, 1.1, 2.1, 2.1, 1.1, 2.1, 4.1, 1.1, 3.1, 1.1, 2.1, 2.1};
        double rectYcoordinates [] = {2.1, 2.1, 2.1, 4.1, 3.1, 3.1, 1.1, 1.1, 0.1, 2.1, 1.1, 0.1};
        double rectZcoordinates [] = {4.1, 1.1, -1.1, 1.1, 2.1, 0.1, 1.1, 2.1, -0.1, 4.1, 3.1, -1.1};
	shared_ptr<XdmfArray> rectXarray = XdmfArray::New();
        rectXarray->insert(0, rectXcoordinates, 12, 1, 1);
        shared_ptr<XdmfArray> rectYarray = XdmfArray::New();
        rectYarray->insert(0, rectYcoordinates, 12, 1, 1);
        shared_ptr<XdmfArray> rectZarray = XdmfArray::New();
        rectZarray->insert(0, rectZcoordinates, 12, 1, 1);
        std::vector<shared_ptr<XdmfArray> > coordinatecontainer;
        coordinatecontainer.push_back(rectXarray);
        coordinatecontainer.push_back(rectYarray);
        coordinatecontainer.push_back(rectZarray);
        shared_ptr<XdmfRectilinearGrid> rectgrid = XdmfRectilinearGrid::New(coordinatecontainer);
        rectgrid->setName("Rectilinear Grid Example");
        shared_ptr<XdmfInformation> rectgridinfo = XdmfInformation::New("Rectilinear Grid", "This is an example of a rectilinear grid");
        shared_ptr<XdmfAttribute> rectglobalID = XdmfAttribute::New();
        rectglobalID->setType(XdmfAttributeType::GlobalId());
        rectglobalID->setCenter(XdmfAttributeCenter::Node());
        rectglobalID->setName("Global Node Equivalencies");
        int task3globalnodes [] = {2, 7, 9, 0, 8, 6};
        rectglobalID->insert(0, task3globalnodes, 6, 1, 1);
        shared_ptr<XdmfInformation> rectglobalIDinfo = XdmfInformation::New("Global Node Equivalencies", "These are the global nodes that associate with the local nodes");
        rectglobalID->insert(rectglobalIDinfo);
        shared_ptr<XdmfTime> recttime = XdmfTime::New(5.0);
        shared_ptr<XdmfInformation> recttimeinfo = XdmfInformation::New("Time", "The time of the rectiliniear grid");
        recttime->insert(recttimeinfo);
        rectgrid->setTime(recttime);
        rectgrid->insert(rectglobalID);


        shared_ptr<XdmfArray> regbrick = XdmfArray::New();
        double regbrickvals [] = {10, 10, 10};
        regbrick->insert(0, regbrickvals, 3, 1, 1);
        shared_ptr<XdmfArray> regdimensions = XdmfArray::New();
        int regdimensionvals [] = {5, 5, 5};
        regdimensions->insert(0, regdimensionvals, 3, 1, 1);
        shared_ptr<XdmfArray> regorigin = XdmfArray::New();
        double regoriginvals [] = {0, 0, 0};
        regorigin->insert(0, regoriginvals, 3, 1, 1);
        shared_ptr<XdmfRegularGrid> reggrid = XdmfRegularGrid::New(regbrick, regdimensions, regorigin);
        reggrid->setName("Regular Grid Example");
        shared_ptr<XdmfInformation> reggridinfo = XdmfInformation::New("Regular Grid", "This is an example of a regular grid");
        shared_ptr<XdmfTime> regtime = XdmfTime::New(5.0);
        shared_ptr<XdmfInformation> regtimeinfo = XdmfInformation::New("Time", "This is the time for the regular grid");
        regtime->insert(regtimeinfo);
	reggrid->setTime(regtime);
        shared_ptr<XdmfAttribute> regglobalID = XdmfAttribute::New();
        regglobalID->setType(XdmfAttributeType::GlobalId());
        regglobalID->setCenter(XdmfAttributeCenter::Node());
        regglobalID->setName("Global Node Equivalencies");
        int task4globalnodes [] = {3, 6, 1, 4, 2, 5, 9};
        regglobalID->insert(0, task4globalnodes, 7, 1, 1);
        shared_ptr<XdmfInformation> regglobalIDinfo = XdmfInformation::New("Global Node Equivalencies", "These are the global nodes that associate with the local nodes");
        regglobalID->insert(regglobalIDinfo);
	reggrid->insert(regglobalID);



        std::vector<shared_ptr<XdmfAttribute> > nodeholder;
        nodeholder.push_back(unglobalID);
        nodeholder.push_back(curvglobalID);
        nodeholder.push_back(rectglobalID);
        nodeholder.push_back(regglobalID);
        std::vector<shared_ptr<XdmfMap> > mapcollection = XdmfMap::New(nodeholder);

        ungrid->insert(mapcollection[0]);
        curvgrid->insert(mapcollection[1]);
        rectgrid->insert(mapcollection[2]);
        reggrid->insert(mapcollection[3]);

        /*
        the version of XdmfMap::New() used here returns a number of maps equal to the number of attributes it was provided with.
        */
        for (int i = 0; i<mapcollection.size(); i++)
	{
		gridHolder->insert(mapcollection[i]);
	}

        gridHolder->insert(ungrid);
        gridHolder->insert(curvgrid);
        gridHolder->insert(rectgrid);
        gridHolder->insert(reggrid);

        shared_ptr<XdmfGridCollection> secondaryHolder = XdmfGridCollection::New();
        secondaryHolder->setName("Secondary grid collection");
        gridHolder->insert(secondaryHolder);
        /*
        grid collections can be placed inside other grid collections
        */

        primaryDomain->insert(gridHolder);
        /*
        grids can be inserted into the domain in the same way as the grid collection
        */
        primaryDomain->insert(ungrid);
        primaryDomain->insert(curvgrid);
        primaryDomain->insert(rectgrid);
        primaryDomain->insert(reggrid);

        shared_ptr<XdmfHDF5Writer> exampleHeavyWriter = XdmfHDF5Writer::New("testoutput.h5");
        shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New("testoutput.xmf", exampleHeavyWriter);


        primaryDomain->accept(exampleWriter);

  return 0;
}
