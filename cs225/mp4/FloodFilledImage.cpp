#include "cs225/PNG.h"
#include <list>
#include <iostream>

#include "colorPicker/ColorPicker.h"
#include "imageTraversal/ImageTraversal.h"

#include "Point.h"
#include "Animation.h"
#include "FloodFilledImage.h"

using namespace cs225;

/**
 * Constructs a new instance of a FloodFilledImage with a image `png`.
 *
 * @param png The starting image of a FloodFilledImage
 */
FloodFilledImage::FloodFilledImage(const PNG & png){
  /** @todo [Part 2] */
  //no other member variables
  img_ = new PNG();
  *img_ = png;
}

/**
 * Adds a flood fill operation to the FloodFillImage.  This function must store the operation,
 * which will be used by `animate`.
 *
 * @param traversal ImageTraversal used for this FloodFill operation.
 * @param colorPicker ColorPicker used for this FloodFill operation.
 */
void FloodFilledImage::addFloodFill(ImageTraversal & traversal, ColorPicker & colorPicker) {
  /** @todo [Part 2] */
  //add the traversal and color picker to the queue;
    ops_.push_back(Operation(&traversal, &colorPicker));
 }

/**
 * Creates an Animation of frames from the FloodFill operations added to this object.
 *
 * Each FloodFill operation added by `addFloodFill` is executed based on the order
 * the operation was added.  This is done by:
 * 1. Visiting pixels within the image based on the order provided by the ImageTraversal iterator and
 * 2. Updating each pixel to a new color based on the ColorPicker
 *
 * While applying the FloodFill to the image, an Animation is created by saving the image
 * after every `frameInterval` pixels are filled.  To ensure a smooth Animation, the first
 * frame is always the starting image and the final frame is always the finished image.
 *
 * (For example, if `frameInterval` is `4` the frames are:
 *   - The initial frame
 *   - Then after the 4th pixel has been filled
 *   - Then after the 8th pixel has been filled
 *   - ...
 *   - The final frame, after all pixels have been filed)
 */
Animation FloodFilledImage::animate(unsigned frameInterval) const {
  Animation animation;
  PNG fill = *img_;
  /** @todo [Part 2] */
  //dp every traversal listed in the queue
  //use for loop with iterator here and update animation every 'frame interval times'
  int frame = 0;
  for(unsigned i=0; i <  ops_.size(); i++){   //while there are still operations remaining
    Operation currentOp = ops_[i];
    animation.addFrame(fill);
    for(ImageTraversal::Iterator it = (currentOp.img_trav)->begin(); it != (currentOp.img_trav)->end(); ++it){
      Point currentPoint = *it;
      HSLAPixel & pxl = fill.getPixel(currentPoint.x, currentPoint.y);
      pxl = (currentOp.cp)->getColor(currentPoint.x, currentPoint.y);
      frame++;
      if(frame%frameInterval==0){
        animation.addFrame(fill);
      }
    }
    animation.addFrame(fill);
  }
  return animation;
}
