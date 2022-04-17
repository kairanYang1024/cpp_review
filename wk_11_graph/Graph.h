/**
 * G = {V,E} V=set of vertices, E=set of edges
 * n = |V|, m = |E|
 * incident edge: (undirected) I(v) = {(x,v) in E}
 * degree(v) = |I(v)|
 * in directed graphs (digraphs), having in-degree and out-degree denoting # of input edges and outward edges
 * adjacent(v) = {x : (x,v) in E}
 * path: sequence of vertices connected by edges
 * cycle: path start from and end with the same node.
 * 
 * simple graph: no self-loop, no multi edges (simplified to the same edge connecting two nodes) (A) -- (B)
 * subgraph: G' = (V', E') s.t. V' subset of V, E' subset of E, and (u,v) in E' --> u in V' and v in V'.
 * 
 * the minimum edges required for a <not-connected> graph: 0, don't put any edges btwn vertices
 * for a <connected> graph: n-1, each edge connects two nodes, to connect all nodes needs # of nodes - 1
 * 
 * the maximum edges required for a graph (no multi-edges and self-loops): n(n-1)/2 as O(n^2)
 * with multi-edges and self loops: infinite.
 * so, O(n) <= m <= O(n^2)
 * 
 * sum of degrees of nodes: 2n, each outbound edge of a node is going to be an inbound edge of another node, vice versa.
 * (trivial if have no inbound/outbound edges: set 0, 2*0 = 0), proved.
 * 
 */

/**
 * Graph ADT: data member: vertices V, edges E, and a data structure maintaining structures btwn vtcs and edges
 * 
 * Functions: 
 * insertVertex(K key);
 * insertEdge(Vertex v1, Vertex v2, K key); //num on labeling the edge
 * removeVertex(Vertex v);
 * removeEdge(Vertex v1, Vertex v2);
 * 
 * incidentEdges(Vertex v);
 * areAdjacent(Vertex v1, Vertex v2);
 * 
 * origin(Edge e); //for directed graphs
 * destination(Edge e);
 * 
 * Edgelist: naive implementation, list of v, list of e in a vector/array/hashtable of vertices and edges respectively
 * V: [u,v,w,z]
 * E: [(u,v,a), (v,w,b), (u,w,c), (w,z,d)]
 * 
 * insert vertex: O(1) amortized
 * remove vertex: O(m) since need to kill every edge that associated with the vertex
 * areAdjacent: travel every edge in edgelist to check, O(m)
 * incidentEdges: O(m) to scan the edge list.
 * 
 * insert edge: O(1) amortized (O(1) checking vertices exists, O(1) add to the list (hash table))
 * remove edge: O(1) amortized (O(1) checking vertices exists, O(1) pop from the list)
 * 
 * 
 * Adjacency Matrix: favored by static graph (as in markov chain/numeric analysis schema, no insertion/deletion)
 * value of entry: 0 if not an edge existed, 1 if an edge existed between nodes described by row/column labels.
 * matrix is symmetric if undirected, a triangle of the square if you can.
 * 
 * or: let the entry of the adj matrix be NULL if not an edge existed, or the ptr to the edge object in the edgelist if
 * they are connected.
 * 
 * insert vertex: O(1) in vertex list, O(n) to resize a row and column in the adjmat. --> O(n)
 * remove vertex: O(n) to remove row and column, and to delete the connections corresponding to that vertex --> O(n)
 * areAdjacent: O(1) as accessing indices.
 * incidentEdges: look upon the row/column (separately if directed, looking at either one if undirected) --> O(n)
 * 
 * Adjacency List: combined the first two ideas, better for dynamic graphs (w/ insert and remove)
 * create: a linkedlist of all incident edges (created on their ptrs) on a given vertex of the vertex list
 * and the edges incident to vertex v has its vertex v entry be a ptr to vertex v's address instead.
 * 
 * insert vertex: O(1)
 * remove vertex: O(1) in removing from the vertex list, O(deg(v)) in tracking the llist ptrs to edge list entries and remove them as well
 * areAdjacent: O(deg(v)) to track the llist to check, practically the minimum of the two arguments
 * incidentEdge: O(deg(v)) to return the llist from v
 * insert edge: O(1) assuming llist add from head
 * remove edge: O(deg(v)) popping arbitrary values from llist and its entry in edgelist, practically the maximum of two arguments (since need to remove both references)
 * 
 * if not using llst for adj list but hash table: areAdjacent O(1), remove edge O(1), incidentEdge O(deg(v)) to copy entries
 */


#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <functional> //included std::hash


//implementing the graph using an "adjacency map" where node is associated with a set of edges, unordered.
//has only basic functionalities of a graph, not implementing traversals and utility algorithms (see wk_12)
template <typename T, typename K>
class Graph {
    private:
        //inner class declaration
        class Vertex { //wrapper class for the nodes in the graph
            private:
                //making the vertex content not updatable from outside at its location (can still be copy-rewrite in assignment op)
                T name; //here, we require the name to be unique to each vertex
                //may have other fields for algorithmic uses
            public:
                Vertex(const T& nameref) : name(nameref) {}
                Vertex(const Vertex& another) : name(another.name) {} //making the vertex w/ same content at another memory location
                Vertex& operator=(const Vertex& rhs) {
                    name = rhs.name; 
                    return *this;
                }
                //define two vertices are equal by content, checking duplication
                const bool operator==(const Vertex& another) const {
                    return this->name == another.name;
                }
                const T& getname() const {
                    return name;
                }
                friend std::ostream& operator<<(std::ostream &os, const Vertex& v) { //supporting printing
                    return os << v.name;
                }
        };

