#ifndef __cinterface_h__
#define __cinterface_h__

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
#include "CSparseMatrix.h"

using namespace std;

/**
 * Interface class that stores methods for comunicating with user
 * All loaded matrices are stored in map in this class.
 */
class CInterface
{
    public:
        CInterface ();
        ~CInterface ();

        /**
         * Intro screen when program is started
         */
        void intro ();

        /**
         * Scans for new matrix and saves it to memory
         */
        void scan ( void );

        /**
         * Checks variable names ( cant be keywords , numbers ... )
         * Returns false if name is forbidden.
         */
        bool checkVarName ( const string varName );

        /**
         * Finds matrix by variable name in map
         * If found it returns iterator
         * If not found return m_matrices . end()
         */
         map < string, unique_ptr < CMatrix > > :: iterator findMatrix ( const string varName );

        /**
         * Decides if matrix is dense or sparse and saves it.
         * 
         * @param matrix - pointer to matrix that is being saved
         * @param zeroCount - count of zeros in matrix
         *  if more than half of elements is 0 then the matrix is sparse
         */
        void save ( const string saveName, unique_ptr < CMatrix > & matrix, int zeroCount );

        /**
         * Counts zeros in given unique pointer to matrix
         * If > 50% of elements are 0s , the matrix is Sparse
         * Othervise its dense
         * 
         * @param res unique pointer to matrix
         * @return int Count of zeros in matrix
         */
        int countZeros ( unique_ptr < CMatrix > & res );

        /**
         * Prints out entered matrix
         */
        void print ();

        /**
         * Scans matrix name and deletes it
         * It can also delete all matrices if "all" is entered.
         */
        void deleteMatrix ();

        /**
         * Prints out all comands user can use
         */
        void showHelp ();

        /**
         * Read operations with matrices from user
         * 
         * @param res - matrix that the operation will be saved to
         */
        void readOperation ( const string result );

        /**
         * Read commands from user
         */
        bool readCommand ();

        /**
         * Adds two matrices together if they have legal dimensions
         * If not, throws exception
         */
        void add ( map < string, unique_ptr < CMatrix > > :: iterator firstArg,
                   map < string, unique_ptr < CMatrix > > :: iterator secondArg,
                   const string result );

        /**
         * Substracts two matrices from each other if they have legal dimensions
         * If not, throws exception
         */    
        void  substract ( map < string , unique_ptr < CMatrix > > :: iterator firstArg,
                          map < string , unique_ptr < CMatrix > > :: iterator secondArg,
                          const string result );

        /**
         * Multiplies two matrices together if they have legal dimensions
         * If not, throws exception
         */
        void  multiply ( map < string , unique_ptr < CMatrix > > :: iterator firstArg,
                         map < string , unique_ptr < CMatrix > > :: iterator secondArg,
                         const string result );

        /**
         * Multiplies matrix with number
         */
        void  multiply ( map < string , unique_ptr < CMatrix > > :: iterator firstArg,
                         double secondArg , const string result );
        
        /**
         * Merges two matrices
         * Can merge to side or to bottom
         * 
         */
        void  merge ( map < string , unique_ptr < CMatrix > > :: iterator firstArg,
                      const string result );

        /**
         * Cuts matrix to given height and width
         * 
         */
        void  cut ( map < string , unique_ptr < CMatrix > > :: iterator firstArg,
                    const string result );
        /**
         * Counts determinant of m x m matrix
         */
        void determinant ();

        /**
         * Counts rank of m x n matrix
         * 
         */
        void rank ();

    private:
        map < string, unique_ptr < CMatrix > > m_matrices; ///< matrices are stored in map indentified by string variable
};

#endif /*__cinterface_h__*/