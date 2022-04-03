#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "BTree.h"

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::string;
using std::vector;

const int SMALL_TEST_SIZE = 16;
const unsigned MEDIUM_ORDER = 8;
const int MEDIUM_TEST_SIZE = 1024;
const unsigned LARGE_ORDER = 50;
const int LARGE_TEST_SIZE = 1000000;

int main() {
    BTree<int> small_tree;
    cout << "Testing adding (and balancing) in the small case.\n";
    for(int i = 0; i < SMALL_TEST_SIZE; i++) {
        small_tree.insert(i);
    } 
    cout << "Print the tree in order, the tree should be in order.\n";
    small_tree.printTree();
    cout << "\n";
    cout << "The tree should be perfectly balanced\n";
    cout << "height of tree " << small_tree.height() << "\n";
    cout << "\n";

    BTree<int> medium_tree(MEDIUM_ORDER);
    cout << "Testing adding (and balancing) in the medium case.\n";
    for(int i = 0; i < MEDIUM_TEST_SIZE; i++) {
        medium_tree.insert(i);
        if(medium_tree.contains(i) == false) {
            cout << "Error: cannot find the newly inserted value " << i << "\n";
            break;
        }
    } 
    cout << "Print the tree in order, the tree should be in order.\n";
    medium_tree.printTree();
    cout << "\n";
    cout << "The tree should be perfectly balanced\n";
    cout << "height of tree " << medium_tree.height() << "\n";
    cout << "\n";

    BTree<double> large_tree(LARGE_ORDER);
    cout << "Testing adding (and balancing) in the large (and non-integer) case.\n";
    srand(time(NULL));
    for(int i = 0; i < LARGE_TEST_SIZE; i++) {
        int rng = rand() % LARGE_TEST_SIZE;
        double rng_frac = rng / 100.0; 
        //maybe erroneous for rng value defined on double, maybe because of == precision
        large_tree.insert(rng_frac);
        if(large_tree.contains(rng_frac) == false) {
            cout << "Error: cannot find the newly inserted value " << rng_frac << "\n";
            break;
        }
    } 
    cout << "The tree should be perfectly balanced\n";
    cout << "height of tree " << large_tree.height() << "\n";
    cout << "\n";
    return 0;
}