#include "List.h"
#include <iostream>
#include <stack>
//hpp file still constitutes as part of List.h header but using cpp syntax, not appeared in g++ compile list
//resolves linker errors involving generics and nest
using std::cout;
using std::stack;

template<typename T>
List<T>::List() { //default ctor, nothing
    head_ = NULL;
}

template<typename T>
List<T>::~List() { //recycle all the node data
    _clear();
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& rhs) {
    _clear(); //clear old data for this instance
    ListNode* curr = rhs.head_;
    stack<T> temp; //use a helper container to get the copy done in right order
    while(curr) {
        temp.push(curr->data);
        curr = curr->next;
    }
    while(!temp.empty()) {
        addFront(temp.top()); //copy the data of rhs into this list instance as ListNodes
        temp.pop();
    }
    return *this;
}

template<typename T>
List<T>::List(const List<T>& other) : List() { //invoke an empty list
    *this = other; //using the overloaded assignment op '=' to save work
}

template<typename T>
T& List<T>::operator[](unsigned index) { //access time linear O(n)
    ListNode* curr = head_;
    while(index > 0 && curr->next) { //ptr != 0 resolves to true, ==0 resolves to false
        curr = curr->next;
        index --;
    }
    return curr->data;
}

//if passed as const List<T>&, only this version is accessible
template<typename T>
const T& List<T>::operator[](unsigned index) const { //access time linear O(n)
    ListNode* curr = head_;
    while(index > 0 && curr->next) { 
        curr = curr->next;
        index --;
    }
    return curr->data;
}

template<typename T>
bool List<T>::equals(const List<T>& other) const { //check in O(n)
    //const ListNode* p: the pointer is pointing to a value that shouldn’t be changed, equivalent to ListNode const* p
    //so it can still change its reference
    //if it goes like ListNode* const p: constant pointer to ListNode, p cannot point to another Node
    //if it goes like const ListNode* const p, it's a double lock
    const ListNode* thisCurr = head_;
    const ListNode* otherCurr = other.head_;
    while(thisCurr) {
        if(!otherCurr) {
            return false; //other is shorter than this
        }
        if(thisCurr->data != otherCurr->data) { //require != operator impl'd for class T
            return false;
        }
        thisCurr = thisCurr->next; //only changing reference is OK, not overwriting 
        otherCurr = otherCurr->next;
    }
    //if they are same length, after thisCurr iterates over, otherCurr should be NULL.
    //second statement false if otherCurr is longer
    return (thisCurr == NULL) && (otherCurr == NULL);
}

template<typename T>
void List<T>::addFront(const T& elem) { //linked list capacity is 'unlimited' unlike array having fixed size
    ListNode* newnode = new ListNode(elem);
    newnode->next = head_; //link old head (can be NULL) to new node
    head_ = newnode; //reupdate head reference ptr
}

template<typename T>
void List<T>::popFront() {
    ListNode* dead = head_;
    if(head_) head_ = head_ -> next;
    delete dead;
    dead = NULL;
}

//friend function overload operator <<, escape the problem of inner class and linker if directly declaring friend <<
template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& list) {
  return list.print(os);
}

template <typename T>
std::ostream& List<T>::print(std::ostream& os) const {
    // List format will be [(1)(2)(3)], etc.
    os << "[";
    // Note that this works correctly for an empty list.
    ListNode* cur = head_;
    while (cur) {
        os << "(" << cur->data << ")";
        cur = cur->next;
    }
    os << "]";
    return os;
}

//iterator-related functions
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
    //initialize the sentinel as head ptr
    return List<T>::ListIterator(head_);
}

template <typename T>
typename List<T>::ListIterator List<T>::end() const {
    //initialize the sentinel as one pass the end, which is NULL
    return List<T>::ListIterator(NULL);
}



//private functions:
//use typename to specify type of ListNode* under List<T> enclosing type
template<typename T>
typename List<T>::ListNode* List<T>::_find(const T& data) { 
    ListNode* curr = head_;
    while(curr) {
        if(curr->data == data) { //require operator== impled
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

template<typename T>
void List<T>::_clear() {
    while(head_) {
        popFront();
    } 
}