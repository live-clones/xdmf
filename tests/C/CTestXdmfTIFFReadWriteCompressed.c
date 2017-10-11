#include "tiff.h"
#include "tiffio.h"
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfTIFFController.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include "stdio.h"
#include "assert.h"

int main()
{
  TIFF* tif = TIFFOpen("compressedstripoutput.tif", "w");

  int status = 0;
  unsigned int i = 0;
  unsigned int j = 0;

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
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, XdmfArrayTypeGetElementSize(XDMF_ARRAY_TYPE_INT32, &status));   // Strangely this is in bytes when dealing with Compression
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, 5); // Only LZW encoding works in any fashion

     TIFFSetField(tif,
                  TIFFTAG_ROWSPERSTRIP,
                  TIFFDefaultStripSize(tif, w*sampleSize));

    unsigned int scanlineSize = TIFFScanlineSize(tif);

    for (i = 0; i < h; ++i)
    {
      scanline = (unsigned int *) _TIFFmalloc(scanlineSize);
      for (j = 0; j < pixelWidth; ++j)
      {
        scanline[j] = i * j;
      }
      TIFFWriteScanline(tif, scanline, i, 0);
      _TIFFfree(scanline);
    }
  }

  TIFFClose(tif);

  unsigned int dims[1] = {1000 * 1000};

  XDMFTIFFCONTROLLER * controller = XdmfTIFFControllerNew("compressedstripoutput.tif",
                                                          XDMF_ARRAY_TYPE_INT32,
                                                          dims,
                                                          1,
                                                          &status);

  XDMFARRAY * checkarray = XdmfArrayNew();

  XdmfArrayInsertHeavyDataController(checkarray, (XDMFHEAVYDATACONTROLLER *)controller, 1);

  XdmfArrayRead(checkarray, &status);

  for (i = 0; i < 1000; ++i)
  {
    for (j = 0; j < 1000; ++j)
    {
      assert(((int *)XdmfArrayGetValue(checkarray, i * 1000 + j, XDMF_ARRAY_TYPE_INT32, &status))[0] == i * j);
    }
  }

  return 0;
}
