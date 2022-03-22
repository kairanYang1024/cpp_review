#pragma once

#include "Shape.h"
#include "uiuc/HSLAPixel.h"
/* public inheritance makes public members of the base class public in the derived class, 
and the protected members of the base class remain protected in the derived class.
protected inheritance makes the public and protected members of the base class protected in the derived class.
private inheritance makes the public and protected members of the base class private in the derived class.
private members of the base class are inaccessible to the derived class. */

class Square : public Shape { //public inheritance, 99% usage of inheri is public inheritance
    public:
        //when initialize a derived class, the derived class must construct the base class
        //by default uses default ctor of the base class
        //custom ctor can be used w/ initialization list
        Square(double width, uiuc::HSLAPixel color);
        double getArea() const;
    
    private:
        uiuc::HSLAPixel color_; //colored square

};