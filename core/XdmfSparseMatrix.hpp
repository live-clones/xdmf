/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfSparseMatrix.hpp                                                */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFSPARSEMATRIX_HPP_
#define XDMFSPARSEMATRIX_HPP_

// Includes
#include "XdmfCore.hpp"
#include "XdmfArray.hpp"

/**
 * @brief Sparse matrix implemented as compressed row storage.
 *
 * An XdmfSparseMatrix provides routines for interacting with a sparse
 * matrix.  It is stored internally in compressed row storage.
 */
class XDMFCORE_EXPORT XdmfSparseMatrix : public XdmfItem {

public:

  /**
   * Create a new XdmfSparseMatrix.
   *
   * @param numberRows number of rows in matrix.
   * @param numberColumns number of columns in matrix.
   *
   * @return constructed XdmfSparseMatrix.
   */
  static shared_ptr<XdmfSparseMatrix> New(const unsigned int numberRows,
                                          const unsigned int numberColumns);

  virtual ~XdmfSparseMatrix();

  LOKI_DEFINE_VISITABLE(XdmfSparseMatrix, XdmfItem);
  static const std::string ItemTag;

  /**
   * Get the column index array.
   *
   * @return array containing column indices for nonzero entries of
   * matrix. This is the same size as values.
   */
  shared_ptr<XdmfArray> getColumnIndex();

  std::map<std::string, std::string> getItemProperties() const;

  std::string getItemTag() const;

  /**
   * Get the name of the sparse matrix.
   *
   * @return a string containing the name of the sparse matrix.
   */
  std::string getName() const;

  /**
   * Get the number of columns in the sparse matrix.
   *
   * @return the number of columns in the sparse matrix.
   */
  unsigned int getNumberColumns() const;

  /**
   * Get the number of rows in the sparse matrix.
   *
   * @return the number of rows in the sparse matrix.
   */
  unsigned int getNumberRows() const;

  /**
   * Get the row pointer array.
   *
   * @return array containing indices into column array for each
   * row. This is the size of the number of rows in the matrix +
   * 1. The last value is the number of nonzero entries in the matrix
   */
  shared_ptr<XdmfArray> getRowPointer();

  /**
   * Get the values array.
   *
   * @return array containing values of nonzero entries of matrix.
   */
  shared_ptr<XdmfArray> getValues();

  /**
   * Get values as a string in two dimensional format.
   *
   * @return string representation of matrix.
   */
  std::string getValuesString() const;

  /**
   * Set the column index array.
   *
   * @param columnIndex array containing column indices for nonzero
   * entries of matrix. This is the same size as values.
   */
  void setColumnIndex(const shared_ptr<XdmfArray> columnIndex);

  /**
   * Set the name of the sparse matrix.
   *
   * @param name a string containing the name to set.
   */
  void setName(const std::string & name);

  /**
   * Set the row pointer array.
   *
   * @param rowPointer array containing indices into column array for
   * each row. This is the size of the number of rows in the matrix +
   * 1. The last value is the number of nonzero entries in the matrix
   */
  void setRowPointer(const shared_ptr<XdmfArray> rowPointer);

  /**
   * Set the values array.
   *
   * @param values array containing values of nonzero entries of
   * matrix.
   */
  void setValues(const shared_ptr<XdmfArray> values);

  virtual void traverse(const shared_ptr<XdmfBaseVisitor> visitor);

protected:

  XdmfSparseMatrix(const unsigned int numberRows,
                   const unsigned int numberColumns);

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               const std::vector<shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

private:

  XdmfSparseMatrix(const XdmfSparseMatrix &);  // Not implemented.
  void operator=(const XdmfSparseMatrix &);  // Not implemented.

  shared_ptr<XdmfArray> mColumnIndex;
  std::string mName;
  unsigned int mNumberColumns;
  unsigned int mNumberRows;
  shared_ptr<XdmfArray> mRowPointer;
  shared_ptr<XdmfArray> mValues;
};

#ifdef _WIN32
#endif

#endif /* XDMFSPARSEMATRIX_HPP_ */
