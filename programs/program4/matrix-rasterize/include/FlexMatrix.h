/**
 * FlexMatrix
 *
 * Gives a BaseMatrix the ability to read and write itself into a C-style 2D array
 *
 * requires gzip library (-lz option in gcc)
 *
 *  Created on: Sep 01, 2015
 *      Author: rmarshall42
 */

#ifndef FMATRIX_H
#define FMATRIX_H


#include <zlib.h>
#include "BaseMatrix.h"

#undef VERSION_STRING
#define VERSION_STRING "FlexMatrix 0.1"

#define GZ_BUFFER_SZ 8192

template<class T>
  class FlexMatrix : public BaseMatrix<T> {
  public:
    T** data2d_;
    //using BaseMatrix<T>::BaseMatrix; // inherit constructors from BaseMatrix (c++11 gcc 4.8+)
    FlexMatrix<T>() : BaseMatrix<T>(){

      data2d_ = NULL;
    }

    FlexMatrix<T>(unsigned rows, unsigned cols) : BaseMatrix<T>( ){
      data2d_ = NULL;
      /*this->rows_ = rows;
      this->cols_ = cols;
      this->data_ = new T[rows * cols] ;*/
    }


    FlexMatrix<T>(unsigned rows, unsigned cols, bool is2d) : BaseMatrix<T>( ){

      data2d_ = NULL;
      this->rows_ = rows;
      this->cols_ = cols;

      if(is2d)
      {
        this->set_data_layer_ndims(2);
        this->allocate(rows,cols);
      }
      else
      {
        this->data_ = new T[rows * cols] ;
      }
    }

    ~FlexMatrix<T>() ;
    // overrides from BaseMatrix -------
    T get_val(unsigned row, unsigned col) ;
    T get_value(unsigned row, unsigned col) ;
    void set_val(unsigned row, unsigned col, T value) ;
    void set_value(unsigned row, unsigned col, T value) ;
    void resize(unsigned rows, unsigned cols);
    void get_neighbors(unsigned row, unsigned col, T* neighbors);
    void add_ghost_cells(unsigned grows, unsigned gcols, T value);
    T& operator()(unsigned row, unsigned col) ;
    T operator()(unsigned row, unsigned col) const ;
    FlexMatrix<T>& operator*=(T value);
    FlexMatrix<T>& operator=(BaseMatrix<T> const& m);
    void zero_fill();
    // ----------------------------------

    void allocate(size_t nrows, size_t ncols, T** ptrptr);
    void load_from_ptrptr(size_t nrows, size_t ncols, T** ptrptr);
    T** export_to_ptrptr();
    void load_from_matrix(BaseMatrix<T> M);
    int data_layer_ndims();
    std::string about() ;

  protected:

    //T* data_ = NULL;

    bool is_allocated();
    void allocate(size_t nrows, size_t ncols);
    void set_data_layer_ndims(size_t s);
  private:

    void set(size_t row, size_t col, T value);
  } ;
// ----------------------------------------------------------------------

class MatrixAllocException : public std::exception {
  virtual const char* what() const throw ()
  {
    return "Allocation error" ;
  }
} malex ;

// --------------------------------------------------------------------------
template<class T>
  FlexMatrix<T>::~FlexMatrix()
  {
    if (data2d_ != NULL) delete[] data2d_ ;
  } // ------------------------------------------------------------------------


template<typename T>
  void FlexMatrix<T>::set_data_layer_ndims(size_t s)
  {

    switch(s)
    {
      case 1:{
          if(this->data2d_ != NULL) delete[] this->data2d_;
        break;
      }
      case 2:{
          if(this->data_ != NULL) delete[] this->data_;
        break;
      }
      default:{
        fprintf(stderr, "invalid data layer dims: %d\n", (int)s);
        break;
      }

    }
    this->data_ndims_ = s;
  } // ------------------------------------------------------------------------




template<typename T>
  void FlexMatrix<T>::set(size_t row, size_t col, T value)
  {
    if (this->read_only_) { throw romex ;  }

    if (this->data_ != NULL && data2d_ == NULL)
      this->data_[this->cols_ * row + col] = value ;
    else if (data2d_ != NULL && this->data_ == NULL)
      data2d_[row][col] = value;
    else
    {/* big bad error no data layer allocated*/ }
  }



template<typename T>
  std::string FlexMatrix<T>::about()
  {
    return VERSION_STRING ;
  } // ------------------------------------------------------------------------


template<typename T>
bool FlexMatrix<T>::is_allocated()
{
  return (data2d_ != NULL);
}


template<typename T>
int FlexMatrix<T>::data_layer_ndims()
{
  return this->data_ndims_;
}






