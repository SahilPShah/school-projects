
/**
 * @file heap.cpp
 * Implementation of a heap class.
 */
//zero-indxed heap
template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    return 0;
}

//returns the index of the left-child
template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    return 2*currentIdx + 1;

}
//returns the index of the right child
template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    return 2*currentIdx + 2;
}
//returns the index of the parent
template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    return floor((currentIdx-1)/2);
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    //if both right and left indeces are out of bounds
    if(rightChild(currentIdx) < _elems.size() || leftChild(currentIdx) < _elems.size())
        return true;
    else
        return false;
}

//Assuming currentIdx has a child
template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    if(hasAChild(currentIdx) && rightChild(currentIdx) < _elems.size()){
        if(higherPriority(_elems[leftChild(currentIdx)], _elems[rightChild(currentIdx)]))
            return leftChild(currentIdx);
        else
            return rightChild(currentIdx);
    }
    else
        return leftChild(currentIdx);
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{

    /*algorithm: check if the highest priority child is a higher priority than the node
    *if so, swap
    *then call heapifyDown recursively until a leaf has been reached or the node is a higher priority
    */
    //if the currrent index does not have a child
    if(!hasAChild(currentIdx))
        return;
    if(rightChild(currentIdx) >= _elems.size()){
        if(higherPriority(_elems[leftChild(currentIdx)], _elems[currentIdx]))
            std::swap(_elems[currentIdx], _elems[leftChild(currentIdx)]);
    }
    else{
        size_t maxPriority = maxPriorityChild(currentIdx);
        if(higherPriority(_elems[maxPriority], _elems[currentIdx])){
            std::swap(_elems[currentIdx], _elems[maxPriority]);
            heapifyDown(maxPriority);
        }
    }

}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx == root())
        return;
    size_t parentIdx = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
        std::swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(parentIdx);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{

}

//add all elements and then call heapifyDown()
//
template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems)
{
    for(size_t i = root(); i < elems.size(); i++){
        _elems.push_back(elems[i]);
    }
    size_t parentIdx = parent(_elems.size() - 1); //find the parent of the lowest right leaf
    for(size_t i = parentIdx; i > 0; i--)
      heapifyDown(i);
    heapifyDown(root());

}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    if(empty())
      return T();
    else {
      std::swap(_elems[root()], _elems[_elems.size() - 1]);
      T output = _elems[_elems.size() - 1];
      _elems.pop_back();
      heapifyDown(root());
      return output;
    }
}

//return what would be popped
template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    if(empty())
        return T();
    else
        return _elems[root()];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    _elems.push_back(elem); // push the element to the back
    heapifyUp(_elems.size() - 1); //restore the heap property
}

template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    return (_elems.size() == 0); //return if the size of the vector is zero

}

//put elements from heap into a vector
template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}
