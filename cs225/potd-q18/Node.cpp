#include "Node.h"

using namespace std;

void sortList(Node **head) {
  //CASE: {} or {n}
  if(*head==NULL || (*head)->next_ == NULL){
    return;
  }
  Node* ptr = *head;
  Node* temp1= NULL;
  Node* temp2 = NULL;
  Node* temp3 = NULL;//2 and 3 will only be used for the middle of the list
  //CASE: {x,y}
  if((*head)->next_->next_ == NULL){
    if((*head)->data_ > (*head)->next_->data_){
      temp1 = (*head)->next_;
      (*head)->next_=NULL;
      temp1->next_ = *head;
      *head = temp1;
    }
  }
  Node* prev = NULL;
  bool swaps = true;
  while(swaps){
    ptr = *head;
    swaps = false;
    //check if the head node needs to be swapped
    if(ptr->data_ > ptr->next_->data_){
      swaps = true;
      temp1 = ptr->next_;
      ptr->next_ = ptr->next_->next_;
      temp1->next_ = *head;
      *head = temp1;
    }
    while(ptr->next_->next_!=NULL){
      if(ptr->next_->data_ > ptr->next_->next_->data_){
        temp1 = ptr->next_;
        temp2 = temp1->next_;
        temp3 = temp2->next_;
        swaps = true;
        ptr->next_ = temp2;
        temp2->next_ = temp1;
        temp1->next_ = temp3;
      }
      prev = ptr;//will be used for when this loop exit. there is a possible swap of the last two elements of the list
      ptr=ptr->next_;
    }
    if(ptr->next_->next_ == NULL){
      if(ptr->data_ > ptr->next_->data_){
        swaps = true;
        temp1 = ptr->next_;
        prev->next_ = temp1;
        temp1->next_ = ptr;
        ptr->next_ = NULL;
      }
    }
  }
}

Node::Node() {
    numNodes++;
}

Node::Node(Node &other) {
    this->data_ = other.data_;
    this->next_ = other.next_;
    numNodes++;
}

Node::~Node() {
    numNodes--;
}

int Node::numNodes = 0;
