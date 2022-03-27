#pragma once

//linked memory stored data 'together' w/ link to the location of memory
//usually by 'ptrs'

//linked list: 0+ listnode elements linked together
//head pointer stores the link to beginning of list
//nullptr marks the end of the list

template <class T>
class List {
    private:
        // nested class are private by default
        // however, even private members of nested class are accessible by the enclosing class
        // the nested class has access to all names (private, protected, etc) to which the enclosing class has access, 
        // but it is otherwise independent and has no special access to the this pointer of the enclosing class.
        // Declarations in a nested class can use only type names, static members, and enumerators from the enclosing class. (safe)
        // after C++11 can use all members from enclosing, need to verify version first.
        // Inside the scope of the enclosing, i.e. List::ListNode()
        class ListNode { //ListNode: struct/class generic
            public:
                T data; //each ListNode should host a copy of the original data, not the reference of data
                ListNode* next;

                ListNode() : next(NULL) {} //let the data be constructed by the ctor of type T
                ListNode(const T& data) : data(data), next(NULL) {}

                // Copy constructor & assignment operator: Constructs/assigns this node to be identical to the other,
                // although this one does not exist at the same memory location as other (deep copy),
                // and so anything that referred to other by address is NOT automatically
                // referring to this node. (in other words, we need ListNode* a = b then change a does not auto change b, vice versa)
                ListNode(const ListNode& other) : data(other.data), next(other.next) {}
                ListNode& operator=(const ListNode& other) {
                    data = other.data;
                    next = other.next;
                    return *this;
                }

        }; //serves as a fwd declaration
        ListNode* head_;

        //have to be private since ListNode* type is inaccessible to outside world
        ListNode* _find(const T& data); //find the node that has the data specified by argument
        void _clear(); //helper for releasing memory of this data structure

    public: 
        //creating ctor/dtor for memory safety
        List();
        List(const List<T>& other);
        List<T>& operator=(const List<T>& rhs); //return a reference to avoid copying list when return
        ~List();

        T& operator[](unsigned index); //synonymous of get(), access time linear O(n)
        //const-correct version of [], if const List<T>& passed as argument, it's required to be const
        //const version of function prototypes are only allowed to be used in this 'read only' version
        const T& operator[](unsigned index) const; 

        void addFront(const T& elem); //using T& to not copying the data into the argument, extra performance
        void popFront(); //no return, just data modification

        //O(n) add/delete elements in the list
        void add(unsigned index, const T& elem); //add(index, elem) means letting elem to be the index-th node of the list
        void pop(unsigned index); //pop(index) means deleting the index'th element in the list

        // Two lists are equal if they have the same length and the same data items in each position.
        // This check runs in O(n) time.
        bool equals(const List<T>& other) const;
        bool operator==(const List<T>& other) const {
            return equals(other);
        }
        bool operator!=(const List<T>& other) const {
            return !equals(other);
        }

        // Output a string representation of the list.
        // This requires that the data type T supports stream output itself.
        // This is used by the operator<< overload defined in this file.
        std::ostream& print(std::ostream& os) const;

        //iterator of the list, extend from std::iterator interface
        class ListIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            //iterator implementation: allow us to do for-each loop
            //iterator points to the data like a pointer, have access operator *
            //iterator advances to the next data (& recede to the previous), have increment op ++ /decrement op --
            //iterator need to check which location it lands on: !=, ==
            private:
                //should have access to ListNode if have it "previously" declared
                ListNode* position_;
            public:
                ListIterator(): position_(NULL) {}
                ListIterator(ListNode* x) : position_(x) {}
                //dtor implicitly declared to extend its parent class

                ListIterator& operator++() { //pre-increment, returns the already-changed iter
                    position_ = position_->next;
                    return *this;
                }
                //we can also do post-increment but it's wasteful

                bool operator==(const ListIterator& rhs) {
                    //ListNode* default == compares the memory addr of position_, it suffices the iterator's use on it
                    return this->position_ == rhs.position_;
                }
                bool operator!=(const ListIterator& rhs) {
                    return ! (*this == rhs);
                }
                const T& operator*() { //accessor operator
                    return position_->data;
                }
                //about arrow operator and its 'recursive' property:
                //https://stackoverflow.com/questions/10677804/how-arrow-operator-overloading-works-internally-in-c
                //we are not going to implement a full list nor a full iterator so we leave it here
        };
        //instantiate iterators at 'beginning' and 'end' of the list as sentinels
        ListIterator begin() const;
        ListIterator end() const;

};

#include "List.hpp"  //do such to avoid linker error involving nested class and generics, exactly how it works remained unknown to me
