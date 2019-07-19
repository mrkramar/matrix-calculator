#ifndef __csparsematrix_h__
#define __csparsematrix_h__

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

#include "CMatrix.h"
#include "CDenseMatrix.h"

using namespace std;

/**
 * Sparse matrix
 * Data is stored in map
 * Map key is pair of dimensions ( height , width )
 * All other elements are zeros
 */
class CSparseMatrix : public CMatrix
{
    private:
        map < pair < int , int > , double > m_matrix;
    public:
        CSparseMatrix ();
        ~CSparseMatrix ();
        CSparseMatrix ( unique_ptr < CMatrix > & x );  

        void setElem ( int height , int width , double value );
        double getElem ( int height , int width ) const;

        unique_ptr < CMatrix > cut ( int fromHeight , int fromWidth , int cutHeight , int cutWidth ) const;
        unique_ptr < CMatrix > transpose () const;

        CMatrix & operator = ( const CMatrix & x );
};

#endif /* __csparsematrix_h__ */