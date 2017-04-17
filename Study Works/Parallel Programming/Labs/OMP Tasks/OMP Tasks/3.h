//
// Created by aleksey on 28.11.16.
//

#ifndef TASKSOPENMP_3_H
#define TASKSOPENMP_3_H

#include <iostream>
#include <ctime>
#include <omp.h>
#include "Tree.h"
using namespace std;

int par, prec;
int found_count;

//Обрабатываем узел
void find(struct node *tree)
{
    // Подсчитываем число узлов
    num_nodes++;
    // Связываем с каждым узлом какую-то работу
    // Работа имеет разную вычислительную сложность для различных вершин
    //work(tree->num);
    // Выводим номер узла, который обработали
    //cout << tree->num << endl;
    if (abs(tree->num - par) <= prec)
        found_count += 1;

    if (tree->left)
        find(tree->left);
    if (tree->right)
        find(tree->right);
    return;
}

//Обрабатываем узел
void find_parallel(struct node *tree)
{
    // Подсчитываем число узлов
    #pragma omp atomic
    num_nodes++;
    // Связываем с каждым узлом какую-то работу
    // Работа имеет разную вычислительную сложность для различных вершин
    // work(tree->num);
    // Выводим номер узла, который обработали
    //cout << tree->num << " " << omp_get_thread_num() << endl;
    if (abs(tree->num - par) <= prec)
        #pragma omp atomic
        found_count += 1;

    #pragma omp task
    if (tree->left)
        find_parallel(tree->left);
    #pragma omp task
    if (tree->right)
        find_parallel(tree->right);
    return;
}

void task_3a(node *tree, int par, int prec){
    cout << "3a) Поиск элементов abs(N - " << par << ") < " << prec << endl;
    ::par = par;
    ::prec = prec;
    found_count = 0;
    num_nodes = 0;

    clock_t start, finish; // переменные для измерения времени
    double time1, time2;
    start = clock();
    find(tree);
    finish = clock();
    time1 = (double)(finish - start)/CLOCKS_PER_SEC;
    cout << "Time serial is " << time1 << endl;
    cout << "Число элементов (abs(N-par) <= prec) = " << found_count;
    found_count = 0;
    num_nodes = 0;

    start = clock();
    #pragma omp parallel
    {
    #pragma omp single
        {
            find_parallel(tree);
        }
    }
    finish = clock();
    time2 = (double)(finish - start)/CLOCKS_PER_SEC;

    cout << "Время выполнения параллельного варианта" << time2 << endl;
    cout << "Число вершин " << num_nodes << endl;
    cout << "Число элементов (abs(N-par) <= prec) = " << found_count;
}


//Обрабатываем узел
void find_parallel_atomic(struct node *tree)
{
    // Подсчитываем число узлов
    #pragma omp atomic
    num_nodes++;
    // Связываем с каждым узлом какую-то работу
    // Работа имеет разную вычислительную сложность для различных вершин
    // work(tree->num);
    // Выводим номер узла, который обработали
    //cout << tree->num << " " << omp_get_thread_num() << endl;
        if (abs(tree->num - par) <= prec)
            #pragma omp atomic
            found_count += 1;

#pragma omp task
    if (tree->left)
        find_parallel_atomic(tree->left);
#pragma omp task
    if (tree->right)
        find_parallel_atomic(tree->right);
    return;
}

void task_3b(node *tree, int par, int prec){
    cout << "3б) [Atomic] Поиск элементов abs(N - " << par << ") < " << prec << endl;
    ::par = par;
    ::prec = prec;
    found_count = 0;
    num_nodes = 0;

    clock_t start, finish; // переменные для измерения времени
    double time1, time2;
    start = clock();
    find(tree);
    finish = clock();
    time1 = (double)(finish - start)/CLOCKS_PER_SEC;
    cout << "Время посоледовательного варианта " << time1 << endl;
    cout << "Число вершин (abs(N-par) <= prec) = " << found_count;
    found_count = 0;
    num_nodes = 0;

    start = clock();
#pragma omp parallel
    {
#pragma omp single
        {
            find_parallel_atomic(tree);
        }
    }
    finish = clock();
    time2 = (double)(finish - start)/CLOCKS_PER_SEC;

    cout << "Время параллельного варианта (atomic) " << time2 << endl;
    cout << "Число вершин " << num_nodes << endl;
    cout << "Число вершин (abs(N-par) <= prec) = " << found_count;
}


int first_found_number = 0;
//Обрабатываем узел
void find_until_first(struct node *tree)
{
    // Подсчитываем число узлов
    #pragma omp atomic
    num_nodes++;
    // Связываем с каждым узлом какую-то работу
    // Работа имеет разную вычислительную сложность для различных вершин
    // work(tree->num);
    // Выводим номер узла, который обработали
    //cout << tree->num << " " << omp_get_thread_num() << endl;

    if (abs(tree->num - par) <= prec)
    {
        #pragma omp atomic
        found_count += 1;
        first_found_number = tree->num;
        return;
    }

#pragma omp task
    if (tree->left)
        find_until_first(tree->left);
#pragma omp task
    if (tree->right)
        find_until_first(tree->right);
    return;
}

void task_3ge(node * tree, int par, int prec){
    cout << "3ге) Последовательный и параллельный варианты поиска до 1 встретившейся вершины" << endl;
    ::par = par;
    ::prec = prec;
    found_count = 0;
    num_nodes = 0;
    first_found_number = -1;

    clock_t start, finish; // переменные для измерения времени
    double time1, time2;
    start = clock();
    find_until_first(tree);
    finish = clock();
    time1 = (double)(finish - start)/CLOCKS_PER_SEC;
    cout << "Последовательный вариант " << time1 << endl;
    cout << "Число вершин " << num_nodes << endl;
    cout << "Первая найденная " << first_found_number << endl;
    found_count = 0;
    num_nodes = 0;
    first_found_number = -1;

    start = clock();
    #pragma omp parallel
    {
    #pragma omp single
        {
            find_until_first(tree);
        }
    }
    finish = clock();
    time2 = (double)(finish - start)/CLOCKS_PER_SEC;

    cout << "Параллельный вариант " << time2 << endl;
    cout << "Число вершин " << num_nodes << endl;
    cout << "Первая найденная " << first_found_number << endl;
    found_count = 0;
    num_nodes = 0;
    first_found_number = -1;
}
#endif // TASKSOPENMP_3_H

