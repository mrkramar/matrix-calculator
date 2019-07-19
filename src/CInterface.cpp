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

#include "CInterface.h"

using namespace std;

CInterface :: CInterface ()
{
    intro();
}

CInterface :: ~CInterface (){}

void CInterface :: intro ()
{
    cout << " \033[1;31m "<< R"(
                           ____                         
                    |\/| /\ ||__)|\_/                    
                    |  |/~~\||  \|/ \
             __          __            _____  __  
            /  ` /\ |   /  `|  ||    /\ |/  \|__) 
            \__,/~~\|___\__,\__/|___/~~\|\__/|  \
    -----------------------------------------------------)" 
    << "\033[0m" << endl;
    showHelp();
}

bool CInterface :: checkVarName ( const string varName )
{
    if ( isdigit ( varName [ 0 ] ) )
    {
        cout << "Number as variable name is forbidden!" << endl;
        return false;
    }
    if ( varName == "merge" || varName == "cut"  || varName == "transpose" ||
         varName == "gem"   || varName == "rank" || varName == "determ"    || 
         varName == "inverse" )
    {
        cout << "Name cannot be a keyword! Use something different." << endl;
        return false;
    }
    return true;
}

void CInterface :: scan ()
{
    cin . clear ();
    string var; 
    int height, width, zeroCount = 0;  

    cin >> var >> height >> width; 
    if ( ! checkVarName ( var ) )
        return;

    cout << var << " : " <<height << " x " << width << endl;
    CDenseMatrix matrix ( height, width );

    double input;
    for ( int h = 0 ; h < height ; h++ )
        for ( int w = 0 ; w < width ; w++ )
        {
            cin >> input;
            matrix . setElem ( h , w , input );
            if ( input == 0 )
                zeroCount++;
        }

    unique_ptr < CMatrix > saveMatrix = make_unique < CDenseMatrix > ( matrix );  
    save ( var, saveMatrix, zeroCount );
}

void CInterface :: save ( const string saveName, unique_ptr < CMatrix > & matrix, int zeroCount )
{
    if ( zeroCount >= ( ( matrix -> m_height * matrix -> m_width ) / 2 ) ) 
    {
        cout << "Saving matrix as Sparse..." << endl;
        CSparseMatrix matrix1 ( matrix );
        unique_ptr < CMatrix > save = make_unique < CSparseMatrix > ( matrix1 );
        m_matrices [ saveName ] = move ( save );
    }
    else
    {
        cout << "Saving matrix as Dense..." << endl;
        m_matrices [ saveName ] = move ( matrix );
    }
    cout << "Matrix saved!" << endl;
}

void CInterface :: print ()
{
    cin . clear ();
    string var;
    cin >> var;

    auto it = findMatrix ( var );
    if ( it == m_matrices . end() )
        return;
    cout << endl << endl;
    it -> second -> print();
    cout << endl;
}

void CInterface :: deleteMatrix ()
{
    cin . clear ();
    string var;
    cin >> var;

    if ( var == "all" )
    {
        cout << "Deleting all matrices."<< endl;
        m_matrices . clear ();   
        return;
    }

    auto it = m_matrices . find ( var );
    if ( it != m_matrices . end() )
    {
        cout << "Deleting matrix " << var << endl;
        m_matrices . erase ( it );
    }
    else
    {
        cout << "Matrix not found." << endl;
    }
}

void CInterface :: showHelp ()
{  
    cout << endl;
    cout << "Available commands:" << endl << endl;
    cout << "scan VAR HEIGHT WIDTH" << endl;
    cout << "print VAR" << endl;
    cout << "VAR1 = VAR2 + VAR3" << endl;
    cout << "VAR1 = VAR2 - VAR3" << endl;
    cout << "VAR1 = VAR2 * VAR3" << endl;
    cout << "VAR1 = NUMBER * VAR2" << endl;
    cout << "VAR1 = merge VAR2 VAR3 [side/bottom]" << endl;
    cout << "VAR1 = cut VAR2 CUT_FROM_H CUT_FROM_W CUT_NEW_H CUT_NEW_W" << endl;
    cout << "VAR1 = transpose VAR2" << endl;
    cout << "VAR1 = gem VAR2" << endl;
    cout << "rank VAR1" << endl;
    cout << "determinant VAR1" << endl;
    cout << "VAR1 = inverse VAR2" << endl;
    cout << "delete VAR1" << endl;
    cout << "delete all" << endl;
    cout << "exit" << endl;
    cout << endl;
}

