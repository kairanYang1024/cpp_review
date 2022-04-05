#include "Cube.h"
#include <iostream>

Cube::Cube() {
    length_ = 1;
}

Cube::Cube(double length) {
    length_ = length;
}

Cube::Cube(const Cube& another) {
    length_ = another.length_; //deep copy
}

Cube::~Cube() {

}

Cube& Cube::operator=(const Cube& another) {
    length_ = another.length_; //deep copy
    return *this; //dereference of kwd 'this': instance of the class itself
}

bool Cube::operator<(const Cube& another) {
    return this->length_ < another.length_;
}

std::ostream& operator<<(std::ostream &os, const Cube& cube) {
    return os << "Cube(" << cube.getLength() << ")";
}

//getter & setter functions
double Cube::getVolume() const { //:: as scope operator, define Cube (class)'s behavior
    return length_ * length_ * length_;
}

double Cube::getSurfaceArea() const {
    return 6 * length_ * length_;
}

double Cube::getLength() const { //explicit getter
    return length_;
}

void Cube::setLength(double length) {
    length_ = length;
}