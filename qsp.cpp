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

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortParallel(int arr[], int low, int high, int depth = 0) {
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp task shared(arr) if(depth < 3)
        quickSortParallel(arr, low, pi - 1, depth + 1);
        
        #pragma omp task shared(arr) if(depth < 3)
        quickSortParallel(arr, pi + 1, high, depth + 1);
        
        #pragma omp taskwait
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
    
    omp_set_num_threads(numThreads);

    stopwatch S1;
    S1.start();
    
    #pragma omp parallel
    {
        #pragma omp single nowait
        quickSortParallel(array, 0, size - 1);
    }
    
    S1.stop();
    
    std::cerr << "elapsed time: " << S1.getTime() << std::endl;
    
    delete[] array;
    return 0;
}