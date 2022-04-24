#include "Graph.h"
#include "../wk_10_union_find/DisjointSets.h" //make sure to change this based on your own folder path
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <utility>
#include <stdexcept>
#include <cassert>

/**
 * Graph searches (= traversals): visit every single node in the graph, find interesting substructure
 * difference between graph traversal and tree traversal: 
 * tree is ordered, rooted (begin) and completeness (end -- when reaching a leaf)
 * graph is unordered, no obvious start (prespecified) and no notion of completeness (end), possibility of looping forever
 * 
 * BFS: visit every adjacent node before visiting 2-degree nodes away, maintaining a queue.
 * maintain an accessing adjacent nodes as well + a set of recording which node is visited (can be the original set in Graph.h)
 * until all nodes are visited.
 * 
 * BFS(G): outputs an labeling of the edges on G as "discovery" edges and "cross" edges.
 * "discovery edge" = edge that connects to a node that has not been visited before from node v.
 * "cross edge" = edge that connects to a node that has been already visited from node v.
 * after the entire traversal, produces a subgraph of "spanning trees" of discovery edges
 *  
 * BFS(G,v): performs the actual BFS starting from the node, marking each node "visited" or "un-visited",
 * and marking each edge "visited", "disco", or "cross".
 * add only the nodes having "discovery" edges in the queue.
 * 
 * counting components: one BFS will sweep nodes connected, so # of bfs sweeped = # of components in the graph G.
 * cycle detection: if found a "cross" edge, we know there's a cycle (visiting nodes visited).
 * running time: O(|V| + 2|E|) = O(n+m), for every node, accessing sum of the degrees of each node is 2*m
 * when sparse: O(n), when dense O(n^2).
 * 
 * shortest path: only available for a shortest path from the argument node.
 * also, cross-path won't contribute to the (distance from the start).
 * 
 * DFS: use stack structure / by recursion
 * mark discovery edge and "back edge -- leading back to previous nodes in the path"
 * call only the nodes with discovery edge down the recursion.
 * running time: O(n+m) = O(|V|+|E|)
 * 
 * 
 * Minimum Spanning Tree: creating spanning tree (acyclic graph) with minimum edge weight
 * 
 * Kruskal's algorithm: using minHeap (priority queue), to have an organize list of edges (not nec sorted) ASC 
 * in edgeweights + using a disjointset of all elements inside the set. 
 * If the vertices involved in the edge <are> belong to the <same> set, do nothing, 
 * else union the sets together (these edges will be included in the MST.)
 * 
 * after visiting every edge OR the entire disjoint structure is a set: terminate the algo and record edges that
 * results in a union of two sets in dset structure.
 * 
 * running time: build dset O(n) + buildHeap O(m) + removemin O(m*log m) (same as sorted array runtime)
 * (+ each union of the heapified edgelist is O(m* log_iter(m)) ~= O(m*))
 * when graph is dense, the total complexity is O(n^2 log n), when sparse, complexity is O(n log n)
 * 
 * 
 * Prim's algorithm: based on the <partition property>, given distinct components u, v; the 
 * minimum <edge> connecting u and v must be part of some minimum spanning tree. 
 * 
 * In this setting, we take u as <label set> and v as <unlabeled set>, goal is to grow u by finding the 
 * minimum edge that crosses over u and v, start with u = single node argument.
 * 
 * Maintain a dynamic set of incidental edges (or vertices) bordering labeled territory u and uncharted v, 
 * to avoid recomputing each node in u's incident edges that do not belong to the labeled subgraph
 *  (messy to check & slow down).
 * 
 * implementation: edge-less, make a map of vertex to its predecessor to simulate this "edge"
 * and a set of distances, starting from defining d[s] = 0, setting other vertex d[v] = inf, initially
 * update d[w] if d[v] + cost(v, w) < d[w] and not included into the labeled set T.
 * 
 * running time: in adj list is O(n log n + m log n), where n log n comes from the n-time remove_min() in 
 * the min-heap of vertex distances; m log n comes from the 2m degree sum of the graph, for each edge,
 * requiring an update of weight (+inf to a number, at least) to the minheap, which is O(log n).
 * https://stackoverflow.com/questions/20430740/time-complexity-of-prims-algorithm
 * 
 * 
 * Single-source shortest path algorithm (SSSP): path of smallest weight given 2 random vertices.
 * 
 * Dijkstra's algorithm:
 * 
 * 
 */

/*
    Note: functions written in this file are not part of the Graph class, however, still being included in
    the Graph.h header file because these functions are utilities involving Graphs, and for a quicker compilation
    in the g_algo.cpp file.
*/

