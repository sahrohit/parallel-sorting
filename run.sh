#!/bin/bash

# Build All Executables
make all

# Array of executables to test
EXECUTABLES=("qss" "qsp" "mss" "msp" "bbs" "bbp" "reference")

# Array of input sizes to test
SIZES=(2500 5000 7500 10000 25000 50000 75000 100000 250000 500000)

# Array of thread counts to test for parallel versions
THREAD_COUNTS=(2 4 8 16 32)

# Fixed seed for reproducibility
SEED=42

# Output file
OUTPUT_FILE="benchmark_results.csv"

# Create or overwrite the output file with headers
echo "Algorithm,Input Size,Threads,Runtime" > $OUTPUT_FILE

# Function to extract runtime from program output
get_runtime() {
    echo "$1" | grep "elapsed time:" | awk '{print $3}'
}

# Run benchmarks
for size in "${SIZES[@]}"; do
    echo "Running benchmarks for size $size..."
    
    for exe in "${EXECUTABLES[@]}"; do
        # For parallel versions, test different thread counts
        if [[ $exe == *"p"* ]]; then
            for threads in "${THREAD_COUNTS[@]}"; do
                echo "  Running $exe with $threads threads..."
                output=$(./$exe $size $threads $SEED 2>&1)
                runtime=$(get_runtime "$output")
                echo "$exe,$size,$threads,$runtime" >> $OUTPUT_FILE
            done
        else
            # For serial versions, just run once with thread count 1
            echo "  Running $exe..."
            output=$(./$exe $size 1 $SEED 2>&1)
            runtime=$(get_runtime "$output")
            echo "$exe,$size,1,$runtime" >> $OUTPUT_FILE
        fi
    done
done

echo "Benchmarking complete. Results saved to $OUTPUT_FILE"

# Create summary statistics
echo -e "\nGenerating summary..."
echo -e "\nAverage runtimes by algorithm and input size:"
awk -F',' '
    NR>1 {
        sum[$1","$2]+=$4; 
        count[$1","$2]++
    } 
    END {
        for (key in sum) {
            split(key, parts, ",");
            printf "%-10s Size: %-10s Avg Time: %f\n", parts[1], parts[2], sum[key]/count[key]
        }
    }' $OUTPUT_FILE
