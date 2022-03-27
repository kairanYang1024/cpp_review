/*
 stack ADT: (LIFO last in first out) data structure
 create: empty stack
 push: add to top of stack
 pop: remove from top of stack (only one exit)
 empty: true if container empty
*/

#include <iostream>
#include <vector>
#include <stack>
//usually we use std::deque to combine functionalities of stack and queue
int main() {
    std::stack<std::string> s;
    s.push("a");
    s.push("b");
    s.push("mommy");
    //peek()
    std::cout << "first pop: " << s.top() << "\n"; //a
    s.pop(); //void pop
    s.push("uwu");
    std::cout << "second pop: " << s.top() << "\n"; //b

    /*
        array based: store tracker at which element (water level) at.
        expand when reaching index 0 -- amortize works

        list based: do addFront and popFront to match stack behavior
        all ops are O(1)
    */
   return 0;
}