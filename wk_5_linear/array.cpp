#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;

int main() {
    //array stores data in contiguous memory space
    //'initializer list' - kind of syntax 
    int vals[10] = {2,4,5,7,11,13,17,18,20,25};
    //size of the type of data is known, offset is manually calculable
    //pointer arithmetic
    cout << sizeof(int) << endl;
    int offset = (long) &(vals[2]) - (long) &(vals[0]);
    cout << "offset byte width between elem 2 and 0 is " << offset << "\n";
    
    //array is full when size of array is equal to capacity
    //copy data over when 'realloc' resize happening
    //dynamically growing: resizing when capacity is reached
    std::vector<int> ints {1,2,3};
    cout << "Initial capacity: " << ints.capacity() << endl;
    ints.push_back(5);
    cout << "Size after adding: " << ints.size() << endl;
    //doubled the initial capacity, if capa=0 after adding an elem becomes 1
    cout << "Capa after adding: " << ints.capacity() << endl;
    offset = (long) &(ints[2]) - (long) &(ints[0]);
    //in vector the data are sequentially stored so its core is an array
    cout << "Memory separation: " << offset << endl;

    /*
        resize strategy: add 2 capacity whenever is full:
        analysis: for resize occurrence 0,1,2,3,...(r=n/2): makes 0,2,4,6,8,...,2*r copies
        in total:  sum(2*r) = r^2+r = (n/2)^2+(n/2) = O(n^2) copies prop to n
        average work per element: O(n^2)/n = O(n) amortized (each adding elem expected copies)

        resize strategy: double the capacity whenever is full
        analysis: for resize occurrence 0,1,2,...(r=log(n)), makes 0,2,4,...,2^r copies
        sum(0,2,4,...2^r) = 2*(2^r - 1) = 2(2^log(n) - 1) = 2(n-1), O(n) copies prop to n
        average work per element: O(n)/n = O(1), amortized (each adding elem expected copies)
    */


    //2d arrays: essentially 1d in memory
    int arr[5][2] = {1,2,3,4,5,6,7,8,9,0};
    //alternatively:
    int arr2[5][2] = {{1,2}, {3, 4}, {5,6}, {7,8}, {9,0}};
    //string array of 2d char, remember sizeof (bytes) c-string is length+1
    // In sized arrays if we skip some values while initialization, then those values are automatically taken as 0 by compiler.
    char weekdays[7][11] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    cout << "buffer char: " << (int) weekdays[0][10] << "\n";
    // entire row is also inited to 0 = '\0'
    cout << "Saturday? " << weekdays[5] << " Not seen.\n";

    // in C++ we can skip the first dimension in array, the rest dimensions cannot be skipped.
    // If we skip the size of first dimension in array then we must provide all the initializers(values)
    // so that C++ can create the adequate memory for all values.
    int square[][2]={{1,1},{2,4},{3,9}};
    // value skipping is ok
    int broken[][2] = {{1,1},{2}, {}};
    // no substructure is also ok, they simply need 1 number to resolve the other
    int totallybroken[][2] = {1,1,2,4,3,8,9};

    //ptr, ptr of arrays, explicit 2d arrays -- all works
    int* ptrs[5]; //each row is a ptr to memo on heap
    for(int i = 0; i < 5; i++) {
        ptrs[i] = new int[i+1];//uneven 'arrays'
        for(int j = 0; j < i+1; j++) {
            ptrs[i][j] = j;
        }
        delete[] ptrs[i]; //clean up
    }

    //entirely uneven array: double ptrs, letting both layers of memory on heap
    srand(time(NULL));
    int height = rand() % 100 + 1;
    //stack mem pointing to contiguous memory region on heap of size(int)*height
    int** uneven = new int*[height]; 
    cout << "mem addr of entire array on heap: " << uneven << "\n";
    cout << "size of the uneven array: " << height << "\n";
    for(int i = 0; i < height; i++) {
        int width = rand() % 50 + 1;
        uneven[i] = new int[width];
        if(i < 10) {
            //even here you can see the heap alloc'd rows are tabbed with some memory, not entire word-to-word continuous.
            cout << "mem addr of row ptr " << i << ":" << uneven[i] << "\n";
            cout << "size of the uneven array row " << i << ":" << width << "\n";
        }
        for(int j = 0; j < width; j++) {
            uneven[i][j] = rand() % 1000;
        }
    }
    //also clean them one by one
    for(int i = 0; i < height; i++) {
        delete[] uneven[i];
    }
    delete[] uneven;
    return 0;
}