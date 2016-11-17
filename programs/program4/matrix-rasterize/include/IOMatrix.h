/**
 * IOMatrix
 *
 * Gives a BaseMatrix the ability to read and write itself as a file.
 *  Can also handle gzip compression.
 *
 * requires gzip library (-lz option in gcc)
 *
 *  Created on: Feb 06, 2015
 *      Author: rmarshall42
 */

#ifndef IMATRIX_H
#define IMATRIX_H


#include <zlib.h>
#include "FlexMatrix.h"

#undef VERSION_STRING
#define VERSION_STRING "IOMatrix 0.1"

#define GZ_BUFFER_SZ 8192

template<class T>
  class IOMatrix : public FlexMatrix<T> {
  public:
    //IOMatrix<T>() ;
    //IOMatrix<T>(unsigned rows, unsigned cols) ;
    //IOMatrix<T>(IOMatrix const& m) ; // not implemented

   // using FlexMatrix<T>::FlexMatrix; // inherit constructors from BaseMatrix (c++11 gcc 4.8+)
    
    IOMatrix<T>() : FlexMatrix<T>(){}

    IOMatrix<T>(unsigned rows, unsigned cols, bool is2d) : FlexMatrix<T>( rows,  cols, is2d){

      this->rows_ = rows;
      this->cols_ = cols;

      /*if(is2d)
      {
        this->set_data_layer_ndims(2);
        this->allocate(rows,cols);
      }*/
    }

    int load_from_file(std::string& filepath) ;
    int load_from_file(unsigned rows, unsigned cols, std::string& filepath) ;
    int load_from_file(unsigned rows, unsigned cols, std::string& filepath, unsigned hoffset, unsigned voffset);
    std::string about() ;

  protected:
    bool gzip_enabled_ ;
    void load_from_file_gz(gzFile* infile) ;
    void load_from_file_gz(gzFile* infile, unsigned hoffset, unsigned voffset);

  } ;
// ----------------------------------------------------------------------

class MatrixIOException : public std::exception {
  virtual const char* what() const throw ()
  {
    return "Error reading input file" ;
  }
} mioex ;

// --------------------------------------------------------------------------

/*template<class T>
  IOMatrix<T>::IOMatrix()
  {
    this->rows_ = 0 ;
    this->cols_ = 0 ;
    this->ndims_ = 2 ;
    this->dims_ = NULL ;
    this->data_ = NULL ;

    this->max_ = (T) std::numeric_limits<int>::min() ;
    this->min_ = (T) std::numeric_limits<int>::max() ;

  }*/ // ------------------------------------------------------------------------

/*template<class T>
  IOMatrix<T>::IOMatrix(unsigned rows, unsigned cols)
  {
    this->rows_ = rows ;
    this->cols_ = cols ;

    if (rows == 0 || cols == 0)
      throw zszex ;

    this->data_ = new T[rows * cols] ;
    this->ndims_ = 2 ;
    this->dims_ = NULL ;

    this->max_ = (T) std::numeric_limits<int>::min() ;
    this->min_ = (T) std::numeric_limits<int>::max() ;

  }*/ // ------------------------------------------------------------------------

template<typename T>
  void IOMatrix<T>::load_from_file_gz(gzFile* infile)
  {

    int rownum = 0 ;
    std::string line ;

    unsigned char unzipBuffer[GZ_BUFFER_SZ] ;
    unsigned int unzippedBytes ;

    while (true)
    {
      unzippedBytes = gzread(*infile, unzipBuffer, GZ_BUFFER_SZ) ;
      if (unzippedBytes > 0)
      {
        for (unsigned i = 0; i < unzippedBytes; i++)
        {

          if (unzipBuffer[i] == '\n')
          {
            std::vector<std::string> row = stringutils::split(line, ' ') ;
            std::string val ;
            std::vector<std::string>::iterator strit = row.begin() ;

            for (unsigned j = 0; strit != row.end(); strit++, j++)
            {
              val = *strit ;
              // see if it is has a decimal point
              if (val.find(".") != std::string::npos)
                this->data_[this->cols_ * (rownum) + j] = atof(val.c_str()) ;
              else
                this->data_[this->cols_ * (rownum) + j] = atoi(val.c_str()) ;

              if (this->data_[this->cols_ * (rownum) + j] > this->max_)
                this->max_ = this->data_[this->cols_ * (rownum) + j] ;

              if (this->data_[this->cols_ * (rownum) + j] < this->min_)
                this->min_ = this->data_[this->cols_ * (rownum) + j] ;
            }

            rownum++ ;
            line = "" ;

          }
          else
          {
            line.append(1, unzipBuffer[i]) ;
          }
        }
      }
      else
      {
        break ;
      }
    }

  }


