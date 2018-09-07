#ifndef MyColorPicker_H
#define MyColorPicker_H

#include "ColorPicker.h"
#include "../cs225/HSLAPixel.h"
#include "../Point.h"

using namespace cs225;

/**
 * A color picker class using your own color picking algorithm
 */
class MyColorPicker : public ColorPicker {
public:

  MyColorPicker(int hue, int hue2);
  HSLAPixel getColor(unsigned x, unsigned y);

private:
  int hue_;
  int hue2_;
};

#endif
