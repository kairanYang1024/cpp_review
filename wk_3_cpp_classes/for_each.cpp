#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
int main() {
    vector<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);

    //for-each loop, the : operator is defined by iterator objs
    for(int x : arr) { //creates a copy each time when accessing elem of arr
        cout << "This item has value: " << x << endl;
        x = 114514; //does nothing in the arr
    }    

    for(int& x : arr) { //directly accessing elems of arr as names of x, r/wable
        x += 100;
        cout << "Modifying the item to value " << x << endl;
    }

    for(const int& x : arr) { //directly accessing elems of arr as names of x, rable only
        cout << "This item has value: " << x << " faster for not copying" << endl;
    }

    //unsigned numbers: don't do algebraic ops w/ signed for the sake of representation of bits
    //also, no comparisons like in arr.size(): unsigned long w/ int i = 0
}