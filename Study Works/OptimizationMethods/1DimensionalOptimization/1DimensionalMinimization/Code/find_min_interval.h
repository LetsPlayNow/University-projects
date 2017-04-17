// Поиск интервала нахождения минимума 
// Используется алгоритм Дэвиса-Свенна-Кэмпи
#pragma once
#include <cstdio>
#include "f_x.h"

// Отрезок локализации минимума
// Могут ли быть a и b дробными?
struct MinInterval{
	float a; // Начало
	float b; // Конец

public:
	MinInterval(float a_o = -1.0, float b_o = -1.0) : a(a_o), b(b_o){}

	void log()
	{
		printf("a: %f\nb: %f\n", a, b);		
		puts("=================================");
	}
};

// Алгоритм Дэвиса-Свенна-Кэмпи
// Поиск интервала нахождения минимума
// * param[in]     float | x_0: начальная точка
// * param[in]		 float | h: шаг (> 0)
// * param[in/out] MinInterval | min_i: интервал, содержащий минимум
// * param[out]    bool: успех работы функции
// ! Функция не проверет корректность вводимых аргументов
void findMinInterval(float x_0, float h, MinInterval & min_i);