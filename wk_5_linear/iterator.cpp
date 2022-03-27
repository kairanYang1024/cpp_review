#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main() {
    //iterator implementation: allow us to do for-each loop
    //iterator points to the data like a pointer, have access operator *
    //iterator advances to the next data (& recede to the previous), have increment op ++ /decrement op --
    //iterator need to check which location it lands on: !=, ==
    //begin() and end(): ctor of iterators in their associated classes, begin() init to the beginning of the list,
    //end() inits to one past the end of list
    vector<int> vlist = {1,2,3,4,5,6,7};
    //here we can see iterator is an public inner class of vector
    for(vector<int>::iterator it = vlist.begin(); it != vlist.end(); ++it) {
        cout << "we are at: " << (*it) << endl;
    }

    //auto-resolve: since init'd with function begin(), its type will be resolved so no need to explicitly declare
    for(auto it = vlist.begin(); it != vlist.end(); ++it) {
        cout << "we are still at: " << (*it) << endl;
    }
    return 0;
}