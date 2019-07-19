#ifndef __cdensematrix_h__
#define __cdensematrix_h__

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

using namespace std;

/**
 * Dense matrix
 * All data is stored in 2D vector
 */
class CDenseMatrix : public CMatrix
{
    private:
        vector < vector < double > > m_matrix;
    public:
        CDenseMatrix ();
        ~CDenseMatrix ();
        CDenseMatrix ( int height, int width );

        void setElem ( int height, int width, double value );
        double getElem ( int height, int width ) const;
        
        unique_ptr < CMatrix > cut ( int fromHeight, int fromWidth, int cutHeight, int cutWidth ) const;
        unique_ptr < CMatrix > transpose () const;

        CMatrix & operator = ( const CMatrix & x );
};

#endif /* __cdensematrix_h__ */