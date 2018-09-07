#include <iterator>
#include <cmath>
#include <list>
#include <stack>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "DFS.h"

/**
 * Initializes a depth-first ImageTraversal on a given `png` image,
 * starting at `start`, and with a given `tolerance`.
 * @param png The image this DFS is going to traverse
 * @param start The start point of this DFS
 * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
 * it will not be included in this DFS
 */
DFS::DFS(const PNG & png, const Point & start, double tolerance):img_(png),start_(start),tol_(tolerance) {
  /** @todo [Part 1] */
  //use initializer list to Initialize the variables
}

/**
 * Returns an iterator for the traversal starting at the first point.
 *return the current instance of the iterator. Begin should be the current instance
 */
ImageTraversal::Iterator DFS::begin() {
  /** @todo [Part 1] */
  //implement one parameter constructor in image traversal::iterator class
  return ImageTraversal::Iterator(this,start_);
}

/**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
ImageTraversal::Iterator DFS::end() {
  /** @todo [Part 1] */
  return ImageTraversal::Iterator();
}

/**
 * Adds a Point for the traversal to visit at some point in the future.
 */
void DFS::add(const Point & point) {
  /** @todo [Part 1] */
  pts_.push(point);
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point DFS::pop() {
  /** @todo [Part 1] */
  Point top = pts_.top();
  pts_.pop();
  return top;
}

/**
 * Returns the current Point in the traversal.
 */
Point DFS::peek() const {
  if(!pts_.empty())
    return pts_.top();
  return Point(0,0);
}

/**
 * Returns true if the traversal is empty.
 */
bool DFS::empty() const {
  return (pts_.empty() ? true:false);
}

PNG DFS::getImage(){
  return img_;
}
Point DFS::getStart() const{
  return start_;
}
double DFS::getTolerance() const{
  return tol_;
}
