#include "potd.h"
#include <iostream>

using namespace std;

void insertSorted(Node **head, Node *insert) {
  // your code here!
  //check if insert should be placed at the head
  if(*head == NULL){
    *head = insert;
    return;
  }
  Node *previous = NULL;
  Node *current = *head;
  while(current->data_ < insert->data_ && current->next_ != NULL){
    previous = current;
    current = current->next_;
  }
  if(current->next_ == NULL && current->data_ < insert->data_){
    current->next_ = insert;
    return;
  }
  if(previous == NULL){
    *head = insert;
    insert->next_ = current;
    return;
  }

  previous->next_ = insert;
  insert->next_ = current;
}
