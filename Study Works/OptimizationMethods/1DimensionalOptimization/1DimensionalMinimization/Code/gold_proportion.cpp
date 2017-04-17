#include "gold_proportion.h"

float gold_proportion_method(MinInterval min_i, float eps)
{
	float tao = find_tao(),
				interval_lenght = min_i.b - min_i.a;
	float x_1, x_2;

	do
	{
		x_1 = min_i.a + (min_i.b - min_i.a) / pow(tao, 2);
		// a + (1 - tao)(b - a)
		x_2 = min_i.a + (min_i.b - min_i.a) / tao;
		// a + tao(b - a)
		log(x_1, x_2);


		if (f(x_1) <= f(x_2))
		{
			min_i.b = x_2;
			x_2 = x_1;
			x_1 = min_i.a + min_i.b - x_2; // what is (a + b)?
		}

		else
		{
			min_i.a = x_1;
			x_1 = x_2;
			x_2 = min_i.a + min_i.b - x_1;
		}

		min_i.log();
	} while (((min_i.b - min_i.a) / 2) >= eps);
	
	return (min_i.a + min_i.b) / 2;
}

float find_tao()
{
	return (pow(5, 0.5) + 1) / 2;
}

void log(float x_1, float x_2)
{
	printf("x_1 = %f.3 | f(x_1) = %f.3\n", x_1, f(x_1));
	printf("x_2 = %f.3 | f(x_2) = %f.3\n", x_2, f(x_2));
	puts("==========================================");
}