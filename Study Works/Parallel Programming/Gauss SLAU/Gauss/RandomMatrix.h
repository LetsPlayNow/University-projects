#pragma once
#include "RandomNumber.h"
#include "RandomVector.h"
#include <iostream>

double ** CreateRandomMatrix(int n);
void PrintMatrix(double ** matrix, int size);
void DeleteMatrix(double ** matrix, int size);