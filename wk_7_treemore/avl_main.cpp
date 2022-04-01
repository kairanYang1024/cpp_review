#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include "Dictionary.h" //this Dictionary.h from wk_7 folder (AVL-improved)

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::string;
using std::vector;
using std::unordered_map;

int main() {
    Dictionary<int, string> avl;
    cout << "Testing adding (and balancing)\n";
    const int TEST_SIZE = 1000000; //1 MB = 2^20 bytes, log(1000000) should be bounded by 20
    for(int i = 0; i < TEST_SIZE; i++) {
        avl.insert(i, std::to_string(i));
    } 

    cout << "The tree should be perfectly balanced\n";
    cout << "height of tree " << avl.height() << " should be less than 20\n";

    cout << "Testing finding, generating query values..." << "\n";
    srand(time(NULL));
    for(int i = 0; i < 35; i++) {
        int query = rand() % TEST_SIZE;
        cout << "Should have find query value " << query << " in tree: " << std::boolalpha <<
         avl.contains(query) << "\n";
        if(i < 10) {
            cout << "Value returning: " << avl.find(query) << "\n";
        } 
    }
    cout << "\nTesting small number of removals.\n";
    for(int i = 0; i < 10; i++) {
        int query = rand() % TEST_SIZE;
        cout << "Should have removed and returned query value " << query << " in tree: " << avl.remove(query) << "\n";
        cout << query << " is no more " << std::boolalpha << !avl.contains(query) << "\n";
        cout << "Re-add " << query << " for future testing...\n";
        avl.insert(query, std::to_string(query));
    }
    cout << "The tree still preserve its height after inserting back, avl.height(): " << avl.height() << "\n";
    cout << "\nTesting removing nodes in the tree...large query values loaded, expecting tree height to shrink.\n";
    for(int i = 1000; i < TEST_SIZE; i++) {
        avl.remove(i);
    }
    cout << "The tree shrinks to a balanced tree of height " << avl.height() << " (should be less than 10)\n";

    cout << "\nTesting constructing dictionary from unordered map\n";
    unordered_map<int, string> mappa;
    string tsukis[] {"Mutsuki", "Kisaragi", "Yayoi", "Uzuki", "Satsuki", "Minatsuki", "Fuzuki", "Hatsuki", "Nagatsuki"
    , "Kannatsuki", "Shimotsuki", "Shiwasu"};
    for(int i = 0; i < 12; i++) {
        mappa[i+1] = tsukis[i];
    }
    Dictionary<int, string> wafu_getsumei(mappa);
    cout << "The inorder traversal shall print the name of the Japanese months\n";
    wafu_getsumei.printTree();
    cout << "This tree should have a height of three: " << std::boolalpha << (wafu_getsumei.height() == 3) << "\n";
    wafu_getsumei.remove(3);
    wafu_getsumei.remove(9);
    try {
        wafu_getsumei.remove(3);
    } catch(std::runtime_error& e) {
        cerr << e.what() << " Error thrown from removing invalid key.\n";
    }
    try {
        wafu_getsumei.remove(0);
    } catch(std::runtime_error& e) {
        cerr << e.what() << " Error thrown from removing nonexistant key.\n";
    }
    wafu_getsumei.printTree();
    cout << "\nSuccess!\n";
    return 0;
}