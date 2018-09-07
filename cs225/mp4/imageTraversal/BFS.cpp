#include <iterator>
#include <cmath>
#include <list>
#include <queue>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "BFS.h"

using namespace cs225;

/**
 * Initializes a breadth-first ImageTraversal on a given `png` image,
 * starting at `start`, and with a given `tolerance`.
 * @param png The image this BFS is going to traverse
 * @param start The start point of this BFS
 * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
 * it will not be included in this BFS
 */
BFS::BFS(const PNG & png, const Point & start, double tolerance): img_(png),start_(start),tol_(tolerance){
  /** @todo [Part 1] */
}

/**
 * Returns an iterator for the traversal starting at the first point.
 */
ImageTraversal::Iterator BFS::begin() {
  /** @todo [Part 1] */
  return ImageTraversal::Iterator(this, start_);
}

/**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
ImageTraversal::Iterator BFS::end() {
  /** @todo [Part 1] */
  return ImageTraversal::Iterator();
}

/**
 * Adds a Point for the traversal to visit at some point in the future.
 */
void BFS::add(const Point & point) {
  /** @todo [Part 1] */
  pts_.push(point);
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point BFS::pop() {
  Point top = pts_.front();
  pts_.pop();
  return top;
}

/**
 * Returns the current Point in the traversal.
 */
Point BFS::peek() const {
  if(!pts_.empty())
    return pts_.front();
  return Point(0,0);
}

/**
 * Returns true if the traversal is empty.
 */
bool BFS::empty() const {
  return (pts_.empty() ? true:false);
}

PNG BFS::getImage(){
  return img_;
}
Point BFS::getStart() const{
  return start_;
}
double BFS::getTolerance() const{
  return tol_;
}
