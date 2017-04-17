#include "paul.h"


void log(vector <x_fx> vec, const char * name)
{
	puts("========================================");
	for (int i = 0; i < vec.size(); i++)
	{
		printf("x_%d = %.3f | f(x_%d) = %.3f\n", i+1, vec[i]._x, i+1, vec[i]._fx);
	}
}

float paul_method(MinInterval min_i, float eps)
{
	float x;
	vector <x_fx> x_i(3);
	x_i[0] = x_fx(min_i.a);
	x_i[1] = x_fx((min_i.a + min_i.b) / 2);
	x_i[2] = x_fx(min_i.b);

	do
	{
		x_fx x_ = *std::min_element(x_i.begin(), x_i.end());

		float x_1 = x_i[0]._x;
		float x_2 = x_i[1]._x;
		float x_3 = x_i[2]._x;

		x_fx x4(((pow(x_2, 2) - pow(x_3, 2)) * f(x_1) + (pow(x_3, 2) - pow(x_1, 2)) * f(x_2) + (pow(x_1, 2) - pow(x_2, 2)) * f(x_3)) /
			(2 * ((x_2 - x_3) * f(x_1) + (x_3 - x_1) * f(x_2) + (x_1 - x_2) * f(x_3))));

		log(x_i, "x_i");
		x4.print("x_4");
		x_.print("x^");

		if (abs(x4._x - x_._x) <= eps)
			return x = x4._x;


		x_i.push_back(x4);


		vector <x_fx> min_3(3);
		vector <float> x_s;
		for (int i = 0; i < x_i.size(); i++)
		{
			x_s.push_back(x_i[i]._x);
		}

		std::sort(x_s.begin(), x_s.end());
		vector <x_fx>::iterator min_element = std::min_element(x_i.begin(), x_i.end());
		int index;
		for (int i = 0; i < x_s.size(); i++)
		{
			if (min_element->_x == x_s[i])
			{
				index = i;
				break;
			}
		}
		min_3[0] = x_s[index-1];
		min_3[1] = x_s[index];
		min_3[2] = x_s[index+1];

		x_i = min_3;
	} while (true);
}