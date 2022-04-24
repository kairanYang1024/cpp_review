#include "Graph.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string> 
#include <iostream>
#include <utility>
#include <stdexcept>
#include <cassert>

//initialize the containers, all are on stack
template <typename T, typename K>
Graph<T,K>::Graph() : vertex_list_(), edge_list_(), adjacency_map_() {}

template <typename T, typename K>
Graph<T,K>::Graph(const std::vector<T>& namelist) : Graph() {
    for(const T& name : namelist) { //create vertex one by one
        Vertex v(name); //create a vertex v from its nameref 'elem'
        if(!vertex_list_.count(v)) { 
            //This defaults to equal_to<Key>, which returns the same as applying the equal-to operator (a==b). 
            //The unordered_set object uses this expression to determine whether two element keys are equivalent. 
            // so it will check whether vertex_list_ has vertex having same name with v, if so we skip this (and call dtor on v)
            // else insert the vertex as new vertex in vlist and adjmap.
            vertex_list_.insert(v);
            adjacency_map_[v] = std::unordered_set<Edge, EdgeHash>(); //bind a newly create unordered set to each vertex in adj_map
        }
    }
}

template <typename T, typename K>
Graph<T,K>::Graph(const Graph<T,K>& other) : Graph() { //copy every content of 'other' to 'this', in deep copies
    //copy the vertices first, assume ov are not duplicated.
    for(const Vertex& ov: other.vertex_list_) {
        Vertex v = ov; //deep copy = overloaded
        vertex_list_.insert(v);
    }

    //then check the edges of 'other' to construct edges of 'this', assume oe are not duplicated
    for(const Edge& oe : other.edge_list_) {
        //using vertex names of the edges from 'other' to check the local vertex list
        const Vertex& oefrom = oe.getsource();
        const Vertex& oeto = oe.getsink();
        const K& oelabel = oe.getlabel();
        if(vertex_list_.count(oefrom) && vertex_list_.count(oeto)) {
            //here, uses the shortcut Vertex == comparing the names of the vertices on .count() method to avoid address comparison
            //unordered_set.find() returns an iterator to the object rather than object itself
            auto fromiter = vertex_list_.find(oefrom); //finding the local copies of the oefrom and oeto
            auto toiter = vertex_list_.find(oeto);
            Edge e((*fromiter), (*toiter), oelabel);
            edge_list_.insert(e);
        }
    }

    //lastly, check the adj map of 'other' to construct each vertex's incidental edges
    for(const std::pair<Vertex, std::unordered_set<Edge, EdgeHash>>& oadjset : other.adjacency_map_) {
        if(vertex_list_.count(oadjset.first)) {
            const Vertex& v = *(vertex_list_.find(oadjset.first));
            adjacency_map_[v] = std::unordered_set<Edge, EdgeHash>();
            const std::unordered_set<Edge, EdgeHash>& ovincs = oadjset.second;
            for(const Edge& oe: ovincs) {
                if(edge_list_.count(oe)) { //two edges are the same if having same endpoints
                    auto eiter = edge_list_.find(oe);
                    adjacency_map_.at(v).insert((*eiter));
                }
            }
        }
    }
}