//return a (set of) spanning tree of the original graph
template <typename T, typename K>
Graph<T, K> bfs_sptree(const Graph<T,K>& graph) {
    using Vertex = typename Graph<T,K>::Vertex;
    using Edge = typename Graph<T,K>::Edge;
    using VertexHash = typename Graph<T,K>::VertexHash;
    using EdgeHash = typename Graph<T,K>::EdgeHash;

    const auto& vertices = graph.vertices();
    std::unordered_set<Vertex, VertexHash> memo_set;
    std::unordered_set<Edge, EdgeHash> edgemarker;
    for(const Vertex& v : vertices) {
        if(memo_set.count(v) == 0) {
            bfs(graph, v, memo_set, edgemarker);
        }
    }

    //build the spanning tree(s) based on the edgemarker.
    assert(memo_set.size() == vertices.size()); //should have visited all vertices in the graph.
    std::vector<T> copied_list;
    for(const Vertex& v : memo_set) {
        copied_list.push_back(v.getname());
    }
    Graph<T,K> spanning_tree(copied_list);
    for(const Edge& e : edgemarker) {
        const Vertex& from = e.getsource();
        const Vertex& to = e.getsink();
        const K& lab = e.getlabel();
        spanning_tree.insertEdge(from.getname(), to.getname(), lab);
    }
    return spanning_tree;
}

//same algorithm as in bfs_sptree but instead of generating a subgraph it only returns the # of connected components
//founded in the graph.
template <typename T, typename K>
int countComponents(const Graph<T,K>& graph) {
    using Vertex = typename Graph<T,K>::Vertex;
    using Edge = typename Graph<T,K>::Edge;
    using VertexHash = typename Graph<T,K>::VertexHash;
    using EdgeHash = typename Graph<T,K>::EdgeHash;

    const auto& vertices = graph.vertices();
    std::unordered_set<Vertex, VertexHash> memo_set;
    std::unordered_set<Edge, EdgeHash> edgemarker;
    int conn_components = 0;
    for(const Vertex& v : vertices) {
        if(memo_set.count(v) == 0) {
            bfs(graph, v, memo_set, edgemarker);
            conn_components++;
        }
    }
    assert(memo_set.size() == vertices.size()); //should have visited all vertices in the graph.
    return conn_components;
}

//marking the edges to be "discovery" only, cross-edge are not recorded.
template <typename T, typename K>
void bfs(const Graph<T,K>& graph, const typename Graph<T,K>::Vertex& start, 
std::unordered_set<typename Graph<T,K>::Vertex, typename Graph<T,K>::VertexHash>& memo_set,
std::unordered_set<typename Graph<T,K>::Edge, typename Graph<T,K>::EdgeHash>& edgemarker) {
    using Vertex = typename Graph<T,K>::Vertex;
    using Edge = typename Graph<T,K>::Edge;

    std::queue<Vertex> processor;
    processor.push(start);
    memo_set.insert(start);
    while(!processor.empty()) {
        const Vertex curver = processor.front();
        processor.pop();
        std::vector<T> adjnodes = graph.adjacentNodes(curver.getname());
        for(const T& adj_name : adjnodes) {
            //let's make them in copy right now...since everything in this function is a copy.
            Vertex neighbor(adj_name);
            if(memo_set.count(neighbor) == 0) {
                memo_set.insert(neighbor);
                processor.push(neighbor);
                //must stick to the address as specified by the class design.
                Edge discovery = Edge(*(graph.vertices().find(curver)), *(graph.vertices().find(neighbor))); 
                discovery.setlabel(graph.edges().find(discovery)->getlabel());
                //we make the single-direction edge here, using insertEdge() to paste the bidirections later.
                edgemarker.insert(discovery); 
            } else {
                //we know the neighboring node is visited and so the edge corresponding to 'neighbor' is a cross edge
                //no-op in this setting
                continue;
            }
        }
    }
}

//overwriting std::hash on the pair of generics.
struct PairHash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

//overwriting comparing order on the pair of generics, originally want to impl on Edge directly, but failed.
struct PairLabelComp {
    template <typename T, typename K>
    const bool operator()(const std::pair<std::pair<T,T>, K>& a, const std::pair<std::pair<T,T>, K>& b) const {
        //comparator works in the opposite order from the operator overloading
        //because of the input parameter ordering
        return b.second < a.second; 
    }
};

//this method is useful in Kruskal and Dijkstra of undirected (bidirectional) graphs
template<typename T, typename K>
std::vector<std::pair<std::pair<T,T>, K>> simplifyEdgelist(const Graph<T,K>& graph) {
    using Edge = typename Graph<T,K>::Edge;
    //container for non-reversed edges in future edge-sortings
    std::unordered_map<std::pair<T,T>, K, PairHash> single_edges; 
    const auto& edgelist = graph.edges();
    for(const Edge& e : edgelist) {
        const T& first = e.getsource().getname();
        const T& second = e.getsink().getname();
        const K& label = e.getlabel();
        std::pair<T,T> curPair = std::make_pair(first, second);
        std::pair<T,T> reversePair = std::make_pair(second, first); //the reverse vertex-edge
        if(single_edges.count(reversePair)) {
            continue;
        }
        single_edges.emplace(curPair, label);
    }
    assert(single_edges.size()*2 == edgelist.size()); //assert the edge numbers are halved
    return std::vector<std::pair<std::pair<T,T>, K>>(single_edges.begin(), single_edges.end());
}

