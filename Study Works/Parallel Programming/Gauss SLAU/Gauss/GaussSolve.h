#pragma once
#include "RandomMatrix.h"
#include <mpi.h>
#include <iostream>
using std::cout;
using std::endl;

void GaussSolveSerial(double **A, double * b, double * X, int n);
void GaussSolveMPI(double **A, double * b, double * X, int n);
void GaussSolveOpenMP(double **A, double * b, double * X, int n);