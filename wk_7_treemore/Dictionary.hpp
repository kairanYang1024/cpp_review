#include "Dictionary.h"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <cstring>

//building the AVL tree from an unorderd map
template <typename K, typename V>
Dictionary<K,V>::Dictionary(const std::unordered_map<K,V>& source_map) : Dictionary() {
    for(auto i = source_map.begin(); i != source_map.end(); ++i) { //using iterator on the map
        insert(i->first, i->second); //iterator i has access to data pair<K,V>, i->first is key, i->second is value
    }
}


template <typename K, typename V>
V& Dictionary<K,V>::find(const K& key) {
    TreeNode*& result = _find(key, root_);
    if(!result) throw std::runtime_error("Key not found"); //included in stdexcept 
    return result->value;
}

template <typename K, typename V> //read only version of find()
const V& Dictionary<K,V>::find(const K& key) const {
    TreeNode*& result = _find(key, root_);
    if(!result) throw std::runtime_error("Key not found"); 
    return result->value;
}

template <typename K, typename V>
bool Dictionary<K,V>::contains(const K& key) {
    TreeNode*& result = _find(key, root_);
    return result != NULL; //or return result, however would seem to be violating type 
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
    //invoking helper _find_insert();
    _find_insert(key, value, root_);
}

template <typename K, typename V>
void Dictionary<K,V>::_find_insert(const K& key, const V& value, TreeNode*& node) {
    if(!node) { 
        //we create the node as a leaf with height 0, insert on the correct position in the tree according to order
        //leaf node is balanced, no need to check its balance
        node = new TreeNode(key, value);
    } else if(key == node->key) {
        std::cout << "Key " << key << " already existed, updating value...\n";
        node->value = value; //require type V has deep-copy operator=
    } else if(key < node->key) {
        _find_insert(key, value, node->left);
        // propagate upstream to 1) update height of the current node
        // 2) check whether the node is out of balance after insertion
        _ensureBalance(node);  
    } else {
        _find_insert(key, value, node->right);
        // propagate upstream to 1) update height of the current node
        // 2) check whether the node is out of balance after insertion
        _ensureBalance(node);
    }
}

// return a copy of the value once it has been removed from tree structure, require 3 copies to get to caller
// if V is very big, suggestion: keep a reference of V instead of real copy
template <typename K, typename V>
V Dictionary<K,V>::remove(const K& key) { //this copies a lot of V, might be slow if V has lot of data
    V retval = _find_remove(key, root_);
    return retval;
}

template <typename K, typename V>
V Dictionary<K,V>::_find_remove(const K& key, TreeNode*& curr) { //this copies a lot of V, might be slow if V has lot of data
    if(!curr) throw std::runtime_error("error in _find_remove(): key not found");
    else if(key == curr->key) return _remove(curr);
    else if(key < curr->key) {
        V retval = _find_remove(key, curr->left);
        _ensureBalance(curr); //a node has been removed, do rotation and update height
        return retval;
    } else {
        V retval = _find_remove(key, curr->right);
        _ensureBalance(curr); //a node has been removed, do rotation and update height
        return retval;
    }
}

template <typename K, typename V>
V Dictionary<K,V>::_remove(TreeNode*& node) { //pass by reference to actually modify the ptr
    if(!node) throw std::runtime_error("error in _remove(): key not found");
    if(!node->left && !node->right) { //zero-child remove, base case
        V data = node->value; //copy number 1
        delete node;
        node = NULL;
        return data; //copy number 2
    } else if(!node->left) { //only has right node
        TreeNode* temp = node;
        node = node->right; 
        V data = temp->value; //copy number 1
        delete temp;
        temp = NULL;
        return data; //copy number 2
    } else if(!node->right) { //only has left node
        TreeNode* temp = node;
        node = node->left;
        V data = temp->value; //copy number 1
        delete temp;
        temp = NULL;
        return data; //copy number 2
    } else {
        return _iop_remove(node);
    }
}

//find the iop of the node argument, swap the addr of node and iop, remove node and making iop at location of node argument
template <typename K, typename V>
V Dictionary<K,V>::_iop_remove(TreeNode*& node) {
    if(!node) throw std::runtime_error("error in _iop_remove(): target node is null.");
    V retval = _iop_remove(node, node->left, true);
    //propagate upward height (since a node inside the left subtree is deleted) and possibly balance
    if(node->left) _ensureBalance(node->left);
    _ensureBalance(node);
    return retval;
}