        class Edge { //wrapper class for the edges in the graph connecting two vertices
            private:
                //making the label content not updatable from outside at its location (can still be copy-rewrite in assignment op)
                K label; //require to be comparable
                //only maintain a reference to the vertex it connecting, copying content will be too troublesome
                //in undirected graph, the from-to schematic does not matter, but in directed graph the edge is directed in (from, to) tuple
                const Vertex& from;
                const Vertex& to;
            public:
                //the arguments are required for a valid edge, try not to invoke default Edge ctor.
                Edge(const Vertex& fromref, const Vertex& toref) : from(fromref), to(toref), label() {} //default key assume uniform
                Edge(const Vertex& fromref, const Vertex& toref, const K& key) : from(fromref), to(toref), label(key) {}
                //voided Edge class's copy ctor and assignment op, not passing the reference from another edge in another graph
                //but only instantiate vertex references from the copied vertices in the local graph object.
                const bool operator==(const Edge& another) const { 
                    //check duplications to prevent multi-edges, two edges are same if they have same (from, to) vertex pair
                    //however, in directed graph (a,b) and (b,a) are considered different edges
                    return this->from.getname() == another.from.getname() && this->to.getname() == another.to.getname(); 
                }
                const K& getlabel() const {
                    return label;
                }
                const Vertex& getsource() const {
                    return from;
                }
                const Vertex& getsink() const {
                    return to;
                }
                friend std::ostream& operator<<(std::ostream &os, const Edge& e) { //supporting printing
                    return os << "(" << e.from << "," << e.to << "):" << e.label;
                }
        };

        /*
            hashing functions implemented as functional objects (functors) in C++ to enable hashing for class Vertex and Edge
            reference: https://ianyepan.github.io/posts/cpp-custom-hash/ : Method 1 Function Object with Struct
            "Perhaps the most commonly used method (and also my preferred method) is to create a function object (functor)
            using a struct, and implement its operator() function. This function is to be qualified as const, 
            and takes a const reference to the custom type and returns a size_t. 
             
            Defining a function object outside as a standalone struct also gives it the flexibility of 
            “generic programming” with templates. Meaning that it can work with pairs of different template arguments, 
            not just strings and ints.

            Well... in python & java: __hash__() will suffice
        */

        struct VertexHash {
            auto operator()(const Vertex &v) const -> std::size_t {
                return std::hash<T>{}(v.getname());
            }
        };
        struct EdgeHash {
            auto operator()(const Edge &e) const -> std::size_t {
                return VertexHash{}(e.getsource()) ^ ((VertexHash{}(e.getsink()) << 1) >> 1);
            }
        };

        //inner variables, this time adding custom auxilary hash function as the last <type> of the hashset/hashmap
        //note: hashset and hashmap keys are REQUIRED const to not modify the key to mess up the data structure
        std::unordered_set<Vertex, VertexHash> vertex_list_;
        std::unordered_set<Edge, EdgeHash> edge_list_;
        std::unordered_map<Vertex, std::unordered_set<Edge, EdgeHash>, VertexHash> adjacency_map_;

    public:
        //ctors
        Graph();
        Graph(const std::vector<T>& vertlist); //build from source vertex list, initially unconnected
        Graph(const Graph<T,K>& other); //to allow copy of Graph, will make copying of Vertex/Edge class
        Graph<T,K>& operator=(const Graph<T,K>& rhs); //to allow copy-ref of Graph
        ~Graph();
        
        //API
        void insertVertex(const T& nameref);
        T removeVertex(const T& vremname); //returns a copy, if key not found throw an error, vertex will be destroyed so voiding const
        void insertEdge(const T& vfromname, const T& vtoname, const K& key); //to not let Edge class complain const-binding
        K removeEdge(const T& vfromname, const T& vtoname); //returns a copy to the label, if not found throw an error
        bool areAdjacent(const T& vname, const T& wname) const; //whether v and w are adjacent vertices in the graph, described by their names
        std::vector<T> adjacentNodes(const T& vname) const; //returning the set of nodes that v adjacent to described by vname
        //if directed, only outward incident will be such simple and fast operation under this implementation.

        //statistics:
        int countVertices() const;
        int countEdges() const;

        //printing functions
        void print_all();
        void print_vertexlist();
        void print_edgelist();
        void print_adjmap();
    
    private: 
        //helper functions of the APIs
        //since Vertex and Edge are internal to the graph structure, they are only utilized as private methods.
        void _insertVertex(const Vertex& vnew);
        T _removeVertex(const Vertex& vrem); //also removes the edges associated with vrem
        void _insertEdge(const Vertex& vfrom, const Vertex& vto, const K& key);
        K _removeEdge(const Vertex& vfrom, const Vertex& vto); //actually return the edge instance to accomodate _removeVertex()
        bool _areAdjacent(const Vertex& v, const Vertex& w) const;
        std::vector<Edge> _incidentEdges(const Vertex& v) const; //returning the set of edges that v is incident to.
        std::vector<Vertex> _adjacentNodes(const Vertex& v) const; //returning the set of nodes that v is adjacent to   
};

#include "Graph.hpp"