//In C++, the programmer has control over the memory and lifecycle of each var
//Default var addr are in stack, called stack memory

//4 components: name (sig in compiler), type (sig in compiler), value, location in memory (addr)

#include <iostream>

void foo() {
    int x = 42;
    std::cout << "x in foo(): "<< x << '\n';
    std::cout << "&x in foo(): " << &x << '\n'; //0x7ff...3274, lower than addr in main() ...3294
    //signifying the stack grows downward as more and more function calls.
    //32 bytes for memory call on the function foo(), and the C++ internal stuff taking place
}

int main() {
    int num = 7;
    std::cout << "Value: " << num << "\n";
    std::cout << "Address: " << &num << "\n";
    //0x7ff...3294 (12 hex digits) means a 64-bit machine (2^64-1) virtual mem addresses in the range
    //only making use of 48 bits, but still signifies 64 bit machine.
    //high memory addr (big number): stack, low memory addr (small): heap
    //stack grow from top to bottom, heap grow from bottom to top if define 'bottom' as 0x0
    //stack memory is associated w/ current function, the var's lifecycle is equivalent to the function
    //which means they cannot be returned to the callers, even if returning pointers, 
    //bcz pointer points to stack mem that's probably recycled.
    //accessing 'recycled' variables can invoke undefined behavior and may segfault in some compilers.
    foo();

    //pointer: store memory address of data, 'level of indirection from the data'
    int* p = &num;
    std::cout << "Content of p: " << p << " equal to addr of num? " << (&num == p) << "\n";
    std::cout << "Dereference of p: " << *p << " equal to val of num? " << (num == *p) << '\n';
    int value_p = *p; //copying value from addr p to a stack data
    *p = 42; //write memory 42 to the addr pointed by p, which is num
    std::cout << "Content of p: " << *p << " equal to val of num? " << (num == *p) << '\n'; //ALWAYS True
    //may be counterintuitive, but p and &num are the same, modifying *p is modifying num
    std::cout << "Copied to value_p: " << value_p << " equal to val of num? " << (num == value_p) << '\n'; //False
    return 0;
}