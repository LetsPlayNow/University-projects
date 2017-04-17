//
// Created by aleksey on 16.10.16.
//

#ifndef PARALLELPRACTICE_2_TASK2_H
#define PARALLELPRACTICE_2_TASK2_H
#include <math.h>
#include <iostream>
#include <omp.h>
#include <string>
#include <fstream>
#include "unistd.h"
#include <vector>

namespace Task2
{
    extern omp_lock_t cout_lock;
    extern omp_lock_t task_lock;
    extern double delay_seconds;

    void delay();
    void MonatomicOutput(std::string str);

    typedef void (*Task2)(int n);

    void Task2Serial(int n);

    void Task2Parallel(int n);

    void Task2ParallelLocks(int n);

    void Task2Tasks(int n);

    void TestTask2(int n);
}

#endif //PARALLELPRACTICE_2_TASK2_H
