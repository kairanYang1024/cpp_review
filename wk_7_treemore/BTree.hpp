#include "BTree.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <utility>

using std::vector;
using std::string;

template <typename K>
BTree<K>::BTree(unsigned order) : root_(NULL) {
    if(order <= 2) {
        std::cerr << "Warning: the 'order' argument has invalid value, adjusted to default value 3.\n";
        order_ = 3;
    } else {
        order_ = order;
    }
}

template <typename K>
BTree<K>::BTree(const std::vector<K>& source_list, unsigned order) : BTree(order) {
    //should build a BTree from the source_list, do not assume source_list is sorted
    for(const K& elem : source_list) {
        insert(elem);
    }
}

template <typename K>
BTree<K>::~BTree() {
    std::cout << "dtor called for BTree.\n";
    _clear();
}

template <typename K>
unsigned BTree<K>::order() const {
    return order_;
}

template <typename K>
int BTree<K>::height() const {
    return _height(root_);
}

template <typename K>
int BTree<K>::_height(BTreeNode* curr) const {
    if(!curr) return -1;
    return curr->height_;
}

template <typename K>
void BTree<K>::printTree() {
    _printTreeInOrder(root_);
}

//in-order means to call the left, print curr, then call right
//in this setting of multiple children, it means call left child, print current element, then call right child
//iteratively for all keys inside curr
template <typename K>
void BTree<K>::_printTreeInOrder(BTreeNode* curr) {
    if(!curr) return;
    if(curr->keycount_ == 0) {
        std::cout << " ";
        return;
    }
    vector<K>& keys = curr->keys_;
    vector<BTreeNode*>& children = curr->children;
    for(unsigned i = 0; i < curr->keycount_; i++) {
        _printTreeInOrder(children[i]);
        std::cout << keys[i] << " ";
    }
    _printTreeInOrder(children[curr->keycount_]);
}


template <typename K>
bool BTree<K>::contains(const K& key) {
    return _contains(root_, key);
}

template <typename K>
bool BTree<K>::_contains(BTreeNode*& node, const K& key) {
    if(!node) throw std::runtime_error("Error in contains(node, key): node is nullptr");
    
    //perform linear search on keys of the node, assume keys inserted in ascending order
    unsigned i = 0;
    vector<K>& keys = node->keys_;
    vector<BTreeNode*>& children = node->children;
    //this loop locate the key's relative index in the keys array, even if not finding key at this layer, it quickly
    //locate the correct child ptr to dive into the deeper level
    for(i; i < node->keycount_ && keys[i] <= key; i++) {
        if(i < node->keycount_ && key == keys[i]) {
            return true;
        }
    }
    if(node->isLeaf()) {
        return false; //haven't find the key even in the leaf node, consider giving up
    } else { //children[i] is the left adjacent child of keys[i], and children[keycount_] rightmost child
        return _contains(children[i], key);
    }
}

template <typename K>
void BTree<K>::insert(const K& key) {
    //edge case: default ctor does not create node on heap, do it here
    if(!root_) root_ = new BTreeNode(); 

    _find_insert(root_, key);

    // checking whether root_ has maxed capacity or not, if so create a new node and modify tree structure
    if(root_->keycount_ >= order()) {
        BTreeNode* newroot = new BTreeNode();
        newroot->height_ = root_->height_+1;
        newroot->children.push_back(root_);
        _split_child(newroot, 0); //0th children (thus the root_ node) need to be splited
        root_ = newroot; //update the root ptr
    }
}

template <typename K>
void BTree<K>::_find_insert(BTreeNode*& node, const K& key) {
    if(!node) throw std::runtime_error("Error in _find_insert(node, key): node is nullptr");
    
    unsigned i = 0;
    vector<K>& keys = node->keys_;
    vector<BTreeNode*>& children = node->children;
    for(i; i < node->keycount_ && keys[i] < key; i++) {
        if(i < node->keycount_ && key == keys[i]) {
            std::cout << "Warning: key already existed in this tree, insert() failed, return to the caller\n";
            return;
        }
    }

    if(node->isLeaf()) {
        keys.insert(keys.begin() + i, key);
        //add the front and end child pointers if the node is initially empty
        if(node->keycount_ == 0) {  
            //add the front ptr, this happened only once in the entire lifecycle of BTree: when it only has an empty node!
            children.push_back(NULL);
        } 
        //normally, add the new ptr to be at right of new key value
        children.insert(children.begin() + i + 1, NULL);
        node->keycount_ ++;

    } else { //children[i] is the left adjacent child of keys[i], and children[keycount_] rightmost child
        _find_insert(children[i], key);
        //after insertion, if the child has maxed its capacity, call split_child
        //split node if capacity is maxed, this recursive check effectively cascade upward node update
        if(node->children[i]->keycount_ >= order()) { 
            _split_child(node, i);
        } 
    }
}

