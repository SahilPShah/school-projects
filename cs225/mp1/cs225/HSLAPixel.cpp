/**
 * @file HSLAPixel.cpp
 * Implementation of the HSLAPixel class for use in with the PNG library.
 *
 * @author CS 225: Data Structures
 */

#include "HSLAPixel.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace cs225 {
    //default construct -- completely white, opaque pixel
    HSLAPixel::HSLAPixel(){
      h=360;
      s=1.0;
      l=1.0;
      a=1.0;//lower value yeild semi-transparency
    }
    HSLAPixel::HSLAPixel(double hue, double saturation, double luminance){
      h=hue;
      s=saturation;
      l=luminance;
    }
    HSLAPixel::HSLAPixel(double hue, double saturation, double luminance, double alpha){
      h=hue;
      s=saturation;
      l=luminance;
      a=alpha;
    }

}
