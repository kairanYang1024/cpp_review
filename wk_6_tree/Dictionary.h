//Binary search tree: ordered binary tree capable of searching
//binary tree is BST if for every node in the tree, nodes in left subtree less than itself
//nodes in right subtree are greater than itself
//in other words, inorder traversal of BST prints ascending order

//dictionary = {key, data} pair, see python dictionary
//login email --> profile data
//phone number --> phone record
//website url --> webpage object (file)
//key = unique distinct identifier, data = copiable data

/*Dictionary ADT: 
    find -> find data associated w/ a key in the dictionary
    insert -> add a k-v pair to the dictionary
    remove -> removes a key from the dictionary
    empty -> true if dictionary is empty
    (notice no update/change in this dictionary ADT, assume static)
*/

//store both key and data in the node BUT we only show key

#pragma once

template<typename K, typename V>
class Dictionary { //a static structure, we can modify value but never key
    public:
        Dictionary() : root_(NULL) {} //just create empty dict
        //voided argument ctor, this tree structure is prune to the order of the input list 
        //voided copy ctor and copy assignment op since value V might take a lot of rsrcs to reallocate
        //just do the default shallow copying
        ~Dictionary();

        //BST find: O(logn) (best case: perfectly balanced) <= O(h) <= O(n) (worst case = linkedlist)
        //quantify the balance: balance factor = height(T_right) - height(T_left)
        //ideally: keep the balancing small
        //balanced BST: a BST where every node has a balancing factor of +-1 or 0
        V& find(const K& key);
        const V& find(const K& key) const; 

        //insert using find, O(h)
        void insert(const K& key, const V& value);

        // remove: O(h) + extra work.
        // remove leaf = simple, just remove it
        // remove node with 1 child: remove it and replaced by its child
        // remove node with 2 children: remove it, its position replaced by its iop 
        // (in-order predecessor, the one node in front of it in inorder traversal)
        // since BST's special structure, the IOP "must" be the rightmost node in the left subtree
        // finding iop: O(h), since need to dive to the left subtree
        // remove iop: must have no-children OR one-children, utilizing the simpler cases
        // return a copy of the value once it has been removed from tree structure, require 3 copies to get to caller
        V remove(const K& key);

        //in-order printing tree content
        void printTree(); 
        //return height of the tree
        int height() const;
        //return whether the tree is empty
        bool empty() const;
    
    private:
        class TreeNode {
            public:
                const K key; //require K to implement comparable <, <=, ==
                V value;
                TreeNode *left, *right;
                TreeNode() : left(NULL), right(NULL) {}
                TreeNode(const K& key, const V& data) : key(key), value(data), 
                    left(NULL), right(NULL) {}      
        };
        TreeNode* root_;

        //helper methods of the ADT interface functions
        TreeNode*& _find(const K& key, TreeNode*& curr) const;
        V _remove(TreeNode*& node);
        TreeNode*& _iop_of(TreeNode*& curr) const; //return the iop of the node curr
        TreeNode*& _rightmost_of(TreeNode*& curr) const; //return the rightmost node of the node curr

        //**dangerous pointer-manipulation**
        //swap the reference of node1 and node2, return that reference of node2 at the position (addr) of node1
        TreeNode*& _swap_nodes(TreeNode*& node1, TreeNode*& node2); 

        //if we need to do sth with the node location in the tree, pass by ref&, if just need to access, plain ptr works
        void _printTreeInOrder(TreeNode* curr);
        int _height(TreeNode* curr) const;

        void _clear();
        void _clearupNode(TreeNode* curr);
};

#include "Dictionary.hpp"