#ifndef __cmatrix_h__
#define __cmatrix_h__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cstring>

using namespace std;

/**
 * Abstract class Matrix with virtual functions that derived classes use
 */
class CMatrix
{
    friend class CInterface;
    protected:

        void print () const;
        
        unique_ptr < CMatrix > merge ( const CMatrix & x , const string direction ) const;
        unique_ptr < CMatrix > operator * ( const CMatrix & x ) const;
        unique_ptr < CMatrix > operator + ( const CMatrix & x ) const;
        unique_ptr < CMatrix > operator - ( const CMatrix & x ) const;
        unique_ptr < CMatrix > operator * ( double x ) const;        
        
        /**
         * Cuts matrix from given position to given dimensions
         */
        virtual unique_ptr < CMatrix > cut ( int fromHeight , int fromWidth ,
                                             int cutHeight , int cutWidth ) const = 0;
        /**
         * Transposes matrix
         */
        virtual unique_ptr < CMatrix > transpose () const = 0;
        
        virtual CMatrix & operator = ( const CMatrix & x ) = 0;

        /**
         * Makes Gauss elimination to selected matrix
         */
        unique_ptr < CMatrix > gaussElim () const;
        
        /**
         * Makes inverse matrix from m x m matrix
         * If dimensions are ilegal , throws an exception
         * Algorithm inspired by https://martin-thoma.com/inverting-matrices/
         */
        unique_ptr < CMatrix > inverse () const;

        /**
         * Counts determinant of m x m matrix
         * If dimensions are ilegal , throws an exception
         * returns double value
         */
        double determinant () const;

        /**
         * Counts rank of matrix
         */
        int rank () const;

   
    public:
        virtual ~CMatrix (){}

        /**
         * Sets element in given position
         */
        virtual void setElem ( int height , int width , double value ) = 0;

        /**
         * Returns element from given position
         */
        virtual double getElem ( int height , int width ) const = 0;
        
        int m_height;
        int m_width;
};

/**
 * This exception is thrown where an operation with ilegal dimension of matrices is used
 */
class ilegalDimensionException : public exception
{
    public:
        const char* msg();
};

class irregularException : public exception
{
    public:
        const char* msg();
};

#endif /* __cmatrix_h__ */