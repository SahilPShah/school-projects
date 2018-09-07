/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "TreeTraversals/InorderTraversal.h"
#include "TreeTraversals/TreeTraversal.h"
#include <iostream>
/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    cout << endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 *in order traversal of the tree prints the values in the tree from right to left
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
    //your code here
    mirror(root);
}

template <typename T>
void BinaryTree<T>::mirror(Node* root){
  Node* temp = NULL;
  if(!root){
    return;
  }
  //swap
  temp = root->right;
  root->right = root->left;
  root->left = temp;
  mirror(root->left);
  mirror(root->right);
}

/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedIterative() const
{
    // your code here
    InorderTraversal<T> iot(getRoot());
    T data = (*iot.begin())->elem;
    for(typename TreeTraversal<T>::Iterator it = iot.begin(); it!=iot.end(); ++it){
      if((*it)->elem < data)
        return false;
      data = (*it)->elem;
    }

    return true;
}

/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedRecursive() const
{
  return isOrdered(getRoot());
}

template <typename T>
bool BinaryTree<T>::isOrdered(Node* subroot) const{
  if(!subroot){
    return true;
  }
  if(subroot->left && (findLargest(subroot->left) > subroot->elem))
    return false;
  if(subroot->right && (findSmallest(subroot->right) < subroot->elem))
    return false;
  return isOrdered(subroot->left) && isOrdered(subroot->right);
}

template <typename T>
T BinaryTree<T>::findLargest(Node* subroot) const{
  BinaryTree<T>::Node* leftTree = subroot;
  while(leftTree->right){
    leftTree = leftTree->right;
  }
  return leftTree->elem;
}
template <typename T>
T BinaryTree<T>::findSmallest(Node* subroot) const{
  BinaryTree<T>::Node* rightTree = subroot;
  while(rightTree->left){
    rightTree = rightTree->left;
  }
  return rightTree->elem;
}

/**
 * creates vectors of all the possible paths from the root of the tree to any leaf
 * node and adds it to another vector.
 * Path is, all sequences starting at the root node and continuing
 * downwards, ending at a leaf node. Paths ending in a left node should be
 * added before paths ending in a node further to the right.
 * @param paths vector of vectors that contains path of nodes
 */
template <typename T>
void BinaryTree<T>::printPaths(vector<vector<T>> &paths) const
{
  if(!root)
    return;
  std::vector<T> vector;
  _printPathRecursive(root, paths, vector);
}
template <typename T>
void BinaryTree<T>::_printPathRecursive(Node* subroot, vector<vector<T>> & paths, vector<T> vector) const{
//check for null
if(!(subroot->left || subroot->right)){
  paths.push_back(vector);
  vector.pop_back();
  return;
}
if(subroot->left)
  _printPathRecursive(subroot->left, paths, vector);
if(subroot->right)
  _printPathRecursive(subroot->right, paths, vector);
}

/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
    // your code here
    return -1;
}