template<typename T>
  void IOMatrix<T>::load_from_file_gz(gzFile* infile, unsigned hoffset, unsigned voffset)
  {

    int rownum = 0 ;
    std::string line ;

    unsigned char unzipBuffer[GZ_BUFFER_SZ] ;
    unsigned int unzippedBytes ;

    while (true)
    {
      unzippedBytes = gzread(*infile, unzipBuffer, GZ_BUFFER_SZ) ;
      if (unzippedBytes > 0)
      {
        for (unsigned i = 0; i < unzippedBytes; i++)
        {

          if (unzipBuffer[i] == '\n')
          {
            std::vector<std::string> row = stringutils::split(line, ' ') ;
            std::string val ;
            std::vector<std::string>::iterator strit = row.begin() ;

            for (unsigned j = hoffset; strit != row.end()-hoffset; strit++, j++)
            {
              val = *strit ;
              // see if it is has a decimal point
              if (val.find(".") != std::string::npos)
                this->data_[this->cols_ * (rownum) + j] = atof(val.c_str()) ;
              else
                this->data_[this->cols_ * (rownum) + j] = atoi(val.c_str()) ;

              if (this->data_[this->cols_ * (rownum) + j] > this->max_)
                this->max_ = this->data_[this->cols_ * (rownum) + j] ;

              if (this->data_[this->cols_ * (rownum) + j] < this->min_)
                this->min_ = this->data_[this->cols_ * (rownum) + j] ;
            }

            rownum++ ;
            line = "" ;

          }
          else
          {
            line.append(1, unzipBuffer[i]) ;
          }
        }
      }
      else
      {
        break ;
      }
    }

  }






template<typename T>
  int IOMatrix<T>::load_from_file(unsigned rows, unsigned cols,
      std::string& filepath)
  {

    unsigned j = 0;
    int rownum = 0 ;
    std::string line ;

    this->set_size(rows, cols) ;

    if (filepath.substr(filepath.find_last_of(".") + 1) == "gz")
    {
      gzFile inFileZ = gzopen(filepath.c_str(), "rb") ;
      if (inFileZ == NULL)
      {
        printf("Error: Failed to gzopen %s\n", filepath.c_str()) ;
        exit(0) ;
      }

      std::string cell_value = "" ;
      load_from_file_gz(&inFileZ) ;
      gzclose(inFileZ) ;

    }
    else
    {
      std::ifstream infile(filepath.c_str()) ;

      if (!infile.is_open())
      {
        std::cerr << filepath << std::endl ;

        throw mioex ;
      }

      if (infile.is_open())
      {
        std::getline(infile, line) ;
        while (infile)
        {
          std::vector<std::string> row = stringutils::split(line, ' ') ;
          std::string val ;
          std::vector<std::string>::iterator strit = row.begin() ;
          j = 0 ;

          for ( ; strit != row.end(); strit++, j++)
          {
            val = *strit ;

            if (val.find(".") != std::string::npos) // is a float
              this->data_[this->cols_ * (rownum) + j] = atof(val.c_str()) ;
            else if (! stringutils::is_numeric(val) && val.size() == 1) // is a char
            {
              this->data_[this->cols_ * (rownum) + j] = val[0] ;
            }
            else
              // is an int
              this->data_[this->cols_ * (rownum) + j] = atoi(val.c_str()) ;

            if (this->data_[this->cols_ * (rownum) + j] > this->max_)
              this->max_ = this->data_[this->cols_ * (rownum) + j] ;

            if (this->data_[this->cols_ * (rownum) + j] < this->min_)
              this->min_ = this->data_[this->cols_ * (rownum) + j] ;

          }

          rownum++ ;
          std::getline(infile, line) ;
        }
        infile.close() ;

      }
      else
      {
        std::cout << "Unable to open file" ;
      }
    }

    return (rownum) * j ;
  } // ------------------------------------------------------------------------


