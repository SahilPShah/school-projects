#include "list.h"

int main() {
  List<unsigned> list;
  list.insertBack(15);
  list.insertBack(84);
  list.insertBack(24);
  list.insertBack(546);
  list.print(cout<<endl);
  cout<<endl;
  list.sort();
  list.print(cout<<endl);
  return 0;
}
