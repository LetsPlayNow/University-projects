#include "Matrix2d.h"


Matrix2d::Matrix2d(int rows, int cols)
{
    _matrix = new double[rows * cols];
    _rows = rows;
    _cols = cols;
}

Matrix2d::Matrix2d(double * data, int cols, int rows)
{
    _cols = cols;
    _rows = rows;

    _matrix = new double[_rows * _cols];
    for (int i = 0; i < _cols * _rows; i++)
        _matrix[i] = data[i];
}

Matrix2d::Matrix2d(Matrix2d & other)
{
    operator=(other);
}



Matrix2d Matrix2d::Random(int rows, int cols)
{
    Matrix2d matrix(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix(i, j) = rand();
 
    return matrix;
}

Matrix2d::~Matrix2d()
{
    delete[] _matrix;
}

int Matrix2d::ColsCount()
{
    return _cols;
}

int Matrix2d::RowsCount()
{
    return _rows;
}

double * Matrix2d::Data()
{
    return _matrix;
}

double & Matrix2d::operator()(int row, int col)
{
    return _matrix[row*_cols + col];
}

Matrix2d & Matrix2d::operator=(Matrix2d & other)
{
    delete[] _matrix;

    _cols = other._cols;
    _rows = other._rows;
    _matrix = new double[_cols * _rows];
    for (int i = 0; i < _cols * _rows; i++)
        _matrix[i] = other._matrix[i];

    return *this;
}

Matrix2d Hstack(Matrix2d a, Matrix2d b)
{
    int resultCols = a._cols + b._cols;
    int resultRows = std::min(a._rows, b._rows);
    Matrix2d c(resultRows, resultCols);

    for (int i = 0; i < resultRows; i++)
    {
        for (int j = 0; j < resultCols; j++)
        {
            if (j < a._cols)
                c(i, j) = a(i, j);
            else
                c(i, j) = b(i - b._rows, j - b._cols);
        }
    }

    return c;
}

ostream & operator<<(ostream & stream, Matrix2d matrix)
{
    for (int i = 0; i < matrix._rows; i++)
    {
        for (int j = 0; j < matrix._cols; j++)
            stream << std::setprecision(4) << (i, j) << ' ';
        stream << std::endl;
    }

    return stream;
}
