#include "Cube.h"

using namespace uiuc;

//implementation of class Cube
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