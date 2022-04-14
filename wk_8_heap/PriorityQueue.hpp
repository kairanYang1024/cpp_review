#include "PriorityQueue.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

template <typename T>
PriorityQueue<T>::PriorityQueue() : size_(1), capacity_(2) {
    data_ = new T[capacity_];
}

template <typename T>
PriorityQueue<T>::PriorityQueue(const T* source_array, unsigned size) : size_(size+1) {
    unsigned exponent = (unsigned) std::ceil(std::log2(size_)); 
    capacity_ = (unsigned) std::pow(2, exponent);
    data_ = new T[capacity_];
    // do a std::copy to improve performance? nah
    // std::copy(std::begin(source_array), std::end(source_array), data_+1);
    for(unsigned i = 1; i < size_; i++) {
        data_[i] = source_array[i-1];
    }
    _buildHeap(size);
    std::cout << "size of the input array is " << size << "\n";
    std::cout << "Capacity is " << capacity_ << " and size_ is " << size_ << "\n";
}

template <typename T>
PriorityQueue<T>::PriorityQueue(const std::vector<T>& source_list) {
    size_ = source_list.size()+1;
    unsigned exponent = (unsigned) std::ceil(std::log2(size_)); 
    capacity_ = (unsigned) std::pow(2, exponent);
    data_ = new T[capacity_];
    //do a std::copy to improve performance
    //std::copy(source_list.begin(), source_list.end(), data_+1);
    for(unsigned i = 1; i < size_; i++) {
        data_[i] = source_list[i-1];
    }
    _buildHeap(size_-1);
    std::cout << "size of the input array is " << source_list.size() << "\n";
    std::cout << "Capacity is " << capacity_ << " and size_ is " << size_ << "\n";
}

template <typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& other) {
    capacity_ = other.capacity_;
    size_ = other.size_;
    data_ = new T[capacity_];
    //do a std::copy to improve performance
    //std::copy(std::begin(other.data_), std::end(other.data_), data_);
    for(unsigned i = 0; i < capacity_; i++) {
        data_[i] = other.data_[i];
    }
    std::cout << "capacity of the input PriorityQueue is " << other.capacity_ << ", size is " << other.size_ << "\n";
    std::cout << "this now has capacity of " << capacity_ << " and size_ " << size_ << "\n";
}

template <typename T>
PriorityQueue<T>& PriorityQueue<T>::operator=(const PriorityQueue<T>& rhs) {
    if(data_) delete[] data_;
    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    data_ = new T[capacity_];
    //do a std::copy to improve performance
    // std::copy(std::begin(rhs.data_), std::end(rhs.data_), data_);
    for(unsigned i = 0; i < capacity_; i++) {
        data_[i] = rhs.data_[i];
    }
    std::cout << "capacity of the input PriorityQueue is " << rhs.capacity_ << ", size is " << rhs.size_ << "\n";
    std::cout << "this now has capacity of " << capacity_ << " and size_ " << size_ << "\n";
    return *this;
}

template <typename T>
PriorityQueue<T>::~PriorityQueue() {
    delete[] data_;
    data_ = NULL;
}

template <typename T>
void PriorityQueue<T>::_growArray() {
    //we do not relie on realloc() since it might not correctly initialize type ctors if T not primitive type
    //however, this function should achieve the same as realloc(data_, 2*capacity_*sizeof(T)) does if T primitive type
    T* newData = new T[capacity_ * 2];
    //copy every elem of old array to new, including the inaccessible elems
    for(unsigned i = 0; i < capacity_; i++) { 
        newData[i] = data_[i];
    }
    capacity_ *= 2; //update capacity_;
    delete[] data_; //deallocate the memory
    data_ = newData;
}

//printing the content of the min heap linearly.
template <typename T>
void PriorityQueue<T>::printHeap() {
    std::cout << "Printing the heap content, the data should display all inserted data and no empty cell.\n";
    for(unsigned i = 1; i < size_; i++) {
        //require T printalbe (ostream-able) by overloading <<
        std::cout << "[" << data_[i] << "]"; 
    }
    std::cout << "\n";
}

template <typename T>
T& PriorityQueue<T>::peek() {
    return data_[1]; //start at 1st index, 0th left for sentinel
}

template <typename T>
const T& PriorityQueue<T>::peek() const {
    return data_[1]; //start at 1st index, 0th left for sentinel
}