map < string, unique_ptr < CMatrix > > :: iterator CInterface :: findMatrix ( const string varName )
{
    map < string , unique_ptr < CMatrix > > :: iterator it = m_matrices . find ( varName );
    if ( it != m_matrices . end () )
        return it;
    else 
    {
        cout << "Matrix " << varName << " not found" << endl;
        return m_matrices . end();
    }
}

int CInterface :: countZeros ( unique_ptr < CMatrix > & res )
{
    int zeroCount = 0;
    for ( int h = 0 ; h < res -> m_height ; h++ )
        for ( int w = 0 ; w < res -> m_width ; w++ )
        {
            if ( res -> getElem( h , w ) == 0 )
                zeroCount++;
        }

    return zeroCount;
}

void CInterface :: add ( map < string , unique_ptr < CMatrix > > :: iterator firstArg,
                         map < string , unique_ptr < CMatrix > > :: iterator secondArg,
                         const string result )
{
    unique_ptr < CMatrix > res;
    try
    {  
        res = * ( firstArg -> second ) + * ( secondArg -> second );
    }
    catch ( ilegalDimensionException & e )
    {
        cout << e . msg () << endl;
        return;
    }
    save ( result, res, countZeros ( res ) );
}

void CInterface :: substract ( map < string, unique_ptr < CMatrix > > :: iterator firstArg,
                               map < string, unique_ptr < CMatrix > > :: iterator secondArg,
                               const string result )
{
    unique_ptr < CMatrix > res;
    try
    {  
        res = * ( firstArg -> second ) - * ( secondArg -> second );
    }
    catch ( ilegalDimensionException & e )
    {
        cout << e . msg () << endl;
        return;
    }
    save ( result, res, countZeros ( res ) );
}

void CInterface :: multiply ( map < string, unique_ptr < CMatrix > > :: iterator firstArg,
                              map < string, unique_ptr < CMatrix > > :: iterator secondArg,
                              const string result )
{
    unique_ptr < CMatrix > res;
    try
    {  
        res = *( firstArg -> second ) * *( secondArg -> second );
    }
    catch ( ilegalDimensionException & e )
    {
        cout << e . msg () << endl;
        return;
    }
    save ( result, res, countZeros ( res ) );
}

void CInterface :: multiply ( map < string, unique_ptr < CMatrix > > :: iterator firstArg,
                              double secondArg, const string result )
{
    unique_ptr < CMatrix > res;
    try
    {  
        res = * ( firstArg -> second ) * secondArg;
    }
    catch ( ilegalDimensionException & e )
    {
        cout << e . msg () << endl;
        return;
    }
    save ( result , res , countZeros ( res ) );
}

void CInterface :: merge ( map < string, unique_ptr < CMatrix > > :: iterator firstArg,
                           const string result )
{
    unique_ptr < CMatrix > res;
    string direction, thirdArg;
    cin >> thirdArg >> direction;
    if ( direction != "bottom" && direction != "b" && direction != "side" && direction != "s" )
    {
        cout << "Wrong direction! Use bottom or b or side or s." << endl;
        return;
    }
    auto secondArg = findMatrix ( thirdArg );
    if ( secondArg != m_matrices . end () )
    {
        try
        {
            res = firstArg -> second -> merge ( * ( secondArg -> second ) , direction );
        }
        catch ( ilegalDimensionException & e )
        {
            cout << e . msg () << endl;
            return;
        }
        save ( result, res, countZeros ( res ) );
    }
}

void CInterface :: cut ( map < string, unique_ptr < CMatrix > > :: iterator firstArg,
                         const string result )
{
    unique_ptr < CMatrix > res;
    int cutHeight, cutWidth, fromHeight, fromWidth;
    cin >> fromHeight >> fromWidth >> cutHeight >> cutWidth;
    try
    {
        res = firstArg -> second -> cut ( fromHeight , fromWidth , cutHeight , cutWidth );
    }
    catch ( ilegalDimensionException & e )
    {
        cout << e . msg () << endl;
        return;
    }
    save ( result, res, countZeros ( res ) );
}

