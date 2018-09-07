#include "Node.h"

using namespace std;

void mergeList(Node *first, Node *second) {
  // your code here!
  Node* current1 = first;
  Node* current2 = second;
  Node* temp1 = NULL;
  Node* temp2 = NULL;
  while(current1 != NULL && current2 != NULL){
    temp1 = current1->next_;
    current1->next_ = current2;
    temp2 = current2->next_;
    if(temp1 == NULL){
      break;
    }
    current2->next_ = temp1;
    current1 = temp1;
    current2 = temp2;
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
