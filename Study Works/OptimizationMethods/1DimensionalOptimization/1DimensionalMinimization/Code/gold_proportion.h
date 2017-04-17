#include "find_min_interval.h"
#include "f_x.h"

// Нахождение минимума на отрезке
// Метод Золотого Сечения
// * param[in] eps (> 0): параметр точности поиска
// * param[in/out] min_i: интервал, содержащий минимум
// * param[out] x*: координаты минимума функции f
// ! Функция не проверет корректность вводимых аргументов
float gold_proportion_method(MinInterval min_i, float eps);

// ~ 1.618
float find_tao();

void log(float x_1, float x_2);