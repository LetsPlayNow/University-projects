#pragma once
#include <iostream>
#include <omp.h>
#include <vector>

typedef std::vector<std::vector<double> > matrix;
double matrices_multiplication(int n, bool is_parallel = false) {
    double start, end;

    std::vector<double> row(n, 0.0);
    matrix a(n, row), b(n, row), c(n, row);
    start = omp_get_wtime();
    #pragma omp parallel for if(is_parallel == true)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i][j] = a[i][k] * b[j][k];
            }
        }
    }
    end = omp_get_wtime();

    return end - start;
}


double vectors_multiplication(int n, bool is_parallel = false) {
    double start, end;
 
    std::vector<double> a_vec(n*n, 1.0), b_vec(n*n, 1.0), c_vec(n*n, 0.0);
    start = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            #pragma omp parallel for if(is_parallel)
            for (int k = 0; k < n; k++) {
                c_vec[i*n + j] += a_vec[i*n + k] * b_vec[j*n + k]; // i*n + k
            }
        }
    }
    end = omp_get_wtime();

    return end - start;
}

// Matrix multiplication
// n - matrix dimension
double task1(int n) {
    std::cout << "Matrices" << "(" << n << ";" << n << ") " << "multiplication" << std::endl;
    double matrices_multiplication_time = matrices_multiplication(n);
    double vectors_multiplication_time = vectors_multiplication(n);
    std::cout << "Matrices multiplication with cash misses calculated for: " 
        << matrices_multiplication_time << std::endl;
    std::cout << "Matrices (in vector form) multiplication without cash misses calculated for: " 
        << vectors_multiplication_time << std::endl;

    double speed_up = matrices_multiplication_time / vectors_multiplication_time;
    std::cout << "Speed up: " << speed_up << std::endl;
    return speed_up;
}


#include <fstream>
void test_task1() {
    std::cout << "Test task 1" << std::endl;
    std::ofstream file("task1_results.tsv");
    std::vector<int> n = { 50, 100, 150, 200 };
    for (int i = 0; i < n.size(); i++) {
        double speed_up = task1(n[i]); 
        file << n[i] << '\t' << speed_up << '\t';
        std::cout << std::endl;
    }
    file.close();
}
