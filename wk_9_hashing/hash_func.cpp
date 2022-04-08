#include <iostream>
#include <ctime>
#include <string>
#include <algorithm>
#include <unordered_map>

using std::cout;
using std::endl;
using std::cerr;
using std::cin;
using std::string;
using std::unordered_map;

/*
    hashing function: reducing certain procedures into quick short time operations
    hashing = define a keyspace and goal is to make the keyspace into a range of int
    e.g. locker number --> student name, should be unique for each key
    map number input into a fixed sized range, this mapping is the hash function
    Hash table has: 
    1. function maps key types to an unique [array index]
    2. an array to store in the [dictionary]
    3. collision handling
*/

//naive thoughts:
//hashing value decided by name initials, onto (surjective)
int hash1(string name);

//dice rolling function based on rng -- petals around the rose
//e.g. 3, 5 has petals of 2, 5 has petals of 4, hashing val is combinations of 2 and 4
//problem: not representing the number range well (2,4,... only even numbers)
int hash2(string name);


/*
    hash function consists two parts:
    hash: transforming input into an integer, not worrying the balance of integer
    compression: hash func can be compressed in the range of N
    (self-inventing hash function are usually hard to produce and weak.)

    good hash function has characteristics:
    1. computation time is in O(1) 
    2. deterministic, hashing the same piece of data returns the same number.
    3. satisfies the SUHA (simple uniform hashing assumption): result of hash algo 
    must be uniform across all keys in keyspace: P(h(a)) == P(h(b)) == 1/N if a!=b, N=|keyspace|

    sample small size vs. production code --> all keyspaces (maybe a union of keyspaces), defensive programming
    map of a string of arbitrary length into an 8-char string: good with text corpus
    but if format is url: all first 8 characters are the same https://

    collision handling: view array elements as linked lists
    S = {16,8,4,13,29,11,22}, |S|=n=7 will produce:
    idx
    0
    1  22 -> 29 -> 8
    2  16
    3
    4  11 -> 4
    5
    6  13
    load factor of the table: num of elems into the table / array size
    generally load factor grows, amount to retrieve a certain element grows as well.
    
    load factor < 1 does not guarantee immediate retrieval:
    in the previous example it has load factor of 1
    but still has couple of elements retrieving in >1 time
    
    Double hashing & probing:
    when collide: look at next location in the array, write to the empty slot
    idx
    0  22 (relocate from 1)
    1  8
    2  16
    3  29 (relocate from 1)
    4  4
    5  11 (relocate from 4)
    6  13
    handling the collision at worst case takes O(N)

    primary cluster: set of values all in a simple slot, rest be sparse.
    double hashing: 2nd hashing function:
    try h(k) = (k+0*h_2(k)) % 7, if full...
    generally: h(k, i) = (h_1(k) + i*h_2(k))%7
    set h2 = 5 - (k % 5) where 5 and 7 coprime

    29%7=1, apply double hashing 29%5=4, 5-4=1 as step function
    step size = 2, (1 + 2*1)%7 = 3

    11%7=4, 5-11%5=4, (4 + 2*4) % 7 = 5
    idx
    0  22
    1  8
    2  16
    3  29
    4  4
    5  11
    6  13

    as load factor increases, the running time increases as well (hard to find correct steps)
    running time proportional to the load factor, not the data size itself
    graph the expected collision value to show: load factor <= 0.6 makes O(1) performance in hash computation
    when array resize: do not copy elements in place, change modulo
    instead: rehashing, original spot is not the new spot when doubled its size, rehash every single value

    depending the application build, use llist pointer to do separate chaining or store the hash inside the array?
    when data size is big, use ptr, if small size we can use array, trade between collision and accessing

    nearest-neighbor search --> AVL & KDTree, NOT in hash table O(n)
    dictionary --> AVL & hash table
*/

int hash(int arg);
int h1(int arg, int mod);
int h2(int arg, int stepmod);

#define N 3000
#define STEP_LENGTH 17 // this better to be a prime
int hashArray[N];

int main() {
    std::fill_n(hashArray, N, -1); //pre-init to -1 to show this is empty slot
    srand(time(NULL));
    int test_size = 0.95 * N;
    int total_operations_made = 0;
    cout << "our test size is " << test_size << "\n";
    for(int i = 0; i < test_size; i++) {
        double load_factor = i / (double) test_size;
        int collision_time = hash(rand());
        total_operations_made += collision_time;
        if(i % 100 == 0) { //report every 100 epochs
            cout << "at size " << i << " the load factor is " << load_factor << "\n";
            cout << "and the total operations made is counted: " << total_operations_made << " times.\n"; 
        }
    }

    // in C++, a hash table (hash map) is implemented as unordered_map.h, 
    // has no iterator as ::lower_bound() nor ::upper_bound() -- range-finding abilities
    // while map.h is implemented by a R-B tree in O(log n) insert/find/remove
    // ::load_factor(), return the load_factor of the hash table
    // can also set ::max_load_factor, can be even > 1.0
    cout << "Testing std::unordered_map: \n";
    unordered_map<int ,string> checker;
    checker.max_load_factor(0.95);
    for(int i = 0; i < test_size; i++) {
        int mykey = rand();
        checker.insert({mykey, std::to_string(mykey)});
    }

    int counter = 0;
    for(auto& p : checker) {
        if(counter < 50) cout << "[" << p.first << ":" << p.second << "]\n";
        counter++;
    }
    cout << "The hash table has " << checker.bucket_count() << " slots in its internal array\n";
    cout << "Success!\n";
    return 0;
}

int hash(int arg) { //return the number of relocations to find empty spot.
    int stepsize = 0; //this also measures the collision time happened in placing the arg
    //handling collision, increases stepsize
    //WARNING, may be infinite loop
    while(hashArray[(h1(arg, N) + stepsize*(STEP_LENGTH - h2(arg, STEP_LENGTH))) % N] != -1 && stepsize < N) {
        //cout << (h1(arg, N) + stepsize*(STEP_LENGTH - h2(arg, STEP_LENGTH))) % N << "\n";
        stepsize++;
    }
    if(stepsize >= N) {
        cerr << "Hashing failed, trapped in infinite loop, arg write to array canceled\n";
        return stepsize+1;
    }
    hashArray[(h1(arg, N) + stepsize*(STEP_LENGTH - h2(arg, STEP_LENGTH))) % N] = arg;
    return stepsize + 1;
}

int h1(int arg, int mod) { //O(1) operation, count as 1
    return arg % mod;
}

int h2(int arg, int stepmod) { //O(1) operation, count as 1
    return arg % stepmod;
}