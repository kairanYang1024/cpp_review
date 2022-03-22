#include "Shape.h"

//initializer list
//used for: init a base class, init the curr class using another ctor, init default vals of members
Shape::Shape() : Shape(1) {
    //nothing
}

//primitive type initialization call carries functionality
Shape::Shape(double width) : width_(width) { 
    //nothing
}

double Shape::getWidth() const {
    return width_;
}