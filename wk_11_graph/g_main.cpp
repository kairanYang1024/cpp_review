#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include "Graph.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::boolalpha;
using std::unordered_set;
using std::unordered_map;

#define SMALL_SIZE 30
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

template<typename T>
void print_vector(const vector<T>& vec) {
    cout << "[";
    for(const T& elem : vec) {
        cout << " " << elem << " ";
    }
    cout << "]\n";
}

int main() {
    cout << "=== Testing tiny graph ===" << endl;
    Graph<int, int> tinygra;
    for(int i = 0; i < 5; i++) {
        tinygra.insertVertex(i);
    }
    cout << "tinygra should have 5 vertices: " << tinygra.countVertices() << " and 0 edges: " << tinygra.countEdges() << "\n";
    tinygra.print_all();
    tinygra.insertEdge(0, 2, 1);
    tinygra.insertEdge(2, 1, 1);
    tinygra.insertEdge(2, 3, 1);
    tinygra.insertEdge(3, 4, 1);
    tinygra.insertEdge(1, 0, 1);
    tinygra.insertEdge(1, 4, 1);
    cout << "tinygra should have 6 edges: " << tinygra.countEdges() << " and they are (not ignoring reverses): \n";
    tinygra.print_edgelist();
    cout << "closer look: \n";
    tinygra.print_adjmap();
    cout << "testing removing edge: \n";
    tinygra.removeEdge(3, 4);
    cout << "tinygra should remove edge (3,4) in both directions: \n";
    tinygra.print_adjmap();
    cout << "testing removing vertex: \n";
    tinygra.removeVertex(2);
    cout << "After removing vertex 2 there should be 4 vertices: " << tinygra.countVertices() 
    << " and 2 edges: " << tinygra.countEdges() << endl;
    cout << "closer look: \n";
    tinygra.print_all();
    cout << "Querying adjacent nodes and checking adjacency: \n";
    for(int i = 0; i < 5; i++) {
        if(i == 2) continue;
        cout << "vertex " << i << "'s adjacent nodes: ";
        print_vector(tinygra.adjacentNodes(i));
    }
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            cout << "node " << i << " and node " << j << " are adjacent? " << boolalpha << tinygra.areAdjacent(i, j) << ".\n";
        }
    }
    cout << "\n=== Testing small graph (random) ===" << endl;
    srand(time(NULL));
    vector<string> testvec;
    for(int i = 0; i < SMALL_SIZE; i++) {
        testvec.push_back(gen_random(STRLEN)); //push back random generated strings of length 5
    }
    Graph<string, int> smolgra(testvec);
    smolgra.print_all();
    cout << "Randomly adding edges: \n";
    for(int i = 0; i < 3*SMALL_SIZE; i++) {
        string node1 = testvec[rand() % SMALL_SIZE];
        string node2 = node1;
        while(node2.compare(node1) == 0) {
            node2 = testvec[rand() % SMALL_SIZE];
        }
        smolgra.insertEdge(node1, node2, rand() % 1000);
    }
    cout << "smolgra has " << smolgra.countVertices() << " vertices and " << smolgra.countEdges() << " edges.\n";
    cout << "Testing copy ctor and assignment op\n";
    Graph<string, int> smolgra2(smolgra);
    string ranstr = gen_random(STRLEN+1);
    smolgra2.insertVertex(ranstr);
    for(int i = 0; i < SMALL_SIZE; i++) {
        smolgra2.insertEdge(ranstr, testvec[i], i); //add to every other vertex in the new graph
    }
    cout << "smolgra has " << smolgra.countVertices() << " vertices and " << smolgra.countEdges() << " edges.\n";
    cout << "while smolgra2 has " << smolgra2.countVertices() << " vertices and " << smolgra2.countEdges() << " edges.\n";
    smolgra = smolgra2;
    cout << "smolgra now has " << smolgra.countVertices() << " vertices and " << smolgra.countEdges() << " edges.\n";
    cout << "and smolgra2 has " << smolgra2.countVertices() << " vertices and " << smolgra2.countEdges() << " edges.\n";
    cout << "Now they are the same UwU.\n";

    cout << "\nSuccess!\n";
    return 0;
}
