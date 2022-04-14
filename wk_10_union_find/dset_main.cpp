#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <utility>
#include "DisjointSets.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::boolalpha;

#define SMALL_SIZE 40
#define LARGE_SIZE 1000000
#define STRLEN 5

string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

int main() {
    cout << " === testing small disjoint sets structure === " << endl;
    DisjointSets<int> smallsets;
    for(int i = 0; i < 10; i++) {
        smallsets.make_set(i);
    }
    cout << "Each newly created set has its root itself: \n";
    for(int i = 0; i < 10; i++) {
        cout << i << " 's root is " << smallsets.find(i) << " itself: " << boolalpha << (i == smallsets.find(i)) << endl;
    }
    cout << "This dsets structure should has size 10 and num_sets 10.\n";
    cout << "size: " << smallsets.size() << ", num_sets: " << smallsets.num_sets() << "\n"; 
    smallsets.print_uptree();
    cout << "Union the set into two factions, one small, one big.\n";
    for(int i = 0; i < 3; i++) {
        smallsets.unionize(i, i+1);
    }
    for(int i = 9; i > 4; i--) {
        smallsets.unionize(i, i-1);
    }
    cout << "This dsets structure should has size 10 and num_sets 2.\n";
    cout << "size: " << smallsets.size() << ", numsets: " << smallsets.num_sets() << "\n"; 
    smallsets.print_uptree();

    cout << "\n === testing small generic disjoint sets structure === " << endl;
    srand(time(NULL));
    vector<string> testvec;
    for(int i = 0; i < SMALL_SIZE; i++) {
        testvec.push_back(gen_random(STRLEN)); //push back random generated strings of length 5
    }
    DisjointSets<string> genset(testvec);
    cout << "This disjoint set has size: " << genset.size() << " and num_sets " << genset.num_sets() << "\n";
    cout << "Test random union:\n";
    for(int i = 0; i < SMALL_SIZE * 3 / 4; i++) {
        int ridx1 = rand() % SMALL_SIZE;
        int ridx2 = rand() % SMALL_SIZE;
        genset.unionize(testvec[ridx1], testvec[ridx2]);
    }
    cout << "The number of sets should be greatly reduced: " << genset.num_sets() << "\n";
    genset.print_uptree(); //because we union by size, we can see a great agglunating effect here
    cout << "Test random query:\n";
    for(int i = 0; i < SMALL_SIZE/4; i++) {
        int ridx = rand() % SMALL_SIZE;
        string rep_name = genset.find(testvec[ridx]);
        cout << "the string " << testvec[ridx] << " has representative " << rep_name << " in its set\n";
    }
    cout << "Testing printSets()\n";
    genset.print_sets();
    //big set test case omitted, not so useful and hard to write test cases (need time to develop)
    cout << "\nSuccess!\n";
    return 0;
}

