#include <iostream>
#include <vector>
#include <string>
#include "BTree.h"

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::string;
using std::vector;

//TODO: implementation bugged
int main() {
    BTree<int> query_tree(8);
    cout << "Testing adding (and balancing) in the base case\n";
    const int TEST_SIZE = 64; //1 MB = 2^20 bytes, log(1000000) should be bounded by 20
    for(int i = 0; i < TEST_SIZE; i++) {
        query_tree.insert(i);
    } 

    cout << "The tree should be perfectly balanced\n";
    cout << "height of tree " << query_tree.height() << "\n";

    cout << "Print the tree in order, the tree should be in order\n";
    query_tree.printTree();
    cout << "\n";

    return 0;
}