template<typename T>
  int IOMatrix<T>::load_from_file(unsigned rows, unsigned cols,
      std::string& filepath, unsigned hoffset, unsigned voffset)
  {

    unsigned j = 0;
    int rownum = 0 ;
    std::string line ;

    this->set_size(rows, cols) ;

    if (filepath.substr(filepath.find_last_of(".") + 1) == "gz")
    {
      gzFile inFileZ = gzopen(filepath.c_str(), "rb") ;
      if (inFileZ == NULL)
      {
        printf("Error: Failed to gzopen %s\n", filepath.c_str()) ;
        exit(0) ;
      }

      std::string cell_value = "" ;
      load_from_file_gz(&inFileZ,hoffset,voffset) ;
      gzclose(inFileZ) ;

    }
    else
    {
      std::ifstream infile(filepath.c_str()) ;

      if (!infile.is_open())
      {
        std::cerr << filepath << std::endl ;

        throw mioex ;
      }

      if (infile.is_open())
      {
        std::getline(infile, line) ;
        while (infile)
        {
          std::vector<std::string> row = stringutils::split(line, ' ') ;
          std::string val ;
          std::vector<std::string>::iterator strit = row.begin() ;
          j = 0 ;

          for ( ; strit != row.end(); strit++, j++)
          {
            val = *strit ;

            if (val.find(".") != std::string::npos) // is a float
              this->data_[this->cols_ * (rownum) + j] = atof(val.c_str()) ;
            else if (! stringutils::is_numeric(val) && val.size() == 1) // is a char
            {
              this->data_[this->cols_ * (rownum) + j] = val[0] ;
            }
            else
              // is an int
              this->data_[this->cols_ * (rownum) + j] = atoi(val.c_str()) ;

            if (this->data_[this->cols_ * (rownum) + j] > this->max_)
              this->max_ = this->data_[this->cols_ * (rownum) + j] ;

            if (this->data_[this->cols_ * (rownum) + j] < this->min_)
              this->min_ = this->data_[this->cols_ * (rownum) + j] ;

          }

          rownum++ ;
          std::getline(infile, line) ;
        }
        infile.close() ;

      }
      else
      {
        std::cout << "Unable to open file" ;
      }
    }

    return (rownum) * j ;
  } // ------------------------------------------------------------------------




/**

  Required format

  r c on the first line


*/
template<typename T>
  int IOMatrix<T>::load_from_file(std::string& filepath)
  {
    unsigned i, j ;
    //unsigned count = 0 ;
    int rownum = 0 ;

    if (filepath.substr(filepath.find_last_of(".") + 1) == "gz")
    {
      gzFile inFileZ = gzopen(filepath.c_str(), "rb") ;
      if (inFileZ == NULL)
      {
        printf("Error: Failed to gzopen %s\n", filepath.c_str()) ;
        exit(0) ;
      }
      unsigned char unzipBuffer[GZ_BUFFER_SZ] ;
      unsigned int unzippedBytes ;
      std::vector<unsigned char> unzippedData ;
      while (true)
      {
        unzippedBytes = gzread(inFileZ, unzipBuffer, GZ_BUFFER_SZ) ;
        if (unzippedBytes > 0)
        {
          for (i = 0 ; i < unzippedBytes; i++)
          {
            unzippedData.push_back(unzipBuffer[i]) ;
          }
        }
        else
        {
          break ;
        }
      }
      gzclose(inFileZ) ;
    }
    else
    {
      std::ifstream infile(filepath.c_str()) ;

      if (!infile.is_open())
      {
        std::cerr << filepath << std::endl ;

        throw mioex ;
      }

      std::string line ;

      if (infile.is_open())
      {
        std::getline(infile, line) ;

        while (! infile.eof())
        {
          std::vector<std::string> row = stringutils::split(line, ' ') ;

          if (rownum == 0)
          {
            this->rows_ = atoi(row[0].c_str()) ;
            this->cols_ = atoi(row[1].c_str()) ;

            this->set_size(this->rows_, this->cols_) ;

          }
          else
          {               
            std::string val ;
            std::vector<std::string>::iterator strit = row.begin() ;
            j = 0 ;

            for ( ; strit != row.end(); strit++, j++)
            {
              val = *strit ;

              if (val.find(".") != std::string::npos) // is a float
                this->data_[this->cols_ * (rownum - 1) + j] = atof(
                    val.c_str()) ;
              else
                // is an int
                this->data_[this->cols_ * (rownum - 1) + j] = atoi(
                    val.c_str()) ;

              if (this->data_[this->cols_ * (rownum - 1) + j] > this->max_)
                this->max_ = this->data_[this->cols_ * (rownum - 1) + j] ;

              if (this->data_[this->cols_ * (rownum - 1) + j] < this->min_)
                this->min_ = this->data_[this->cols_ * (rownum - 1) + j] ;
            
            }
          }

          rownum++ ;
          
          std::getline(infile, line) ;

        }
        infile.close() ;
      }
      else
      {
        std::cout << "Unable to open file" ;
      }
    }

    return (rownum - 1) * j ;
  } // ------------------------------------------------------------------------

template<typename T>
  std::string IOMatrix<T>::about()
  {
    return VERSION_STRING ;
  } // ------------------------------------------------------------------------

// ------------------------------------------------------------------------

template<typename T>
  std::ostream& operator<<(std::ostream& out, IOMatrix<T>& M)
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
