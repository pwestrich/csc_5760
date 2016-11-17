/**
 *
 * BaseMatrix
 *
 * This is the ancestor of all specialized Matrix classes.
 *
 *  Created on: Dec 20, 2013
 *      Author: rmarshall42
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <map>
#include <algorithm>
#include <zlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <regex>

#include "stringutils.h"
typedef float value_t;
enum Direction {
  N, NE, E, SE, S, SW, W, NW
} ;

#define VERSION_STRING "BaseMatrix 0.1"

template<class T>
  class BaseMatrix {
  public:

    bool read_only_ ;
    bool dirty_ ; // data has been modified, metadata like min_,max_ needs to be recalculated

    BaseMatrix<T>() ;

    BaseMatrix<T>(unsigned rows, unsigned cols) ;
    BaseMatrix<T>(unsigned rows, unsigned cols, T** arr) ;
    BaseMatrix<T>(BaseMatrix const& m) ; // not implemented

    T& operator()(unsigned row, unsigned col) ;
    T operator()(unsigned row, unsigned col) const ;

    ~BaseMatrix<T>() ;

    BaseMatrix& operator=(BaseMatrix const& m) ;
    BaseMatrix& operator*=(T value) ;
    BaseMatrix& operator*(T value) ;
    BaseMatrix& operator+(T value) ;
    BaseMatrix& operator+(BaseMatrix const& m) ;
    BaseMatrix& operator+=(BaseMatrix const& m) ;
    BaseMatrix& operator+=(T value) ;
    BaseMatrix<T>& operator*(BaseMatrix const& m) ;

    unsigned num_rows() const ;
    unsigned num_cols() const ;
    T* get_data() const ;
    std::string& get_type(T val) ;


    T* begin();
    T* get_address_at(unsigned row, unsigned col);
    void set_size(unsigned rows, unsigned cols) ;
    void resize(unsigned rows, unsigned cols) ;
    unsigned get_dim(unsigned dim) ;
    void set_dim(unsigned dim, unsigned h) ;
    void set_val(unsigned row, unsigned col, T value) ;
    void set_value(unsigned row, unsigned col, T value) ;
    void add_ghost_cells(unsigned grows, unsigned gcols, T value) ;
    T get_neighbor(unsigned row, unsigned col, Direction d) ;
    void get_neighbors(unsigned row, unsigned col, T* neighbors) ;
    T get_val(unsigned row, unsigned col) ;
    T get_value(unsigned row, unsigned col) ;
    void zero_fill() ;
    void random_fill() ;
    BaseMatrix transpose() ;
    BaseMatrix flip_horizontal() ;
    BaseMatrix flip_vertical() ;

    std::vector<int> find(T value) ;
    std::vector<int> find(T value, float eps) ;
    std::vector<int> find(T value, float eps, int row) ;
    std::vector<int> find(T value, float eps, int row, int col) ;

    T get_max() ;
    T get_min() ;
    T get_max(bool) ;
    T get_min(bool) ;

    std::string about() ;

  protected:
    unsigned rows_, cols_, ndims_ , data_ndims_;
    size_t* dims_ ;
    T* data_ ;
    T max_ ;
    T min_ ;

    T max() ;
    T min() ;
    T max(bool) ;
    T min(bool) ;

    void calc_max() ;
    void calc_min() ;
    void calc_max(bool) ;
    void calc_min(bool) ;

    void load_from_file_gz(gzFile* infile) ;/** @TODO: move to IOMatrix*/

  } ;
// ----------------------------------------------------------------------
// --------------------------------------------------------------------------

class MatrixZeroSizeException : public std::exception {
  virtual const char* what() const throw ()
  {
    return "Matrix constructor has 0 size" ;
  }
} zszex ;

class MatrixInvalidDimsExcaption : public std::exception {
  virtual const char* what() const throw ()
  {
    return "Invalid Matrix dimensions" ;
  }
} idmex ;

class MatrixIndexOutOfBoundsException : public std::exception {
  virtual const char* what() const throw ()
  {
    return "Matrix index out of bounds" ;
  }
} iobex ;

