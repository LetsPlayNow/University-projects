using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NDimensionalPrimitives;


namespace GradientMethods.Math
{
  using Function = System.Func<double, double>;

  class OneDimensionalMinimization
  {
    public static double FindMin(Function f, double x_0, double eps, double h = 0.1)
    {
      Interval intervalWithMin = FindMinInterval(f, x_0, h);
      double specialEps = 0.0001; // Градиентный метод очень чувствителен к этому (если здесь будет 0.1, метод накроется)
      double minPoint = GoldProportionMethod(f, intervalWithMin, specialEps); 
      return minPoint;
    }

    private static double GoldProportionMethod(Function f, Interval intervalWithMin, double eps)
    {
	    double tao = (System.Math.Pow(5.0, 0.5) + 1.0) / 2.0;
	    double x_1, x_2;

	    do
	    {
		    x_1 = intervalWithMin.a + (intervalWithMin.Length) / System.Math.Pow(tao, 2);
		    // a + (1 - tao)(b - a)
		    x_2 = intervalWithMin.a + (intervalWithMin.Length) / tao;
		    // a + tao(b - a)

		    if (f(x_1) <= f(x_2))
		    {
			    intervalWithMin.b = x_2;
			    x_2 = x_1;
			    x_1 = intervalWithMin.a + intervalWithMin.b - x_2;
		    }
		    else
		    {
			    intervalWithMin.a = x_1;
			    x_1 = x_2;
			    x_2 = intervalWithMin.a + intervalWithMin.b - x_1;
		    }

	    } while (((intervalWithMin.Length) / 2) >= eps);
	    return intervalWithMin.Center;
    }

    /// <summary>
    /// Поиск отрезка локализации минимума
    /// </summary>
    /// <param name="f">Минимизируемая функция</param>
    /// <param name="x_0">Опорная точка</param>
    /// <param name="h">Шаг поиска (>0)</param>
    /// <returns></returns>
    private static Interval FindMinInterval(Function f, double x_0, double h)
    {
      Interval intervalWithMin = new Interval();
      // Объявление переменных
      double x_k = x_0, // x_(k)
			      x_1,
		        x_k_prev; // x_(k-1)

      int k = 1; // Счетчик итераций

      // Алгоритм
      // Случай 1: Точка находится в интервале [x_0 - h; x_0 + h]
      if (f(x_0) <= f(x_0 + h) && f(x_0 - h) >= f(x_0))
      {
	      intervalWithMin.a = x_0 - h;
	      intervalWithMin.b = x_0 + h;
      }

      else
      {
	      // Точка находится в интервале [x_0; b]
	      if (f(x_0) > f(x_0 + h))
	      {
		      intervalWithMin.a = x_0;
		      x_1 = x_0 + h;
	      }

	      // Точка находится в интервале [a; x_0]
	      else
	      {
		      intervalWithMin.b = x_0;
		      x_1 = x_0 - h;
		      h = -h;
	      }

	      x_k_prev = x_1;
	      k++;

	      bool interval_founded = false;
	      do
	      {
		      x_k = x_0 + System.Math.Pow(2.0, (k - 1)) * h; // Коэффициент 2 позволяет расширять область поиска на каждой итерации
		      interval_founded = f(x_k_prev) <= f(x_k); // Нашли первую точку, в которой значение функции начало возрастать
		
		      if (interval_founded)
		      {
			      if (h > 0)
				      intervalWithMin.b = x_k;
			      else
				      intervalWithMin.a = x_k;
		      }

		      else
		      {
			      if (h > 0)
				      intervalWithMin.a = x_k_prev;
			      else
				      intervalWithMin.b = x_k_prev;
		      }

		      x_k_prev = x_k;
		      k++;
	      } while (!interval_founded);
      }

      // Можно здесь подправить интервал
      // TODO костыль
      if (intervalWithMin.a < 0) intervalWithMin.a = 0;
      if (intervalWithMin.b < 0) intervalWithMin.b = intervalWithMin.a + 30 * h;
      return intervalWithMin;
    }
  }

  // [a; b]
  public class Interval
  {
    public double a;
    public double b;

    public Interval(double a = 0.0, double b = 0.0)
    {
      this.a = a;
      this.b = b;
    }

    public double Length 
    {
      get { return b - a; }
    }

    public double Center
    {
      get { return (a + b) / 2; }
    }
  }
}
