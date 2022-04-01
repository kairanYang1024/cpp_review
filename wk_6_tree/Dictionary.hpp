#include "Dictionary.h"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <cstring>

template <typename K, typename V>
V& Dictionary<K,V>::find(const K& key) {
    TreeNode*& result = _find(key, root_);
    if(!result) throw std::runtime_error("Key not found"); //included in stdexcept 
    return result->value;
}

template <typename K, typename V> //read only version of find()
const V& Dictionary<K,V>::find(const K& key) const {
    TreeNode*& result = _find(key, root_);
    if(!result) throw std::runtime_error("Key not found"); //included in stdexcept 
    return result->value;
}

template <typename K, typename V> //we forward declare typename so that TreeNode type is resolved
typename Dictionary<K,V>::TreeNode*& Dictionary<K,V>::_find(const K& key, TreeNode*& curr) const {
    if(!curr) return curr; //still return the reference to that NULL node, instead of returning plain NULL
    if(key == curr->key) return curr;
    else if(key < curr->key) return _find(key, curr->left);   //require < implemented for type K
    else return _find(key, curr->right);
}

template <typename K, typename V>
void Dictionary<K,V>::insert(const K& key, const V& value) {
    //invoking helper _find, return the reference to ptr for the purpose of adding node
    TreeNode*& result = _find(key, root_); 
    //guard the case of inserting element already existed in the tree, will lose subtree if renew it
    if(result) {
        std::cout << "Key " << key << " already existed, updating value...\n";
        result->value = value; //require type V has deep-copy operator=
        return; 
    }
    result = new TreeNode(key, value); 
}

template <typename K, typename V>
V Dictionary<K,V>::remove(const K& key) { //this copies a lot of V, might be slow if V has lot of data
    TreeNode*& remNode = _find(key, root_);
    if (!remNode) throw std::runtime_error("error: remove() used on non-existent key"); 
    return _remove(remNode);
}

template <typename K, typename V>
V Dictionary<K,V>::_remove(TreeNode*& node){ //pass by reference to actually modify the ptr
    //zero-child remove, base case
    if(!node->left && !node->right) {
        V data = node->value;
        delete node;
        node = NULL;
        return data; 
    } else if(!node->left) { //only has right node
        TreeNode* temp = node;
        node = node->right;
        V data = temp->value;
        delete temp;
        temp = NULL;
        return data;
    } else if(!node->right) { //only has left node
        TreeNode* temp = node;
        node = node->left;
        V data = temp->value;
        delete temp;
        temp = NULL;
        return data;
    } else {
        TreeNode*& iop = _iop_of(node); //iop node may be a leaf, or having a left child, impossible to have right child
        if(!iop) throw std::runtime_error("Despite two children detected, iop is not found, internal structure error");
        //swap iop and the current node by reference, so that the iop is at the position of node and node at position of old iop
        //at this phase, the BST structure is temporarily violated
        TreeNode*& moved_target = _swap_nodes(node, iop); 
        return _remove(moved_target);
    }
}

template <typename K, typename V>
typename Dictionary<K,V>::TreeNode*& Dictionary<K,V>::_iop_of(TreeNode*& curr) const { 
    //return the iop of the node curr
    if(!curr) return curr; //NULL returned
    if(!curr->left) return curr->left; //NULL returned
    return _rightmost_of(curr->left);
}

template <typename K, typename V>
typename Dictionary<K,V>::TreeNode*& Dictionary<K,V>::_rightmost_of(TreeNode*& curr) const {
    //return the rightmost node of the node curr
    //can do iteratively or recursively
    //iterative issue: need to keep track of a temp reference BUT reference CANNOT be reassigned in C++
    //wrapping by double ptr ** as in BinaryTree<T>::_create_from_list()
    if(!curr) return curr; //error case, should be captured by _iop_of above, but we should consider calling this individually
    if(!(curr->right)) return curr; //base case, this is the rightmost node
    return _rightmost_of(curr->right); //recursive case
}

