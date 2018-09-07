#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#define UNSIGNED_MAX 4294967295
/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  img_trav_=NULL;
}

ImageTraversal::Iterator::Iterator(ImageTraversal* img_trav, Point start) : img_trav_(img_trav){
  //need to have access to all memeber variables that are a part of the traversals
    start_ = start;
    img_ = img_trav->getImage();
    tol_ = img_trav->getTolerance();
    visited_.resize(img_.width(), vector<bool>(img_.height()));
    for(int i = 0; i < (int)visited_.size(); i++){
      for(int j=0; j < (int)visited_[i].size(); j++){
        visited_[i][j] = false;
      }
    }
    visited_[start_.x][start_.y]=true;
    img_trav_->add(start_);
    current_ = img_trav->peek();
}

void ImageTraversal::Iterator::addNeighbors(){
/**@todo [Part 1] -- personal defined helper function*/

//Initialize the visited vector


//add right neighbor
  HSLAPixel & pxl = img_.getPixel(start_.x, start_.y);
  if(current_.x+1 < img_.width())
    if(calculateDelta(pxl, img_.getPixel(current_.x+1, current_.y)) < tol_)
        img_trav_->add(Point(current_.x+1, current_.y));
  //below
  if(current_.y+1 < img_.height())
    if(calculateDelta(pxl, img_.getPixel(current_.x, current_.y+1)) < tol_)
        img_trav_->add(Point(current_.x, current_.y+1));
  //add left neighbor
  //ensure that x-1 is not Out of bounds
  if(current_.x)
    if(calculateDelta(pxl, img_.getPixel(current_.x-1, current_.y)) < tol_)
        img_trav_->add(Point(current_.x-1, current_.y));
  //add upper neighbor
  //ensure that y-1 is not out of bounds
  if(current_.y)
    if(calculateDelta(pxl, img_.getPixel(current_.x, current_.y-1)) < tol_)
        img_trav_->add(Point(current_.x, current_.y-1));

}
/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  addNeighbors();
  //pop off all neighbors that have been visited already
  while(visited_[current_.x][current_.y]){
    //check to make sure the stack/queue is not empty
    if(img_trav_->empty()){
      img_trav_=NULL;
      break;
    }
    //get the neighbor at the front/top of the queue/stack
    current_ = img_trav_->pop();
  }

  visited_[current_.x][current_.y] = true;
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return current_;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  bool thisEmpty = false;
  bool otherEmpty = false;

  if (img_trav_ == NULL) { thisEmpty = true; }
  if (other.img_trav_ == NULL) { otherEmpty = true; }

  if (!thisEmpty)  { thisEmpty = img_trav_->empty(); }
  if (!otherEmpty) { otherEmpty = other.img_trav_->empty(); }

  if (thisEmpty && otherEmpty) return false; // both empty then the traversals are equal, return true
  else if ((!thisEmpty)&&(!otherEmpty)) return (img_trav_ != other.img_trav_); //both not empty then compare the traversals
  else return true; // one is empty while the other is not, return true

}