/**
 * @brief recursively tracking the node, the node's current "iop ancestor" while traversing the tree, 
 *  and keep track on node adjacency. Once found the node, delete it and update heights & balance factor of nodes
 *  along the path.
 * 
 * @tparam K  key type of Dictionary
 * @tparam V  value type of Dictionary
 * @param target  the node to be removed
 * @param iopAncestor the current node that is in the lineage of finding "target"'s IOP
 * @param iopAdjacent whether the IOP found is adjacent to "target" or not
 * @return V the value to be removed from the tree
 */
template <typename K, typename V>
V Dictionary<K,V>::_iop_remove(TreeNode*& target, TreeNode*& iopAncestor, bool iopAdjacent) {
    if (!target) throw std::runtime_error("ERROR: _iopRemove(TreeNode*& target, TreeNode*& iopAncestor): target is null");
    if (!iopAncestor) throw std::runtime_error("ERROR: _iopRemove(TreeNode*& targetNode, TreeNode*& iopAncestor): iopAncestor is null");
    if(!(iopAncestor->right)) { //base case, we found IOP
        //swap target to position of iop and iop to position of target
        TreeNode*& moved_iop = _swap_nodes(target, iopAncestor); 
        return _remove(moved_iop);
    } else { //recursive case, not found IOP, but value is catched after call
        V retval = _iop_remove(target, iopAncestor->right, false);
        if(!iopAdjacent){//if in the callstack of further recursions (target->left->right->...)
            if(iopAncestor) { 
                /** if the iopAncestor reference still exist (extra check on safety, 
                 * should be catched by multiple condtions along the callstack)
                 * 
                 * if things worked correctly, the IOP itself should be at the original place of removal detected
                 * update its height, and possibly call balance.
                 * 
                 * the nodes along the way to find iop: since a node is deleted along the path, should update height
                 * of any node that involves in this path. 
                 * 
                 * if iopAdjacent is true, that case height update and catching balance is in 
                 * if(node->left) _ensureBalance(node->left); in the previous overload of this function.
                 * **/
                _ensureBalance(iopAncestor);
            }
        }
        return retval;
    }
}

//swap the addr of node1 and node2, return that reference of addr of node2 at the position (reference) of node1
//the argument node1 and node2 themselves are not changed
//no matter the argument order, always return the addr content of the originally higher node of these two after swapping
//the higher node to the lower position, and lower node to higher position 
//However, argument order DOES matter if node1 and node2 are not adjacent, in this case, node1 always higher than node2 when passing in
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
    } else { //if node1 and node2 are not adjacent, just return the second node's position (ref)
        std::swap(node1->left, node2->left);
        std::swap(node1->right, node2->right);
        std::swap(node1, node2);
        return node2;
    }
}

//master program for deciding which type of unbalancing the node has.
//also, update the height of curr after balancing AND/OR insertion
template <typename K, typename V>
void Dictionary<K,V>::_ensureBalance(TreeNode*& curr) { 
    if(!curr) return; //no-op if run on a NULL node

    int balance_factor = _balance_factor(curr);
    if(balance_factor < -2 || balance_factor > 2) { //edge-case checking
        std::cerr << "Node [" << curr->key << "] has a balance factor of " << balance_factor << "\n"; 
        std::cerr << "Its left sub-branch has height " << _height(curr->left) << ", right sub-branch has height "
        << _height(curr->right) << "\n";
        throw std::runtime_error("Not a single node should have a balance factor magnitude greater than 2, stop.");
    }

    if(balance_factor == 2) { //the node is heavy on right 
        int right_balance = _balance_factor(curr->right);
        if(right_balance == 1 || right_balance == 0) { //"stick" rotation 1
            _rotateLeft(curr);
        } else if(right_balance == -1) { //"elbow" rotation 3
            _rotateRightLeft(curr);
        } else {  
            //unexpected value, if the child node still has balance factor >= 2 or <= -2
            //it means the balance for that child is not done properly
            std::cerr << "Node [" << curr->key << "] has a RIGHT balance factor of " << right_balance << "\n";
            std::cerr << "Its right node: [" << curr->right->key << "]\n"; 
            throw std::runtime_error("Not a single node lower in subtree should have a balance factor magnitude greater than or equal to 2, stop.");
        }
    } else if(balance_factor == -2) { //the node is heavy on left
        int left_balance = _balance_factor(curr->left);
        if(left_balance == -1 || left_balance == 0) { // "stick" rotation 2
            _rotateRight(curr);
        } else if(left_balance == 1) {
            _rotateLeftRight(curr);
        } else {
            //unexpected value, if the child node still has balance factor >= 2 or <= -2
            //it means the balance for that child is not done properly
            std::cerr << "Node [" << curr->key << "] has a LEFT balance factor of " << left_balance << "\n";
            std::cerr << "Its left node: [" << curr->left->key << "]\n"; 
            throw std::runtime_error("Not a single node lower in subtree should have a balance factor magnitude greater than or equal to 2, stop.");
        }
    }
    //update the current node's height after balancing it, for sure
    //even though sometimes none of these balancing are called, we still update the height after inserting a node
    //which needs to be propagate through the upstream nodes all the way to the root
    _update_height(curr);
    int final_balance = _balance_factor(curr);
    if(final_balance < -1 || final_balance > 1) {
        std::cerr << "Balance factor " << final_balance << " is invalid.\n";
        throw std::runtime_error("Invalid balance factor after balancing taken place.");
    }
}


