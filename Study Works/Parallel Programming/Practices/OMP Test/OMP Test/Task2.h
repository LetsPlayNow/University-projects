#pragma once
#include <omp.h>
#include <vector>
#include <iostream>


template<class T>
void print_vector(std::vector<T> vec) {
    for (unsigned int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << ' ';
    }
}


// Returns time of execution
double bubble_sort(std::vector<double> a) {
    int n = a.size();
    double start, end;
    bool array_sorted = false;
    
    start = omp_get_wtime();
    
    while (!array_sorted) {
        array_sorted = true;
        for (int i = 0; i < n-1; i++) {
            if (a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
                array_sorted = false;
            }
        }
    }
    end = omp_get_wtime();
    
    return end - start;
}

void bubble_sort_range(std::vector<double> & a, int start_index, int end_index) {
    bool array_sorted = false;

    while (!array_sorted) {
        array_sorted = true;
        for (int i = start_index; i <= end_index; i++) {
            if (a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
                array_sorted = false;
            }
        }
    }
}

// TODO use iterators here to pass merge areas in this func
void merge_arrays(std::vector<double> & a, int first_start, int first_end, int second_start, int second_end) {
    std::vector<double> temp(second_end - first_start + 1);
    int i = first_start, j = second_start;
    int k = 0;
    while (i <= first_end && j <= second_end) {
        if (a[i] < a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        } 
    }

    i--; j--; k--;

    // Copy lasts elements from first or second part
    if (i == first_end) {
        while (j <= second_end) {
            temp[k++] = a[j++];
        }
    }
    else if (j == second_end) {
        while (i <= second_end) {
            temp[k++] = a[i++];
        }
    }

    // Copy temp to a[first_start, second_end]
    
    for (int l = 0; l < temp.size(); l++) {
        a[first_start+l] = temp[l];
    }
}

// Returns time of execution
double merge_sort(std::vector<double> a, int blocks_count = 4) {
    int n = a.size();

    double start, end;
    start = omp_get_wtime();

    int block_size = n / blocks_count;
    #pragma omp parallel for 
    for (int i = 0; i < blocks_count-1; i++) {
        bubble_sort_range(a, i*block_size, (i + 1)*block_size-1);
    }

    // Merge (only for blocks_count % 2 == 0)
    //if (blocks_count % 2 != 0) return -1.0;
    //while (blocks_count > 1) {
    //    for (int i = 0; i < blocks_count; i += 2) {
    //        int block_size = n / blocks_count;
    //        // merge_arrays(a, i*block_size, (i + 1)*block_size-1, 
    //                        //(i + 1)*block_size, (i+2)*block_size - 1);
    //    }
    //    blocks_count /= 2;
    //}

    /*
    m - count of blocks
    O_1 (n^2)
    O_m ((n/m)^2)=n^2/m^2
    Speed_up = (O_m / O_1) = m^2
    */

    end = omp_get_wtime();

    return end - start;
}


// Sort array length of n
double task2(int n, int blocks_count) {
    std::cout << "Matrices" << "(" << n << ";" << n << ") " << "multiplication" << std::endl;
    std::vector<double> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = rand();
    }

    double bubble_sort_time = bubble_sort(a);
    double merge_sort_time = merge_sort(a, blocks_count);
    std::cout << "Bubble sort continious for vector size of " << n << " lasts: "
        << bubble_sort_time << "(c)" << std::endl;
    std::cout << "Merge sort for vector size of " << n << " with " << blocks_count << " tasks lasts: "
        << merge_sort_time << "(c)" << std::endl;

    double speed_up = bubble_sort_time / merge_sort_time;
    std::cout << "Speed up: " << speed_up << std::endl;
    return speed_up;
}

#include <fstream>
void test_task2() {
    std::cout << "Test task 2" << std::endl;
    std::ofstream file("task2_results.tsv");
    int n = pow(10, 3);
    std::vector<int> blocks_counts = {4, 8, 16, 32};
    for (int i = 0; i < blocks_counts.size(); i++) {
        double speed_up = task2(n, blocks_counts[i]);
        double effectivity = speed_up / blocks_counts[i];
        std::cout << "Effectivity is " << effectivity << std::endl;
        file << blocks_counts[i] << '\t' << effectivity << '\t\t';
        std::cout << std::endl;
    }
    
    file.close();
}