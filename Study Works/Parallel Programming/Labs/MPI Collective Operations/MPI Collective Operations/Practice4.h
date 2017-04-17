#pragma once
#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <time.h>

using std::wcout;
using std::endl;

namespace Practice
{
    void Task1(int rowLength)
    {
        int procRank, procSize;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

        bool isMain = procRank == 0;

        double * matrix = NULL; // (rows, cols)
        int rowsCount = procSize;
        if (isMain)
        {
            matrix = new double[rowsCount * rowLength];
            for (int i = 0; i < rowsCount * rowLength; i++)
                matrix[i] = rand();
        }

        double * vector = new double[rowLength];
        MPI_Scatter(matrix, rowLength, MPI_DOUBLE, 
                    vector, rowLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
        double vectorSumm = 0.0;
        for (int i = 0; i < rowLength; i++)
            vectorSumm += vector[i];

        cout << "matrix[" << procRank << "] summ: " << vectorSumm << endl;

        double matrixSumm = 0.0;
        MPI_Reduce(&vectorSumm, &matrixSumm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (isMain)
            cout << "matrix summ " << matrixSumm << endl;

        if (isMain)
            delete[] matrix;

        delete[] vector;
    }


    void Task2(int rowLength)
    {
        int procRank, procSize;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

        bool isMain = procRank == 0;

        int * matrix = NULL; // (rows, cols)
        int rowsCount = procSize;
        if (isMain)
        {
            matrix = new int[rowsCount * rowLength];
            for (int i = 0; i < rowsCount * rowLength; i++)
                matrix[i] = rand() % 100 + i;
        }

        int * vector = new int[rowLength];
        MPI_Scatter(matrix, rowLength, MPI_INT,
                    vector, rowLength, MPI_INT, 0, MPI_COMM_WORLD);

        int maxInVector[2] = { 0, procRank };
        for (int i = 0; i < rowLength; i++)
            if (vector[i] > maxInVector[0])
                maxInVector[0] = vector[i];

        cout << "Max in matrix[" << maxInVector[1] << "] summ: " << maxInVector[0] << endl;

        int maxInMatrix[2];
        MPI_Reduce(&maxInVector, &maxInMatrix, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

        if (isMain)
            cout << "Max in matrix found in row " << maxInMatrix[1] << " is " << maxInMatrix[0] << endl;

        if (isMain)
            delete[] matrix;

        delete[] vector;
    }


    void Task3(int rowLength)
    {
        int procRank, procSize;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

        bool isMain = procRank == 0;

        double * matrix = NULL; // (rows, cols)
        double * vector = new double[rowLength]; // (cols, 1)
        int rowsCount = procSize;
        if (isMain)
        {
            matrix = new double[rowsCount * rowLength];
            for (int i = 0; i < rowsCount * rowLength; i++)
                matrix[i] = rand() % 100 + 1;

            // vector = new double[rowLength];
            for (int i = 0; i < rowLength; i++)
                vector[i] = rand() % 100 + 1;
        }

        // Send vector b
        MPI_Bcast(vector, rowLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Send matrix[procRank]
        double * matrixVector = new double[rowLength];
        MPI_Scatter(matrix, rowLength, MPI_DOUBLE,
                    matrixVector, rowLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // matrix[procRank] * vector => resultVector[procRank]
        double elementOfResultVector = 0.0;
        for (int i = 0; i < rowLength; i++)
            elementOfResultVector += matrixVector[i] * vector[i];

        // Send to master
        double * resultVector = NULL;
        if (isMain)
            resultVector = new double[procSize];

        MPI_Gather(&elementOfResultVector, 1, MPI_DOUBLE, 
                    resultVector, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Print result vector to screen
        if (isMain)
        {
            cout << "Result vector is:" << endl;
            cout.precision(4);
            for (int i = 0; i < procSize; i++)
                cout << resultVector[i] << endl;
            cout << endl;
        }
          

        if (isMain)
        {
            delete[] matrix;
            delete[] vector;
            delete[] resultVector;
        }
        else 
        {
            delete[] matrixVector;
        }     
    }
}
