#include "XdmfDomain.hpp"
#include "XdmfWriter.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfInformation.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfArray.hpp"

int main(int, char **)
{
	shared_ptr<XdmfDomain> primaryDomain = XdmfDomain::New();
	shared_ptr<XdmfInformation> domaininfo = XdmfInformation::New("Domain", "This is the primary data structure in Xdmf");
	shared_ptr<XdmfInformation> domaininfoinfo = XdmfInformation::New("Information", "Information can have information");
	domaininfo->insert(domaininfoinfo);
	primaryDomain->insert(domaininfo);


	shared_ptr<XdmfInformation> mergedInformation = XdmfInformation::New("XIncludes", "3");
	shared_ptr<XdmfInformation> sourceInformation = XdmfInformation::New("testoutput.xmf", "xpointer(//Xdmf/Domain/Grid[1])");
	mergedInformation->insert(sourceInformation);
	sourceInformation = XdmfInformation::New("testoutput2.xmf", "xpointer(//Xdmf/Domain/Grid[1])");
	mergedInformation->insert(sourceInformation);
        sourceInformation = XdmfInformation::New("editedtestoutput.xmf", "xpointer(//Xdmf/Domain/Grid[1])");
        mergedInformation->insert(sourceInformation);
	primaryDomain->insert(mergedInformation);


	shared_ptr<XdmfInformation> secondaryDomainInfo = XdmfInformation::New("Domain", "This is the primary data structure in Xdmf");
	secondaryDomainInfo->insert(domaininfoinfo);
	primaryDomain->insert(secondaryDomainInfo);


	shared_ptr<XdmfHDF5Writer> exampleHeavyWriter = XdmfHDF5Writer::New("combinedout.h5");
	shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New("combinedout.xmf", exampleHeavyWriter);

	//set this to false to stop the writer from parsing information for XIncludes
	exampleWriter->setXPathParse(true);


	primaryDomain->accept(exampleWriter);

	return 0;
}
