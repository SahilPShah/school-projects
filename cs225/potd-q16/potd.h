#ifndef POTD_H
#define POTD_H

#include <string>

using namespace std;

struct Node {
    int data_;
    Node *next_;
};

string stringList(Node *n);
void insert(int data, Node** head);

#endif