template <typename T>
void PriorityQueue<T>::insert(const T& elem) {
    //if adding a new element will exceed capacity: double the capacity
    if(size_ + 1 > capacity_) {
        _growArray(); //invoke realloc()
    }
    //first insert the element to the end of the array (at index size_)
    data_[size_] = elem;
    //check for recursive out of orderness, adjust them
    _heapifyUp(size_);
    //update the counter, this does not impact the recursion upward, unlike removeMin()
    size_++; 
}

template <typename T>
T PriorityQueue<T>::removeMin() {
    T minval = data_[1];
    data_[1] = data_[size_-1];
    data_[size_-1] = minval;
    // this makes the element "inaccessible" from the pque
    // it's still there, will be really deleted in dtor() or overwritten by insert()
    size_--;  
    //heapify down after the old root removed to ensure safety of recursing down.
    _heapifyDown(1);
    return minval;
}

template <typename T>
void PriorityQueue<T>::_heapifyUp(unsigned index) {
    if(index == 0) throw std::runtime_error("Accessing invalid index 0");
    if(_isRoot(index)) { //base case where we can terminate the cascade, no parent to swap
        return;
    }
    unsigned parent_idx = _getParent(index);
    if(data_[index] < data_[parent_idx]) { //require T comparable and < overwritten
        //swap the elements by copy assignment op OR std::swap?
        //std::swap relies on copy/assignment semantics, so doesn't matter

        //require T copy ctor and assignment op be deep copy
        T temp = data_[parent_idx];
        data_[parent_idx] = data_[index];
        data_[index] = temp;
    }
    _heapifyUp(parent_idx);
}

template <typename T>
void PriorityQueue<T>::_heapifyDown(unsigned index) {
    if(index >= size_)  throw std::runtime_error("Accessing invalid index outside array size (does not imply outside array capacity, but possible");
    if(_isLeaf(index)) { //base case when we terminate the cascade, no child to swap
        return;
    }
    //we use minimum of the two children to swap downstream, so that the min child after swap will be the minimal node in its subtree
    //this ensures the subtree structure is a minheap, that's why we cascade downward to keep every level like that
    unsigned minchild_idx = _getMinChild(index); 
    if(data_[minchild_idx] < data_[index]) { //does not invoke > operator, might not be overloaded in some minimal class impl.
        T temp = data_[minchild_idx];
        data_[minchild_idx] = data_[index];
        data_[index] = temp;
    }
    _heapifyDown(minchild_idx);
}

template <typename T>
unsigned PriorityQueue<T>::_getParent(unsigned index) const {
    if(_isRoot(index)) {
        throw std::runtime_error("Error: accessing the parent of the root index, not permitted.\n");
    } 
    return index / 2;
}

template <typename T>
unsigned PriorityQueue<T>::_getMinChild(unsigned parent) const {
    if(_isLeaf(parent)) {
        throw std::runtime_error("Error: accessing the children of a leaf index, not permitted.\n");
    }
    unsigned left = parent * 2;
    unsigned right = parent * 2 + 1;
    //if the current node has a single child (has to be the left child, since complete tree)
    if(right >= size_) return left;
    //else return the index of which the element is smaller
    return data_[left] < data_[right] ? left : right;
}

template <typename T>
bool PriorityQueue<T>::_isRoot(unsigned index) const {
    return index == 1;
}

template <typename T>
bool PriorityQueue<T>::_isLeaf(unsigned index) const {
    //remember the definition of size_ is the actual size of the array (meaningful elements + 0th sentinel)
    return index * 2 >= size_;
}

template <typename T>
void PriorityQueue<T>::_buildHeap(unsigned size) {
    //using heapifyDown(): the leaf nodes are ignored, it's already heap by definition
    //this argument size is ACTUALLY size_-1 after building the heap, it's the size of the source array
    //start from size/2 to 1, the build operation is O(n)
    for(unsigned i = _getParent(size); i > 0; i--) { //do not heapify down the sentinel node
        //heapifyup takes O(n logn) for each node
        //heapifydown from i=1 (root) to parent(size) could do extra work
        //heapifydown from parent(size) downto i=1 is the optimized, despite each heapify is O(log n)
        //the height of each element differs, the larger height, the fewer elements in that layer
        _heapifyDown(i);
    }
}


