#include "TreeNode.h"

TreeNode::RotationType balanceTree(TreeNode*& subroot) {
  TreeNode* subleft = subroot->left_;
  TreeNode* subright = subroot->right_;
  if(rightHeavy(subroot) && rightHeavy(subright)) {
    subroot->right_ = subright->left_;
    subright->left_ = subroot;
    subroot = subright;

    return TreeNode::left;
  }
  else if(leftHeavy(subroot) && leftHeavy(subleft)) {
    subroot->left_ = subleft->right_;
    subleft->right_ = subroot;
    subroot = subleft;

    return TreeNode::right;
  }
  else if(rightHeavy(subroot) && leftHeavy(subright)) {
    TreeNode* subrightleft = subright->left_;

    subright->left_ = subrightleft->right_;
    subrightleft->right_ = subright;
    subroot->right_ = subrightleft;

    subroot->right_ = subright->left_;
    subright->left_ = subroot;
    subroot = subright;

    return TreeNode::rightLeft;
  }
  else if(leftHeavy(subroot) && rightHeavy(subleft)) {
    TreeNode* subleftright = subleft->right_;

    subleft->right_ = subleftright->left_;
    subleftright->left_ = subleft;
    subroot->left_ = subleftright;

    subroot->left_ = subleft->right_;
    subleft->right_ = subroot;
    subroot = subleft;

    return TreeNode::leftRight;
  }
  return TreeNode::right;
}
