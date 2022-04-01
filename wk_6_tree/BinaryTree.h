//each element in the tree is a node, each connection between 2 nodes is an edge
//edges: not labeled, attaching no value, identified by the endpoints
//tree is directed, always, from root to leaf.
//a special case (rooted) of directed acyclic graphs (DAGs)

//parent node is the node where the current node is receiving incoming edge
//children node is the node that current node pointed to.
//if a node has 0 children, it is a leaf, a node can have 0 ~ a lot children nodes
//sibling: at same node sharing parent node
//ancestor: set of nodes that are above the current node

//complete (binary) tree: a tree where all of the levels except the leaf level are filled,
//AND all the leaf nodes are pushed to the left first.

//full (binary) tree: a tree where each node except leaf nodes having 2 children

//perfect tree: both complete and full, a tree where each internal nodes having 2 children and 
//except leaf level, since all (internal) levels are filled, leaf level is also filled.
//level-k perfect tree hosted the maximum number of nodes a tree of level k can have
//height of a tree = number of edges existed in the longest path from root to a leaf
#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
class BinaryTree {
    public:
        BinaryTree();
        BinaryTree(const std::vector<T>& contents); //create a tree in level-order fashion from a linear structure
        BinaryTree(const BinaryTree<T>& other); //deep copying tree to another memory location
        BinaryTree<T>& operator=(const BinaryTree<T>& rhs); //return a reference to avoid copying tree when return
        ~BinaryTree();

        //tree traversal: visit every node of the tree exactly once 
        //preorder: output node value, go left, go right; pre=output value early
        //inorder: go left, output node value, go right; in=output value between call left() and right()
        //postorder: go left, go right, output node value; post=output value late
        //level-order traversal: read every level at a time, BFS, not based on recursion
        void preorder();
        void inorder();
        void postorder();
        void levelorder(); //require a queue

        //tree search: searching a value in the tree and return, if not present, return false
        bool hasElem(const T& elem) const;

        //adding nodes (change is searching the entire tree, delete involves tree balancing, not implemented here)
        // void add_dumb(const T& elem); //add to the leftmost node, trivial
        // void add(const T& elem); //smartly adding treenodes from left to right

        //getters for instance properties
        int height() const;
        
    private:
        class TreeNode {
            public:
                T data;
                TreeNode *left, *right;
                TreeNode() : left(NULL), right(NULL) {} //let ctor of T define the construction of T instance
                TreeNode(const T& data) : data(data), left(NULL), right(NULL) {}
                TreeNode(const TreeNode& other) : data(other.data), left(other.left), right(other.right) {}
                TreeNode& operator=(const TreeNode& other) {
                    data = other.data;
                    left = other.left;
                    right = other.right;
                    return *this;
                }

                void displayValue() { //require T type printable
                    std::cout << data; 
                }
        };

        TreeNode* root_;
        void _clear();  //helper for releasing memory of this data structure
        void _create_from_list(const std::vector<T>& contents); //helper for creating tree memory from a known source list

        //helper method for displaying a node value
        void _shout(TreeNode* curr) {
            if(!curr) {
                std::cout << "Warning: dereferencing null ptr\n";
                return;
            }
            curr->displayValue();
            std::cout << " "; //tabbing 
        }

        //check height of subtree rooted at curr
        int _height(TreeNode* curr) const { 
            if(!curr) return -1;
            return 1 + std::max(_height(curr->left), _height(curr->right));
        } 

        //search algorithm based on preorder (from shallow to deep)
        bool _hasElem(const T& elem, TreeNode* curr) const { //require == implemented in class
            if(!curr) return false;
            if(curr->data == elem) return true;
            return _hasElem(elem, curr->left) || _hasElem(elem, curr->right);
        }

        //helper methods of the traversals
        void _preorder(TreeNode* curr) {
            if(!curr) return;
            _shout(curr);
            _preorder(curr->left);
            _preorder(curr->right);
        }

        void _inorder(TreeNode* curr) {
            if(!curr) return;
            _inorder(curr->left);
            _shout(curr);
            _inorder(curr->right);
        }

        void _postorder(TreeNode* curr) {
            if(!curr) return;
            _postorder(curr->left);
            _postorder(curr->right);
            _shout(curr);
        }

        //copy the data of the source node into the current tree in the same location of the source tree
        //and return the newly copied node, recursively deep-copying the children ptr values as well
        //node copying in pre-order fashion, mirroring post-order deletion
        TreeNode* _copy_node(const TreeNode* rhs) { 
            if(!rhs) return NULL;
            //the copy ctor of class TreeNode will not work 
            //since it does not copy the data but just memory location of the children ptrs
            TreeNode* lhs = new TreeNode(rhs->data);
            lhs->left = _copy_node(rhs->left);
            lhs->right = _copy_node(rhs->right);
            return lhs;
        }

        //release node memory on heap in postorder (only viable) traversal fashion
        void _clearupNode(TreeNode* curr) {
            if(!curr) return;
            _clearupNode(curr->left);
            _clearupNode(curr->right);
            curr->left = NULL; //assume free on curr->left happens, renull it so wont be visiting again
            curr->right = NULL; //assume free on curr->right happens, renull it so wont be visiting again
            delete curr;
            //can also do delete curr, curr= NULL;  no difference
        }
};


#include "BinaryTree.hpp" //attaching implementation