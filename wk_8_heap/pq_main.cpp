#include "PriorityQueue.h"
#include "Cube.h"
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <ctime>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stack;

const int SMALL_TEST_SIZE = 1024; //1 thousand of cubes, the capacity will be 2k cubes since the 0th index is a buffer
const int LARGE_TEST_SIZE = 1048576; //1 million of cubes, the capacity will be 2M cubes since 0th idx is buffer

int main() {
    PriorityQueue<int> heap;

    cout << " === 10 calls to heap.insert() === " << endl;
    heap.insert(4);
    heap.insert(10);
    heap.insert(2);
    heap.insert(22);
    heap.insert(45);
    heap.insert(18);
    heap.insert(-8);
    heap.insert(95);
    heap.insert(13);
    heap.insert(42);
    cout << "The minimum element should be -8: " << heap.peek() << endl;
    cout << "The printHeap() function should print the heaps in correct order without trailing zeros "<< endl;
    heap.printHeap();

    cout << " === 6 calls to heap.removeMin() === " << endl;
    for(int i = 0; i < 6; i++) {
        heap.removeMin();
    }
    cout << "The minimum element should be 22: " << heap.peek() << endl;
    cout << "The printHeap() function should print the heaps in correct order without trailing zeros "<< endl;
    heap.printHeap();

    cout << "\n === Testing custom type comparable: Cube === " << endl;
    //require cubes themselves to be stack memory
    PriorityQueue<Cube> cubeLiner;
    cout << " === 10 calls to heap.insert() === " << endl;
    for(int i = 0; i < 10; i++) {
        double custom_length = (10 - i) / 25.7;
        Cube c = Cube(custom_length);
        cubeLiner.insert(c);
    }
    cout << "The minimum element should be closest to 0: " << cubeLiner.peek() << endl;
    cout << "The printHeap() function should print the heaps in correct order without trailing zeros "<< endl;
    cubeLiner.printHeap();

    cout << " === 6 calls to heap.removeMin() === " << endl;
    for(int i = 0; i < 6; i++) {
        cubeLiner.removeMin();
    }
    cout << "The minimum element should be 0.27ish: " << cubeLiner.peek() << endl;
    cout << "The printHeap() function should print the heaps in correct order without trailing zeros "<< endl;
    cubeLiner.printHeap();

    cout << "\n === Overall testing (and testing buildHeap() in linear time)! === " << endl;
    cout << "Loading objects...\n";
    Cube cubies[SMALL_TEST_SIZE];
    for(int i = 0; i < SMALL_TEST_SIZE; i++) {
        cubies[i] = Cube(SMALL_TEST_SIZE - i);
    }
    cout << "Build a small cube priority queue of size 1024 UwU\n";
    PriorityQueue<Cube> small(cubies, SMALL_TEST_SIZE);
    cout << "The smallest element should be 1: " << small.peek() << "\n";
    cout << "Removing half of its elements: \n";
    for(int i = 0; i < SMALL_TEST_SIZE / 2; i++) {
        small.removeMin();
    }
    cout << "The smallest element should be 513: " << small.peek() << "\n";
    cout << "\nTesting copy ctor and copy assignment op are deep copies (voided)\n";
    cout << "Deploy copy ctor: \n";
    //I realize that this program has limited stack space for each thread, could be less than a couple megabytes
    //sadgee.
    PriorityQueue<Cube>* small2 = new PriorityQueue<Cube>(small);
    small2->removeMin();
    cout << "The smallest element of original pque should still be 513: " << small.peek() << "\n";
    cout << "The smallest element of the copied pque should be 514: " << small2->peek() << "\n";
    cout << "Deploy assignment operator\n";
    small = *small2;
    cout << "The smallest element of original pque should be 514 by now: " << small.peek() << "\n";
    delete small2;

    //for the array build on a heap, the individual elements do not need to declare new piece of memory
    //remember that new means malloc, if new [] malloc'd enough space no need to do that again
    //(does not follow the copy assignment operator schema)
    //e.g. int* arr = new int[10]; arr[1] = 1, not arr[1] = new int(1)
    //here we use vector instead and vector may use something different to optimize stack space so don't worry
    cout << "\n === Build a large heap on heap! === " << endl;
    vector<Cube> cargo;
    cargo.reserve(LARGE_TEST_SIZE);
    srand(time(NULL));
    cout << "Deploying objects based on rng values: \n";
    double minranda = LARGE_TEST_SIZE;
    for(int i = 0; i < LARGE_TEST_SIZE; i++) {
        int rando = rand() % LARGE_TEST_SIZE;
        double randa = (double) rando / 1000.0;
        if(randa < minranda) minranda = randa;
        cargo.push_back(Cube(randa));
    }
    cout << "Size of the source vector: " << cargo.size() << "\n";
    cout << "Deploy a new pqueue from building on vector\n";
    PriorityQueue<Cube>* large = new PriorityQueue<Cube>(cargo);
    cout << "The minimum element should has sidelength of minranda " << minranda 
        << " :" << std::boolalpha << (minranda == large->peek().getLength()) << "\n";
    cout << "Popping elements to the small array: \n";
    stack<Cube> trashCollector;
    for(int i = SMALL_TEST_SIZE; i < LARGE_TEST_SIZE; i++) {
        trashCollector.push(large->removeMin());
    }
    cout << "trashCollector should has a relatively large top value: " << trashCollector.top() << "\n";
    cout << "and, the leftover min value should be a little bit larger than the value above " << large->peek() << "\n";
    delete large;
    cout << "Test done.\n";
    cout << "Success!\n";
    return 0;
}