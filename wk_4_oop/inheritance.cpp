//inheritance: allow for a class to inherit all members of the base class into derived class
//build specialized/generic version of the objects
//inheritance geneology: the shared data on root, specific data on leaf
#include <iostream>
#include <vector>
#include "Shape.h"
#include "Square.h"
#include "uiuc/HSLAPixel.h"

using uiuc::HSLAPixel;

int main() {
    Shape s(4);
    Square sq(4, HSLAPixel(240, 1, 0.5));
    std::cout << "width: " << sq.getWidth() << " area: " << sq.getArea() << "\n";
    std::cout << "base class width: " << s.getWidth() << "\n";

    //C++ functions are not virtual by default, if need polymorphism need to say virtual 
    Shape& sq_shape = sq;
    //not allowed:
    //std::cout << "width: " << sq_shape.getWidth() << " area: " << sq_shape.getArea() << "\n";
    return 0;
}