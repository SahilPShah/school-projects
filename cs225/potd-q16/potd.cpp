#include "potd.h"
#include <iostream>
#include <sstream>

using namespace std;

string stringList(Node *head) {
    // your code here!
    //check for empty set
    if(head!=NULL){
      Node *temp = head;
      int index = 0;
      stringstream output;
      while(temp!=NULL){
        //append string
        output <<"Node "<< index << ": " <<temp->data_ ;
        temp = temp->next_;
        index++;
        //if the next node isnt null
        if(temp!=NULL)
          output << " -> ";
      }
      //convert stringstream into string
      return output.str();;
    }
    return "Empty list";
}

void insert(int data, Node ** head){
  //create the list
  Node *e = new Node();
  //save the current head pointer
  Node *temp = *head;
  //set the data of the node
  e->data_ = data;
  //set the node next pointer to the current head
  e->next_ = temp;
  //set the new head to the address of the node;
  *head = e;
}