class MatrixReadOnlyWriteException : public std::exception {
  virtual const char* what() const throw ()
  {
    return "Cannot modify read-only Matrix data" ;
  }
} romex ;
// --------------------------------------------------------------------------
// -------------------------------------------------------------------------


template<class T>
  BaseMatrix<T>::BaseMatrix()
  {
    rows_ = 0 ;
    cols_ = 0 ;
    ndims_ = 2 ;
    data_ndims_ = 1;
    dims_ = NULL ;
    data_ = NULL ;
    read_only_ = false ;
    dirty_ = false ;
    max_ = (T) std::numeric_limits<T>::min() ;
    min_ = (T) std::numeric_limits<T>::max() ;

  } // ------------------------------------------------------------------------

template<class T>
  BaseMatrix<T>::BaseMatrix(unsigned rows, unsigned cols) :
      rows_(rows), cols_(cols)

  {
    if (rows == 0 || cols == 0)
      throw zszex ;

    data_ = new T[rows * cols] ;
    ndims_ = 2 ;
    data_ndims_ = 1;
    dims_ = NULL ;
    read_only_ = false ;
    dirty_ = false ;
    max_ = (T) std::numeric_limits<T>::min() ;
    min_ = (T) std::numeric_limits<T>::max() ;

  } // ------------------------------------------------------------------------

template<class T>
  BaseMatrix<T>::BaseMatrix(unsigned rows, unsigned cols, T** arr)
  {
    if (rows == 0 || cols == 0)
      throw zszex ;

    rows_ = rows ;
    cols_ = cols ;
    data_ = new T[rows * cols] ;
    ndims_ = 2 ;
    data_ndims_ = 1;
    dims_ = NULL ;
    read_only_ = false ;
    dirty_ = false ;
    max_ = (T) std::numeric_limits<T>::min() ;
    min_ = (T) std::numeric_limits<T>::max() ;

    for(size_t i = 0; i < rows_; ++i)
      for(size_t j = 0; j < cols_; ++j)
      {
        data_[cols_ * i + j] = arr[i][j];
      }
  } // ------------------------------------------------------------------------

template<class T>
BaseMatrix<T>::BaseMatrix(BaseMatrix<T> const& m)
{
    T* data = m.get_data();

    rows_ = m.num_rows() ;
    cols_ = m.num_cols() ;
    data_ = new T[rows_ * cols_] ;

    ndims_ = 2 ;
    data_ndims_ = 1;
    dims_ = NULL ;
    read_only_ = false ;
    dirty_ = false ;
    max_ = (T) std::numeric_limits<T>::min() ;
    min_ = (T) std::numeric_limits<T>::max() ;

    for(unsigned i = 0;  i < rows_ * cols_; ++i)
          data_[i] = data[i];
    
}




template<class T>
  BaseMatrix<T>::~BaseMatrix()
  {
    if (data_ != NULL) delete[] data_ ;
  } // ------------------------------------------------------------------------

