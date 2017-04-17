#pragma once
#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector> 

using std::cout;
using std::endl;

namespace LabWork
{
    void Task1(int taskSize)
    {
        int procRank, procSize;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

        int * package = new int[taskSize];

        int senderRank = rand() % procSize;
        if (senderRank == procRank)
        {
            const int taskSize = 100;
            for (int i = 0; i < taskSize; i++)
                package[i] = i;
        }

        MPI_Bcast(package, taskSize, MPI_INT, senderRank, MPI_COMM_WORLD);

        if (procRank != senderRank)
            cout << procRank << " got array from " << senderRank;

        delete[] package;
    }


    void Task2(int rowLength)
    {
        int procRank, procSize;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);


        int * matrix = new int[procSize * rowLength];
        int * vector = new int[rowLength];

        MPI_Scatter(matrix, rowLength, MPI_INT, vector, rowLength, MPI_INT, 0, MPI_COMM_WORLD);

        // modify array
        for (int i = 0; i < rowLength; i++)
            vector[i] = i;

        MPI_Gather(vector, rowLength, MPI_INT, matrix, rowLength, MPI_INT, 0, MPI_COMM_WORLD);

        bool isMain = procRank == 0;
        if (isMain)
        {
            for (int i = 0; i < procSize; i++)
            {
                for (int j = 0; j < rowLength; j++)
                    cout << matrix[i*rowLength + j] << ' ';
                cout << endl;
            }
        }


        delete[] vector;
        delete[] matrix;
    }

    // Allgather, AlltoAll
    void Task3(int taskSize)
    {
        int procRank, procSize;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

        int * vector = new int[taskSize];
        for (int i = 0; i < taskSize; i++)
            vector[i] = 1;

        int subTaskSize = taskSize / procSize;
        int * subVector = new int[subTaskSize];

        MPI_Scatter(vector, subTaskSize, MPI_INT, subVector, subTaskSize, MPI_INT, 0, MPI_COMM_WORLD);
        int summ = 0;
        for (int i = 0; i < subTaskSize; i++)
            summ += subVector[i];


        int * summs = new int[procSize];
        MPI_Allgather(&summ, 1, MPI_INT, summs, 1, MPI_INT, MPI_COMM_WORLD);
        int totalSumm = 0;
        for (int i = 0; i < procSize; i++)
            totalSumm += summs[i];

        cout << "Proc " << procRank << " got " << totalSumm << endl;

        delete[] vector;
        delete[] subVector;
        delete[] summs;
    }

    void Task4(int taskSize)
    {
        int procRank, procSize;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

        int * vector = new int[taskSize];
        srand(procRank);
        for (int i = 0; i < taskSize; i++)
            vector[i] = rand();

        // local min and max
        int localMin = vector[0];
        int localMax = vector[0];

        for (int i = 0; i < taskSize; i++)
        {
            if (localMin > vector[i]) localMin = vector[i];
            if (localMax < vector[i]) localMax = vector[i];
        }

        int globalMin, globalMax;
        MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&localMax, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);


        bool isMain = procRank == 0;
        if (isMain)
        {
            cout << "Global min: " << globalMin << endl;
            cout << "Global max: " << globalMax << endl;
        }

        delete[] vector;
    }

    typedef double(*matrFunc)(double); // math func 
    void IndividualTask(int rangeLength)
    {
        int procSize, procRank;

        MPI_Comm_size(MPI_COMM_WORLD, &procSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

        std::vector<matrFunc> functions(procSize, sin); // many sinuses 
        double * range = new double[rangeLength];

        bool isMain = procRank == 0;
        if (isMain)
        {
            double start = 0.0;
            double finish = 150.0;
            double step = abs(start - finish) / rangeLength;
            for (int i = 0; i < rangeLength; i++)
                range[i] = start + i*step;
        }

        MPI_Bcast(range, rangeLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Calculate function values
        double * functionValues = new double[rangeLength];
        for (int i = 0; i < rangeLength; i++)
            functionValues[i] = functions[procRank](range[i]);


        double * allFunctionsValues = NULL; // NULL is very important here
        if (isMain)
            allFunctionsValues = new double[rangeLength * procSize];

        MPI_Gather(functionValues, rangeLength, MPI_DOUBLE,
            allFunctionsValues, rangeLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (isMain)
        {
            for (int i = 0; i < procSize; i++)
            {
                cout << "Function " << i << " values: ";
                for (int j = 0; j < rangeLength; j++)
                    cout << allFunctionsValues[i*rangeLength + j] << ' ';
                cout << endl;
            }
        }

        if (isMain)
            delete[] allFunctionsValues;

        delete[] functionValues;
        delete[] range;
    }
}