template <typename T, typename K>
Graph<T,K>& Graph<T,K>::operator=(const Graph<T,K>& other) {
    //release previously stored contents, calling each element's dtor
    vertex_list_.clear();
    edge_list_.clear();
    adjacency_map_.clear();

    //same as the copy ctor
    for(const Vertex& ov: other.vertex_list_) {
        Vertex v = ov; //deep copy = overloaded
        vertex_list_.insert(v);
    }

    //then check the edges of 'other' to construct edges of 'this', assume oe are not duplicated
    for(const Edge& oe : other.edge_list_) {
        //using vertex names of the edges from 'other' to check the local vertex list
        const Vertex& oefrom = oe.getsource();
        const Vertex& oeto = oe.getsink();
        const K& oelabel = oe.getlabel();
        if(vertex_list_.count(oefrom) && vertex_list_.count(oeto)) {
            //here, uses the shortcut Vertex == comparing the names of the vertices on .count() method to avoid address comparison
            auto fromiter = vertex_list_.find(oefrom); //finding the local copies of the oefrom and oeto
            auto toiter = vertex_list_.find(oeto);
            Edge e((*fromiter), (*toiter), oelabel);
            edge_list_.insert(e);
        }
    }

    //lastly, check the adj map of 'other' to construct each vertex's incidental edges
    for(const std::pair<Vertex, std::unordered_set<Edge, EdgeHash>>& oadjset : other.adjacency_map_) {
        if(vertex_list_.count(oadjset.first)) {
            const Vertex& v = *(vertex_list_.find(oadjset.first));
            adjacency_map_[v] = std::unordered_set<Edge, EdgeHash>();
            const std::unordered_set<Edge, EdgeHash>& ovincs = oadjset.second;
            for(const Edge& oe: ovincs) {
                if(edge_list_.count(oe)) { //two edges are the same if having same endpoints
                    auto eiter = edge_list_.find(oe);
                    adjacency_map_.at(v).insert((*eiter));
                }
            }
        }
    }

    return *this;
}

template <typename T, typename K>
Graph<T,K>::~Graph() {
    //release their contents, calling each element's dtor
    vertex_list_.clear();
    edge_list_.clear();
    adjacency_map_.clear();
    //then call each dtor of the containers, not shown here
}

template <typename T, typename K>
void Graph<T,K>::insertVertex(const T& nameref) {
    //handle duplications here
    Vertex vnew(nameref);
    //either both vertexlist and adjacency map contains newv or not, cant have the case where one have and another don't
    assert(vertex_list_.count(vnew) == adjacency_map_.count(vnew));
    if(vertex_list_.count(vnew)) {
        std::cerr << "Warning: node " << nameref << " already existed in the graph, terminating insertion...\n";
        return;
    }
    _insertVertex(vnew);
}

template <typename T, typename K>
void Graph<T,K>::_insertVertex(const Vertex& vnew) {
    vertex_list_.insert(vnew);
    adjacency_map_[vnew] = std::unordered_set<Edge, EdgeHash>(); //fresh new adjset without any edge joining
}

//returns a copy, if key not found throw an error, vertex will be destroyed so voiding const
template <typename T, typename K>
T Graph<T,K>::removeVertex(const T& vremname) {
    Vertex vtemp(vremname);
    if(!vertex_list_.count(vtemp)) {
        throw std::runtime_error("Error: the queried node cannot be found in the graph's vertex list!");
    } 
    const Vertex& vrem = *(vertex_list_.find(vtemp));
    T name = _removeVertex(vrem);
    return name;
}

//also removes the edges associated with vrem, at both side (from vrem and to vrem) regardless of directed/undirected
template <typename T, typename K>
T Graph<T,K>::_removeVertex(const Vertex& vrem) { 
    std::vector<Edge> incidents = _incidentEdges(vrem);
    for(Edge& vreminci : incidents) {
        const Vertex& from = vreminci.getsource();
        const Vertex& to = vreminci.getsink();
        _removeEdge(from, to);
        _removeEdge(to, from);
    }
    
    T name = vrem.getname();
    //remove both the key and value (hashmap on edge, should be empty now)
    adjacency_map_.erase(vrem); 
    //remove vrem's identity in the vertex list, this should come after adjacency map's remove 
    //to ensure adjacency's remove is not voided
    vertex_list_.erase(vrem); 
    return name;
}

