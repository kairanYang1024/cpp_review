#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <functional> //included std::hash


//implementing the graph using an "adjacency map" where node is associated with a set of edges, unordered.
//has only basic functionalities of a graph, not implementing traversals and utility algorithms (see wk_12)
template <typename T, typename K>
class Graph {
    public: //to make global scope type access and typedef working
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
                K label; //require to be comparable and addictive
                //only maintain a reference to the vertex it connecting, copying content will be too troublesome
                //in undirected graph, the from-to schematic does not matter, but in directed graph the edge is directed in (from, to) tuple
                const Vertex& from;
                const Vertex& to;
            public:
                //the arguments are required for a valid edge, try not to invoke default Edge ctor.
                Edge(const Vertex& fromref, const Vertex& toref) : from(fromref), to(toref), label() {} //default key assume uniform
                Edge(const Vertex& fromref, const Vertex& toref, const K& key) : from(fromref), to(toref), label(key) {}
                //voided copy ctor and assignment operator to accomodate const Vertex& semantic

                //check duplications to prevent multi-edges, two edges are same if they have same (from, to) vertex pair
                //however, in directed graph (a,b) and (b,a) are considered different edges
                const bool operator==(const Edge& another) const { 
                    return this->from.getname() == another.from.getname() && this->to.getname() == another.to.getname(); 
                }
                const K& getlabel() const {
                    return label;
                }
                void setlabel(const K& labelkey) { //cannot be casted in an const-Edge instance
                    label = labelkey;
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

    private:
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
        K sumEdgeWeights() const; //require K summative

        //utilities:
        const std::unordered_set<Vertex, VertexHash>& vertices() const;
        const std::unordered_set<Edge, EdgeHash>& edges() const;
        
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
#include "Graph_algo.hpp"