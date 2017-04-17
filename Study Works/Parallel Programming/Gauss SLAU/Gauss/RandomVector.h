#pragma once
#include "RandomNumber.h"
#include <iostream>

double * CreateRandomVector(int size);
void PrintVector(double * vec, int size);
void DeleteVector(double * vec);