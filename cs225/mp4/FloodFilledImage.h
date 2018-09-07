/**
 * @file FloodFilledImage.h
 * Definition for a class to do flood fill on an image
 */
#ifndef FLOODFILLEDIMAGE_H
#define FLOODFILLEDIMAGE_H

#include "cs225/PNG.h"
#include <list>
#include <iostream>
#include <vector>
#include "colorPicker/ColorPicker.h"
#include "imageTraversal/ImageTraversal.h"

#include "Point.h"
#include "Animation.h"

using namespace cs225;
/**
 *This class is used to do flood fill on an image
 */
class FloodFilledImage {
public:
  FloodFilledImage(const PNG & png);
  void addFloodFill(ImageTraversal & traversal, ColorPicker & colorPicker);
  Animation animate(unsigned frameInterval) const;

  struct Operation{
    ImageTraversal* img_trav;
    ColorPicker* cp;
    Operation(ImageTraversal* trav, ColorPicker* color): img_trav(trav), cp(color){}
  };

private:
	/** @todo [Part 2] */
	/** add private members here*/

  PNG* img_;
  std::vector<Operation> ops_; //use a queue to store the operations given by addFloodFill


};

#endif
