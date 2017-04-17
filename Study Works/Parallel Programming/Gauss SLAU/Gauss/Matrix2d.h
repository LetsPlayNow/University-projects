#pragma once
#include <iostream>
#include <iomanip>
#include <algorithm>
using std::ostream;

class Matrix2d
{
public:
    Matrix2d(int rows, int cols);
    Matrix2d(double * data, int cols, int rows);
    Matrix2d(Matrix2d & other);

    ~Matrix2d();

    static Matrix2d Random(int rows, int cols); // create matrix with random values
    friend Matrix2d Hstack(Matrix2d a, Matrix2d b); // add columns b to a 
    friend Matrix2d Vstack(Matrix2d a, Matrix2d b); // add rows b to a 

    // getters
    int ColsCount();
    int RowsCount();
    double * Data();

    // operators
    double & operator()(int row, int col);
    Matrix2d & operator=(Matrix2d & other);
    friend ostream & operator<<(ostream & cout, Matrix2d matrix);
private:
    double * _matrix;
    int _rows;
    int _cols;
};


