#include "../cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"

using namespace cs225;



MyColorPicker::MyColorPicker(int hue, int hue2){
  hue_ = hue;
  hue2_ = hue2;
}

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */
HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
  /* @todo [Part 3] */
  int c;
  if((x/100)%200)
    c=hue_;
  else
    c=hue2_;
  HSLAPixel color(c, 1, .5);
  return color;
}
