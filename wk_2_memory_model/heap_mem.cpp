//heap mem: exist longer than the lifecycle of its host function & controlled by the programmer
//use 'new' operator to return a ptr to the mem addr storing the new data
//1: allocate mem on heap (malloc())
//2: initialize the data structure (calls constructor if user-defined)
//3: return a ptr to that data structure

//can invoke bad_alloc when error, malloc() error is NULL
//revoke by 'delete' operator which wraps up free()


#include <iostream>
using std::cout;
using std::endl;

int main() {
    //undefined random integer (allocated by malloc)
    int* newPtr = new int; //stack mem (&newPtr) point to heap memory (newPtr = new int)
    std::cout << "*newPtr: " << *newPtr << "\n";
    std::cout << "newPtr: " << newPtr << "\n"; //low addr and grow up
    std::cout << "&newPtr: " << &newPtr << "\n";

    //heap mem addr high can be a result of address space layout randomization, security reasons
    //but they are lesser than stack values for sure
    *newPtr = 42;
    std::cout << "*newPtr assigned to 42" << "\n";
    std::cout << "*newPtr: " << *newPtr << "\n";
    std::cout << "newPtr: " << newPtr << "\n";
    std::cout << "&newPtr: " << &newPtr << "\n";
    delete newPtr; //pay attention to double free() if setting two pointers to same addr
    newPtr = NULL; //closure

    int *x = new int;
    int &y = *x; //reference var: alias another piece of mem, just a name
    y = 4;

    cout << &x << endl; //large number
    cout << x << endl; //small number
    cout << *x << endl; //4, set y = *x = 4

    cout << &y << endl; //=x
    cout << y << endl; //4
    // cout << *y << endl; cannot deref a ptr, compiler-error

    int *arr; //array can be decayed to a ptr (lose length)
    int size = 3;
    arr = new int[size]; //x is an int ptr on heap
    for (int i = 0; i < size; i++) {
        arr[i] = i + 3; //3,4,5
        cout << arr[i] << " ";
    }
    cout << '\n';
    //equivalent of new int[], delete[] is delete w/ knowing how many elements in that contiguous mem space
    delete[] arr; 
    //if delete x for x an array, it may cause memory leak
    return 0;
}