void CInterface :: determinant ()
{
    string var;
    cin >> var;
    auto it = findMatrix ( var );
    if ( it == m_matrices . end () )
    {
        return;
    }
    double det;
    try
    {
        det = it -> second -> determinant();
    }
    catch ( irregularException & e )
    {
        cout << e . msg () << endl;
        return;
    }
    cout << "Determinant : " << det << endl;
}

void CInterface :: rank ()
{
    string var;
    cin >> var;
    auto it = findMatrix ( var );
    if ( it == m_matrices . end () )
    {
        return;
    }
    int r;
    try
    {
        r = it -> second -> rank();
    }
    catch ( ilegalDimensionException & e )
    {
        cout << e . msg () << endl;
        return;
    }
    cout << "Rank : " << r << endl;
}

void CInterface :: readOperation ( const string result )
{
    string firstArg, secondArg, thirdArg;
    cin . clear ();
    cin >> firstArg >> secondArg;
    unique_ptr < CMatrix > res;

    if ( secondArg == "+" || secondArg == "-" || secondArg == "*" )
    {
        cin >> thirdArg;

        map < string, unique_ptr < CMatrix > > :: iterator itFirst;
        map < string, unique_ptr < CMatrix > > :: iterator itSecond;

        if ( isdigit ( thirdArg [ 0 ] ) )
        {
            cout << "Cannot use MATRIX * NUMBER multiplication. Did you mean NUMBER * MATRIX?" << endl;
            return;
        }

        if ( ( itFirst  = findMatrix ( thirdArg ) ) == m_matrices . end () )
        {
            return;
        }

        if ( secondArg == "*" && isdigit ( firstArg [ 0 ] ) ) // if third argument is number
        {
            multiply ( itFirst, stod ( firstArg ), result );
            return;
        }

        if ( ( itSecond = findMatrix ( firstArg ) ) == m_matrices . end () )
        {
            return;
        }
        if ( secondArg == "+" ) // addition
        {    
            add ( itFirst, itSecond, result );
        }
        else if ( secondArg == "-" ) // substraction
        {
            substract ( itFirst, itSecond, result );
        }
        else if ( secondArg == "*" ) // multiplication
        {
            multiply ( itFirst, itSecond, result );
        }
    }
    else
    {       
        auto it = findMatrix ( secondArg );
        if ( it == m_matrices . end () )
        {
            return;
        }
        if ( firstArg == "transpose" )
        {         
            res = it -> second -> transpose ();
            save ( result, res, countZeros ( res ) );               
        }
        if ( firstArg == "merge" )
        {
            merge ( it, result );
        }
        if ( firstArg == "cut" )
        {
            cut ( it, result );
        }
        if ( firstArg == "gem" )
        {
            res = it -> second -> gaussElim ();
            save ( result, res ,countZeros ( res ) );
        }
        if ( firstArg == "inverse" )
        {
            try
            {
                res = it -> second -> inverse ();
                save ( result, res ,countZeros ( res ) );
            }
            catch ( irregularException & e )
            {
                cout << e . msg () << endl;
                return;
            }
        }
    }
}


bool CInterface :: readCommand ()
{
    string command;
    cin . clear ();
    cin >> command; 

    if ( command == "scan" )
    {
        scan ();
    }
    else if ( command == "delete" )
    {
        deleteMatrix();
    }
    else if ( command == "help" ) 
    {
        showHelp ();
    }
    else if ( command == "print" )
    {
        print ();
    }
    else if ( command == "determinant" ) 
    {
        determinant ();
    }
    else if ( command == "rank" ) 
    {
        rank ();
    }
    else if ( command == "exit" ) 
    {   
        return false;
    }
    else 
    {
        char eq;
        cin >> eq;
        if ( eq == '=' )
        {
            readOperation( command );
        }
        else
        {
            cout << "Unknown commad. Use command help" << endl;
            readCommand ();
        }
    }
    return true;
}