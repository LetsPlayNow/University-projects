using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NDimensionalPrimitives;

namespace MethodRandomSearch
{
  /// <summary>
  /// Причина ошибок может быть заключено в условии завершения работы алгоритма
  /// Оно определено лишь максимальным числом итераций
  /// </summary>
  public static class RandomSearch
  {
    // To make real safe random
    static RandomSearch()
    {
      random = new Random();
    }
    private static Random random;


    /// <summary>
    /// Метод случайного поиска для функции n переменных
    /// </summary>
    /// <param name="basicPoint">Начальная точка x0</param>
    /// <param name="a">Коэффициент расширения (1;+inf): 1.618 </param>
    /// <param name="b">Коэффициент сжатия (0;1): 0.618 </param>
    /// <param name="M">Максимальное число неудачно выполненых испытаний на данной итерации :(3n)</param>
    /// <param name="t0">Начальная величина шага</param>
    /// <param name="R">Минимальная величина шага</param>
    /// <param name="N">Максимальное число итераций</param>
    /// <returns>Точка минимума</returns>
    public static PointN MethodRandomSearch(F function, PointN basicPoint, double a, double b, double t0, double R, int N, int M)
    {
      double tk = t0; // Length of k-th step
      PointN xk = basicPoint; // Опорная точка
      PointN yj; // Точки, лежащие на гиперсфере радиуса tk с центром в точке xk
      PointN zj; // Точка, получающаяся после прыжка
      int dimensionsCount = basicPoint.Coordinates.Count;
      int k = 0, j = 1;

      do
      {
        // Step 2 Make random vector
        VectorN randomVector = new VectorN(dimensionsCount);
        MakeRandomVector(randomVector); // TODO What will be after this operator?
        Console.WriteLine("> Generated random vector {0}", randomVector.ToString());

        // Step 3 Find yj
        yj = xk + (randomVector / randomVector.Length) * tk;

        // Step 4
        // Good step
        bool isCurrentStepGood = function.Value(yj) < function.Value(xk);
        bool isNextStepAlsoGood = false;
        if (isCurrentStepGood)
        {
          Console.WriteLine("> Good step F(yj {0}) < F(xk {1})", yj.ToString(), xk.ToString());
          zj = xk + (yj - xk) * a;
          isNextStepAlsoGood = function.Value(zj) < function.Value(xk); 
          if (isNextStepAlsoGood)
          {
            Console.WriteLine("> Good direction F(zj {0}) < F(xk {1})", zj.ToString(), xk.ToString());
            xk = zj;
            tk *= a;
            k++;

            bool tooManyIterations = k == N;
            if (tooManyIterations)
            {
              Console.WriteLine("Limit of iterations N = {0} exceeded", N);
              return xk;
            }
              
            else
              j = 1;
          }
        }
         
        // Bad step
        if (!isCurrentStepGood || !isNextStepAlsoGood)
        {
          bool maxNumberOfFailsReached = j == M;
          if (!maxNumberOfFailsReached)
          {
            j++;
          }
          else if (maxNumberOfFailsReached)
          {
            Console.WriteLine("Number of bad steps from this point is exceeded");
            if (tk <= R)
            {
              Console.WriteLine("Step size (tk) is less than Low limit (R)");
              return xk;
            }
            else if (maxNumberOfFailsReached && tk > R)
            {
              Console.WriteLine("Increase step size (t0) and repeat search");
              tk *= b;
              j = 1;
            }
          }
        }
      } while (true);
    }

    static void MakeRandomVector(VectorN vector)
    {
      int dimensionsCount = vector.Components.Count;
      for (int i = 0; i < dimensionsCount; i++)
        vector.Components[i] = GetRandomNumber();
    }

    /// <summary>
    /// Random value to make random vector
    /// </summary>
    /// <returns>Random value from interval [-1; 1]</returns>
    static double GetRandomNumber()
    {
      return 2 * random.NextDouble() - 1; 
    }
  }
}
