#include "RandomMatrix.h"

double ** CreateRandomMatrix(int n)
{
    double ** matrix = new double *[n];
    for (int i = 0; i < n; i++)
        matrix[i] = CreateRandomVector(n);

    return matrix;
}

void PrintMatrix(double ** matrix, int size)
{
    for (int i = 0; i < size; i++)
        PrintVector(matrix[i], size);
    std::cout << std::endl;
}

void DeleteMatrix(double ** matrix, int size)
{
    for (int i = 0; i < size; i++)
        DeleteVector(matrix[i]);
    delete[] matrix;
}