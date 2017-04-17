#include "RandomVector.h"

double * CreateRandomVector(int size)
{
    double * vec = new double[size];
    for (int i = 0; i < size; i++)
        vec[i] = RandomNumber();

    return vec;
}

void PrintVector(double * vec, int size)
{
    for (int i = 0; i < size; i++)
        std::cout << vec[i] << ' ';
    std::cout << std::endl;
}

void DeleteVector(double * vec)
{
    delete[] vec;
}