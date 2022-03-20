#include "Cube.h"
#include <iostream>

using namespace uiuc;

//implementation of class Cube
Cube::Cube() {
    length_ = 1;
    std::cout << "Ctor (default) invoked\n";
}

Cube::Cube(double length) {
    length_ = length;
    std::cout << "Ctor invoked\n";
}

Cube::Cube(const Cube& another) {
    length_ = another.length_; //deep copy
    std::cout << "Copy ctor invoked\n";
}

Cube::~Cube() {
    std::cout << "Dtor invoked\n";
}

Cube& Cube::operator=(const Cube& another) {
    length_ = another.length_; //deep copy
    std::cout << "Assignment operator invoked\n";
    return *this; //dereference of kwd 'this': instance of the class itself
}

double Cube::getVolume() { //:: as scope operator, define Cube (class)'s behavior
    return length_ * length_ * length_;
}

double Cube::getSurfaceArea() {
    return 6 * length_ * length_;
}

double Cube::getLength() { //explicit getter
    return length_;
}

void Cube::setLength(double length) {
    length_ = length;
}