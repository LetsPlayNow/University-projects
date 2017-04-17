#include "Test.h"

bool areEquals(double x1, double x2)
{
    return fabs(x2 - x1) < 0.0001;
}

string boolToStr(bool isCorrect)
{
    if (isCorrect)
        return "CORRECT";
    else
        return "INVALID";
}

bool isCorrectAnswer(double ** A, double * b, double * X, int n)
{
    // Решение библиотеки Eigen
    MatrixXf A_(n, n);
    VectorXf b_(n);
    VectorXf X_(n);

    for (int i = 0; i < n; i++)
        b_(i) = b[i];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A_(i, j) = A[i][j];

    X_ = A_.colPivHouseholderQr().solve(b_);


    bool isCorrectX = true;
    for (int i = 0; i < n; i++)
        isCorrectX &= areEquals(X[i], X_(i));

    return isCorrectX;
}

double TestSerial(int n)
{
    // Мое решение
    double **A = CreateRandomMatrix(n);
    double *b = CreateRandomVector(n);
    double *X = CreateRandomVector(n);

    double start = omp_get_wtime();
    GaussSolveSerial(A, b, X, n);
    double end = omp_get_wtime();
    double serial_time = end - start;

    bool isCorrectAnswerSerial = isCorrectAnswer(A, b, X, n);

    DeleteMatrix(A, n);
    DeleteVector(b);
    DeleteVector(X);

    cout << "[Serial( " << n << " )] " << boolToStr(isCorrectAnswerSerial) << " Time: " << serial_time << endl;
    return serial_time;
}

double TestOMP(int n)
{
    // Мое решение
    double **A = CreateRandomMatrix(n);
    double *b = CreateRandomVector(n);
    double *X = CreateRandomVector(n);

    double start, end;
    start = omp_get_wtime();
    GaussSolveOpenMP(A, b, X, n);
    end = omp_get_wtime();
    double openmp_time = end - start;

    bool isCorrectAnswerOpenMP = isCorrectAnswer(A, b, X, n);

    DeleteMatrix(A, n);
    DeleteVector(b);
    DeleteVector(X);

    cout << "[OpenMP( " << n << " )] " << boolToStr(isCorrectAnswerOpenMP) << " Time: " << openmp_time << endl;
    return openmp_time;
}

double TestMPI(int n)
{
    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);

    double **A = NULL;
    double *b = NULL;
    double *X = NULL;

    // Выделение памяти
    bool isMain = procRank == 0;
    if (isMain)
    {
        A = CreateRandomMatrix(n);
        b = CreateRandomVector(n);
        X = CreateRandomVector(n);
    }

    // Тестирование
    MPI_Barrier(MPI_COMM_WORLD);
    double start, end;
    if (isMain) start = MPI_Wtime();
    GaussSolveMPI(A, b, X, n);
    if (isMain) end = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    double mpi_time = 0.0;
    if (isMain) mpi_time = end - start;

    // Проверка решения и вывод результатов
    if (isMain)
    {
        bool isCorrectAnswerMPI = isCorrectAnswer(A, b, X, n);
        cout << "[ MPI  ( " << n << " )] " << boolToStr(isCorrectAnswerMPI) << " Time: " << mpi_time << endl;
    }

    if (isMain)
    {
        DeleteMatrix(A, n);
        DeleteVector(b);
        DeleteVector(X);
    }

    return mpi_time;
}
