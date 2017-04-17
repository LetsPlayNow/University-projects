//
// Created by aleksey on 16.10.16.
//
#include "Task1.h"
#include <fstream>

namespace Task1 {
    omp_lock_t cout_lock;
    omp_lock_t task_lock;
    double delay_seconds = 0.1;
    int taskSize=10000;

    void MonatomicOutput(std::string str) {
        omp_set_lock(&cout_lock);
        std::cout << str << std::endl;
        omp_unset_lock(&cout_lock);
    }

    void Read(int i){
        // MonatomicOutput("Read " + std::to_string(i));
    }

    std::vector<int> ReadFile(int i) {
        std::vector<int> data(taskSize);
        std::ifstream file("files/" + std::to_string(i));
        for (int j = 0; j < taskSize; ++j)
            file >> data[j];

        return data;
    }

    void Write(int i) {
        // MonatomicOutput("Write " + std::to_string(i));
    }

    void WriteFile(int i, std::vector<int> & data){
        std::ofstream file("files/" + std::to_string(i));
        for (int j = 0; j < taskSize; ++j)
            file << data[j];
    }

    void Calculate(int i) {
        // MonatomicOutput("Calculate " + std::to_string(i));
        delay();
    }


    void InitFiles(int n){
        std::vector<int> data;
        for (int i = 0; i < taskSize; ++i)
            data[i] = i;

        for (int j = 0; j < n; ++j) {
            WriteFile(j, data);
        }
    }

    // Один поставщик - много потребителей
    // Последовательный вариант
    void Serial(int n) {
        for (int i = 0; i < n; i++) {
            Read(i);
            Calculate(i);
            Write(i);
        }
    }


    void ParallelCycle(int n) {
    #pragma omp parallel
        {
            #pragma omp single
            for (int i = 0; i < n; i++)
                Read(i);

            #pragma omp for
            for (int i = 0; i < n; i++) {
                Calculate(i);
                Write(i);
            }
        }
    }

    void ParallelWithLocks(int n) {
        std::vector<omp_lock_t> locks(n+1);
        #pragma omp parallel
        {
            int j = omp_get_thread_num();
            if (omp_get_thread_num() == 0) {
                for (int i = 0; i < n; i++) {
                    omp_init_lock(&locks[i]);
                    omp_set_lock(&locks[i]);
                }
            }

            #pragma omp barrier

            if (j == 0)
            {
                for (int i = 0; i < n; i++)
                {
                    Read(i);
                    omp_unset_lock(&locks[i]);
                }
            } else {
                omp_set_lock(&locks[j]);
                Calculate(j);
                Write(j);
                omp_unset_lock(&locks[j]);
            }
        }
    }


    void ParallelWithTasks(int n) {
        #pragma omp parallel
        {
            #pragma omp single
            {
                for (int i = 0; i < n; i++)
                {
                    Read(i);
                    #pragma omp task
                    {
                        Calculate(i);
                        Write(i);
                    }
                }
            }
        }
    }

    void TestTask1(int n) {
        std::cout << "Task 1. Один поставщик, много потребителей" << std::endl;
        std::cout << "Количество " << n << std::endl;
        omp_set_num_threads(n + 1);
        std::vector<Task1> tasks{Serial, ParallelCycle, ParallelWithLocks, ParallelWithTasks};
        std::vector<std::string> task_names{"Serial", "ParallelCycle", "Parallel with locks", "Tasks"};
        for (int i = 0; i < tasks.size(); i++) {
            std::cout << task_names[i] << std::endl;
            double start = omp_get_wtime();
            tasks[i](n);
            double end = omp_get_wtime();
            std::cout << "Time: " << end - start;
            std::cout << std::endl << std::endl;
        }
    }

    void delay() { usleep(delay_seconds * 1000); }
}