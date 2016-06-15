#include "XdmfHDF5Writer.hpp"

int main(int, char **)
{
        //#initialization begin

        std::string newPath = "Your file path goes here";
        bool replaceOrig = true;
        shared_ptr<XdmfHDF5Writer> exampleWriter = XdmfHDF5Writer::New(newPath, replaceOrig);

        //#initialization end

        //#setChunkSize begin

        int newChunk = 10;
        //creates blocks in sets of 10 slots

        exampleWriter->setChunkSize(newChunk);

        //#setChunkSize end

        //#getChunkSize begin

        int exampleChunk = exampleWriter->getChunkSize();

        //#getChunkSize end

        //#setUseDeflate

        bool useDeflate = true;

        exampleWriter->setUseDeflate(useDeflate);

        //#setUseDeflate

        //#getUseDeflate

        bool isUsingDeflate = exampleWriter->getUseDeflate();

        //#getUseDeflate

        //#setDeflateFactor

        int newDeflateFactor = 6;

        exampleWriter->setDeflateFactor(newDeflateFactor);

        //#setDeflateFactor

        //#getDeflateFactor

        int currentDeflateFactor = exampleWriter->getDeflateFactor();

        //#getDeflateFactor

        return 0;
}