// do not let Edge class complain const-binding
// in this version of Graph, this function will add both directions since undirected
template <typename T, typename K>
void Graph<T,K>::insertEdge(const T& vfromname, const T& vtoname, const K& key) { 
    Vertex vfromtemp(vfromname);
    Vertex vtotemp(vtoname);
    //checking validity of arguments
    if(vfromtemp == vtotemp) {
        std::cerr << "Warning: nodes duplicated as endpoints of the edge, required no self loop.\n";
        return;
    }
    if(!vertex_list_.count(vfromtemp)) {
        std::cerr << "Warning: node " << vfromname << " is nonexistent in the graph, terminating insertion...\n";
        return;
    }
    if(!vertex_list_.count(vtotemp)) {
        std::cerr << "Warning: node " << vtoname << " is nonexistent in the graph, terminating insertion...\n";
        return;
    }
    const Vertex& vfrom = *(vertex_list_.find(vfromtemp));
    const Vertex& vto = *(vertex_list_.find(vtotemp));
    //bidirection since undirected
    _insertEdge(vfrom, vto, key);
    _insertEdge(vto, vfrom, key);
} 

//but in this private version, will only add the direction specified by the order
template <typename T, typename K>
void Graph<T,K>::_insertEdge(const Vertex& vfrom, const Vertex& vto, const K& key) {
    Edge newedge(vfrom, vto, key);
    if(edge_list_.count(newedge)) { //handling duplication
        std::cerr << "Warning: edge " << newedge << " already existed in the graph, terminating insertion...\n";
        return;
    }
    edge_list_.insert(newedge);
    adjacency_map_.at(vfrom).insert(newedge); //left vto not touched for directed case
}

// returns a copy to the label, if not found throw an error
// in this version of Graph, this function will remove both directions since undirected
template <typename T, typename K>
K Graph<T,K>::removeEdge(const T& vfromname, const T& vtoname) {
    Vertex vfromtemp(vfromname);
    Vertex vtotemp(vtoname);
    if(!vertex_list_.count(vfromtemp)) {
        throw std::runtime_error("Error: the queried 'from' node cannot be found in the graph's vertex list!");
    }
    if(!vertex_list_.count(vtotemp)) {
        throw std::runtime_error("Error: the queried 'to' node cannot be found in the graph's vertex list!");
    }
    const Vertex& vfrom = *(vertex_list_.find(vfromtemp));
    const Vertex& vto = *(vertex_list_.find(vtotemp));
    K label = _removeEdge(vfrom, vto);
    //only called in undirected case
    _removeEdge(vto, vfrom); //these two edges have the same label in undirected case
    return label;
}

//however, this private version will only remove the direction specified by the order
template <typename T, typename K>
K Graph<T,K>::_removeEdge(const Vertex& vfrom, const Vertex& vto) { 
    Edge etemp(vfrom, vto);
    if(!edge_list_.count(etemp)) {
        throw std::runtime_error("Error: the queried edge cannot be found in the graph's edge list!");
    }
    //make sure both edgelist and adj map contains the edge in query
    assert(edge_list_.count(etemp) == adjacency_map_.at(vfrom).count(etemp)); 
    const Edge& erem = *(edge_list_.find(etemp));
    K eremlabel = erem.getlabel();
    edge_list_.erase(erem);
    adjacency_map_.at(vfrom).erase(erem);
    //make sure neither edgelist nor adj map contains the edge in query
    assert(edge_list_.count(etemp) == adjacency_map_.at(vfrom).count(etemp));
    return eremlabel;
}

//whether v and w are adjacent vertices in the graph, described by their names, in directed case: not symmetric.
template <typename T, typename K>
bool Graph<T,K>::areAdjacent(const T& vname, const T& wname) const {
    Vertex vtemp(vname);
    Vertex wtemp(wname);
    if(!vertex_list_.count(vtemp)) {
        std::cerr << "Warning: node " << vname << " is nonexistent in the graph, exiting routine and returning false...\n";
        return false;
    }
    if(!vertex_list_.count(wtemp)) {
        std::cerr << "Warning: node " << wname << " is nonexistent in the graph, exiting routine and returning false...\n";
        return false;
    }
    const Vertex& v = *(vertex_list_.find(vtemp));
    const Vertex& w = *(vertex_list_.find(wtemp));
    return _areAdjacent(v, w);
} 

