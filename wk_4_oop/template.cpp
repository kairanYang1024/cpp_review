#include <iostream>
#include <vector>

//template type: special type that takes on diff types when the type is initialized
//e.g. std::vector<T>
//::push_back(T); ::operator[](unsigned int) == ::at(); 
//::size(); 

//templated functions/classes: template variable declared before the function/class body
template <typename T>
T generic_max(T a, T b) {
    //require > implemented in type T
    if(a > b) return a;
    return b;
}

int main() {
    
    std::vector<int> v;
    for(int i = 0; i < 100; i++) {
        v.push_back(i*i);
    }
    std::cout << v[12] << std::endl;
    
    //templated variables checked at compile time to allow for errors to be caught
    //before running the program
    //no max(Cube(4), Cube(20)) since > op is not supported in class Cube
    std::string first = "Hello";
    std::string second = "World"; //return > op, if str literal then comparing the addr
    std::cout << generic_max(3,5) << " " <<  generic_max(3.0, 4.86) << " " << generic_max('a', 'b') 
    << " " << generic_max(first, second) << std::endl; 
    
    return 0;
}