/** @override */
template<typename T>
  T& FlexMatrix<T>::operator()(unsigned row, unsigned col)
  {//fprintf(stderr, "HERP\n");
    if (row >= this->rows_ || col >= this->cols_)
    {
      std::cerr << row << " ?= " << this->rows_ << std::endl ;
      std::cerr << col << " ?= " << this->cols_ << std::endl ;
      throw iobex ;
    }



    if(data_layer_ndims() == 1){ 
//fprintf(stderr, "******************* T& operator() called ****** 1 ****** %d ******* \n",(this->cols_ * row + col));

//fprintf(stderr, "%f \n", this->data_[this->cols_ * row + col]);
//fprintf(stderr, "DERP\n");
      return this->data_[this->cols_ * row + col];
    }
    else if (data_layer_ndims() == 2){ 
  //    fprintf(stderr, "******************* T& operator() called ****** 2 ******** %d,%d ***** \n",row,col);
//fprintf(stderr, "%f \n", data2d_[row][col]);
//fprintf(stderr, "DERP\n");
      return data2d_[row][col];
    }
    else { 
      fprintf(stderr, "operator() data_ndims_=%d \n", data_layer_ndims()); exit(1);

       }
    //return ((this->data_ != NULL) ? this->data_[this->cols_ * row + col] : data2d_[row][col]);
    

  } // ------------------------------------------------------------------------

/** @override */
template<typename T>
  T FlexMatrix<T>::operator()(unsigned row, unsigned col) const
  {
    if (row >= this->rows_ || col >= this->cols_)
    {
      throw iobex ;
    }
    fprintf(stderr, "******************* T operator() called ******************* \n");
    //return ((this->data_ != NULL) ? this->data_[this->cols_ * row + col] : data2d_[row][col]);
    return get_val(row,col);
  } // ------------------------------------------------------------------------

/** @override */
template<typename T>
  FlexMatrix<T>& FlexMatrix<T>::operator*=(T value)
  {
    if (this->read_only_)
    {
      throw romex ;
    }

    for (unsigned i = 0; i < this->rows_; i++)
      for (unsigned j = 0; j < this->cols_; j++)
      {
        set(i, j, (get_val(i,j) * value));
        //data_[cols_ * i + j] *= value ;
      }
    this->dirty_ = true ;
    
    return *this ;
  } // ------------------------------------------------------------------------

/** @override */
template<typename T>
  FlexMatrix<T>& FlexMatrix<T>::operator=(BaseMatrix<T> const& m)
  {
    if (this->read_only_)
    {
      throw romex ;
    }

    if (this->rows_ != m.num_rows() || this->cols_ != m.num_cols())
    { fprintf(stderr, "######################################(%d,%d) != (%d,%d)\n",this->rows_, this->cols_,m.num_rows(), m.num_cols());
      this->rows_ = m.num_rows() ;
      this->cols_ = m.num_cols() ;

      resize(this->rows_, this->cols_) ;
    }

   // std::memcpy(data_, m.data_, (sizeof(T) * cols_ * rows_)) ;

    if (! is_allocated()) allocate(m.num_rows(),m.num_cols());

    for(size_t i = 0;i<this->rows_;i++){
      for(size_t j = 0; j < this->cols_;j++){
        data2d_[i][j] = m.get_value(i,j);
      }
    }



    this->dirty_ = true ;

    return *this ;
  } // ------------------------------------------------------------------------


/** @override */
template<typename T>
  T FlexMatrix<T>::get_val(unsigned row, unsigned col)
  {
    return get_value(row,col);
    //return ((this->data_ != NULL) ? this->data_[this->cols_ * row + col] : 
    //data2d_[row][col]);
  } // ------------------------------------------------------------------------


/** @override */
template<typename T>
  T FlexMatrix<T>::get_value(unsigned row, unsigned col)
  {
    return ((this->data_ != NULL) ? this->data_[this->cols_ * row + col] : data2d_[row][col]);
  } // ------------------------------------------------------------------------


/** @override */
template<typename T>
  void FlexMatrix<T>::set_val(unsigned row, unsigned col, T value)
  {
    set_value(row,col,value);
  } // ------------------------------------------------------------------------


/** @override */
template<typename T>
  void FlexMatrix<T>::set_value(unsigned row, unsigned col, T value)
  {
    if (this->read_only_) { throw romex ;  }

    if (this->data_ != NULL && data2d_ == NULL)
      this->data_[this->cols_ * row + col] = value ;
    else if (data2d_ != NULL && this->data_ == NULL)
      data2d_[row][col] = value;
    else
    {/* big bad error no data layer allocated*/ }
  } // ------------------------------------------------------------------------



/** @override */
template<typename T>
  void FlexMatrix<T>::resize(unsigned rows, unsigned cols)
  {
    if (this->read_only_)  { throw romex ;  }

    // 
    switch(data_layer_ndims())
    {
      case 1:{
        //fprintf(stderr, "*************** delete on resize..\n" );
        if (this->data_ != NULL)  delete[] this->data_ ;
        this->rows_ = rows ;
        this->cols_ = cols ;
        this->data_ = new T[rows * cols] ;

        break;
      }
      case 2:{
        allocate(rows,cols);

        break;
      }
      default:{
        fprintf(stderr, 
          "resize:data layer ndims mismatch: %d\n", this->data_ndims_  );
      }


    }


  } // ------------------------------------------------------------------------



