#include "DisjointSets.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

//ctors
template <typename T>
DisjointSets<T>::DisjointSets() : uptree_(), indices_(), rev_indices_(), size_(0), num_sets_(0){
    
}

template <typename T>
DisjointSets<T>::DisjointSets(const T*& raw_data, int length) : DisjointSets() {
    if(length < 0) throw std::runtime_error("invalid array size < 0");
    int effective_idx = 0;
    for(int i = 0; i < length; i++) { 
        if(indices_.count(raw_data[i])) continue; //handling duplicates
        uptree_.push_back(-1);
        indices_[raw_data[i]] = effective_idx; //preserve order of the index when making the disjoint sets, recorded in the mapping
        rev_indices_[effective_idx] = raw_data[i];
        effective_idx++;
    }
    size_ = effective_idx;
    num_sets_ = effective_idx;
}

template <typename T>
DisjointSets<T>::DisjointSets(const std::vector<T>& raw_data) : DisjointSets() {
    if(raw_data.size() < 0) throw std::runtime_error("invalid vector size < 0"); //not usually going to happen, unlike the array version
    int effective_idx = 0;
    for(int i = 0; i < raw_data.size(); i++) { 
        if(indices_.count(raw_data[i])) continue; //handling duplicates
        uptree_.push_back(-1);
        indices_[raw_data[i]] = effective_idx; //preserve order of the index when making the disjoint sets, recorded in the mapping
        rev_indices_[effective_idx] = raw_data[i];
        effective_idx++;
    }
    size_ = effective_idx;
    num_sets_ = effective_idx;
}

template <typename T>
DisjointSets<T>::DisjointSets(const DisjointSets<T>& other) {
    // use copy functions to quickly copy the data from arg
    // https://stackoverflow.com/questions/14977632/copying-stdvector-prefer-assignment-or-stdcopy
    uptree_ = std::vector<int>(); //empty ctor init
    uptree_.reserve(other.size_);
    uptree_.assign(other.uptree_.begin(), other.uptree_.end()); //using the iterators
    
    //utilizing copy ctors of the other argument, assume deep copies (if not going to have troubles)
    indices_(other.indices_); 
    rev_indices_(other.rev_indices_); 

    size_ = other.size_;
    num_sets_ = other.num_sets_;
}

template <typename T>
DisjointSets<T>& DisjointSets<T>::operator=(const DisjointSets<T>& rhs) {
    //clear the containers first
    uptree_.clear();
    indices_.clear();
    rev_indices_.clear();

    //realloc space if rhs has much larger size
    uptree_.reserve(rhs.size_); 
    uptree_.assign(rhs.uptree_.begin(), rhs.uptree_.end());

    // using the member's copy assignment op here, they might invoke their own clear functions inside
    // but for safety we invoke clear() explicitly
    indices_ = rhs.indices_;
    rev_indices_ = rhs.rev_indices_; 

    size_ = rhs.size_;
    num_sets_ = rhs.num_sets_;
    return *this;
}


//API-related functions
template <typename T>
void DisjointSets<T>::make_set(const T& elem) { //O(1)
    if(indices_.count(elem)) { //handling duplicates
        std::cerr << "Warning: element " << elem << " already existed in the list!\n";
        return;
    }

    //growing size based on current size
    uptree_.push_back(-1);
    indices_[elem] = size_;
    rev_indices_[size_] = elem;

    //update counters
    size_++;
    num_sets_++;
}

template <typename T>
void DisjointSets<T>::unionize(const T& k1, const T& k2) {
    //"de-reference" the keys 
    if(!indices_.count(k1)) {
        std::cerr << "Warning, key " << k1 << " not found in the set\n";
        return;
    }
    if(!indices_.count(k2)) {
        std::cerr << "Warning, key " << k2 << " not found in the set\n";
        return;
    }
    int idx1 = indices_.at(k1);
    int idx2 = indices_.at(k2);
    _unionize(idx1, idx2); //call the core function
}

template <typename T>
void DisjointSets<T>::_unionize(int idx1, int idx2) {
    //if belong to the same set, no-op
    //ensuring set belonging operation is O(log* n) no matter these indices belong to same set or not
    int rootidx1 = _find_repidx(idx1);
    int rootidx2 = _find_repidx(idx2);
    if(rootidx1 == rootidx2) return;

    if(uptree_[rootidx2] < uptree_[rootidx1]) { //if set 2 has size greater than set 1
        uptree_[rootidx2] += uptree_[rootidx1]; //union by size
        uptree_[rootidx1] = rootidx2; //reassign rootidx1's parent as rootidx2, merge set 1 into 2
    } else { //if set 1 has size greater than size 2, or if their size same then the first argument is the new root
        uptree_[rootidx1] += uptree_[rootidx2];
        uptree_[rootidx2] = rootidx1;
    }
    num_sets_--; //one set has been unionized into the other, effectively decrease by 1
}


template <typename T>
const T& DisjointSets<T>::find(const T& key) {
    if(!indices_.count(key)) {
        throw std::runtime_error("Warning: key not found in the set, throwing error");
       //this is the behavior of the function defined by programmer, need to specify (can also throw an error)
    }
    int keyidx = indices_.at(key);
    int rootidx = _find_repidx(keyidx); //also do path compression when traversing the subtree
    if(!rev_indices_.count(rootidx)) { 
        // if we cannot find the element corresponding to an index
        // it means we update the sets incorrectly and the program has to be aborted.
        std::string msg = "Fatal: root index ";
        msg += std::to_string(rootidx); 
        msg += " has no corresponding elements.\n";
        throw std::runtime_error(msg);
    }
    return rev_indices_.at(rootidx); //auto-casted to const
}   

template <typename T>
int DisjointSets<T>::_find_repidx(int keyidx) {
    if(uptree_[keyidx] < 0) return keyidx;
    //perform path-compression here
    int rootidx = _find_repidx(uptree_[keyidx]);
    
    if(uptree_[keyidx] != rootidx) uptree_[keyidx] = rootidx;
    return rootidx;
}


//other helpers
template <typename T>
int DisjointSets<T>::size() {
    return size_;
}

template <typename T>
int DisjointSets<T>::num_sets() {
    return num_sets_;
}

template <typename T>
void DisjointSets<T>::print_uptree() {
    std::cout << "uptree: [";
    for(int i = 0; i < size_; i++) {
        std::cout << " " << i << ":" << uptree_[i] << " ";
    }
    std::cout << "]\n";
}

template <typename T>
void DisjointSets<T>::print_sets() {
    //TODO: print sets in linear order fashion, can be achieved using a queue and a memory set
    //this implementation is O(n^2), be careful
    std::unordered_set<int> roots; 
    for(int i = 0; i < size_; i++) { //first scan: keep the roots
        if(uptree_[i] < 0) roots.insert(i);
    }
    std::cout << " {";
    for(const int& root : roots) {
        std::cout << "{";
        for(int i = 0; i < size_; i++) {
            if(_find_repidx(i) == root) {
                std::cout << " " << i << " ";
            }
        }
        std::cout << "} ";
    }
    std::cout << "}\n";
}