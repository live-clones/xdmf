//
// @brief Construct mesh from an ExodusII file.
//

#if !defined(dft_ExodusReader_h)
#define dft_ExodusReader_h

#include <Xdmf.h>
#include <XdmfSet.h>

//
//
//

namespace dft
{

  /*!
   * @brief ExodusReader class encapsulates the operation of reading from a
   *        ExodusII file containing finite element mesh and boundary sets.
   *        Data is read and stored directly into Xdmf format.
   */

  class ExodusReader
  {
    public:
      /*!
       * Constructor.
       */
      ExodusReader();

      /*!
       * Destructor.
       */
      ~ExodusReader();

      /*!
       * Read the contents of the file and store them internally.
       *
       * @param fileName a const char * containing the path to the exodus ii file to read
       * @param parentElement Pointer to XdmfElement the parent xdmf element to hold the grid - this
       *        should be an XdmfDomain for insertion into the top of the xdmf file, but could be an XdmfGrid
       *        if collections are desired.
       *
       * @return Pointer to XdmfGrid containing the mesh information read in from the exodus ii file
       */
      XdmfGrid * read(const char * fileName, XdmfElement * parentElement);

    private:
      /*!
       * Convert from exodus ii to xdmf cell types
       *
       * @param exoElemType a char * containing the elem_type of the current topology being read
       * @param numPointsPerCell an int the number of points per cell for the current topology being read
       *
       * @return a XdmfInt32 of the xdmf topology type equivalent to the exodus topology being read
       */
      XdmfInt32 DetermineXdmfCellType(char * exoElemType, int numPointsPerCell);
  };
}

#endif // dft_ExodusReader_h
