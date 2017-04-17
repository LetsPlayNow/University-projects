#pragma once
#include "f_x.h"
#include "find_min_interval.h"
#include <vector>
#include <algorithm>
using std::vector;

struct x_fx
{
public:
	x_fx()
	{}

	x_fx(float x)
	{
		_x = x;
		_fx = f(x);
	}

	float _x;
	float _fx;

	void print(const char* name)
	{
		printf("%s = %.3f | f(%s) = %.3f\n", name, _x, name, _fx);
	}

	bool operator==(const x_fx other)
	{
		return _x == other._x && _fx == other._fx;
	}
};

inline bool operator<(const x_fx & a, const x_fx & b)
{
	return a._fx < b._fx;
}

// Нахождение минимума на отрезке
// Метод параболической аппроксимации Пауэлла
// * param[in] eps (> 0): параметр точности поиска
// * param[in/out] min_i: интервал, содержащий минимум
// * param[out] x*: координаты минимума функции f
// ! Функция не проверет корректность вводимых аргументов
float paul_method(MinInterval min_i, float eps);

void log(vector <x_fx> vec, const char * name);
float paul_method(MinInterval min_i, float eps);