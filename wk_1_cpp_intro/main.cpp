#include "Cube.h"
#include <iostream>
#include <cassert>
/**
 * std lib: the C++ std lib is organized into many sub-libraries that can be included in any cpp programs
 * iostream: for r/w to files and the console itself, including std::cout
 * all functionality used from stdlib will be part of std namespace
 * namespace allows us to avoid naming conflicts for commonly used names
 * using: to shortcut this namespace schema (for frequently used names)
 */
using std::cout;
using std::cin;
using std::endl;
int main() {
    //Cube c;
    uiuc::Cube c; //we can create our own namespace
    cout << "Enter your defined length for a cube c\n";
    double new_len; //uninitialized
    cin >> new_len;
    assert(new_len >= 0);
    c.setLength(new_len);
    double vol = c.getVolume();
    double len = c.getLength();
    double sur = c.getSurfaceArea();
    cout << "Volume for cube c is: " << vol << endl;
    cout << "length for cube c is : " << len << "\n";
    cout << "SA for cube c is: " << sur << "\n";
    return 0;
}

// g++ on multiple files including Cube.h: have to specify all source file OR specify the routine in the Make file
// each cpp file is compiled to .o file (specify by g++ -c filename.cpp)
// and the .o files will be linked as an executable .exe OR .out (a.out by default invoked by g++)
// so main.cpp does not need to include "Cube.cpp" but only "Cube.h" since both are in one object file