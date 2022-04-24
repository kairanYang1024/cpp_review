/*
 * Implementation of Data Structure DisjointSets in C++, tied to UnionFind algorithm.
 * Disjoint Sets ADT:
 * maintain a collection S = {s0, s1, ..., sk} where each set has a representative member
 * "disjoint" means that none of the sets share a common element ever.
 * API: makeset(k) --> create a member set of collection from the data arg
 * union(k1, k2): unite the sets including k1 and k2, if they belong to diff sets
 * find(k): find the "set" that k belong to by its representation
 * 
 * simple implementation: array index or hashmap as set elements, the values are the
 * "set identity" of that index belong to
 * e.g. {0,1,4}, {2,7}, {3,5,6} in array:
 * idx: 0 1 2 3 4 5 6 7
 * val: 0 0 2 3 0 3 3 2
 * 
 * find(4) = arr[4], O(1)
 * union(4,2): {2,7} updated to be 0, need to overwrite every elem having value 2, O(n)
 * (losing touch)
 * 
 * uptree implementation: store root elem and sth different:
 * value of array is -1 if FOUND the representative elem. AND
 * idx of parent, if we have not found the rep. elem; this structure denoted "uptree"
 * 
 * union(a,b) let arr[b]=a if b is "smaller" than a, this case union is O(1)
 * this leads to indices that having -1 as value roots of their respective sets
 * and cardinality of the entire collection is # of elements having -1 their vals
 * find() efficiency: O(h) where h is the height of the uptree
 * at worst, O(n) where data degenerates to llist.
 * at best: height of the tree is O(1) of any non-rep pointing to the rep, making find O(1)
 * 
 * optimization: smart unionize -- 
 * union by rank (height): how tall is the tree of the node, never make taller tree taller.
 * make the value negative of height - 1, consider height can be 0 violating the structure
 * put shorter tree subtree of taller tree.
 * downside: if the shorter tree is larger, then the work to access every elem in it (height/depth)
 * of that subtree is increased by 1.
 * 
 * alternative: union by size: root node keeps track the size of their set instead of height of uptree
 * always increase the smaller subtree's depth (height).
 * 
 * both ensures log(n) height, how to make better: path compression
 * REMEMBER: the intermediate path nodes are meaningless in this data structure.
 * once find() visits intermediate nodes, we update the intermediates to point to root node
 * (it also reduces the height of other subtrees having parents being intermediate nodes updated)
 * 
 * analysis: runtime of disjoint sets find() is iterated_log (aka almost constant)
 * log*(n) = 0 if n <= 1
 * 1 + log*(log(n)) if n > 1
 * = how many times applying log returns nonzero value
 * amortized to O(1)*
 */

#pragma once
#include <vector>
#include <unordered_map>
#include <string>

//unlike in CS 225, this is a <generic> union-find data structure as in stdlib
template <typename T>
class DisjointSets {
    private:
        //the index mapping of data to the root element of their respective sets.
        //implemented as an uptree-type data structre able to compress path.
        //inserting new data in an array is too verbose to hand-realloc arr size, let vector do that.
        std::vector<int> uptree_; 

        //the actual data elements added to the dsets structure, explicit with its index
        //ensure this map is idx one-to-one to its data, NO duplicate of key T as well, as said "Disjoint"
        //maintain both hashmaps synchronously to ensure fast unionfind structure from generic interface
        std::unordered_map<T, int> indices_;
        std::unordered_map<int, T> rev_indices_;

        //the number of elements in the data map (and in uptree)
        int size_;
        //the number of sets in the data map, initialized to size_ and reduced each time calling unionize() successes
        int num_sets_;

        //since path is compressed, let's do union by size, height is hard to compute after path compression

        /*  1 means union by size, 0 means by height, one instance cannot have both modes
            to switch mode is to recompute every root node value and is O(n)
            default is union by height, can specify its mode via ctor
            bool union_mode_; */

    public:
        //big three, allow deep copy of this class
        DisjointSets(); 
        DisjointSets(const T*& raw_data, int length); //initialize the dsets based on an array
        DisjointSets(const std::vector<T>& raw_data); //initialize the dsets based on a vector
        DisjointSets(const DisjointSets<T>& other);
        DisjointSets<T>& operator=(const DisjointSets<T>& rhs);
        ~DisjointSets() {} //nothing need to do here, everything are stack-allocated, but it calls dtors of member containers

        //API
        void make_set(const T& elem);//create a new set based on element
        bool unionize(const T& k1, const T& k2); //union the sets including k1 and k2, flag true if successfully merged
        const T& find(const T& key); //return the representative element of the set key belongs to

        //statistics
        int size();
        int num_sets(); //return the number of disjoint sets in this container

        //functionals
        void print_uptree(); //printing the uptree to reveal the index - rootnode relationship
        void print_sets(); //formatting the content of the containers in collection of sets format e.g. {{a,b}, {c}, {d,e,f}}

    private: //helper methods
        bool _unionize(int idx1, int idx2); //will encode root node values based on the union mode
        int _find_repidx(int keyidx); //this method also do path-compression on its traversed nodes 
};

#include "DisjointSets.hpp"