/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

/*
*this function determines if a point is smaller than another in a specific dimmension
*/
template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{

  double val1 = first[curDim]; //get he value of the first point in the current dimmension
  double val2 = second[curDim]; //get the value of the second point in the current dimmension
  if(val1<val2) //compare the two values
    return true; //if val1 is smaller than val2, return true
  else if(val1 == val2) //if they are equal
    return (first < second); //return the result of the comparison of the two points
  return false; //default case: return false
}

/*
*This function calculates the Euclidean distance between a potential point and a taret point and determines if it
*is closer than the closet point so far
*/
template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
  double potentialSum = 0;
  double bestSum = 0;
  for(int i = 0; i < Dim; i++){
   potentialSum+= (potential[i] - target[i])*(potential[i] - target[i]);//sum the potential
   bestSum+= (currentBest[i] - target[i])*(currentBest[i] - target[i]);//sum the current best
  }
  if(potentialSum < bestSum)//check if current best is greater than potential best
    return true;
  else if(potentialSum == bestSum)    //if they are equal
    return (potential < currentBest); //return the result of the overloaded < operator defined in the Point class
  return false; //default case
}

/*
*KDTree constructor. This accepts a vector of points that should be used to build the tree
*/
template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints){
     if(newPoints.size() == 0) {
       root = NULL;
       size = 0;
     }
     else {
       vector<Point<Dim>> points (newPoints);
       root = buildTree(root, points, 0, points.size() - 1, 0);
       size = points.size();
     }
}
/*
*Copy connstructor for KDTree. This constructor calls the copy function and set the size
*/
template <int Dim>
KDTree<Dim>::KDTree(const KDTree& other) {
    root = copy(other.root);
    size = other.size;
}
/*
*This function overlaods the assignment operator by clearing the old state and copying the state of the arguement
*/
template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree& rhs) {
    clear(root);
    root = copy(rhs.root);
    size = rhs.size;
    return *this;
}
/*
*This function clears the data in the KDTree by calling the clear function
*/
template <int Dim>
KDTree<Dim>::~KDTree() {
  clear(root);
}
/*
*Calls the findClosest function which finds the closest point to the query point
*/
template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    Point<Dim> currentBest = root->point;
    findClosest(root, query, currentBest, 0);
    //cout << query << " " << currentBest << endl; //print statement used for testing purposes
    return currentBest;
}
/*
*The purpose of this function is to put all elements less than the point at the pivot to the left
*and all points greater than the pivot to the right.
*/
template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& pts, int start, int end, int pivot, int dim){
    Point<Dim> point = pts[pivot]; //value at the pivot
    pts[pivot] = pts[end]; //swap pivot with value at right
    pts[end] = point;
    int storeIndex = start; //the leftmost index
    for(int i = storeIndex; i < end ; i++){ //loop through each index from left to right-1
        if((smallerDimVal(pts[i],point,dim))){ //if the point at the current index is less than the pivot value or they are equal
            //swap them and increment storeIndex
            Point<Dim> temp = pts[i];
            pts[i] = pts[storeIndex];
            pts[storeIndex] = temp;
            storeIndex++;
        }
    }
    point = pts[end];   //swap right and storeIndex values
    pts[end] = pts[storeIndex];
    pts[storeIndex] = point;
    return storeIndex; //this is now the index of the median
}

/*
*helper function to find the median of the points with respect to the nth dimmension
*/
template <int Dim>
int KDTree<Dim>::quickselect(vector<Point<Dim>>& pts, int start, int end, int pivot, int dim, int orig_start){
    if(start==end) //if the recursion is complete
        return start;

    int idx = partition(pts, start, end, floor((start+end)/2), dim);   //index of the first pivot point
    if(idx == orig_start+pivot-1) //if the index and the pivot are the same
        return idx;
    else if(orig_start+pivot-1 < idx)
        return quickselect(pts, start, idx-1, pivot, dim, orig_start);  //call quickselect on the left
    else
        return quickselect(pts, idx+1, end, pivot, dim, orig_start);   //call quickselect on the right
}

/*
*recursively builds KDtree based on result from quickselect and the splitting dimension
*/
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(KDTreeNode* subroot, vector<Point<Dim>>& pts, int start, int end, int dim){
    if(start > end)
        return NULL;
    if(start == end)
        return new KDTreeNode(pts[start]);
    int median = ceil(((double)(end-start+1))/2.0);
    int idx = quickselect(pts, start, end, median, dim, start);
    subroot = new KDTreeNode(pts[idx]);
    subroot->left = buildTree(subroot->left, pts, start, idx-1, (dim+1)%Dim);
    subroot->right = buildTree(subroot->right, pts, idx+1, end, (dim+1)%Dim);
    return subroot;
}
/*
*This function deletes the tree rooted at subroot
*/
template <int Dim>
void KDTree<Dim>::clear(KDTreeNode* subroot) {
  if(subroot == NULL)
    return;
  else {
    clear(subroot->left);
    clear(subroot->right);
    delete subroot;
    subroot = NULL;
  }
}
/*
*this function copies the tree rooted at subroot
*/
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy(KDTreeNode* subroot) {
  if(subroot == NULL)
    return NULL;
  else {
    KDTreeNode* subroot_copy = new KDTreeNode(subroot->point);
    subroot_copy->left = copy(subroot->left);
    subroot_copy->right = copy(subroot->right);
    return subroot_copy;
  }
}
/*
*finds the closest point to the query
*/
template <int Dim>
void KDTree<Dim>::findClosest(KDTreeNode* subroot, const Point<Dim>& query, Point<Dim>& currentBest, int dim) const {
  if(subroot == NULL)
    return;
  if(smallerDimVal(query, subroot->point, dim))
    findClosest(subroot->left, query, currentBest, (dim + 1) % Dim);
  else
    findClosest(subroot->right, query, currentBest, (dim + 1) % Dim);
  if(shouldReplace(query, currentBest, subroot->point))
    currentBest = subroot->point;
  if(fabs(subroot->point[dim] - query[dim]) <= distance(query, currentBest)) {
    if(smallerDimVal(query, subroot->point, dim))
      findClosest(subroot->right, query, currentBest, (dim + 1) % Dim);
    else
      findClosest(subroot->left, query, currentBest, (dim + 1) % Dim);
  }
}
/*
*Compute and return the euclidean distacne between two points
*/
template <int Dim>
double KDTree<Dim>::distance(const Point<Dim>& target, const Point<Dim>& queryPoint) const {
  double radius = 0;
  for(int i = 0; i < Dim; i++)
   radius += ((queryPoint[i] - target[i]) * (queryPoint[i] - target[i]));
  return sqrt(radius);
}