template<typename T>
  T& BaseMatrix<T>::operator()(unsigned row, unsigned col)
  {
    if (row >= rows_ || col >= cols_)
    {
      std::cerr << row << " ?= " << rows_ << std::endl ;
      std::cerr << col << " ?= " << cols_ << std::endl ;
      throw iobex ;
    }

    //if(data_ndims_ != 1) { throw idmex;  }
    //return  *(*(data_ + row) + col);
    return data_[cols_ * row + col] ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::operator()(unsigned row, unsigned col) const
  {
    if (row >= rows_ || col >= cols_)
    {
      throw iobex ;
    }



    return data_[cols_ * row + col] ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T>& BaseMatrix<T>::operator*=(T value)
  {
    if (read_only_)
    {
      throw romex ;
    }

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        data_[cols_ * i + j] *= value ;
      }
    dirty_ = true ;
    return *this ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T>& BaseMatrix<T>::operator*(T value)
  {
    if (read_only_)
    {
      throw romex ;
    }

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        data_[cols_ * i + j] *= value ;
      }

    dirty_ = true ;

    return *this ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T>& BaseMatrix<T>::operator*(BaseMatrix const& m)
  {
    if (m.num_rows() != cols_)
    {
      throw idmex ;
    }
    if (read_only_)
    {
      throw romex ;
    }

    BaseMatrix<T> R ;
    R.set_size(rows_, m.num_cols()) ;

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        T sum = 0.0 ;

        for (unsigned n = 0; n < m.num_cols(); n++)
        {
          sum += data_[cols_ * i + n] * m(n, j) ;
        }

        //write the final sum to the cell in the result
        R.set_val(i, j, sum) ;
      }
    dirty_ = true ;

    return R ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T>& BaseMatrix<T>::operator+(BaseMatrix const& m)
  {
    if (m.num_rows() != rows_ || m.num_cols() != cols_)
    {
      throw idmex ;
    }
    if (read_only_)
    {
      throw romex ;
    }

    BaseMatrix<T> R ;
    R.set_size(rows_, cols_) ;

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        R.set_val(i, j, (data_[cols_ * i + j] + m(i, j))) ;
      }
    dirty_ = true ;

    return R ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T>& BaseMatrix<T>::operator+=(BaseMatrix const& m)
  {
    if (read_only_)
    {
      throw romex ;
    }

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        data_[cols_ * i + j] += m(i, j) ;
      }
    dirty_ = true ;

    return *this ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T>& BaseMatrix<T>::operator+=(T value)
  {
    if (read_only_)
    {
      throw romex ;
    }

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        data_[cols_ * i + j] = data_[cols_ * i + j] + value ;
      }
    dirty_ = true ;

    return *this ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T>& BaseMatrix<T>::operator=(BaseMatrix<T> const& m)
  {
    if (read_only_)
    {
      throw romex ;
    }

    if (rows_ != m.num_rows() || cols_ != m.num_cols())
    {
      rows_ = m.num_rows() ;
      cols_ = m.num_cols() ;

      resize(rows_, cols_) ;
    }

    std::memcpy(data_, m.data_, (sizeof(T) * cols_ * rows_)) ;
    dirty_ = true ;

    return *this ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::calc_min(bool exclude_zero)
  {
    if (this->min_ >= (T) std::numeric_limits<T>::max() - 1)
    {
      this->min_ = this->min(exclude_zero) ;
    }

  } // ------------------------------------------------------------------------


template<typename T>
  T* BaseMatrix<T>::begin()
  {
    return &(data_[0]);

  } // ------------------------------------------------------------------------

template<typename T>
T* BaseMatrix<T>::get_address_at(unsigned row, unsigned col)
{
  return &(data_[cols_ * row + col]) ;
}


template<typename T>
  void BaseMatrix<T>::calc_max(bool exclude_zero)
  {
    if (this->max_ <= (T) std::numeric_limits<T>::min() + 1)
    {
      this->max_ = this->max(exclude_zero) ;
    }

  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::calc_max()
  {
    this->calc_max(false) ;

  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::calc_min()
  {
    this->calc_min(false) ;

  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::get_max()
  {
    this->calc_max(false) ;

    return max_ ;

  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::get_min()
  {
    this->calc_min(false) ;

    return min_ ;

  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::get_max(bool exclude_zero)
  {
    this->calc_max(exclude_zero) ;

    return max_ ;

  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::get_min(bool exclude_zero)
  {
    this->calc_min(exclude_zero) ;

    return min_ ;

  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T> BaseMatrix<T>::transpose()
  {
    unsigned trows = cols_, tcols = rows_ ;
    BaseMatrix<T> mtrx ;

    mtrx.set_size(trows, tcols) ;
    mtrx.zero_fill() ;

    for (unsigned i = 0; i < trows; i++)
    {
      for (unsigned j = 0; j < tcols; j++)
      {
        mtrx.set_val(i, j, data_[trows * j + i]) ;
      }
    }

    return mtrx ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T> BaseMatrix<T>::flip_horizontal()
  {
    unsigned trows = rows_, tcols = cols_ ;
    BaseMatrix<T> mtrx ;

    mtrx.set_size(trows, tcols) ;
    mtrx.zero_fill() ;

    for (unsigned i = 0; i < trows; i++)
    {
      for (unsigned j = 0; j < tcols; j++)
      {
        mtrx.set_val(i, j, data_[(tcols * i) + (tcols - j - 1)]) ;
      }
    }

    return mtrx ;
  } // ------------------------------------------------------------------------

template<typename T>
  BaseMatrix<T> BaseMatrix<T>::flip_vertical()
  {

    unsigned trows = rows_, tcols = cols_ ;
    BaseMatrix<T> mtrx ;

    mtrx.set_size(trows, tcols) ;
    mtrx.zero_fill() ;

    for (unsigned i = 0; i < trows; i++)
    {
      for (unsigned j = 0; j < tcols; j++)
      {
        mtrx.set_val(i, j, data_[(tcols * (trows - i - 1)) + j]) ;
      }
    }

    return mtrx ;
  } // ------------------------------------------------------------------------

template<typename T>
  T* BaseMatrix<T>::get_data() const
  {
    return data_ ;
  } // ------------------------------------------------------------------------

template<typename T>
  unsigned BaseMatrix<T>::get_dim(unsigned dim)
  {
    return dims_[dim] ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::set_dim(unsigned dim, unsigned z)
  {
    dims_[dim] = z ;
  } // ------------------------------------------------------------------------

template<typename T>
  inline
  unsigned BaseMatrix<T>::num_rows() const
  {
    return rows_ ;
  } // ------------------------------------------------------------------------

template<typename T>
  unsigned BaseMatrix<T>::num_cols() const
  {
    return cols_ ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::set_size(unsigned rows, unsigned cols)
  {
    if (read_only_)
    {
      throw romex ;
    }
    resize(rows, cols) ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::get_val(unsigned row, unsigned col)
  {
    return data_[cols_ * row + col] ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::get_value(unsigned row, unsigned col)
  {
    return data_[cols_ * row + col] ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::zero_fill()
  {
    if (read_only_)
    {
      throw romex ;
    }

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        data_[(cols_ * i) + j] = 0.0 ;

      }

    max_ = 0.0 ;
    min_ = 0.0 ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::random_fill()
  {
    if (read_only_)
    {
      throw romex ;
    }
    srand((unsigned) time(NULL)) ;
    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        data_[(cols_ * i) + j] = (rand() % 100) / 10 ;

        if (data_[(cols_ * i) + j] > max_)
          max_ = data_[(cols_ * i) + j] ;

        if (data_[(cols_ * i) + j] < min_)
          min_ = data_[(cols_ * i) + j] ;
      }
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::resize(unsigned rows, unsigned cols)
  {
    if (read_only_)
    {
      throw romex ;
    }

    if (data_ != NULL) delete[] data_ ;
    rows_ = rows ;
    cols_ = cols ;
    data_ = new T[rows * cols] ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::set_val(unsigned row, unsigned col, T value)
  {
    if (read_only_)
    {
      throw romex ;
    }
    data_[cols_ * row + col] = value ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::set_value(unsigned row, unsigned col, T value)
  {
    if (read_only_)
    {
      throw romex ;
    }
    data_[cols_ * row + col] = value ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::add_ghost_cells(unsigned grows, unsigned gcols, T value)
  {
    if (read_only_) {throw romex ; }
    
    T* bak = new T[rows_ * cols_] ;

    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        bak[(cols_ * i) + j] = data_[(cols_ * i) + j] ;
      }

    unsigned ocols = cols_ ; // save original cols_

    resize(rows_ + grows * 2, cols_ + gcols * 2) ;

    for (unsigned i = 0, k = i - grows; i < rows_; i++, k++)
      for (unsigned j = 0, l = j - gcols; j < cols_; j++, l++)
      {
        data_[(cols_ * i) + j] =
            ((i >= grows) && (j >= gcols) && (i < (rows_ - grows))
              && (j < (cols_ - gcols))) ? bak[(ocols * k) + l] : value ;
      }

    delete[] bak ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::get_neighbor(unsigned row, unsigned col, Direction d)
  {

    if (row >= rows_ || col >= cols_)
    {
      throw iobex ;
    }

    switch (d) {
    case N: {
      row-- ;
      break ;
    }
    case S: {
      row++ ;
      break ;
    }
    case E: {
      col++ ;
      break ;
    }
    case W: {
      col-- ;
      break ;
    }
    case NE: {
      row-- ;
      col++ ;
      break ;
    }
    case NW: {
      row-- ;
      col-- ;
      break ;
    }
    case SE: {
      row++ ;
      col++ ;
      break ;
    }
    case SW: {
      row++ ;
      col-- ;
      break ;
    }
    }

    return data_[(cols_ * row) + col] ;
  } // ------------------------------------------------------------------------

template<typename T>
  void BaseMatrix<T>::get_neighbors(unsigned row, unsigned col, T* neighbors)
  {

    if (row >= rows_ || col >= cols_)
    {

      throw iobex ;
    }

    col = (col == 0) ? 1 : col ;

    neighbors[0] = (*this)(row - 1, col) ; // N
    neighbors[1] = (*this)(row - 1, col + 1) ; // NE
    neighbors[2] = (*this)(row, col + 1) ; // E
    neighbors[3] = (*this)(row + 1, col + 1) ; // SE
    neighbors[4] = (*this)(row + 1, col) ; // S
    neighbors[5] = (*this)(row + 1, col - 1) ; // SW
    neighbors[6] = (*this)(row, col - 1) ; // W
    neighbors[7] = (*this)(row - 1, col - 1) ; // NW

  } // ------------------------------------------------------------------------

template<typename T>
  std::vector<int> BaseMatrix<T>::find(T value)
  {
    return find(value, 0) ;
  } // ------------------------------------------------------------------------

template<typename T>
  std::vector<int> BaseMatrix<T>::find(T value, float eps)
  {
    return find(value, eps, -1) ;
  } // ------------------------------------------------------------------------

template<typename T>
  std::vector<int> BaseMatrix<T>::find(T value, float eps, int row)
  {
    return find(value, eps, row, -1) ;
  } // ------------------------------------------------------------------------

template<typename T>
  std::vector<int> BaseMatrix<T>::find(T value, float eps, int row, int col)
  {
    std::vector<int> rvals(3, -1) ;
    rvals[0] = 0 ;

    unsigned i, j ;
    if (row >= 0)
      i = row ;
    else if (col >= 0)
      j = col ;
    else
      i = j = 0 ;

    bool found = false ;

    for ( ; i < rows_ && (!found); i++)
      for ( ; j < cols_ && (!found); j++)
      {
        if (abs(data_[(cols_ * i) + j] - value) <= eps)
        {
          rvals[0] = 1 ;
          rvals[1] = i ;
          rvals[2] = j ;

          found = true ;
        }
      }

    return rvals ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::max()
  {
    return max(false) ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::min()
  {
    return min(false) ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::max(bool exclude_zero)
  {
    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        T v = data_[(cols_ * i) + j] ;

        if (v > max_ && (v != 0 || !exclude_zero))
          max_ = v ;
      }

    return max_ ;
  } // ------------------------------------------------------------------------

template<typename T>
  T BaseMatrix<T>::min(bool exclude_zero)
  {
    for (unsigned i = 0; i < rows_; i++)
      for (unsigned j = 0; j < cols_; j++)
      {
        T v = data_[(cols_ * i) + j] ;

        if (v < min_ && (v != 0 || !exclude_zero))
          min_ = v ;
      }

    return min_ ;
  } // ------------------------------------------------------------------------

template<typename T>
  std::string BaseMatrix<T>::about()
  {
    return VERSION_STRING ;
  } // ------------------------------------------------------------------------

// ------------------------------------------------------------------------
/*template<typename T>
  std::ostream& operator<<(std::ostream& out, BaseMatrix<T>& M)
  {
    unsigned m = M.num_rows() ;
    unsigned n = M.num_cols() ;

    for (unsigned i = 0; i < m; i++)
    {
      for (unsigned j = 0; j < n; j++)
      {
        out << M(i, j) ;

        if (j < n - 1)
        {
          out << " " ;
        }
      }
      out << std::endl ;
    }

    return out << std::endl ;
  } // ------------------------------------------------------------------------
*/
#endif
