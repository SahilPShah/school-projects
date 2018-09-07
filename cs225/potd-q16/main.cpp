#include "potd.h"
#include <iostream>
using namespace std;

int main() {
  // Test 1: An empty list
  Node * head = NULL;
  cout << stringList(head) << endl;

  // Test 2: A list with exactly one node
  insert(4, &head);
  cout << stringList(head) <<endl;


  // Test 3: A list with more than one node
  insert(5, &head);
  insert(6, &head);
  cout<< stringList(head)<<endl;


  return 0;
}
