#include <iostream>
#include <vector>
#include <string>
#include "BinaryTree.h"
#include "Dictionary.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

int main() {
    vector<string> words = {"I", "you", "he", "she", "it", "wumbo", "wumbology", "spongebob"};
    BinaryTree<string> wordTree(words);
    cout << "Testing basic functions: \n";
    cout << "this tree has height: " << wordTree.height() << "\n";

    vector<string> testers = {"I", "yuo", "heh", "she", "it", "mumbo", "mumbology", "spongebob", "patrick", "squidward"};
    for(string& s : testers) {
        if(wordTree.hasElem(s)) cout << "this tree has the word " << s << "\n";
        else cout <<  "this tree doesn't have the word " << s << "\n";
    }

    cout << "\nTesting traversals: \n";
    cout << "Preorder traversal: ";
    wordTree.preorder();
    cout << "\n";
    cout << "Inorder traversal: ";
    wordTree.inorder();
    cout << "\n";
    cout << "Postorder traversal: ";
    wordTree.postorder();
    cout << "\n";
    cout << "Level-order traversal: ";
    wordTree.levelorder();
    cout << "\n";

    cout << "\nTesting copy ctor and operator= of BinaryTree<T> class\n";
    BinaryTree<string> word2 = wordTree;
    cout << "Preorder traversal: ";
    word2.preorder();
    cout << "\n";

    cout << "Reassigning word2 tree... \n";
    word2 = BinaryTree<string>(testers); //invoking dtor to destroy this impermanent right hand side object
    cout << "Preorder traversal: ";
    word2.preorder();
    cout << "\n";

    cout << "\nTesting plain dictionary (no balancing): \n";
    Dictionary<int, string> mymap;
    cout << "Test adding elements...\n";
    mymap.insert(37, "Abraca");
    mymap.insert(19, "Yessa");
    mymap.insert(20, "Drsd");
    mymap.insert(4, "Lella");
    mymap.insert(2, "qquqq");
    mymap.insert(11, "remimi");
    mymap.insert(51, "Zesta");
    mymap.insert(56, "Alpha");
    mymap.insert(56, "Estrella");
    mymap.insert(1, "Baby");
    cout << "Inorder traversal should maintain order:\n";
    mymap.printTree();
    cout << "\n";
    cout << "This tree has height of " << mymap.height() << ", so sad it's not perfectly balanced :(\n";

    cout << "Testing querying keys: \n";
    vector<int> queryvals = {56,37,51,19,20,11,1};
    for(int& i : queryvals) {
        cout << "key " << i << " has associated value " << mymap.find(i) <<"\n";
    }

    cout << "\nTesting key removals: \n";
    for(int& i : queryvals) {
        cout << "key " << i << " has been removed and valued returned: " << mymap.remove(i) << "\n";
    }
    cout << "Inorder traversal should still maintain order:\n";
    mymap.printTree();
    cout << "\n";
    cout << "This tree has height of " << mymap.height() << ", so sad it's not perfectly balanced :(\n";
    cout << "Success\n";
    return 0;
}