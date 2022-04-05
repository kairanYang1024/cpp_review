/**
 * Priority Queue class implemented by minHeap ("min" defined by operator < comparable)
 * The class itself is essentially a wrapper of an array with special structure of accessing its indices
 * warping between indices looks like a tree-like structure, the class itself does not have any linked memory.
 * Thus, this class, unlike BinaryTree.h, Dictionary.h, BTree.h and KDTree.h (optional), has no worries of
 * managing heap memory and preventing leaking, also, has no inner node class.
 * 
 * no total sort, but peeking minimum possible for each sequence of call.
 * Reduced from O(nlogn) to sublinear build time.
 * RemoveMin of sorted and unsorted array (and list) are both O(n) 
 * 
 * Heap property: min_heap
 * a COMPLETE (only last level not filled up) binary tree T is min-heap if:
 * T = {} or
 * T = {r, T.left, T.right}, r < the roots of {T.left, T.right} where both are min_heaps
 * 
 * max_heap can be applied recursively on the > operator as well.
 * 
 * a complete tree can be mapped (isomorphic) to an array
 * using index to represent parent-children relation of tree inside an array
 * starting at 1, not 0.
 * parent = i/2
 * children left = 2*i, children right = 2*1+1
 * 
 * 
 * Heap sort: build a heap: O(n)
 * n * removeMin() to sort the array: O(n * log(n))
 * maybe reverse the order: O(n)
 * Overall: O(n*log(n))
 * This sort is entirely in-place, just like insertion, merge(no allocate temp array), and quicksort.
 * 
 **/

#pragma once
#include <vector>

template <typename T>
class PriorityQueue {
    //O(1) lookup require the class design not to be a linked memory structure
    //a complete tree can be mapped (isomorphic) to an array, do it!
    private:
        //inner array storing data, is heap memory (it's a pun!)
        T* data_; 

        //raw size of the data array, includes 0th element (meaningful size + 1)
        //special property of tree height h, this size_ reaches a perfect tree at 2^(h+1)
        unsigned size_;

        //the capacity of the data array that needs to be doubled once reaching capacity
        //since heap is binary, this should always in powers of 2.
        unsigned capacity_; 

    public:
        //ctor
        PriorityQueue();
        //heapify ctors: using buildHeap()
        PriorityQueue(const T* source_array, unsigned size);
        PriorityQueue(const std::vector<T>& source_list);
        //copies
        PriorityQueue(const PriorityQueue<T>& other);
        PriorityQueue<T>& operator=(const PriorityQueue<T>& rhs);
        //dtor
        ~PriorityQueue();

        //peek the minimum element in the heap, O(1)
        T& peek();
        const T& peek() const;

        // insert an element inside the array, then check maintaining min_heap property
        // by swapping out of ordered values, cascading upward.
        // efficiency of insert is O(log n), cannot do buildHeap on insert element by element
        void insert(const T& elem);

        // can only remove the min element in a pque, cannot do anything else
        // by swapping the first and last element in the pque (otherwise no longer full binary tree)
        // we are going to remove last elem and heapify down the new root
        // efficiency is O(log n)
        T removeMin(); 

        //printing the content of the min heap linearly.
        void printHeap();

    private:
        //wrapper of reallocating content of the array, the pointer might not be preserved
        void _growArray(); 
        //should be in O(n) --> only using heapifyDown
        void _buildHeap(unsigned size);

        //general helper functions
        void _heapifyUp(unsigned index);
        void _heapifyDown(unsigned index);
        unsigned _getParent(unsigned index) const;
        unsigned _getMinChild(unsigned parent) const;
        //general indicator functions
        bool _isRoot(unsigned index) const;
        bool _isLeaf(unsigned index) const;

};

#include "PriorityQueue.hpp"