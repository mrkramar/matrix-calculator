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

#include "CDenseMatrix.h"


using namespace std;

CDenseMatrix :: CDenseMatrix (){}
CDenseMatrix :: ~CDenseMatrix (){}

CDenseMatrix :: CDenseMatrix ( int height, int width )
{
    m_height = height;
    m_width = width;

    m_matrix . resize ( height );
    for ( int h = 0 ; h < height ; h++ )
        m_matrix [ h ] . resize ( width );   
}

void CDenseMatrix :: setElem ( int height, int width, double value )
{
    m_matrix [ height ] [ width ] = value;
}

double CDenseMatrix :: getElem ( int height, int width ) const
{
    return m_matrix [ height ] [ width ];
}

CMatrix & CDenseMatrix :: operator = ( const CMatrix & x )
{
    m_height = x . m_height;
    m_width  = x . m_width;

    m_matrix . resize ( m_height );
    for ( int h = 0 ; h < m_height ; h++ )
        m_matrix [ h ] . resize ( m_width );   

    for ( int h = 0 ; h < m_height ; h++ )
        for ( int w = 0 ; w < m_width ; w++ )
            setElem ( h, w, x . getElem ( h , w ) );

    return *this;
}

unique_ptr < CMatrix > CDenseMatrix :: cut ( int fromHeight, int fromWidth ,
                                             int cutHeight, int cutWidth ) const
{
    fromHeight--; fromWidth--;
    if ( fromHeight < 0 || fromWidth < 0 ) 
        throw ilegalDimensionException ();

    if ( fromHeight + cutHeight > m_height || fromWidth + cutWidth > m_width ) 
        throw ilegalDimensionException ();

    CDenseMatrix ret ( cutHeight , cutWidth );
    for ( int h = 0 ; h < cutHeight ; h++ )
        for ( int w = 0 ; w < cutWidth ; w++ )
            ret . setElem ( h, w, getElem ( h + fromHeight, w + fromWidth ) );

    unique_ptr < CMatrix > tmp = make_unique < CDenseMatrix > ( ret );    
    return tmp;
}

unique_ptr < CMatrix > CDenseMatrix :: transpose () const
{           
    CDenseMatrix ret ( m_width, m_height );

    for ( int h = 0 ; h < ret . m_height ; h++ )
        for ( int w = 0 ; w < ret . m_width ; w++ )
            ret . setElem ( h, w, getElem ( w , h ) );
 
    return make_unique < CDenseMatrix > ( ret );
}

