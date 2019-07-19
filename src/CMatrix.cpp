#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <iomanip>
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
#include <cmath>

#include "CMatrix.h"
#include "CDenseMatrix.h"

using namespace std;


const char* ilegalDimensionException :: msg()
{
    return "Ilegal dimensions of matrix/matrices!";
}

const char* irregularException :: msg()
{
    return "Matrix is irregular, operation cannot be preformed";
}

void CMatrix :: print () const
{
    for ( int h = 0 ; h < m_height ; h++ )
    {
        for ( int w = 0 ; w < m_width ; w++ )
        {
           cout << setprecision ( 2 ) << setw ( 10 ) << left << getElem ( h , w ); 
        }
        cout << endl << endl;
    }   
}

unique_ptr < CMatrix > CMatrix :: operator * ( double x ) const
{                    
    CDenseMatrix ret ( m_height , m_width );
    for ( int h = 0 ; h < m_height ; h++ )
        for ( int w = 0 ; w < m_width ; w++ )
        {
            ret . setElem ( h , w , getElem ( h , w ) * x );
        }

    unique_ptr < CMatrix > tmp = make_unique < CDenseMatrix > ( ret );              
    return tmp;
}

unique_ptr < CMatrix > CMatrix :: operator * ( const CMatrix & x ) const
{
    if ( m_width != x . m_height )
    {
        throw ilegalDimensionException();    
    }
    CDenseMatrix ret ( m_width , x . m_height );

    double valueNew = 0;
    for( int h = 0 ; h < m_height ; h++ )
    {
        for( int y = 0; y < x . m_width ; y++ )
        {
            for( int w ; w < m_width ; w++ )
            {
                valueNew += getElem ( h , w ) * x . getElem ( w , y );
                ret . setElem ( h , y , valueNew );
            }            
        }
    }      
    return make_unique < CDenseMatrix > ( ret ); 
}

unique_ptr < CMatrix > CMatrix :: operator + ( const CMatrix & x ) const
{                    
    if ( m_width != x . m_width && m_height != x . m_height )
    {
        throw ilegalDimensionException();    
    }

    CDenseMatrix ret ( m_height , m_width );
    for ( int h = 0 ; h < m_height ; h++ )
        for ( int w = 0 ; w < m_width ; w++ )
        {
            ret . setElem ( h , w , getElem ( h , w ) + x . getElem ( h , w ) );
        }

    unique_ptr < CMatrix > tmp = make_unique < CDenseMatrix > ( ret );              
    return tmp;
}

unique_ptr < CMatrix > CMatrix :: operator - ( const CMatrix & x ) const
{
    if ( m_width != x . m_width || m_height != x . m_height )
    {
        throw ilegalDimensionException();    
    }

    CDenseMatrix ret ( m_height , m_width );
    for ( int h = 0 ; h < m_height ; h++ )
        for ( int w = 0 ; w < m_width ; w++ )
        {
            ret . setElem ( h , w , getElem ( h , w ) - x . getElem ( h , w ) );
        }
           
    return make_unique < CDenseMatrix > ( ret );
}  

unique_ptr < CMatrix > CMatrix :: merge ( const CMatrix & x, const string direction ) const
{
    if ( direction == "bottom" || direction == "b" )
    {
        if ( m_width != x . m_width )
        {
            throw ilegalDimensionException();
        }
        CDenseMatrix ret ( m_height + x . m_height , m_width );
        for ( int h = 0 ; h < ret . m_height ; h++ )
            for ( int w = 0 ; w < ret . m_width ; w++ )
            {
                if ( h < m_height )
                    ret . setElem ( h , w , getElem ( h, w ) );
                else
                    ret . setElem ( h , w , x . getElem ( h - m_height, w ) );
            }
        return make_unique < CDenseMatrix > ( ret );
    }
    else if ( direction == "side" || direction == "s" )
    {
        if ( m_height != x . m_height )
        {
            throw ilegalDimensionException();
        }
        CDenseMatrix ret ( m_height , m_width + x . m_width );
        for ( int h = 0 ; h < ret . m_height ; h++ )
            for ( int w = 0 ; w < ret . m_width ; w++ )
            {
                if ( w < m_width )
                    ret . setElem ( h , w , getElem ( h , w ) );
                else
                    ret . setElem ( h , w , x . getElem ( h , w - m_width ) );
            }
        return make_unique < CDenseMatrix > ( ret );
    }
    else
        return nullptr;
}

