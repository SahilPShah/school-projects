#include "Stack.h"
// `int size()` - returns the number of elements in the stack (0 if empty)
int Stack::size() const {
  return data_.size();
}

// `bool isEmpty()` - returns if the list has no elements, else false
bool Stack::isEmpty() const {
  return data_.empty();
}

// `void push(int val)` - pushes an item to the stack in O(1) time
void Stack::push(int value) {
  data_.push(value);

}

// `int pop()` - removes an item off the stack and returns the value in O(1) time
int Stack::pop() {
  int data = data_.top();
  data_.pop();
  return data;
}
