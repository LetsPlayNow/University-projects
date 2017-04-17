#pragma once
#include "GaussSolve.h"
#include "RandomMatrix.h"
#include "RandomVector.h"

#include <iostream>
#include <iomanip>
#include <time.h>
#include <omp.h>
#include <string>
#include <Eigen\Dense>

using std::cout;
using std::endl;
using std::string;

using Eigen::MatrixXf;
using Eigen::VectorXf;


bool areEquals(double x1, double x2);

string boolToStr(bool isCorrect);

bool isCorrectAnswer(double **A, double *b, double *X, int n);

double TestSerial(int n);

double TestOMP(int n);

double TestMPI(int n);

