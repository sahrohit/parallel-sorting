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

void bubbleSortParallel(int arr[], int n) {
    bool sorted = false;
    int tmp;
    
    while (!sorted) {
        sorted = true;
        
        #pragma omp parallel private(tmp)
        {
            // Even phase
            #pragma omp for reduction(&&:sorted)
            for (int i = 0; i < n-1; i += 2) {
                if (arr[i] > arr[i+1]) {
                    tmp = arr[i];
                    arr[i] = arr[i+1];
                    arr[i+1] = tmp;
                    sorted = false;
                }
            }
            
            // Odd phase
            #pragma omp for reduction(&&:sorted)
            for (int i = 1; i < n-1; i += 2) {
                if (arr[i] > arr[i+1]) {
                    tmp = arr[i];
                    arr[i] = arr[i+1];
                    arr[i+1] = tmp;
                    sorted = false;
                }
            }
        }
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
    
    bubbleSortParallel(array, size);
    
    S1.stop();
    
    std::cerr << "elapsed time: " << S1.getTime() << std::endl;
    
    delete[] array;
    return 0;
}


