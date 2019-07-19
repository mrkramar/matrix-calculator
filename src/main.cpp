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

/**
 * Launches user interface
 * If command "exit" is used in Interface, it returns false and program ends
 * 
 */
int main ( void )
{   
    CInterface ui;
    while ( ui . readCommand() );
    return 0;
}
