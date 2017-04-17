//
// Created by aleksey on 03.11.16.
//

#ifndef TASKSOPENMP_2A_H
#define TASKSOPENMP_2A_H

#include "Tree.h"
#include <iostream>
#include <omp.h>

using namespace std;

// 2a
void node_process(struct node *tree)
{
    // Подсчитываем число узлов
    num_nodes++;
    // Связываем с каждым узлом какую-то работу
    // Работа имеет разную вычислительную сложность для различных вершин
    work(tree->num);
    // Выводим номер узла, который обработали
    //cout << tree->num << endl;
    if (tree->left)
        node_process(tree->left);
    if (tree->right)
        node_process(tree->right);
    return;
}

void task_2a(node * tree){
    cout << "2a) Последовательная обработка" << endl;
    clock_t start, finish; // переменные для измерения времени
    double time;
    start = clock();
    node_process(tree);
    finish = clock();
    time = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "Время последовательного варианта " << time << endl;
    cout << "Число вершин " << num_nodes << endl;
}

//2b
void node_process_parallel(struct node *tree)
{
    // Подсчитываем число узлов
    #pragma omp atomic
    num_nodes++;
    // Связываем с каждым узлом какую-то работу
    // Работа имеет разную вычислительную сложность для различных вершин
    work(tree->num);
    // Выводим номер узла, который обработали
    //cout << tree->num << " " << omp_get_thread_num() << endl;
    #pragma omp task
    if (tree->left)
        node_process_parallel(tree->left);
    #pragma omp task
    if (tree->right)
        node_process_parallel(tree->right);
    return;
}


void task_2b(node * tree)
{
    cout << "2a) Последовательный и параллельный варианты" << endl;
    clock_t start, finish; // переменные для измерения времени
    double time1, time2;
    start = clock();
    node_process(tree);
    finish = clock();
    time1 = (double)(finish - start)/CLOCKS_PER_SEC;

    start = clock();
    #pragma omp parallel
    {
        #pragma omp single
        {
            node_process_parallel(tree);
        }
    }
    finish = clock();
    time2 = (double)(finish - start)/CLOCKS_PER_SEC;
    cout << "Время последовательного варианта " << time1 << endl;
    cout << "Время параллельного варианта " << time2 << endl;
}


// 2c
void task_2c(node * tree)
{
    cout << "2в) Параллельный вариант без 'pragma omp single'" << endl;
    clock_t start, finish; // переменные для измерения времени
    double time1, time2;
    start = clock();
    node_process(tree);
    finish = clock();
    time1 = (double)(finish - start)/CLOCKS_PER_SEC;
    start = clock();
    #pragma omp parallel
    {
        {
            node_process_parallel(tree);
        }
    }
    finish = clock();
    time2 = (double)(finish - start)/CLOCKS_PER_SEC;
    cout << "Время последовательного варианта " << time1 << endl;
    cout << "Время параллельного варианта " << time2 << endl;
}


// 2d
void node_process_parallel_rising(struct node *tree)
{
    // Подсчитываем число узлов
    #pragma omp atomic
    num_nodes++;

    // Выводим номер узла, который обработали
    //cout << tree->num << " " << omp_get_thread_num() << endl;
    #pragma omp task
    if (tree->left)
        node_process_parallel(tree->left);
    #pragma omp task
    if (tree->right)
        node_process_parallel(tree->right);

    // Связываем с каждым узлом какую-то работу
    // Работа имеет разную вычислительную сложность для различных вершин
    work(tree->num);

    return;
}


void task_2d(node * tree)
{
    cout << "2г) Восходящий и нисходящий варианты" << endl;
    clock_t start, finish; // переменные для измерения времени
    double time1, time2;
    start = clock();
    cout << "Нисходящая обработка" << endl;
    #pragma omp parallel
    {
        #pragma omp single
        {
            node_process_parallel(tree);
        }
    }
    finish = clock();
    time1 = (double)(finish - start)/CLOCKS_PER_SEC;


    start = clock();
    cout << "Восходящая обработка" << endl;
    #pragma omp parallel
    {
        #pragma omp single
        {
            node_process_parallel_rising(tree);
        }
    }
    finish = clock();
    time2 = (double)(finish - start)/CLOCKS_PER_SEC;
    cout << "Время нисходящей параллельной обработки " << time1 << endl;
    cout << "Время восходящей параллельной обработки " << time2 << endl;
}

#endif //TASKSOPENMP_2A_H