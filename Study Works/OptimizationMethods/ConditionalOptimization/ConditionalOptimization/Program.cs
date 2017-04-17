using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using ConditionalOptimization.Math;
using NDimensionalPrimitives;

// TODO А вот здесь вывод от метода Хука - Дживса уже не нужен, так как загромождает буфер
// TODO В некоторых тестах есть две опорные (начальные) точки - x0 и x0_1
namespace ConditionalOptimization
{
  using FunctionND = Func<PointN, double>;
  using SMath = System.Math;
  class Program
  {
    static void Main(string[] args)
    {
      Test1_3();
      //Test2_1();
      //Test2_5();
      //Test2_17();
      Console.ReadLine();
    }

    // COMPLETE
    // X0 - догая сходимость
    // X0_1 - быстрая сходимость
    #region TEST1_1
    // Определение входных данных и вызов метода
    static void Test1_1()
    {
      FunctionND funcND = (PointN point) => { return System.Math.Pow(point.At(0), 2) + System.Math.Pow(point.At(1), 2); }; // z = x^2 + y^2
      PointN x0   = new PointN(0.0, 0.0); 
      PointN x0_1 = new PointN(2.0, 2.0); // доп. вариант базовой точки
      double eps = 0.01;

      // Функции ограничений
      List<FunctionND> limitFunctions = new List<FunctionND>
      { 
        new FunctionND((PointN point) => {return point.At(0) + point.At(1) - 2; }) 
      };
      int m = 0; // Число функций ограничений типа Fi(x) >= 0
      int p = 1; // Общее число функций ограничений

      TestPenaltyFunctionMethod(funcND, limitFunctions, x0, m, p, eps);
    }
    #endregion

    // COMPLETE
    // X0 - быстрая сходимость
    // X0_1 - быстрая сходимость 
    #region TEST1_3
    static void Test1_3()
    {
      FunctionND funcND = (PointN point) => { return System.Math.Pow(point.At(0), 2) + System.Math.Pow(point.At(1), 2); }; // z = x^2 + y^2
      PointN x0   = new PointN(0.0, 1.0);
      PointN x0_1 = new PointN(5.0, 5.0); // доп. вариант базовой точки
      double eps = 0.01;

      // Функции ограничений
      List<FunctionND> limitFunctions = new List<FunctionND>
      { 
        new FunctionND((PointN point) => { return point.At(0) - 1; }),
        new FunctionND((PointN point) => { return 2 - point.At(0) - point.At(1); }),
      };
      int m = 2; // Число функций ограничений типа Fi(x) >= 0
      int p = 2; // Общее число функций ограничений

      TestPenaltyFunctionMethod(funcND, limitFunctions, x0_1, m, p, eps);
    }
    #endregion

    // COMPLETE
    #region TEST2_1
    static void Test2_1()
    {
      FunctionND funcND = (PointN point) => { return (1.0 / 3.0) * System.Math.Pow(point.At(0) + 1, 3); }; // z = (1/3)*(x+1)^3 + y
      PointN x0 = new PointN(0.0, 0.0);
      double eps = 0.01;

      // Функции ограничений
      List<FunctionND> limitFunctions = new List<FunctionND>
      { 
        new FunctionND((PointN point) => { return point.At(0) - 1; }),
        new FunctionND((PointN point) => { return point.At(1); }),
      };
      int m = 2; // Число функций ограничений типа Fi(x) >= 0
      int p = 2; // Общее число функций ограничений

      TestPenaltyFunctionMethod(funcND, limitFunctions, x0, m, p, eps);
    }
    #endregion

