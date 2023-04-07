/*
    Class for optimized array methods using multithreading
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

using namespace std;

class FastArray {
    int *array;
    int size;
    bool result;

    public:
    FastArray(int *array, int size) {
        this->array = array;
        this->size = size;
    }

    bool valueExists(int value) {
        this->result = false;
        vector<int> startIndices;
        vector<int> endIndices;
        int currIndex = 0;
        int increment = sqrt(size);
        while (currIndex < size) {
            startIndices.push_back(currIndex);
            currIndex += increment;
            endIndices.push_back(min(currIndex, size - 1));
        }
        int numThreads = startIndices.size();
        for (int i = 0; i < numThreads; i++) {
            int arg1 = startIndices[i];
            int arg2 = endIndices[i];
            int arg3 = value;
            thread t(&FastArray::valueExistsInRange, this, arg1, arg2, arg3);
            t.join();
        }
        return this->result;
    }

    void valueExistsInRange(int startIndex, int endIndex, int value) {
        for (int currIndex = startIndex; currIndex < endIndex; currIndex++) {
            if (this->array[currIndex] == value) {
                this->result = true;
                return;
            }
        }
    }
};

int main() {
    int* array = new int[10];
    for (int i = 0; i < 10; i++) {
        array[i] = i;
    }
    FastArray fastarray(array, 10);
    cout << fastarray.valueExists(5) << endl;
    cout << fastarray.valueExists(11) << endl;
}