//rotations are local operations (based on an argument of TreeNode*)
//rotations does not impact the overall tree structure (but the subtree rooted at the argument)
//rotations run in O(1) time: reassigning ptrs without loop

//rotation type I
template <typename K, typename V>
void Dictionary<K,V>::_rotateLeft(TreeNode*& curr) { 
    if(!curr) throw std::runtime_error("ERROR: node argument (parent) of _rotateLeft() is NULL");
    if(!curr->right)  throw std::runtime_error("ERROR: right child (main node) is NULL");
    //we modify the node locally, alter the subtree structure, then rewire back to the tree structure
    //the ptr copy meant to be like creating a sub-environment to accomodate the node changes
    TreeNode* parent = curr;
    TreeNode* main = curr->right;
    TreeNode* adopt = curr->right->left; //the node originally left child of "main" now right child of "parent"
    parent->right = adopt;
    main->left = parent;
    curr = main; //rewire the reference to the memory addr of node that has its child ptrs altered
    _update_height(parent); //now "parent" is low in height, update its height (from 3 to 1 in the most basic setting)
    _update_height(main); //"main" is the highest node in this subtree, update its height dep on h(parent)
    //remember, "adopt"'s height remain invariant in this setting (the subtree "adopt" itself is not changed)
    //height of parent then depend on height of adopt (right child changed), and height of main dep on height of parent (left child change)
}

//rotation type II
template <typename K, typename V>
void Dictionary<K,V>::_rotateRight(TreeNode*& curr) { //mirrors _rotateLeft()
    if(!curr) throw std::runtime_error("ERROR: node argument (parent) of _rotateRight() is NULL");
    if(!curr->left)  throw std::runtime_error("ERROR: left child (main node) is NULL");
    TreeNode* parent = curr;
    TreeNode* main = curr->left;
    TreeNode* adopt = curr->left->right;
    parent->left = adopt;
    main->right = parent;
    curr = main;
    _update_height(parent);
    _update_height(main);
}

//rotation type III
template <typename K, typename V>
void Dictionary<K,V>::_rotateRightLeft(TreeNode*& curr) { //a sequence of rotate right then left
    if(!curr) throw std::runtime_error("ERROR: node argument (parent) of _rotateRightLeft() is NULL");
    _rotateRight(curr->right); //after the first rotation the unbalance factor of curr->right might not be satisfying
    _rotateLeft(curr); //but it will be after the second rotation
}

//rotation type IV
template <typename K, typename V>
void Dictionary<K,V>::_rotateLeftRight(TreeNode*& curr) { //a sequence of rotate left then right
    if(!curr) throw std::runtime_error("ERROR: node argument (parent) of _rotateLeftRight() is NULL");
    _rotateLeft(curr->left);
    _rotateRight(curr);
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
    std::cout << "[" << curr->key << ":" << curr->value << "]";
    _printTreeInOrder(curr->right);
}

template <typename K, typename V>
int Dictionary<K,V>::height() const {
    return _height(root_);
}

template <typename K, typename V>
int Dictionary<K,V>::_height(TreeNode* curr) const { //DP way to calculate height stored in the individual nodes
    if(!curr) return -1;
    return curr->height;
}

template <typename K, typename V>
void Dictionary<K,V>::_update_height(TreeNode*& curr) {
    //relies on height of the children nodes, not recursive, might be unsafe, like the size() function of linkedlist
    if(!curr) return; //no-op
    curr->height = 1 + std::max(_height(curr->left), _height(curr->right));
}

template <typename K, typename V>
int Dictionary<K,V>::_balance_factor(TreeNode* curr) const {
    if(!curr) return 0; //trivial defintion: NULL node are balanced
    return _height(curr->right) - _height(curr->left);
}


template <typename K, typename V> 
Dictionary<K,V>::~Dictionary() {
    _clear();
}

template <typename K, typename V>
void Dictionary<K,V>::_clear() {
    if(!root_) return;
    _clearupNode(root_); 
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