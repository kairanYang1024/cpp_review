#include "Cube.h"
#include <iostream>
using uiuc::Cube;

void foo(Cube cute) { //copy ctor on argument
    std::cout << "I'm cute uwu\n";
}

Cube foo2() { //copy ctor on return (twice when binding to a var)
    Cube cute;
    return cute;
}

void sendCube(Cube c) { //calls copy_ctor
    std::cout << "Passing by value\n";
}

void sendCube2(Cube& c) { //nothing invoked
    std::cout << "Passing by reference\n";
}

void sendCube3(Cube* c) { //nothing invoked, create a pointer (8 bytes)
    std::cout << "Passing by pointer\n";
}

int main() {
    //custom-default ctor overwrites auto-default ctor that init length to 0
    uiuc::Cube c; //default initialised, no invoke () if initialise in stack
    std::cout << "Volume: " << c.getVolume() << "\n"; //1

    uiuc::Cube d(2.04); //search list of ctors that has 1 arg
    std::cout << "Volume: " << d.getVolume() << "\n"; //2.04^3

    uiuc::Cube* e = new uiuc::Cube(); //do invoke() when initialize on heap despite no () still compiles
    //good coding behavior, init w/out () only when it's necessary to do
    //without () is indeterminate member like in malloc(), with () the object went zero-initialize like in calloc()
    std::cout << "Volume: " << e->getVolume() << "\n"; //1
    
    foo(c); //copy ctor invoked to copy c to the argument of foo()

    //copied the value from return to main stackframe, copy the val to var c2
    //not copy-ctor invoked on console...why?
    //Running into a compiler optimization technique called "copy elision"; 
    //the call to the copy constructor is being optimized away by the compiler.
    Cube c2 = foo2(); 
    Cube c3 = c2; //copy ctor invoked to copy from c2 to c3

    Cube c4; //default ctor invoked
    c4 = c3; //nothing printed in this line, an (copy) assignment operator
    //still copies the content but c4 is not equal to c3 (deep copy)
    std::cout << "Addr c4: " << &c4 << " c3: " << &c3 << "\n";
    
    //Direct storage: type has no modifier, obj takes up exactly size in memory, in stack
    std::cout << "Size of a cube: " << sizeof(Cube) << "\n";
    std::cout << "Size of a double: " << sizeof(double) << "\n";
    
    //Storage by ptr: takes a "memory address width" of memory
    //points to the allocated space of the obj
    std::cout << "Size of a pointer: " << sizeof(Cube*) << "\n";
    
    //Storage by reference: alias to existing memory by name
    //never stores memory itself, have to alias an (r-value), so its's called (l-value)
    Cube& cube_alias = c4;
    //same memory addr, unlike c4 and c3
    std::cout << "Addr c4: " << &c4 << " cube_alias " << &cube_alias << "\n";
    
    //function pass by: value, pointer, reference
    sendCube(c);
    sendCube2(c);
    sendCube3(&c);

    //return by: value (copy), pointer(memo addr), reference (alias, nothing copied)
    //stack var on the latter two is dangerous.
    delete e; //freeing memory
    return 0;
    //after exiting main(), all created Cubes are destroyed
}