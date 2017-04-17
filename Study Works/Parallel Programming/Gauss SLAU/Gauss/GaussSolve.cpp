#include "GaussSolve.h"

void GaussSolveSerial(double **A, double * b, double * X, int n)
{
    // Выделить память для расширенной матрицы A 
    double ** Ab = new double *[n];
    for (int i = 0; i < n; i++)
        Ab[i] = new double[n + 1];

    // Скопировать данные из А
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Ab[i][j] = A[i][j];

    // Скопировать данные из b
    for (int i = 0; i < n; i++)
        Ab[i][n] = b[i];

    
    // Elimination (исключение или прямой ход)
    for (int i = 0; i < n - 1; i++) // Для всех строк, кроме предпоследней
    {
        // Можно распараллелить
        for (int j = i + 1; j < n; j++) // Для каждой следующей за i-ой строкой
        {
            double divider = Ab[j][i] / Ab[i][i];
            // Можно распараллелить
            for (int k = i; k < n + 1; k++) // Для каждого элемента j-ой строки, начиная с i-ого
            {
                Ab[j][k] -= Ab[i][k] * divider;
            }
        }
    }    

    // Retrace (обратный ход)
    for (int i = n - 1; i >= 0; i--)
    {
        double rowSumm = 0.0; // aj*xj , где j [i+1; n]
        for (int j = i + 1; j < n; j++)
            rowSumm += Ab[i][j] * X[j];

        X[i] = (Ab[i][n] - rowSumm) / Ab[i][i];
    }

    DeleteMatrix(Ab, n);
}


void GaussSolveOpenMP(double **A, double * b, double * X, int n)
{
    // Выделить память для расширенной матрицы A 
    double ** Ab = new double *[n];
    for (int i = 0; i < n; i++)
        Ab[i] = new double[n + 1];

    // Скопировать данные из А
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Ab[i][j] = A[i][j];

    // Скопировать данные из b
    for (int i = 0; i < n; i++)
        Ab[i][n] = b[i];


    // Elimination (исключение или прямой ход)
    for (int i = 0; i < n - 1; i++) // Для всех строк, кроме предпоследней
    {
        #pragma omp parallel for
        for (int j = i + 1; j < n; j++) // Для каждой следующей за i-ой строкой
        {
            double divider = Ab[j][i] / Ab[i][i];
            #pragma omp parallel for
            for (int k = i; k < n + 1; k++) // Для каждого элемента j-ой строки, начиная с i-ого
            {
                Ab[j][k] -= Ab[i][k] * divider;
            }
        }
    }

    // Retrace (обратный ход)
    for (int i = n - 1; i >= 0; i--)
    {
        double rowSumm = 0.0; // aj*xj , где j [i+1; n]
        #pragma omp parallel for reduction(+:rowSumm)
        for (int j = i + 1; j < n; j++)
            rowSumm += Ab[i][j] * X[j];

        X[i] = (Ab[i][n] - rowSumm) / Ab[i][i];
    }

    DeleteMatrix(Ab, n);
}



void GaussSolveMPI(double **A, double * b, double * X, int n)
{
    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);

    bool isMain = procRank == 0;
    double ** Ab = NULL;
    if (isMain)
    {
        // Выделить память для расширенной матрицы A 
        Ab = new double *[n];
        for (int i = 0; i < n; i++)
            Ab[i] = new double[n + 1];

        // Скопировать данные из А
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                Ab[i][j] = A[i][j];

        // Скопировать данные из b
        for (int i = 0; i < n; i++)
            Ab[i][n] = b[i];
    }


    // Elimination (исключение или прямой ход)
    double * Ab_i, * Ab_j; // строки для рабочих процессов 
    if (!isMain)
    {
        Ab_i = new double[n + 1];
        Ab_j = new double[n + 1];
    }

    for (int i = 0; i < n - 1; i++) // Для всех строк, кроме предпоследней
    {
        // Отсылаем i - ую строку всем подпроцессам
        if (isMain) Ab_i = Ab[i];
        MPI_Bcast(Ab_i, n + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        for (int j = i + 1; j < n; j++)
        {    
            int recieverRank = 1;
            if (isMain) // рассылает j-ую строку
            {
                Ab_j = Ab[j];
                MPI_Send(Ab_j, n + 1, MPI_DOUBLE, recieverRank, 0, MPI_COMM_WORLD);

                MPI_Status masterRecvStatus;
                MPI_Recv(Ab_j, n + 1, MPI_DOUBLE, recieverRank, MPI_ANY_TAG, MPI_COMM_WORLD, &masterRecvStatus);

                recieverRank++;
                if (recieverRank > procSize - 1)
                    recieverRank = 1;
            }
            else if (procRank == recieverRank)
            {
                MPI_Status recvStatus;
                MPI_Recv(Ab_j, n + 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &recvStatus);

                // Процесс считает j - ую строку
                double divider = Ab_j[i] / Ab_i[i];
                for (int k = 0; k < n + 1; k++)
                    Ab_j[k] -= Ab_i[k] * divider;

                MPI_Send(Ab_j, n + 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }  

            
        }
    }
    
    if (isMain)
    {
        // Retrace (обратный ход)
        for (int i = n - 1; i >= 0; i--)
        {
            double rowSumm = 0.0; // aj*xj , где j [i+1; n]
            for (int j = i + 1; j < n; j++)
                rowSumm += Ab[i][j] * X[j];

            X[i] = (Ab[i][n] - rowSumm) / Ab[i][i];
        }
    }
    
    if (isMain)
    {
        DeleteMatrix(Ab, n);
    }    
    else 
    {
        delete[] Ab_j;
        delete[] Ab_i;
    }
}
