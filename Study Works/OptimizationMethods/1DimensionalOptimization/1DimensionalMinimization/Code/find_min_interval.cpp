#include "find_min_interval.h"
void findMinInterval(float x_0, float h, MinInterval & min_i)
{
	// Объявление переменных
	float x_k = x_0, // x_(k)
				x_1,
		    x_k_prev; // x_(k-1)

	int k = 1; // Счетчик итераций

	// Алгоритм
	// Случай 1: Точка находится в интервале [x_0 - h; x_0 + h]
	if (f(x_0) <= f(x_0 + h) && f(x_0 - h) >= f(x_0))
	{
		min_i.a = x_0 - h;
		min_i.b = x_0 + h;

		min_i.log();
	}


	else
	{
		// Точка находится в интервале [x_0; b]
		if (f(x_0) > f(x_0 + h))
		{
			min_i.a = x_0;
			x_1 = x_0 + h;
		}

		// Точка находится в интервале [a; x_0]
		else
		{
			min_i.b = x_0;
			x_1 = x_0 - h;
			h = -h;
		}

		printf("x_0 = %.3f | f(x_0) = %.3f\n", x_0, f(x_0));
		min_i.log();
		x_k_prev = x_1;
		printf("x_1 = %.3f | f(x_1) = %.3f\n", x_1, f(x_1));
		k++;
		printf("x_2 = %.3f | f(x_2) = %.3f\n", x_k, f(x_k));
		bool interval_founded = false;
		do
		{
			x_k = x_0 + pow(2, k - 1) * h; // Коэффициент 2 позволяет расширять область поиска на каждой итерации
			interval_founded = f(x_k_prev) <= f(x_k); // Нашли первую точку, в которой значение функции начало возрастать
		
			if (interval_founded)
			{
				if (h > 0)
					min_i.b = x_k;
				else
					min_i.a = x_k;
			}

			else
			{
				if (h > 0)
					min_i.a = x_k_prev;
				else
					min_i.b = x_k_prev;
			}

			x_k_prev = x_k;
			min_i.log();
			k++;

			printf("x_%d = %.3f | f(x_%d) = %.3f\n", k, x_k, k, f(x_k));

		} while (!interval_founded);
	}
}