    // FAILED (метод расходится)
    #region TEST2_4
    static void Test2_4()
    {
      FunctionND funcND = (PointN point) => { return point.At(0) - 2 * point.At(1) * point.At(1) + 4 * point.At(1); }; 
      PointN x0 = new PointN(0.0, 0.0);
      double eps = 0.01;

      // Функции ограничений
      List<FunctionND> limitFunctions = new List<FunctionND>
      { 
        new FunctionND((PointN point) => { return -3 * point.At(0) - 2 * point.At(1) - 6; })
      };
      int m = 0; // Число функций ограничений типа Fi(x) >= 0
      int p = 1; // Общее число функций ограничений

      TestPenaltyFunctionMethod(funcND, limitFunctions, x0, m, p, eps);
    }
    #endregion

    // COMPLETE
    #region TEST2_5
    static void Test2_5()
    {
      FunctionND funcND = (PointN point) => { return 4 * point.At(0) * point.At(0) + 8 * point.At(0) - point.At(1) - 3; }; // z = x^2 + y^2
      PointN x0 = new PointN(0.0, 0.0);
      double eps = 0.01;

      // Функции ограничений
      List<FunctionND> limitFunctions = new List<FunctionND>
      { 
        new FunctionND((PointN point) => { return point.At(0) + point.At(1) + 2; })
      };
      int m = 0; // Число функций ограничений типа Fi(x) >= 0
      int p = 1; // Общее число функций ограничений

      TestPenaltyFunctionMethod(funcND, limitFunctions, x0, m, p, eps);
    }
    #endregion

    // COMPLETE
    #region TEST2_17
    static void Test2_17()
    {
      FunctionND funcND = (PointN p) => { return SMath.Pow(p.At(0), 2) 
                                          + SMath.Pow(p.At(1), 2) 
                                          + 2 * SMath.Pow(p.At(2), 2) 
                                          + SMath.Pow(p.At(3), 2) 
                                          - 5 * p.At(0) 
                                          - 5 * p.At(1) 
                                          - 21 * p.At(2) 
                                          + 7 * p.At(3); };
      PointN x0 = new PointN(0.0, 0.0, 0.0, 0.0);
      double eps = 0.01;

      // Функции ограничений
      List<FunctionND> limitFunctions = new List<FunctionND>
      { 
        new FunctionND((PointN p) => { return 8 - SMath.Pow(p.At(0),2) 
                                              - SMath.Pow(p.At(1), 2)
                                              - SMath.Pow(p.At(2), 2)
                                              - SMath.Pow(p.At(3), 2)
                                              - p.At(0) 
                                              + p.At(1)
                                              - p.At(2)
                                              + p.At(3); }),
        new FunctionND((PointN p) => { return 10 - SMath.Pow(p.At(0),2)
                                              - 2 * SMath.Pow(p.At(1),2)
                                              - SMath.Pow(p.At(2), 2)
                                              - 2 * SMath.Pow(p.At(3), 2)
                                              + p.At(0)
                                              - p.At(3); }),
        new FunctionND((PointN p) => { return 5 - 2 * SMath.Pow(p.At(0),2)
                                              - SMath.Pow(p.At(1),2)
                                              - SMath.Pow(p.At(2),2)
                                              - 2 * p.At(0)
                                              + p.At(1)
                                              + p.At(3); })

      };
      int m = 3; // Число функций ограничений типа Fi(x) >= 0
      int _p = 3; // Общее число функций ограничений

      TestPenaltyFunctionMethod(funcND, limitFunctions, x0, m, _p, eps);
    }
    #endregion
    // Вызов метода и форматированный вывод результатов его работы
    static void TestPenaltyFunctionMethod(FunctionND funcND, List<FunctionND> limitFunctions, PointN x0, int m, int p, double eps)
    {
      PenaltyFunctionMethod pfm = new PenaltyFunctionMethod(funcND, limitFunctions, x0, m, p);
      PointN result = pfm.FindMin(eps);

      Console.Write(String.Join("\n", pfm.Log)); // Выводим все сообщения от этого метода
      Console.WriteLine("\n---------------------------------------------");
      Console.WriteLine("Variable metric method: x_min = {0}", result);
      Console.WriteLine("F(x_min) = {0}", funcND(result));
      Console.WriteLine("=============================================");
    }
  }
}
