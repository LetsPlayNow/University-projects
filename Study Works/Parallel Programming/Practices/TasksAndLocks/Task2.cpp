//
// Created by aleksey on 16.10.16.
//
#include "Task2.h"
#include "Task1.h"

namespace Task2
{
    omp_lock_t cout_lock;
    omp_lock_t task_lock;
    double delay_seconds = 0.1;

    void MonatomicOutput(std::string str) {
        omp_set_lock(&cout_lock);
        std::cout << str << std::endl;
        omp_unset_lock(&cout_lock);
    }

    void Read(int i) {
        //MonatomicOutput("Read " + std::to_string(i));
    }

    void Write(int i) {
        //MonatomicOutput("Write " + std::to_string(i));
    }

    void Calculate(int i) {
        //MonatomicOutput("Calculate " + std::to_string(i));
        delay();
    }

    void delay() { usleep(delay_seconds * 1000); }

    void Serial(int n) {
        for (int i = 0; i < n; i++) {
            Read(i);
            Calculate(i);
            Write(i);
        }
    }

    void ParallelCycle(int n) {
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            Read(i);
            Calculate(i);
            Write(i);
        }
    }

    void ParallelLocks(int n) {
        #pragma omp parallel
        {
            int j = omp_get_thread_num();
            Read(j);
            Calculate(j);
            Write(j);
        }
    }

    void ParallelTasks(int n) {
        #pragma omp parallel
        {
            #pragma omp single
            {
                for (int i = 0; i < n; i++)
                {
                    #pragma omp task
                    {
                        Read(i);
                        #pragma omp task
                        {
                            Calculate(i);
                            #pragma omp task
                            {
                                Write(i);
                            }
                        }
                    }
                }
            }
        }
    }

    void TestTask2(int n) {
        std::cout << "Task 2. Много поставщиков, много потребителей" << std::endl;
        omp_set_num_threads(n);
        std::vector<Task2> tasks { Serial, ParallelCycle, ParallelLocks, ParallelTasks };
        std::vector<std::string> task_names { "Serial", "ParallelCycle", "Parallel with locks", "Tasks" };
        for (int i = 0; i < tasks.size(); i++)
        {
            std::cout << task_names[i] << std::endl;
            double start = omp_get_wtime();
            tasks[i](n);
            double end = omp_get_wtime();
            std::cout << "Time: " << end - start;
            std::cout << std::endl << std::endl;
        }
    }
}