//split the child at split_child_idx of parent's children node ptr vector
//also, promote the middle element of parent->children[split_child_idx] to the parent node
//not recursive, this version isolates the "parent creation" and child split (moved to insert())
template <typename K>
void BTree<K>::_split_child(BTreeNode*& parent, unsigned split_child_idx) {
    if(!parent) throw std::runtime_error("Error in _split_child(): parent is nullptr");
    // here, keycount is equal to order() = m, size of children is equal to m+1
    vector<K>& parent_keys = parent->keys_;
    vector<BTreeNode*>& parent_children = parent->children;
    BTreeNode*& tobeSplit = parent->children[split_child_idx];
    if(!tobeSplit) throw std::runtime_error("Error in _split_child(): child at split_idx is nullptr");
    vector<K>& keys = tobeSplit-> keys_;

    // set middle key to be the middle of the arrays if m odd, right-middle of the array is m even
    K middle_key = keys[order() / 2];
    //erase it from the list of keys
    keys.erase(keys.begin() + order()/2);
    tobeSplit->keycount_ --;
    // promoting the middle key to that parent node
    parent_keys.insert(parent_keys.begin() + split_child_idx, middle_key);
    parent->keycount_ ++;
    
    BTreeNode* neighbor = new BTreeNode();
    //update the height to be as same as tobeSplit's height, its children preserves so the new node is not leaf
    //Also, since it is a copy of the current node's right half, it should have same height as the current node
    neighbor->height_ = tobeSplit->height_; 
    _fill_right(tobeSplit, neighbor);
    //insert the new treenode to the right of split_child_idx
    parent_children.insert(parent_children.begin() + split_child_idx + 1, neighbor); 
    
} 

//populate the newRight node to have content and children ptrs of tobeSplit's right half removing
template <typename K>
void BTree<K>::_fill_right(BTreeNode*& tobeSplit, BTreeNode*& neighbor) {
    vector<K>& keys = tobeSplit->keys_;
    vector<BTreeNode*>& children = tobeSplit->children;

    vector<K>& neighbor_keys = neighbor->keys_;
    vector<BTreeNode*>& neighbor_children = neighbor->children;

    //since new node is created, depopulate the right half to a new vector
    for(unsigned k = order()/2+1; k < order(); k++) {
        //ensure orderly removed and add the keys
        K depop = keys[order()/2];
        keys.erase(keys.begin() + order()/2); //sequentially remove the next elem in the right half of list
        neighbor_keys.push_back(depop);
        tobeSplit->keycount_--;
        neighbor->keycount_++;

        //also move the children ptrs of the right half, start from order()/2+1 all the way to order()
        BTreeNode* adopted = children[order()/2+1];
        children.erase(children.begin() + order()/2+1);
        neighbor_children.push_back(adopted);
        if(k == order() - 1){//dont forget the last one
            BTreeNode*& last_kid = children.back();
            children.pop_back();
            neighbor_children.push_back(last_kid);
        }
    }
}

template <typename K>
void BTree<K>::_clear() {
    if(!root_) return;
    _clearNode(root_);
    root_ = NULL;
}

template <typename K>
void BTree<K>::_clearNode(BTreeNode* curr) {
    if(!curr) return;
    vector<BTreeNode*>& children = curr->children;
    for(unsigned i = 0; i <= curr->keycount_; i++) {
        _clearNode(children[i]);
    }
    //clear the current node, assume each child node is already freed (post-order)
    for(unsigned i = 0; i <= curr->keycount_; i++) {
        children[i] = NULL;
    }
    delete curr;
}