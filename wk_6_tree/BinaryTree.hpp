#include "BinaryTree.h"
#include <iostream>
#include <vector>
#include <deque>

using std::cout;
using std::endl;
using std::deque;

template<typename T>
BinaryTree<T>::BinaryTree() : root_(NULL) {}

//construct a binary tree from vector in iterative way
template<typename T>
BinaryTree<T>::BinaryTree(const std::vector<T>& contents) : BinaryTree() { //wrapper for the create function
    cout << "Invoking value ctor\n";
    _create_from_list(contents);
}

template<typename T>
void BinaryTree<T>::_create_from_list(const std::vector<T>& contents) {
    _clear(); //rezero
    if(contents.empty()) return; 
    root_ = new TreeNode(contents[0]);

    //container to hold reference (as ptr) to the new treenodes and assemble them into a tree
    deque<TreeNode**> childPtrQueue; 
    //REMEMBER: & is reference that aliases to the rvalue's memory address
    //if push directly root->left it would be a plain NULL
    childPtrQueue.push_back(&(root_->left));
    childPtrQueue.push_back(&(root_->right));
    for(unsigned i = 1; i < contents.size(); i++) {
        TreeNode** childRef = childPtrQueue.front();
        //dereference the memory address, revealing a tree pointer binded to that particular memory address
        TreeNode*& childPtr = *childRef;
        childPtr = new TreeNode(contents[i]); //if not by ref, we are just creating TreeNodes but not linking them
        childPtrQueue.pop_front();
        childPtrQueue.push_back(&(childPtr->left));
        childPtrQueue.push_back(&(childPtr->right));
    }
    //let compiler recycle the extra "ptr to NULL"s
}

//deep copying tree to another memory location
template<typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& other) : BinaryTree() {
    cout << "Invoking copy ctor\n";
    root_ = _copy_node(other.root_);
}

//return a reference to avoid copying tree when return
template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree<T>& rhs) {
    cout << "Invoking operator=\n";
    _clear(); //rezero the tree content as in BinaryTree<int> a = b;
    root_ = _copy_node(rhs.root_);
    return *this;
}

template<typename T>
BinaryTree<T>::~BinaryTree() { //invoking helper function
    cout << "Invoking dtor\n";
    _clear();
}

template<typename T>
void BinaryTree<T>::_clear() {
    if(!root_) return;
    _clearupNode(root_); //invoking the recursive helper on root_
    root_ = NULL;
}

//wrapper codes
template<typename T>
void BinaryTree<T>::preorder() {
    _preorder(root_);
}

template<typename T>
void BinaryTree<T>::inorder() {
    _inorder(root_);
}

template<typename T>
void BinaryTree<T>::postorder() {
    _postorder(root_);
}

//level order traversal invoking a queue
template<typename T> 
void BinaryTree<T>::levelorder() {
    if(!root_) return;
    deque<TreeNode*> temp; 
    temp.push_back(root_);
    while(!temp.empty()){
        TreeNode*& curr = temp.front(); //nothing is actually being copied in this process
        _shout(curr);
        if(curr->left) temp.push_back(curr->left);
        if(curr->right) temp.push_back(curr->right);
        temp.pop_front();
    }
}

//getters
template<typename T>
int BinaryTree<T>::height() const {
    return _height(root_);
}

template<typename T>
bool BinaryTree<T>::hasElem(const T& elem) const {
    return _hasElem(elem, root_);
}