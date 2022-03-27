#include <iostream>
#include <cstdlib>
#include <vector>
#include "List.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;


int main() {
    List<int> mylist;
    cout << "Insert elements at front\n";
    mylist.addFront(3);
    mylist.addFront(4);
    mylist.addFront(5);
    mylist.addFront(6);
    mylist.addFront(7);

    //need iterator to for-loop list body, need accessor to for-loop in caller prgms
    cout << "first elem: " << mylist[0] << "\n";
    cout << "2nd elem: " << mylist[1] << "\n";
    cout << "entire list: " << mylist << "\n";
    mylist.popFront();
    cout << "first elem changed: " << mylist[0] << "\n";
    cout << "entire list: " << mylist << "\n";

    List<int> secondlist = mylist; //test copy ctor
    cout << "entire second list: " << secondlist << "\n";
    cout << "they are the 'same' after copy ctor: " << (mylist == secondlist) << "\n";
    secondlist.popFront();
    cout << "and now they are not the same: " << mylist << " " << secondlist << " " << (mylist == secondlist) << "\n";

    //arbitrary position add/removal test:
    mylist.add(0, 7);
    mylist.add(1, 8);
    mylist.pop(1);
    mylist.add(5, 2);
    mylist.add(8, 1); //no-op
    mylist.pop(8); //no-op
    cout << "full list: " << mylist << "\n";
    mylist.pop(0);
    mylist.pop(4);

    //iterator implementation: allow us to do for-each loop
    //iterator points to the data like a pointer, have access operator *
    //iterator advances to the next data (& recede to the previous), have increment op ++ /decrement op --
    //iterator need to check which location it lands on: !=, ==
    //begin() and end(): ctor of iterators in their associated classes, begin() init to the beginning of the list,
    //end() inits to one past the end of list
    cout << "iterator works successfully by printing the content of node: ";
    for(auto it = mylist.begin(); it != mylist.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\n";
    cout << "testing for-each loop: ";
    for(const int& x : mylist) {
        cout << x << " ";
    }
    cout << "\n";
    for(int i = 0; i < 10; i++) {
        cout << "list removal: " << mylist << "\n";
        mylist.pop(0);
    }
    cout << "Finished\n";
    return 0;
}