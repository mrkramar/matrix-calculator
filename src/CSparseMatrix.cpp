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

#include "CSparseMatrix.h"

using namespace std;

CSparseMatrix :: CSparseMatrix (){}
CSparseMatrix :: ~CSparseMatrix (){}
CSparseMatrix :: CSparseMatrix ( unique_ptr < CMatrix > & x )
{
	m_height = x -> m_height;
	m_width  = x -> m_width;

	int value = 0;
	for ( int h = 0 ; h < x -> m_height ; h++ )
        for ( int w = 0 ; w < x -> m_width ; w++ )
        {
            if ( ( value = x -> getElem ( h , w ) ) != 0 )
				setElem ( h , w , value );
        }
}

void CSparseMatrix :: setElem ( int height , int width , double value )
{
	m_matrix . insert ( make_pair ( make_pair ( height , width ) , value ) );
}

double CSparseMatrix :: getElem ( int height , int width ) const
{
	auto it = m_matrix . find ( make_pair ( height , width ) );
    if ( it != m_matrix . end() )
		return it -> second;
	else
		return 0;
}

CMatrix & CSparseMatrix :: operator = ( const CMatrix & x )
{
	double value;
	m_height = x . m_height;
	m_width  = x. m_width;
	for ( int h = 0 ; h < m_height ; h++ )
        for ( int w = 0 ; w < m_width ; w++ )
        {
            if ( ( value = x . getElem ( h , w ) ) != 0 )
				setElem ( h , w , value );
        }
	return *this;
}

unique_ptr < CMatrix > CSparseMatrix :: cut ( int fromHeight , int fromWidth ,
                                              int cutHeight , int cutWidth ) const
{
	fromHeight--; fromWidth--;
    if ( fromHeight < 0 || fromWidth < 0 ) throw ilegalDimensionException ();
    if ( fromHeight > m_height || fromWidth > m_width ) throw ilegalDimensionException ();
    if ( cutHeight  > m_height || cutWidth  > m_width ) throw ilegalDimensionException ();
	CSparseMatrix ret;
	ret . m_height = cutHeight;
	ret . m_width = cutWidth;
	for ( auto it = m_matrix . begin() ; it != m_matrix . end(); it++ )
	{
		if ( it -> first . first >= fromHeight && it -> first . second >= fromWidth &&
			 it -> first . first < fromHeight + cutHeight && 
			 it -> first . second < fromWidth + cutWidth )
		{
			ret . setElem ( it -> first . first , it -> first . second , it -> second );
		}
	}
	return make_unique < CSparseMatrix > ( ret );
}

unique_ptr < CMatrix > CSparseMatrix :: transpose () const
{
	CSparseMatrix ret;
	ret . m_height = m_width;
	ret . m_width = m_height;
	for ( auto it = m_matrix . begin() ; it != m_matrix . end(); it++ )
	{
		ret . setElem ( it -> first . second , it -> first . first , it -> second );
	}
	return make_unique < CSparseMatrix > ( ret );
}


