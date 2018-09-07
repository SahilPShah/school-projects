/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include <cmath>
using namespace std;
template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    if(t){
      Node* top = t->right;
      Node* b= top->left;
      t->right = b;
      top->left = t;
      t =top;

      t->left->height = max(heightOrNeg1(t->left->left), heightOrNeg1(t->left->right));
      t->right->height = max(heightOrNeg1(t->right->left), heightOrNeg1(t->right->right));
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node* top = t->left;
    Node* b = top->right;
    t->left = b;
    top->right = t;
    t=top;

    t->left->height = max(heightOrNeg1(t->left->left), heightOrNeg1(t->left->right));
    t->right->height = max(heightOrNeg1(t->right->left), heightOrNeg1(t->right->right));
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    //do a right or a left-right
    int balance = getBalance(subtree);
    if(balance > 1 && getBalance(subtree->left) >= 0)
      rotateRight(subtree);
    else if(balance > 1 && getBalance(subtree->left) < 0)
      rotateLeftRight(subtree);
    else if(balance < -1 && getBalance(subtree->right) <= 0)
        rotateLeft(subtree);
    else if(balance < -1 && getBalance(subtree->right) > 0)
        rotateRightLeft(subtree);

    fixHeight(subtree);

}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree, const K& key)
{
    // your code here
    //do a right or a left-right
    int balance = getBalance(subtree);
    if(balance > 1 && key < subtree->left->key)
      rotateRight(subtree);
    else if(balance > 1 && key > subtree->right->key)
      rotateLeftRight(subtree);
    else if(balance < -1 && key > subtree->left->key)
      rotateLeft(subtree);
    else if(balance < -1 && key < subtree->right->key)
      rotateRightLeft(subtree);



}

template <class K, class V>
void AVLTree<K, V>::fixHeight(Node * node) {
    if (node == NULL) return;
    node->height = max(heightOrNeg1(node->left),heightOrNeg1(node->right))+1;
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
  //cout << "Rebalancing (" << subtree->value << ")";
      int balance = getBalance(subtree);
      //cout << ", b = " << balance << endl;
      if (balance == 2) {
      	if (getBalance(subtree->right) < 0) {
  		    rotateRightLeft(subtree);
      	}
      	else {
  	    	rotateLeft(subtree);
      	}
      }
      else if (balance == -2) {
      	if (getBalance(subtree->left) > 0) {
  		    rotateLeftRight(subtree);
          rebalance(subtree);
  	    }
  	    else {
  		    rotateRight(subtree);
  	    }
      }
      else {
          fixHeight(subtree);
      }

}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
typename AVLTree<K,V>::Node* AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return subtree;

    if (key < subtree->key) {
        // your code here
        subtree->left = remove(subtree->left, key);
        rebalance(subtree);
    }
    else if (key > subtree->key) {
        subtree->right = remove(subtree->right, key);
        rebalance(subtree);
        // your code here
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            delete subtree;
            subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
            Node* temp = subtree;
            subtree = subtree->left ? subtree->left:subtree->right;
            delete temp;
        } else {
            /* one-child remove */
            // your code here
            Node* temp = subtree->left;
            while(temp->right)
              temp = temp->right;
            swap(subtree, temp);
            subtree->left = remove(subtree->left, temp->key);
            rebalance(subtree->left);
        }
        // your code here
    }
    return subtree;
}

template <class K, class V>
int AVLTree<K,V>::getBalance(Node* subtree){
  if(subtree)
    return heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left);
  return -1;
}
