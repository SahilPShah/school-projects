#include <vector>
#include "BTreeNode.h"


BTreeNode* find(BTreeNode* root, int key) {
  if(root == NULL)
    return NULL;
  for(unsigned i = 0; i < root->elements_.size(); i++) {
    if(root->elements_[i] == key)
      return root;
  }
  if(root->is_leaf_)
    return NULL;

  if(key < root->elements_.front()) {
    for(unsigned j = 0; j < root->children_.size(); j++) {
      if(root->children_[j]->elements_.back() < root->elements_.front())
        return find(root->children_[j], key);
    }
  }
  else if(key > root->elements_.back()) {
    for(unsigned j = 0; j < root->children_.size(); j++) {
      if(root->children_[j]->elements_.front() > root->elements_.back())
        return find(root->children_[j], key);
    }
  }
  else {
    for(unsigned j = 0; j < root->children_.size(); j++) {
      if(root->children_[j]->elements_.front() > root->elements_.front() && root->children_[j]->elements_.back() < root->elements_.back())
        return find(root->children_[j], key);
    }
  }
  return NULL;
}
