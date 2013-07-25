#include <cstdio>
#include <iostream>
#include <sstream>
#include <exodusII.h>
#include "XdmfDomain.hpp"
#include "XdmfError.hpp"
#include "XdmfExodusReader.hpp"
#include "XdmfExodusWriter.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfReader.hpp"
#include "XdmfWriter.hpp"

namespace {

  //
  // print usage
  //
  inline void
  printUsage(const char * programName)
  {

    std::cerr << "usage: " << programName << " "
              << "<input file> [output file]"
              << std::endl;

    //
    //
    //
    return;

  }

  //
  // process command line
  //
  void
  processCommandLine(std::string                  & inputFileName,
                     std::string                  & outputFileName,
                     int                            ac,
                     char                         * av[])
  {

    int c;
    bool errorFlag = false;

    while( (c=getopt(ac, av, "")) != -1 )
      switch(c){
      case '?':
        errorFlag = true;
        break;
      }
    
    if (optind >= ac)
      errorFlag = true;
    else {
      inputFileName = av[optind];
      ++optind;
    }

    if (optind < ac) {
      outputFileName = av[optind];
      ++optind;
    }

    //
    // check errorFlag
    //
    if (errorFlag == true) {
      printUsage(av[0]);
      std::exit(EXIT_FAILURE);
    }

  }

}

/**
 * XdmfExodusConverter is a command line utility for converting
 * between Xdmf and Exodus files. If given an Xdmf file, the tool
 * converts the file to Exodus and if given a path to an Exodus file,
 * the tool converts the file to Xdmf.
 *
 * Usage:
 *     XdmfExodusConverter <path-of-file-to-convert> 
 *                         (Optional: <path-to-output-file>)
 *
 */
int main(int argc, char* argv[])
{

  std::string inputFileName = "";
  std::string outputFileName = "";

  processCommandLine(inputFileName,
                     outputFileName,
                     argc,
                     argv);

  std::cout << inputFileName << std::endl;

  FILE * refFile = fopen(inputFileName.c_str(), "r");
  if (refFile) {
    // Success
    fclose(refFile);
  }
  else {
    std::cout << "Cannot open file: " << argv[1] << std::endl;
    return 1;
  }

  std::string meshName;
  if(outputFileName.compare("") == 0) {
    meshName = inputFileName;
  }
  else {
    meshName = outputFileName;
  }

  if(meshName.find_last_of("/\\") != std::string::npos) {
    meshName = meshName.substr(meshName.find_last_of("/\\") + 1,
                               meshName.length());
  }

  if (meshName.rfind(".") != std::string::npos) {
    meshName = meshName.substr(0, meshName.rfind("."));
  }

  int CPU_word_size = sizeof(double);
  int IO_word_size = 0; // Get from file
  float version;
  const int exodusHandle = 
    ex_open(argv[1], EX_READ, &CPU_word_size, &IO_word_size, &version);
  if(exodusHandle < 0) {
    // Xdmf to Exodus
    shared_ptr<XdmfReader> reader = XdmfReader::New();
    shared_ptr<XdmfDomain> domain = 
      shared_dynamic_cast<XdmfDomain>(reader->read(inputFileName));
    std::stringstream exodusFileName;
    exodusFileName << meshName << ".exo";
    shared_ptr<XdmfExodusWriter> writer = XdmfExodusWriter::New();
    if(domain->getNumberUnstructuredGrids() == 1) {
      const shared_ptr<XdmfUnstructuredGrid> grid = 
        domain->getUnstructuredGrid(0);
      writer->write(exodusFileName.str(),
                    grid);
      std::cout << "Wrote: " << exodusFileName.str() << std::endl;
    }
    else if(domain->getNumberGridCollections() == 1) {
      const shared_ptr<XdmfGridCollection> grid = 
        domain->getGridCollection(0);
      writer->write(exodusFileName.str(),
                    grid);
      std::cout << "Wrote: " << exodusFileName.str() << std::endl;
    }
    else {
      XdmfError::message(XdmfError::FATAL,
                         "Cannot find grid in Xdmf file to convert to "
                         "exodus.");
    }
  }
  else {
    // Exodus to Xdmf
    std::stringstream heavyFileName;
    heavyFileName << meshName << ".h5";
    shared_ptr<XdmfHDF5Writer> heavyDataWriter =
      XdmfHDF5Writer::New(heavyFileName.str());
    heavyDataWriter->setReleaseData(true);
    shared_ptr<XdmfExodusReader> exodusReader = XdmfExodusReader::New();
    shared_ptr<XdmfUnstructuredGrid> readGrid = 
      exodusReader->read(inputFileName,
                         heavyDataWriter);
    shared_ptr<XdmfDomain> newDomain = XdmfDomain::New();
    newDomain->insert(readGrid);
    std::stringstream xmlFileName;
    xmlFileName << meshName << ".xmf";
    shared_ptr<XdmfWriter> writer = XdmfWriter::New(xmlFileName.str(),
                                                    heavyDataWriter);
    newDomain->accept(writer);
    std::cout << "Wrote: " << xmlFileName.str() << std::endl;
  }

  /*

  shared_ptr<XdmfReader> reader = XdmfReader::New();
  shared_ptr<XdmfDomain> domain =
    shared_dynamic_cast<XdmfDomain>(reader->read(inputFileName));

  if(unpartition) {
    if(domain->getNumberGridCollections() == 0) {
      std::cout << "No grid collections to unpartition" << std::endl;
      return 1;
    }
  }
  else {
    if(domain->getNumberUnstructuredGrids() == 0 &&
       domain->getNumberGridCollections() == 0 &&
       domain->getNumberGraphs() == 0) {
      std::cout << "No grids or graphs to partition" << std::endl;
      return 1;
    }
  }

  std::stringstream heavyFileName;
  heavyFileName << meshName << ".h5";
  shared_ptr<XdmfHDF5Writer> heavyDataWriter =
    XdmfHDF5Writer::New(heavyFileName.str());

  shared_ptr<XdmfDomain> newDomain = XdmfDomain::New();

  shared_ptr<XdmfPartitioner> partitioner = XdmfPartitioner::New();
  
  if(unpartition) {
    shared_ptr<XdmfUnstructuredGrid> toWrite =
      partitioner->unpartition(domain->getGridCollection(0));
    newDomain->insert(toWrite);
  }
  else {
    if(domain->getNumberGraphs() == 0) {
      shared_ptr<XdmfUnstructuredGrid> gridToPartition;
      if(domain->getNumberUnstructuredGrids() == 0) {
        // repartition
        gridToPartition =
          partitioner->unpartition(domain->getGridCollection(0));
      }
      else {
        gridToPartition = domain->getUnstructuredGrid(0);
      }
      shared_ptr<XdmfGridCollection> toWrite =
        partitioner->partition(gridToPartition,
                               numPartitions,
                               metisScheme,
                               heavyDataWriter);
      newDomain->insert(toWrite);
    }
    else {
      shared_ptr<XdmfGraph> graphToPartition = domain->getGraph(0);
      partitioner->partition(graphToPartition,
                             numPartitions);
      newDomain->insert(graphToPartition);
    }
  }

  std::stringstream xmlFileName;
  xmlFileName << meshName << ".xmf";
  shared_ptr<XdmfWriter> writer = XdmfWriter::New(xmlFileName.str(),
                                                  heavyDataWriter);
  newDomain->accept(writer);

  std::cout << "Wrote: " << xmlFileName.str() << std::endl;

  */
}
