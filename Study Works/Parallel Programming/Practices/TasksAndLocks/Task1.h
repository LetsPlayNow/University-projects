//
// Created by aleksey on 16.10.16.
//

#ifndef PARALLELPRACTICE_2_TASK1_H
#define PARALLELPRACTICE_2_TASK1_H
#include <math.h>
#include <iostream>
#include <omp.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <vector>

namespace Task1
{

    extern omp_lock_t cout_lock;
    extern omp_lock_t task_lock;
    extern double delay_seconds;

    void delay();
    void MonatomicOutput(std::string str);

    typedef void (*Task1)(int n);
    void Task1Serial(int n);
    void Task1Parallel(int n);
    void Task1ParallelLocks(int n);
    void Task1Tasks(int n);

    void TestTask1(int n);
}

#endif //PARALLELPRACTICE_2_TASK1_H
