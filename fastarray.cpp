/*
    Class for optimized array methods using multithreading
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>

using namespace std;

mutex g_lock;

class FastArray {
    int *array;
    int size;
    int result;

    public:
    FastArray(int *array, int size) {
        this->array = array;
        this->size = size;
    }

    bool valueExists(int value) {
        this->result = false;
        pair<vector<int>, vector<int>> startEndIndices = this->startEndIndices(this->size);
        vector<int> startIndices = startEndIndices.first;
        vector<int> endIndices = startEndIndices.second;
        int numThreads = startIndices.size();
        vector<thread> threads;
        for (int i = 0; i < numThreads; i++) {
            int arg1 = startIndices[i];
            int arg2 = endIndices[i];
            int arg3 = value;
            threads.push_back(thread (&FastArray::valueExistsInRange, this, arg1, arg2, arg3));
        }
        for (int i = 0; i < numThreads; i++) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
        return this->result;
    }

    int sum() {
        this->result = 0;
        pair<vector<int>, vector<int>> startEndIndices = this->startEndIndices(this->size);
        vector<int> startIndices = startEndIndices.first;
        vector<int> endIndices = startEndIndices.second;
        int numThreads = startIndices.size();
        vector<thread> threads;
        for (int i = 0; i < numThreads; i++) {
            int arg1 = startIndices[i];
            int arg2 = endIndices[i];
            threads.push_back(thread (&FastArray::sumInRange, this, arg1, arg2));
        }
        for (int i = 0; i < numThreads; i++) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
        return this->result;
    }

    int minValue() {
        this->result = INT_MAX;
        pair<vector<int>, vector<int>> startEndIndices = this->startEndIndices(this->size);
        vector<int> startIndices = startEndIndices.first;
        vector<int> endIndices = startEndIndices.second;
        int numThreads = startIndices.size();
        vector<thread> threads;
        for (int i = 0; i < numThreads; i++) {
            int arg1 = startIndices[i];
            int arg2 = endIndices[i];
            threads.push_back(thread (&FastArray::minValueInRange, this, arg1, arg2));
        }
        for (int i = 0; i < numThreads; i++) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
        return this->result;
    }

    int maxValue() {
        this->result = INT_MIN;
        pair<vector<int>, vector<int>> startEndIndices = this->startEndIndices(this->size);
        vector<int> startIndices = startEndIndices.first;
        vector<int> endIndices = startEndIndices.second;
        int numThreads = startIndices.size();
        vector<thread> threads;
        for (int i = 0; i < numThreads; i++) {
            int arg1 = startIndices[i];
            int arg2 = endIndices[i];
            threads.push_back(thread (&FastArray::maxValueInRange, this, arg1, arg2));
        }
        for (int i = 0; i < numThreads; i++) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
        return this->result;
    }

    private:
    pair<vector<int>, vector<int>> startEndIndices(int size) {
        vector<int> startIndices;
        vector<int> endIndices;
        int currIndex = 0;
        int increment = sqrt(size);
        while (currIndex < size) {
            startIndices.push_back(currIndex);
            currIndex += increment;
            endIndices.push_back(min(currIndex, size));
        }
        return make_pair(startIndices, endIndices);
    }

    void valueExistsInRange(int startIndex, int endIndex, int value) {
        bool found = false;
        for (int currIndex = startIndex; currIndex < endIndex; currIndex++) {
            if (this->array[currIndex] == value) {
                found = true;
                break;
            }
        }
        if (found) {
            g_lock.lock();
            this->result = true;
            g_lock.unlock();
        }
    }

    void sumInRange(int startIndex, int endIndex) {
        int segmentSum = 0;
        for (int currIndex = startIndex; currIndex < endIndex; currIndex++) {
            segmentSum += this->array[currIndex];
        }
        g_lock.lock();
        this->result += segmentSum;
        g_lock.unlock();
    }

    void minValueInRange(int startIndex, int endIndex) {
        int segmentMin = INT_MAX;
        for (int currIndex = startIndex; currIndex < endIndex; currIndex++) {
            segmentMin = min(segmentMin, this->array[currIndex]);
        }
        g_lock.lock();
        this->result = min(this->result, segmentMin);
        g_lock.unlock();
    }

    void maxValueInRange(int startIndex, int endIndex) {
        int segmentMax = INT_MIN;
        for (int currIndex = startIndex; currIndex < endIndex; currIndex++) {
            segmentMax = max(segmentMax, this->array[currIndex]);
        }
        g_lock.lock();
        this->result = max(this->result, segmentMax);
        g_lock.unlock();
    }
};

int main() {
    int* array = new int[10000];
    for (int i = 0; i < 10000; i++) {
        array[i] = i;
    }
    FastArray fastArray(array, 10000);
    cout << "Result is " << fastArray.valueExists(9) << endl; // expect true
    cout << "Reuslt is " << fastArray.valueExists(10) << endl; // expect false
    cout << "Result is " << fastArray.sum() << endl; // expect 45
    cout << "Result is " << fastArray.minValue() << endl; // expect 0
    cout << "Result is " << fastArray.maxValue() << endl; // expect 9
    return 0;
}