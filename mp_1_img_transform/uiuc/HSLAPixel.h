/**
 * @file HSLAPixel.h
 *
 * @author University of Illinois CS 225 Course Staff
 * @version 2018r1-lab1 - Updated for CS 400
 */

#pragma once

#include <iostream>
#include <sstream>

namespace uiuc {

  // Put your HSLAPixel class definition here.
  // (Remember to end it with a semicolon!)
  class HSLAPixel {
    public:
      //ctors:
      HSLAPixel();
      HSLAPixel(double hue, double sat, double lumi);
      HSLAPixel(double hue, double sat, double lumi, double alpha);
      //member variables
      double h; //hue: [0, 360]
      double s; //saturation: [0, 1]
      double l; //luminance: [0, 1]
      double a; //alpha: [0, 1]
  };

}