//swap the addr of node1 and node2, return that reference of addr of node2 at the position (reference) of node1
//the argument node1 and node2 themselves are not changed
//no matter the argument order, always return the addr content of the originally higher node of these two after swapping
//the higher node to the lower position, and lower node to higher position 
//THIS IMPLEMENTATION is EXTREMELY CONFUSING, be sure to read it multiple times
template <typename K, typename V>
typename Dictionary<K,V>::TreeNode*& Dictionary<K,V>::_swap_nodes(TreeNode*& node1, TreeNode*& node2) {
    //assume node1 and node2 are non-null
    //since node1 and node2 vars are ref, we need to make copy of them
    TreeNode* orig_node1 = node1;
    TreeNode* orig_node2 = node2;
    if(node1 -> left == node2) {
        //std::swap swaps the VALUES of the arguments, in this case, the piece of memory node1->right and node2->right points to
        std::swap(node1->right, node2->right); //right nodes are unrelated by ref, safe to swap by stdswap 
        node1->left = orig_node2->left; //node2 is changed by now, let's be safe
        orig_node2->left = node1;
        node1 = orig_node2;
        //WARNING, this assignment operation loses track of node2's CONTENT, unsafe to use arguments passed in at its caller
        return node1->left; //the location of node1 (to be removed)'s left is where the CONTENT ADDR of node1 resides
    } else if(node1->right == node2) {
        std::swap(node1->left, node2->left);
        node1->right = orig_node2->right;
        orig_node2->right = node1;
        node1 = orig_node2;
        return node1->right;
    } else if(node2->left == node1) {
        std::swap(node1->right, node2->right);
        node2->left = orig_node1->left;
        orig_node1->left = node2;
        node2 = orig_node1;
        return node2->left;
    } else if(node2->right == node1) {
        std::swap(node1->right, node2->right);
        node2->right = orig_node1->right;
        orig_node1->right = node2;
        node2 = orig_node1;
        return node2->right;
    } else {
        //if node1 and node2 are not adjacent, just return the second node's position (ref)
        std::swap(node1->left, node2->left);
        std::swap(node1->right, node2->right);
        std::swap(node1, node2);
        return node2;
    }
}

template <typename K, typename V>
bool Dictionary<K,V>::empty() const {
    return !root_; //if root==NULL, it's empty, else NOT
}

template <typename K, typename V>
void Dictionary<K,V>::printTree() {
    _printTreeInOrder(root_);
}

template <typename K, typename V>
void Dictionary<K,V>::_printTreeInOrder(TreeNode* curr) {
    if(!curr) {//every time, when the tree traversal hits a node, it must hit a NULL ptr before that node
        std::cout << " "; 
        return;
    }
    _printTreeInOrder(curr->left);
    std::cout << "[" << curr->key << " : " << curr->value << "]";
    _printTreeInOrder(curr->right);
}

template <typename K, typename V>
int Dictionary<K,V>::height() const {
    return _height(root_);
}

template <typename K, typename V>
int Dictionary<K,V>::_height(TreeNode* curr) const {
    if(!curr) return -1;
    return 1 + std::max(_height(curr->left), _height(curr->right));
}

template <typename K, typename V> 
Dictionary<K,V>::~Dictionary() {
    std::cout << "Invoking dtor of Dictionary()\n";
    _clear();
}

template <typename K, typename V>
void Dictionary<K,V>::_clear() {
    if(!root_) return;
    _clearupNode(root_); //invoking the recursive helper on root_
    root_ = NULL;
}

template <typename K, typename V>
void Dictionary<K,V>::_clearupNode(TreeNode* curr) {
    if(!curr) return;
    _clearupNode(curr->left);
    _clearupNode(curr->right);
    curr->left = NULL; //assume free on curr->left happens, renull it so wont be visiting again
    curr->right = NULL; //assume free on curr->right happens, renull it so wont be visiting again

    //K,V data resided in TreeNode* curr, after curr is freed the data still linger in the heap, may clean up the board
    //C++ implicitly inserts using S = S<T>; into the class body of S (sizeof(S) outside of class body of S error)
    memset(curr, 0, sizeof(TreeNode));
    delete curr;
}