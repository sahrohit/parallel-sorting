#include <chrono>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <omp.h>

class stopwatch {
private:
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    bool timing;
public:
    stopwatch(): timing(false) {
        t1 = std::chrono::high_resolution_clock::time_point::min();
        t2 = std::chrono::high_resolution_clock::time_point::min();
    }
    void start() {
        if (!timing) {
            timing = true;
            t1 = std::chrono::high_resolution_clock::now();
        }
    }
    void stop() {
        if (timing) {
            t2 = std::chrono::high_resolution_clock::now();
            timing = false;
        }
    }
    void reset() {
        t1 = std::chrono::high_resolution_clock::time_point::min();
        t2 = std::chrono::high_resolution_clock::time_point::min();
        timing = false;
    }
    double getTime() {
        std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
        return elapsed.count();
    }
};

int* randNumArray(const int size, const int seed) {
    srand(seed);
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = std::rand() % 1000000;
    }
    return array;
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    int* L = new int[n1];
    int* R = new int[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    i = 0;
    j = 0;
    k = l;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}
 
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
 
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}

int main(int argc, char** argv) {
    int* array;
    int size, seed, numThreads;

    if (argc < 4) {
        std::cerr << "usage: " << argv[0]
                  << " [amount of random nums to generate] [number of threads to use] [seed value for rand]"
                  << std::endl;
        exit(-1);
    }

    {
        std::stringstream ss1(argv[1]);
        ss1 >> size;
    }
    {
        std::stringstream ss1(argv[2]);
        ss1 >> numThreads;
    }
    {
        std::stringstream ss1(argv[3]);
        ss1 >> seed;
    }

    array = randNumArray(size, seed);

    stopwatch S1;
    S1.start();
    
    mergeSort(array, 0, size - 1);
    
    S1.stop();
    
    std::cerr << "elapsed time: " << S1.getTime() << std::endl;
    
    delete[] array;
    return 0;
}