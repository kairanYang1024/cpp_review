#pragma once
#include <unordered_map>

/**
 * Implementing the class Dictionary using Binary search tree, balanced by AVL data structure.
 * @author Kairan Yang (kairany2@illinois.edu)
**/

/**
 * @brief Balancing factor: the difference in height between rightsubtree and left subtree from the arg node
 * 
 * Balancing type: "stick": denoted by a sequence of at least 2 nodes linked in left or right, creating unbalance
 * e.g. curr->right->right; curr->left->left
 * 
 * Solution: find the lowest node with a |balance factor| >= 2, identify its type and perform rotation
 * 
 * Rotation I (left rotation): find the deepest node of balance factor 2, and its right child balance factor 1
 * break the middle node, raise it up, make its parent its left child, make its original left child its
 * old parent's right child (preserving order of BST) 
 * 
 * Rotation II (right rotation): find the deepest node of balance factor -2, and its right child balance factor -1
 * break the middle node, raise it up, make its parent its right child, make its original right child its
 * old parent's left child (preserving order of BST)
 * 
 * Balancing type: "elbow": denoted by a sequence of 2 nodes linked to right(left) then to left(right), creating unbalance
 * e.g. curr->right->left; curr->left->right
 *        1    2      3      1     2    3
 * Solution: find the lowest node with a |balance factor| >= 2, identify its type and perform rotation
 * 
 * Rotation III(right-left rotation): find deepest node of balance factor 2, and its right child balance factor -1
 * break the "bent" part of the "elbow", elevate node 3 to the middle, 3's left subtree untouched, make node 2 the
 * right child of node 3, and make 3's right subtree as node 2's new left subtree (2's old left subtree is node 3)
 * Then we obtain a pattern of Rotation I, refer to Rotation I for further procedure.
 * 
 * Rotation IV(left-right rotation): find deepest node of balance factor -2, and its left child balance factor 1
 * break the "bent" part of the "elbow", elevate node 3 to the middle, 3's right subtree untouched, make node 2 the
 * left child of node 3, and make 3's left subtree as node 2's new right subtree (2's old right subtree is node 3)
 * Then we obtain a pattern of Rotation II, refer to Rotation II for further procedure.
 *
**/

template<typename K, typename V>
class Dictionary { 
    public:
        Dictionary() : root_(NULL) {} //just create empty dict
        Dictionary(const std::unordered_map<K,V>& source_map); //convert an unorderedmap into treemap dictionary
        //voided copy ctor and copy assignment op since value V might take a lot of rsrcs to reallocate
        //just do the default shallow copying
        ~Dictionary();

        //the structure successfully maintains find() in O(log n)
        V& find(const K& key);
        const V& find(const K& key) const; 
        //contains() just like find() BUT do not return the value, if not find, return false instead of exception
        bool contains(const K& key);

        //insert using find, O(h), insert might cause rotation if satisfies conditions
        void insert(const K& key, const V& value);

        // remove: O(h) + extra work.
        // return a copy of the value once it has been removed from tree structure, require 3 copies to get to caller
        // after the actual remove, might cause rotation if satisfies conditions
        V remove(const K& key);

        void printTree(); //in-order printing tree content
        int height() const; //return height of the tree
        bool empty() const; //return whether the tree is empty
    
    private:
        class TreeNode {
            public:
                //require K to implement comparable <, <=, ==
                const K key;
                V value;

                //stores the relative height of this node in the BST, making it DP to not calculate height every time
                int height; 

                TreeNode *left, *right;
                TreeNode() : left(NULL), right(NULL) {}
                TreeNode(const K& key, const V& data) : key(key), value(data), height(0), 
                    left(NULL), right(NULL) {}      
        };
        TreeNode* root_;

        void _printTreeInOrder(TreeNode* curr);

        //helper methods of the ADT interface functions
        TreeNode*& _find(const K& key, TreeNode*& curr) const;
        void _find_insert(const K& key, const V& value, TreeNode*& node);

        V _find_remove(const K& key, TreeNode*& curr); //public remove() calls this, then this calls _remove(node)
        V _remove(TreeNode*& node); //actually remove the node in argument
        V _iop_remove(TreeNode*& node); //double-children case, swap and remove
        V _iop_remove(TreeNode*& target, TreeNode*& iopAncestor, bool iopAdjacent); //adopted from Coursera
        //**dangerous pointer-manipulation**
        //swap the reference of node1 and node2, return that reference of node2 at the position (addr) of node1
        TreeNode*& _swap_nodes(TreeNode*& node1, TreeNode*& node2); 

        //helper methods for rebalancing the tree
        //master program for deciding which type of unbalancing the node has.
        void _ensureBalance(TreeNode*& curr); 
        //rotations are local operations (based on an argument of TreeNode*)
        //rotations does not impact the overall tree structure (but the subtree rooted at the argument)
        //rotations run in O(1) time: reassigning ptrs without loop
        void _rotateLeft(TreeNode*& curr); //rotation type I
        void _rotateRight(TreeNode*& curr); //rotation type II
        void _rotateRightLeft(TreeNode*& curr); //rotation type III
        void _rotateLeftRight(TreeNode*& curr); //rotation type IV

        int _balance_factor(TreeNode* curr) const; //return the balance factor of node curr
        int _height(TreeNode* curr) const; //getter now relies on curr->height
        void _update_height(TreeNode*& curr); //relies on height of the children nodes, not recursive, might be unsafe

        void _clear();
        void _clearupNode(TreeNode* curr);
};

#include "Dictionary.hpp"