unique_ptr < CMatrix > CMatrix :: gaussElim () const
{
    CDenseMatrix ret;
    ret = * this;
    int n , m;

	n = ret . m_height;
	m = ret . m_width;

    for ( int i = 0 ; i < n ; i++ ) 
    {
        // Search for maximum in this column
        double maxEl = fabs ( ret . getElem ( i , i ) );
        int maxRow = i;
        for ( int k = i + 1 ; k < n ; k++ ) 
        {
            if ( fabs ( ret . getElem ( k , i ) ) > maxEl ) 
            {
                maxEl = fabs ( ret . getElem ( k , i ) );
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        for ( int k = i ; k < m ; k++ ) 
        {
            double tmp = ret . getElem(maxRow, k);
            ret . setElem(maxRow, k, ret . getElem(i, k));
            ret . setElem(i, k,tmp);
        }

        // Make all rows below this one 0 in current column
        for ( int k = i + 1 ; k < ret . m_height ; k++ ) 
        {
            double c = - ret . getElem ( k , i ) / ret . getElem ( i , i );
            if ( c != c ) // in case of dividing by 0
            {
                c = 0;
            }
            for ( int j = i ; j < ret . m_width ; j++ ) 
            {
                if ( i == j ) 
                {
                    ret . setElem ( k , j , 0 );
                } 
                else 
                {
                    ret . setElem ( k , j , ( ret . getElem ( k , j ) + c * ret . getElem ( i , j ) ) );
                }
            }
        }
    }

    return make_unique <CDenseMatrix> ( ret );
}

unique_ptr < CMatrix > CMatrix :: inverse () const
{
    if ( m_height != m_width )
    {
        throw irregularException();
    }
    if ( determinant() == 0 )
    {
        throw irregularException();
    }

	// making singular matrix
    CDenseMatrix single ( m_height , m_width );
    for ( int h = 0 ; h < single . m_height ; h++ )
    {
        for ( int w = 0 ; w < single . m_width ; w++ )
        {
            if ( w == h )
            {
                single . setElem ( h , w , 1 );
            }
            else
            {
                single . setElem ( h , w , 0 );
            }
        }
    }
    unique_ptr < CMatrix > sg; 
    sg = make_unique < CDenseMatrix > ( single );
    unique_ptr < CMatrix > a;
    a = merge ( * sg , "s" ); // merges with singular matrix
    int n = m_height;
    for ( int i = 0 ; i < n ; i++ ) 
    {
        // Search for maximum in this column
        double maxEl = fabs( a -> getElem ( i , i ) );
        int maxRow = i;
        for ( int k = i + 1 ; k < n ; k++) 
        {
            if ( fabs ( a -> getElem ( k , i ) ) > maxEl ) 
            {
                maxEl = a -> getElem ( k , i );
                maxRow = k;
            }
        }
        // Swap maximum row with current row (column by column)
        for ( int k = i ; k < 2 * n ; k++ ) 
        {
            double tmp = a -> getElem ( maxRow , k );
            a -> setElem ( maxRow , k , a -> getElem ( i , k ) );
            a -> setElem ( i , k , tmp );
        }
        // Make all rows below this one 0 in current column
        for ( int k = i + 1 ; k < n ; k++ ) 
        {
            double c = - a -> getElem ( k , i ) / a -> getElem ( i , i );
            if ( c != c )
            {
                c = 0;
            }

            for ( int j = i ; j < 2 * n ; j++ ) 
            {
                if ( i == j ) 
                {
                    a -> setElem ( k , j , 0 );
                } 
                else 
                {
                    a -> setElem ( k , j , ( a -> getElem ( k , j ) + c * a -> getElem ( i , j ) ) );
                }
            }
        }
        for ( int i = n - 1 ; i >= 0 ; i-- ) 
        {
            for ( int k = n ; k < 2 * n ; k++ ) 
            {
                a-> setElem ( i , k , ( a -> getElem ( i , k ) / a -> getElem ( i , i ) ) );
            }

            for ( int rowModify = i - 1 ; rowModify >= 0 ; rowModify-- ) 
            {
                for ( int columModify = n ; columModify < 2 * n ; columModify++ ) 
                {
                    a -> setElem ( rowModify , columModify , ( a -> getElem (rowModify, columModify) - 
                    a -> getElem ( i , columModify ) * a -> getElem ( rowModify , i ) ) );
                }
            }
        }
    }
    return a -> cut ( 1 , m_width + 1 , m_height , m_width );

}

double CMatrix :: determinant () const
{
    if ( m_height != m_width )
    {
        throw irregularException();
    }
    unique_ptr < CMatrix > tmp;
    tmp = gaussElim();
    double ret = 1;
    // multiplies elements on diagonal
    for ( int y = 0 ; y < m_width ; y++ )
    {
        ret *= tmp -> getElem ( y , y );   
    }
    return ret;
}

int CMatrix :: rank () const
{
    unique_ptr < CMatrix > tmp;
    tmp = gaussElim();

    int zeroLines = 0;
    for ( int h = 0 ; h < m_height ; h++ )
    {
        int flagZero = 0;
        for ( int w = 0 ; w < m_width ; w++ )
        {
            if ( tmp -> getElem ( h , w ) == 0 )
            {
                flagZero++;
            }
            else break;
        }
        // if whole line has all elements zeros
        if ( flagZero == m_width )
        {
            zeroLines++;
        }
    }
    return m_height - zeroLines;
}