/** @override */
template<typename T>
  void FlexMatrix<T>::get_neighbors(unsigned row, unsigned col, T* neighbors)
  {

    if (row >= this->rows_ || col >= this->cols_)
    {

      throw iobex ;
    }

    col = (col == 0) ? 1 : col ;

    neighbors[0] = get_val(row - 1, col) ; // N
    neighbors[1] = (*this).get_val(row - 1, col + 1) ; // NE
    neighbors[2] = (*this).get_val(row, col + 1) ; // E
    neighbors[3] = (*this).get_val(row + 1, col + 1) ; // SE
    neighbors[4] = (*this).get_val(row + 1, col) ; // S
    neighbors[5] = (*this).get_val(row + 1, col - 1) ; // SW
    neighbors[6] = (*this).get_val(row, col - 1) ; // W
    neighbors[7] = (*this).get_val(row - 1, col - 1) ; // NW

  } // ------------------------------------------------------------------------



template<typename T>
void FlexMatrix<T>::allocate(size_t nrows, size_t ncols)
{
    T *data = (T*)malloc( nrows*ncols * sizeof(T) );
    data2d_ = (T**)malloc( nrows*sizeof(T*) );
    for (size_t i=0; i<nrows; i++)
        data2d_[i] = &(data[i*ncols]);

    this->rows_ = nrows;
    this->cols_ = ncols;

    set_data_layer_ndims(2);

}

template<typename T>
void FlexMatrix<T>::allocate(size_t nrows, size_t ncols, T** ptrptr)
{
    
    T *ptr = (T*)malloc( nrows*ncols * sizeof(T) );
    ptrptr = (T**)malloc( nrows*sizeof(T*) );
    for (size_t i=0; i<nrows; i++)
        ptrptr[i] = &(ptr[i*ncols]);

}


template<typename T>
    void FlexMatrix<T>::load_from_ptrptr(size_t nrows, size_t ncols, T** ptrptr)
    {

      if(! is_allocated()) allocate(nrows,ncols);


      for(size_t i=0;i<nrows;++i)
        for(size_t j=0;j<ncols;++j)
          data2d_[i][j] = ptrptr[i][j];

      set_data_layer_ndims(2);
    }

template<typename T>
    void FlexMatrix<T>::load_from_matrix(BaseMatrix<T> M)
    {
      
      size_t nrows = M.num_rows(), ncols = M.num_cols();

      if(! is_allocated()) allocate(nrows, ncols);


      for(size_t i=0;i<nrows;++i)
        for(size_t j=0;j<ncols;++j)
          data2d_[i][j] = M.get_value(i,j);

      this->rows_ = nrows;
      this->cols_ = ncols;

      set_data_layer_ndims(2);


    }



template<typename T>
    T** FlexMatrix<T>::export_to_ptrptr()
    {
      
      size_t
        nrows = this->num_rows(),
        ncols = this->num_cols();

      T *data = (T*)malloc( nrows*ncols * sizeof(T) );
      T **ptrptr = (T**)malloc( nrows*sizeof(T *) );

      for (size_t i=0; i<nrows; i++)
        ptrptr[i] = &(data[i*ncols]);

      for(size_t i = 0; i < nrows; ++i)
        for(size_t j = 0; j < ncols; ++j)
          ptrptr[i][j] = this->get_value(i,j);

      return ptrptr;
    }





/** @override */
template<typename T>
  void FlexMatrix<T>::add_ghost_cells(unsigned grows, unsigned gcols, T value)
  {
    if (this->read_only_) {throw romex ; }

    
    T* bak = new T[this->rows_ * this->cols_] ;

    for (unsigned i = 0; i < this->rows_; i++)
      for (unsigned j = 0; j < this->cols_; j++)
      {
        bak[(this->cols_ * i) + j] = get_value(i,j);

        /*((data_layer_ndims() == 1) ?
          ( this->data_[(this->cols_ * i) + j] ) : ( data2d_[i][j] ));*/
      }

    unsigned ocols = this->cols_ ; // save original cols_

    resize(this->rows_ + grows * 2, this->cols_ + gcols * 2) ;

    for (unsigned i = 0, k = i - grows; i < this->rows_; i++, k++)
      for (unsigned j = 0, l = j - gcols; j < this->cols_; j++, l++)
      {
        //this->data_[(this->cols_ * i) + j] =
        set_value(i, j,
          ( ((i >= grows) && (j >= gcols) && (i < (this->rows_ - grows))
            && (j < (this->cols_ - gcols))) ? bak[(ocols * k) + l] : value ) 
        );
      }

    delete[] bak ;
  } // ------------------------------------------------------------------------

/** @override */
template<typename T>
  void FlexMatrix<T>::zero_fill()
  {
    if (this->read_only_)
    {
      throw romex ;
    }

    for (unsigned i = 0; i < this->rows_; i++)
      for (unsigned j = 0; j < this->cols_; j++)
      {
        set(i, j, 0.0) ;
      }

    this->max_ = 0.0 ;
    this->min_ = 0.0 ;
  } // ------------------------------------------------------------------------

// ------------------------------------------------------------------------


template<typename T>
  std::ostream& operator<<(std::ostream& out, FlexMatrix<T>& M)
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




#endif
