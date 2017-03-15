#include "tiff.h"
#include "tiffio.h"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfTIFFController.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include "stdio.h"
#include <assert.h>

int main(int, char **)
{
  TIFF* tif = TIFFOpen("compressedstripoutput.tif", "w");

  if (tif) {
    unsigned int w, h, sampleSize, pixelWidth;
    // set a base size
    pixelWidth = 1000;
    w = 8 * pixelWidth;
    h = 1000;
    sampleSize = 1;
    size_t npixels;
    unsigned int * scanline;

    npixels = w * h;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);  // set the width of the image
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);    // set the height of the image
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, sampleSize);   // set number of channels per pixel
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, XdmfArrayType::UInt32()->getElementSize());   // Strangely this is in bytes when dealing with Compression 
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, 5); // Only LZW encoding works in any fashion

     TIFFSetField(tif,
                  TIFFTAG_ROWSPERSTRIP,
                  TIFFDefaultStripSize(tif, w*sampleSize));

    unsigned int scanlineSize = TIFFScanlineSize(tif);

    for (unsigned int i = 0; i < h; ++i)
    {
      scanline = (unsigned int *) _TIFFmalloc(scanlineSize);
      for (unsigned int j = 0; j < pixelWidth; ++j)
      {
        scanline[j] = i * j;
      }
      TIFFWriteScanline(tif, scanline, i, 0);
      _TIFFfree(scanline);
    }
  }

  TIFFClose(tif);

  shared_ptr<XdmfHeavyDataController> controller = XdmfTIFFController::New("compressedstripoutput.tif",
                                                                           XdmfArrayType::UInt32(),
                                                                           std::vector<unsigned int>(1, 1000 * 1000));

  shared_ptr<XdmfArray> checkarray = XdmfArray::New();

  checkarray->insert(controller);

  checkarray->read();


  for (unsigned int i = 0; i < 1000; ++i)
  {
    for (unsigned int j = 0; j < 1000; ++j)
    {
//      printf("%u ", checkarray->getValue<unsigned int>(i * 1000 + j));
      assert(checkarray->getValue<unsigned int>(i * 1000 + j) == i * j);
    }
//    printf("\n");
  }

  shared_ptr<XdmfHeavyDataController> hyperslabcontroller = XdmfTIFFController::New("compressedstripoutput.tif",
                                                                               XdmfArrayType::UInt32(),
                                                                               std::vector<unsigned int>(2, 0),
                                                                               std::vector<unsigned int>(2, 2),
                                                                               std::vector<unsigned int>(2, 500),
                                                                               std::vector<unsigned int>(2, 1000));

  shared_ptr<XdmfArray> checkarray2 = XdmfArray::New();

  checkarray2->insert(hyperslabcontroller);

  checkarray2->read();

  for (unsigned int i = 0; i < 500; ++i)
  {
    for (unsigned int j = 0; j < 500; ++j)
    {
//      printf("%u ", checkarray2->getValue<unsigned int>(i * 500 + j));
      assert(checkarray2->getValue<unsigned int>(i * 500 + j) == (2 * i) * (2 * j));
    }
//    printf("\n");
  }

  shared_ptr<XdmfHeavyDataController> hyperslabcontroller2 = XdmfTIFFController::New("compressedstripoutput.tif",
                                                                                XdmfArrayType::UInt32(),
                                                                                std::vector<unsigned int>(1, 0),
                                                                                std::vector<unsigned int>(1, 2),
                                                                                std::vector<unsigned int>(1, (1000 * 1000) / 2),
                                                                                std::vector<unsigned int>(1, 1000 * 1000));

  shared_ptr<XdmfArray> checkarray3 = XdmfArray::New();

  checkarray3->insert(hyperslabcontroller2);

  checkarray3->read();

  for (unsigned int i = 0; i < 1000; ++i)
  {
    for (unsigned int j = 0; j < 500; ++j)
    {
//      printf("%u ", checkarray3->getValue<unsigned int>(i * 500 + j));
      assert(checkarray3->getValue<unsigned int>(i * 500 + j) == i * (2 * j));
    }
//    printf("\n");
}

  tif = TIFFOpen("compressedstripoutput.tif", "r");
  unsigned int dircount = 0;
  if (tif) {
    do {
      dircount++;
    } while (TIFFReadDirectory(tif));
    TIFFClose(tif);
  }

  printf("%d directories in file stripoutput.tif\n", dircount);

  tif = TIFFOpen("compresseddirectories.tif", "w");

  if (tif) {
    unsigned int w, h, sampleSize, pixelWidth;
    // set a base size
    pixelWidth = 1000;
    w = 8 * 1000;
    h = 1000;
    sampleSize = 1;
    size_t npixels;
    unsigned int * scanline;

    npixels = w * h;

    for (unsigned int dirID = 0; dirID < 10; ++dirID)
    {
      TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);  // set the width of the image
      TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);    // set the height of the image
      TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, sampleSize);   // set number of channels per pixel
      TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, XdmfArrayType::UInt32()->getElementSize());    // set the size of the channels
      TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
      TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
      TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
      TIFFSetField(tif, TIFFTAG_COMPRESSION, 5); // Only LZW encoding works in any fashion

      unsigned int scanlineSize = TIFFScanlineSize(tif);

      for (unsigned int i = 0; i < h; ++i)
      {
        scanline = (unsigned int *) _TIFFmalloc(scanlineSize);
        for (unsigned int j = 0; j < pixelWidth; ++j)
        {
          scanline[j] = (1 + dirID) * (i + j);
        }
        TIFFWriteScanline(tif, scanline, i, 0);
        _TIFFfree(scanline);
      }
      TIFFWriteDirectory(tif);
    }
  }

  TIFFClose(tif);

  tif = TIFFOpen("compresseddirectories.tif", "r");
  dircount = 0;
  if (tif) {
    do {
      dircount++;
    } while (TIFFReadDirectory(tif));
    TIFFClose(tif);
  }

  printf("%d directories in file directories.tif\n", dircount);

  std::vector<unsigned int> dirstarts;
  dirstarts.push_back(0);
  dirstarts.push_back(0);
  dirstarts.push_back(0);
  std::vector<unsigned int> dirstrides;
  dirstrides.push_back(1);
  dirstrides.push_back(1);
  dirstrides.push_back(1);
  std::vector<unsigned int> dirdims;
  dirdims.push_back(1000);
  dirdims.push_back(1000);
  dirdims.push_back(10);
  std::vector<unsigned int> dirdataspace;
  dirdataspace.push_back(1000);
  dirdataspace.push_back(1000);
  dirdataspace.push_back(10);

  shared_ptr<XdmfHeavyDataController> dircontroller = XdmfTIFFController::New("compresseddirectories.tif",
                                                                         XdmfArrayType::UInt32(),
                                                                         dirstarts,
                                                                         dirstrides,
                                                                         dirdims,
                                                                         dirdataspace);

  shared_ptr<XdmfArray> dirArray = XdmfArray::New();

  dirArray->insert(dircontroller);

  dirArray->read();

  for (unsigned int dimiter = 0; dimiter < 10; ++dimiter)
  {
    for (unsigned int i = 0; i < 1000; ++i)
    {
      for (unsigned int j = 0; j < 1000; ++j)
      {
//        printf("%u ", dirArray->getValue<unsigned int>((1000 * 1000 * dimiter) + i * 1000 + j));
        assert(dirArray->getValue<unsigned int>((1000 * 1000 * dimiter) + i * 1000 + j) == (1 + dimiter) * (i + j));
      }
//      printf("\n");
    }
//    printf("\n\n");
  }

  dirstrides[2] = 2;

  dirdims[2] = 5;

  shared_ptr<XdmfHeavyDataController> dircontroller2 = XdmfTIFFController::New("compresseddirectories.tif",
                                                                          XdmfArrayType::UInt32(),
                                                                          dirstarts,
                                                                          dirstrides,
                                                                          dirdims,
                                                                          dirdataspace);

  shared_ptr<XdmfArray> dirArray2 = XdmfArray::New();

  dirArray2->insert(dircontroller2);

  dirArray2->read();

  for (unsigned int dimiter = 0; dimiter < 10; dimiter += 2)
  {
    for (unsigned int i = 0; i < 1000; ++i)
    {
      for (unsigned int j = 0; j < 1000; ++j)
      {
//        printf("%u ", dirArray->getValue<unsigned int>((1000 * 1000 * (dimiter / 2)) + i * 1000 + j));
        assert(dirArray2->getValue<unsigned int>((1000 * 1000 * (dimiter / 2)) + i * 1000 + j) == (1 + dimiter) * (i + j));
      }
//      printf("\n");
    }
//    printf("\n\n");
  }

  shared_ptr<XdmfHeavyDataController> dircontroller3 = XdmfTIFFController::New("compresseddirectories.tif",
                                                                          XdmfArrayType::UInt32(),
                                                                          std::vector<unsigned int>(1, 0),
                                                                          std::vector<unsigned int>(1, 1),
                                                                          std::vector<unsigned int>(1, 10 * 1000 * 1000),
                                                                          std::vector<unsigned int>(1, 10 * 1000 * 1000));

  shared_ptr<XdmfArray> dirArray3 = XdmfArray::New();

  dirArray3->insert(dircontroller3);

  dirArray3->read();

  for (unsigned int dimiter = 0; dimiter < 10; ++dimiter)
  {
    for (unsigned int i = 0; i < 1000; ++i)
    {
      for (unsigned int j = 0; j < 1000; ++j)
      {
//        printf("%u ", dirArray3->getValue<unsigned int>((1000 * 1000 * dimiter) + i * 1000 + j));
        assert(dirArray3->getValue<unsigned int>((1000 * 1000 * dimiter) + i * 1000 + j) == (1 + dimiter) * (i + j));
      }
//      printf("\n");
    }
//    printf("\n\n");
  }

  shared_ptr<XdmfHeavyDataController> dircontroller4 = XdmfTIFFController::New("compresseddirectories.tif",
                                                                          XdmfArrayType::UInt32(),
                                                                          std::vector<unsigned int>(1, 2),
                                                                          std::vector<unsigned int>(1, 6),
                                                                          std::vector<unsigned int>(1, (10 * 1000 * 1000) / 6),
                                                                          std::vector<unsigned int>(1, 10 * 1000 * 1000));

  shared_ptr<XdmfArray> dirArray4 = XdmfArray::New();

  dirArray4->insert(dircontroller4);

  dirArray4->read();

  unsigned int hdim = 0;
  unsigned int wdim = 2;
  unsigned int dirdim = 0;

  for (unsigned int i = 0; i < dirArray4->getSize(); ++i)
  {
    assert(dirArray4->getValue<unsigned int>(i) == (1 + dirdim) * (hdim + wdim));
    wdim += 6;
    if (wdim >= 1000)
    {
      wdim = wdim % 1000;
      ++hdim;
      if (hdim >= 1000)
      {
        hdim = hdim % 1000;
        ++dirdim;
      }
    } 
  }


  shared_ptr<XdmfWriter> writer = XdmfWriter::New("compressedtiffoutput.xmf");

  dirArray4->release();

  writer->setMode(XdmfWriter::DistributedHeavyData);

  dirArray4->accept(writer);

  shared_ptr<XdmfReader> reader = XdmfReader::New();

  shared_ptr<XdmfArray> readArray = shared_dynamic_cast<XdmfArray>(reader->read("compressedtiffoutput.xmf"));

  readArray->read();

  hdim = 0;
  wdim = 2;
  dirdim = 0;


  for (unsigned int i = 0; i < readArray->getSize(); ++i)
  {
    assert(readArray->getValue<unsigned int>(i) == (1 + dirdim) * (hdim + wdim));
    wdim += 6;
    if (wdim >= 1000)
    {
      wdim = wdim % 1000;
      ++hdim;
      if (hdim >= 1000)
      {
        hdim = hdim % 1000;
        ++dirdim;
      }
    }
  }

  return 0;
}
