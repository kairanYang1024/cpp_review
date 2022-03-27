//abstract data type: ADT, without the implementation, jsut description
/*
    queue ADT: (FIFO = First in, first out)
    create --> create empty queue
    push --> adds data to back of queue
    pop --> removes data from front of queue
    empty --> return true if queue is empty
*/

#include <iostream>
#include <vector>
#include <queue>

int main() {
    std::queue<std::string> q;
    q.push("a");
    q.push("b");
    q.push("mommy");
    //peek()
    std::cout << "first pop: " << q.front() << "\n"; //a
    q.pop(); //void pop
    q.push("uwu");
    std::cout << "second pop: " << q.front() << "\n"; //b

    //implemntation: array-based and LList based
    //array-based: tracker at inserting index, pop at front or back of array
    //all in O(1)* amortized (including resize)

    //list-based: insert front O(1): add'em on the front
    //add tail pointer to track the last elem of list and serve as popping point
    //ensures llist linking memory backward as well (traverse backward enabled)
    //push O(1), pop O(1)

    //all op in a queue is constant operation regardless of capacity 

    return 0;
}