template <typename T, typename K>
bool Graph<T,K>::_areAdjacent(const Vertex& v, const Vertex& w) const {
    for(const Edge& ve : adjacency_map_.at(v)) {
        if(ve.getsink() == w) return true;
    }
    return false;
}

//returning the set of nodes that v adjacent to described by vname
//if directed, only outward incident will be such simple and fast operation under this implementation.
template <typename T, typename K>
std::vector<T> Graph<T,K>::adjacentNodes(const T& vname) const {
    Vertex vtemp(vname);
    if(!vertex_list_.count(vtemp)) {
        std::cerr << "Warning: node " << vname << " is nonexistent in the graph, exiting routine and returning empty list...\n";
        return std::vector<T>();
    }
    const Vertex& v = *(vertex_list_.find(vtemp));
    std::vector<Vertex> adj_vertices = _adjacentNodes(v);
    std::vector<T> adjnames;
    for(const Vertex& adv : adj_vertices) {
        adjnames.push_back(adv.getname());
    }
    return adjnames;
} 

//returning the set of edges that v is incident to.
template <typename T, typename K>
std::vector<typename Graph<T,K>::Edge> Graph<T,K>::_incidentEdges(const Vertex& v) const {
    return std::vector<Edge>(adjacency_map_.at(v).begin(), adjacency_map_.at(v).end());
}

//returning the set of nodes that v is adjacent to, only outward edges in directed case
template <typename T, typename K>
std::vector<typename Graph<T,K>::Vertex> Graph<T,K>::_adjacentNodes(const Vertex& v) const {
    std::vector<Vertex> ret;
    for(const Edge& ve :  adjacency_map_.at(v)) {
        ret.push_back(ve.getsink());
    }
    return ret;
}

//statistics
template <typename T, typename K>
int Graph<T,K>::countVertices() const {
    return vertex_list_.size();
}

template <typename T, typename K>
int Graph<T,K>::countEdges() const {
    return edge_list_.size() / 2; //count both directions as one in undirected case
}

template <typename T, typename K>
K Graph<T,K>::sumEdgeWeights() const {
    K res = K(); //invoking default ctor
    for(const Edge& e : edge_list_) {
        res = res + e.getlabel();
    }
    // potentially dangerous, require object to implement all four arithmetic operators
    // (at least scaling and object addition) 
    return res / 2; 
}

//utilities
template <typename T, typename K>
const std::unordered_set<typename Graph<T,K>::Vertex, typename Graph<T,K>::VertexHash>& Graph<T,K>::vertices() const {
    return vertex_list_;
}

template <typename T, typename K>
const std::unordered_set<typename Graph<T,K>::Edge, typename Graph<T,K>::EdgeHash>& Graph<T,K>::edges() const {
    return edge_list_;
}

//printing functions
template <typename T, typename K>
void Graph<T,K>::print_vertexlist() {
    std::cout << "Vertex List (unordered): ";
    for(const Vertex& v : vertex_list_) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

template <typename T, typename K>
void Graph<T,K>::print_edgelist() {
    std::cout << "Edge List (unordered): ";
    for(const Edge& e: edge_list_) {
        std::cout << e << " ";
    }
    std::cout << "\n";
}

template <typename T, typename K>
void Graph<T,K>::print_adjmap() {
    std::cout << "Adjacency Map (unordered): \n";
    for(const std::pair<Vertex, std::unordered_set<Edge, EdgeHash>>& adjset : adjacency_map_) {
        const Vertex& v = adjset.first;
        const std::unordered_set<Edge, EdgeHash>& incs = adjset.second;
        std::cout << v << " : {";
        for(const Edge& inc : incs) {
            std::cout << inc << " ";
        }
        std::cout << "}\n";
    }
    std::cout << "\n";
}

template <typename T, typename K>
void Graph<T,K>::print_all() {
    print_vertexlist();
    print_edgelist();
    print_adjmap();
}
