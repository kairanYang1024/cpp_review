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

template<typename T, typename K>
void buildedge_tcase1(Graph<T,K>& mygraph) {
    mygraph.insertEdge(0, 2, 1);
    mygraph.insertEdge(2, 1, 1);
    mygraph.insertEdge(2, 3, 1);
    mygraph.insertEdge(3, 4, 1);
    mygraph.insertEdge(1, 0, 1);
    mygraph.insertEdge(1, 4, 1);
}

template<typename T, typename K>
void buildedge_tcase2(Graph<T,K>& mygraph) {
    mygraph.insertEdge('A', 'B', 2);
    mygraph.insertEdge('A', 'D', 17);
    mygraph.insertEdge('A', 'F', 16);
    mygraph.insertEdge('B', 'C', 15);
    mygraph.insertEdge('B', 'D', 5);
    mygraph.insertEdge('C', 'D', 13);
    mygraph.insertEdge('C', 'E', 11);
    mygraph.insertEdge('C', 'H', 5);
    mygraph.insertEdge('D', 'E', 8);
    mygraph.insertEdge('D', 'F', 9);
    mygraph.insertEdge('E', 'F', 12);
    mygraph.insertEdge('E', 'G', 9);
    mygraph.insertEdge('E', 'H', 10);
    mygraph.insertEdge('F', 'G', 4);
    mygraph.insertEdge('G', 'H', 16);
}

int main() {
    cout << "=== Testing BFS on a specified graph ===" << endl;
    Graph<int, int> tinygra;
    for(int i = 0; i < 5; i++) {
        tinygra.insertVertex(i);
    }
    buildedge_tcase1(tinygra);
    cout << "tinygra should have 6 edges: " << tinygra.countEdges() << " and they are (not ignoring reverses): \n";
    tinygra.print_adjmap();
    cout << "tinygra should have only 1 connected component: " << countComponents(tinygra) << "\n";
    Graph<int, int> tinytree = bfs_sptree(tinygra);
    tinytree.print_vertexlist();
    cout << "tinytree should be a tree: " << boolalpha << (tinytree.countVertices() - 1 == tinytree.countEdges())
    << " where it has " << tinytree.countVertices() << " vertices and " << tinytree.countEdges() << " edges.\n";
    tinygra.removeEdge(3, 4);
    tinygra.removeVertex(2);
    cout << "tinygra should have 2 connected components now: " << countComponents(tinygra) << "\n";
    cout << "=== End BFS test ===\n";

    cout << "\n=== Testing MST on a specified graph === \n";
    vector<char> testlist;
    for(char c = 'A'; c < 'I'; c++) {
        testlist.push_back(c);
    }
    Graph<char, int> mtgra(testlist);
    buildedge_tcase2(mtgra);
    cout << "Kruskal algorithm should make a minimum-spanning tree out of a given (undirected) graph.\n";
    Graph<char, int> mttree = kruskalMST(mtgra);
    cout << "mttree has " << mttree.countVertices() << " vertices and " << mttree.countEdges() << " edges";
    cout << " and it is a tree: " << boolalpha << (mttree.countEdges() == mttree.countVertices() - 1) << "\n";
    cout << "mttree's weight should be minimal: " << mttree.sumEdgeWeights() << "\n";
    cout << "closer look:\n";
    mttree.print_adjmap();

    cout << "Prim's algorithm should make a minimum-spanning tree out of a given (undirected) graph.\n";
    Graph<char, int> mttree2 = primMST(mtgra, 'A');
    cout << "mttree has " << mttree2.countVertices() << " vertices and " << mttree2.countEdges() << " edges";
    cout << " and it is a tree: " << boolalpha << (mttree2.countEdges() == mttree2.countVertices() - 1) << "\n";
    cout << "mttree's weight should be minimal: " << mttree2.sumEdgeWeights() << "\n";
    cout << "closer look:\n";
    mttree2.print_adjmap();
    cout << "Prim's algorithm should be versatile in any vertex\n";
    mttree2 = primMST(mtgra, 'E');
    cout << "closer look:\n";
    mttree2.print_adjmap();
    cout << "Prim's algorithm should make a spanning tree with same weight of Kruskal's MST"
    <<  "(not nec same tree though): \n";
    cout <<  "Kruskal: " << mttree.sumEdgeWeights() << " , Prim: " << mttree2.sumEdgeWeights() << ".\n";
    cout << "and they are the same: " << boolalpha << (mttree.sumEdgeWeights() == mttree2.sumEdgeWeights()) << "\n";
    cout << "=== End MST test ===\n";

    cout << "\n=== Testing single-source shortest path algorithm on a specific graph === \n";
    for(char c = 'A'; c < 'I'; c++) {
        cout << "querying shortest path from " << c << " to A\n";
        Graph<char, int> shortestpath = dijkstraPath(mtgra, 'A', c);
        shortestpath.print_edgelist();
        cout << "path weight: " << dijkstraWeight(mtgra, 'A', c) << " and is equal to " 
        << shortestpath.sumEdgeWeights() << "\n";
    }
    cout << "=== End Dijkstra test ===\n";
    cout << "\nGrand Success!\n";
    return 0;
}