template <typename T, typename K>
Graph<T,K> kruskalMST(const Graph<T,K>& graph) {
    using Vertex = typename Graph<T,K>::Vertex;
    using Edge = typename Graph<T,K>::Edge;
    using EdgeHash = typename Graph<T,K>::EdgeHash; //need to sort the edges, resolving collision betwn < and ==?
    using PairLabel = typename std::pair<std::pair<T,T>, K>;

    assert(countComponents(graph) == 1); //if not connected, cannot make a MST out of it.
    //will sort edge based on weights. O(m)
    std::vector<PairLabel> simpEL = simplifyEdgelist(graph);
    //std::pqueue template: type, container, comparator object
    std::priority_queue<PairLabel,std::vector<PairLabel>,PairLabelComp> edge_sorter(simpEL.begin(), simpEL.end()); 
    
    //extract the nametags, O(n)
    std::vector<T> vertexlist;
    for(const Vertex& v : graph.vertices()) {
        vertexlist.push_back(v.getname());
    }
    DisjointSets<T> kruskalForest(vertexlist); //will this invoke the correct vertex-to-int map?
    Graph<T,K> MST(vertexlist); //construct the vertices first
    //process the edges and union them, O(m log m), add edge based on union() result.
    while(!edge_sorter.empty() && kruskalForest.num_sets() > 1) {
        const PairLabel& minedge = edge_sorter.top();
        //directly unpack to T level
        T fromname = minedge.first.first;
        T toname = minedge.first.second;
        K edgelabel = minedge.second;
        edge_sorter.pop();
        if(kruskalForest.unionize(fromname, toname)) { ///if union successes, add that edge to MST
            //if unionization failed (nonexistent keys, same set belonging, etc... --> handled correctly by noop)
            MST.insertEdge(fromname, toname, edgelabel);
        }
    }
    return MST;
}

//used for both Prim's algo and Dijkstra's algo on building pqueue on <T,K> pair
struct VertexLabelComp {
    template <typename T, typename K>
    const bool operator()(const std::pair<T, K>& a, const std::pair<T, K>& b) const {
        //comparator works in the opposite order from the operator overloading
        //because of the input parameter ordering
        return b.second < a.second; 
    }
};

//unlike Kruskal, prim requires a starting point to work with.
//if multiple edges are of same weight, diff starting points may result in diff trees, but guaranteed minimal weight sum
//essentially, a greedy-algorithm proud of its "parititon-property"
template <typename T, typename K>
Graph<T,K> primMST(const Graph<T,K>& graph, const T& start_vtx) {
    using Vertex = typename Graph<T,K>::Vertex;
    using Edge = typename Graph<T,K>::Edge;
    using std::pair;
    assert(countComponents(graph) == 1); //if not connected, cannot make a MST out of it.
    //this version of MST algo is similar to the logic in Dijkstra's algo as well.
    //however, trying to make it "edge-less" when maintaining, O(n)
    std::unordered_set<T> labeled;
    std::unordered_map<T,K> cost; //maps the vertex named T to the temptative cost K from start_vtx to reach
    std::unordered_map<T,T> prev; //maps the vertex to the vertex that reached to it.
    Graph<T,K> MST; //empty graph here
    for(const Vertex& v : graph.vertices()) {
        T name = v.getname();
        cost[name] = 99999999;
        //stop assigning prev[name] to NULL since NULL is to pointers in C/++, unlike in other languages.
        MST.insertVertex(name); //add vertices in the pre-build phase
    }
    cost[start_vtx] = 0;

    //build MST edges based on vector
    std::vector<pair<T,K>> costvec(cost.begin(), cost.end());
    //update prev[] based on cost[]
    std::priority_queue<pair<T,K>, std::vector<pair<T,K>>, VertexLabelComp> costqueue;
    costqueue.push(std::make_pair(start_vtx, 0));
    while(!costqueue.empty()) {
        const pair<T,K>&  minvpair = costqueue.top();
        const T v = minvpair.first;
        const K mincost = minvpair.second;
        costqueue.pop();
        labeled.insert(v);
        for(const T& w : graph.adjacentNodes(v)) {
            Edge dummy(v, w); //for searching index to achieve O(1) extraction of edge weight
            const K& distance = graph.edges().find(dummy)->getlabel(); 
            //for nodes not labeled, update their cost by the edgeweight, not summative.
            if(labeled.count(w) == 0 && distance < cost[w]) { 
                prev[w] = v;
                cost[w] = distance;
                costqueue.push(std::make_pair(w, cost[w]));
            }
        }
    }

    //build the edges of MST based on the stabilized prev relation
    for(const pair<T,T>& edgepair : prev) {
        const T& w = edgepair.first;
        const T& v = edgepair.second;
        Edge dummy(v, w);
        const K& distance = graph.edges().find(dummy)->getlabel(); 
        MST.insertEdge(v, w, distance);
    }
    return MST;
}

//TODO: last bit of the entire course review
template <typename T, typename K>
Graph<T,K> dijkstraPath(const Graph<T,K>& graph, const T& start_vtx, const T& end_vtx) {
    return Graph<T,K>();
}

template <typename T, typename K>
K dijkstraWeight(const Graph<T,K>& graph, const T& start_vtx, const T& end_vtx) {
    return K();
}




