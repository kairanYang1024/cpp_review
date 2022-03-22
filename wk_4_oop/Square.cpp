#include "Square.h"
#include "Shape.h"

//initialization list syntax: Square(a, b) : Shape(a) OR call base ctor explitictly inside
//used for: init a base class, init the curr class using another ctor, init default vals of members
Square::Square(double width, uiuc::HSLAPixel color) : Shape(width) {
    //base class arg ctor called
    color_ = color;
}

double Square::getArea() const {
    return getWidth() * getWidth(); //cannot access private members of base class
}