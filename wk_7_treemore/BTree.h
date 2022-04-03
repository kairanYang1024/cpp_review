/*
    Big-O notation assume universal access time for all data, however, not true in reality.
    store data on disk --> slower than in main memory (virtual memory)
    B-tree: extremely performing in both main memory and disk.
    --> optimize few disk seeks (disk, cloud, server, ... anywhere) as possible.

    B-tree properties: multiple keys and children (not limited to 2) for a given node
    B-tree key in sorted order (ascending usually)
    Order of a B-tree = maximum number of keys a node can have + 1 (max number of children can have)
    A B-tree of order m is an m-ary tree.

    Goal: minimize numbers of reads, the depth of tree is shallow in this setting, thus it will have fewer
    "seeks" than an AVL Binary Search Tree to grep a key given same amount of data.

    The ptrs of the nodes are in between the keys, and on the endpoints of the keys.

    Each internal node has exactly one more children than number of keys
    A root node can be a leaf OR have [2, m] children, and 
    each non-root internal node has [ceil(m/2), m] children. (split result in keys & children halved from m+1) 
    all leaf nodes are at same level, the split is propagating upward (creating new <internal> nodes)
*/

#pragma once
#include <vector>
#include <iostream>
using std::vector;

//This implementation of BTree does not support single element removal, the only "removal" is batch removal in dtor.
template <typename K>
class BTree {
    public:
        BTree() : BTree(3) {} //default ctor, creating binary tree
        BTree(unsigned order); //arg ctor specifying the BTree order 
        BTree(const std::vector<K>& source_list, unsigned order);
        ~BTree();

        //insert a value to a B-Tree Node, will alter the tree structure if the current tree is full
        void insert(const K& key);

        //find a value from a B-Tree node, ACCESS complexity in O(log_m n) but each ACCESS could be slow
        //also performing linear search on each node so total is O(m * log_m n)
        bool contains(const K& key);

        // height() of a B-tree is bounded by the order of log_m(n) where m is the order of the tree
        // since BTree is an m-ary tree
        int height() const;
        //order must be greater than 2, otherwise it wont be able to host data
        unsigned order() const;

        //print the content of the tree by inorder traversing.
        void printTree();
    
    private:
        class BTreeNode {
            public:
                vector<K> keys_;
                unsigned keycount_; //number of keys that are hosted in this node, equivalent to keys_.size()

                //make a vector to access the list of children, in this implementation the children resided on heap
                //however, in real situation the children may resided in disk, cloud, server, etc.
                vector<BTreeNode*> children; 

                int height_;

                //creating empty vectors when the node is created
                BTreeNode() : keycount_(0), height_(0) {
                    keys_ = std::vector<K>();
                    children = std::vector<BTreeNode*>();
                    // keys_.reserve(order()+1); //pre-allocate the elements in thie vector
                    // children.reserve(order()+2);
                }

                bool isLeaf() const {
                    return height_ == 0;
                }

                void printKeys() {
                    for(const K& elem : keys_) std::cout << elem << " ";
                }

        };

        BTreeNode* root_;
        unsigned order_; //categorizes the capacity of children (and keys) each node can host in this tree
        int _height(BTreeNode* curr) const; //relies on the local variable curr->height_;

        //check whether node contains the argument key
        bool _contains(BTreeNode*& node, const K& key);

        //find the appropriate node to add the new key into its key list
        //to be consistent, _find_insert WILL send key to leaf node then propagate upward
        void _find_insert(BTreeNode*& node, const K& key);
        //split the child at split_child_idx of parent's children node ptr vector
        //also, promote the middle element of parent->children[split_child_idx] to the parent node
        //not recursive, this version isolates the "parent creation" and child split (moved to insert())
        void _split_child(BTreeNode*& parent, unsigned split_child_idx); 
        //populate the newRight node to have content and children ptrs of tobeSplit's right half removing
        void _fill_right(BTreeNode*& tobeSplit, BTreeNode*& newRight);

        void _printTreeInOrder(BTreeNode* curr);
        void _clear();
        void _clearNode(BTreeNode* curr);

};

#include "BTree.hpp"