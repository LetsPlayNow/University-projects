#include "find_min_interval.h"
#include "gold_proportion.h"
#include "paul.h"
#include <cstdio>
#include <conio.h>
#include <iostream>

// Прототипы функций
bool find_min_interval_test(MinInterval & min_i);
bool gold_proportion_test(MinInterval min_i);
bool paul_test(MinInterval min_i);
void input_min_i(MinInterval & min_i);

int main()
{
	MinInterval min_i;
	find_min_interval_test(min_i);

	printf("Continue with current interval? (y / n) ");
	char choise;
	std::cin >> choise;
	if (choise == 'n')
	{
		input_min_i(min_i);
	}

	gold_proportion_test(min_i);

	paul_test(min_i);
}


// Тестируем функцию нахождения интервала лоализации минимума
bool find_min_interval_test(MinInterval & min_i)
{
	float x_0, h;

	puts("Find interval with min of single dimension func");
	puts("Please, input x_0 and h");
	printf("x_0: ");
	scanf("%f", &x_0);
	printf("h:   ");
	scanf("%f", &h);
	putchar('\n');

	// Проверки на дурака
	if (h <= 0)
	{
		printf("[ERR] h <= 0");
		_getch();
		return false;
	}

	findMinInterval(x_0, h, min_i);
	printf("Total: a: %f b: %f\n", min_i.a, min_i.b);
	_getch();
	return true;
}


bool gold_proportion_test(MinInterval min_i)
{
	float eps, minimum;

	puts("\nFind minimum of function by Gold Selection Method");
	puts("Please, input eps");
	printf("eps: ");
	scanf("%f", &eps);

	// Проверки на дурака
	if (eps <= 0) // Надо учесть крайние случаи eps (0.00000001)
	{
		printf("[ERR] eps <= 0");
		_getch();
		return false;
	}

	minimum = gold_proportion_method(min_i, eps);
	printf("Total: X_minimum: %f\n", minimum);
	_getch();

	return true;
}

bool paul_test(MinInterval min_i)
{
	float eps, minimum;

	puts("\nFind minimum of function by Paul's Method");
	puts("Please, input eps");
	printf("eps: ");
	scanf("%f", &eps);

	// Проверки на дурака
	if (eps <= 0) // Надо учесть крайние случаи eps (0.00000001)
	{
		printf("[ERR] eps <= 0");
		_getch();
		return false;
	}

	minimum = paul_method(min_i, eps);
	printf("Total: X_minimum: %f\n\n", minimum);
	_getch();

	return true;
}


void input_min_i(MinInterval & min_i)
{
	std::cout << "Please, input a & b for interval ([a; b])" << std::endl;
	std::cin >> min_i.a;
	std::cin >> min_i.b;
}