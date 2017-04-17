using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using GradientMethods.Math; // Для упрощения доступа к классам методов

using MathNet.Numerics.LinearAlgebra; // Математическая библиотека, использовалась для матричных вычислений
using NDimensionalPrimitives; // Моя библиотека N-мерных примитивов (PointN, VectorN)

using System.Diagnostics; // Для бенчмаркинга кода (Stopwatch)
using SMath = System.Math;
namespace GradientMethods
{
  using FunctionNDMathNet = Func<Vector<double>, double>;
  using PointNMathNet = Vector<double>;
  using VectorNMathNet = Vector<double>;

  using FunctionNDByAlex = Func<PointN, double>;
  // PointN
  // VectorN

  class Program
  {
    static void Main(string[] args)
    {
      Test2_1();
      Console.ReadLine();
    }

    // NOTE: Неизвстно, как преобразовать Func<Vector<double>, double> к Func<PointN, double>
    // Поэтому будут использованы две аналогичные функции
    // Для них используются ужасно похожие лямбды.
    // Возможно, этот конфликт можно решить
    // TODO: надо бы сравнить производительность алгоритмов
    // Можно привлечь сюда число итераций например
    // TODO: см в тетради и реши все тестовые задачи
    // COMPLETE
    #region TEST2_1
    static void Test2_1()
    {
      // Input data
      FunctionNDByAlex funcND         =         (PointN point) => { return 4 * System.Math.Pow(point.At(0) - 5.0, 2.0) + System.Math.Pow(point.At(1) - 6.0, 2.0); };
      FunctionNDMathNet funcNDMathNet = (VectorNMathNet point) => { return 4 * System.Math.Pow(point.At(0) - 5.0, 2.0) + System.Math.Pow(point.At(1) - 6.0, 2.0); };
      PointN x0                = new PointN(100.0, 100.0);
      VectorNMathNet x0MathNet = Helpers.PointNToMathNet(x0);
      const double eps = 0.01;

      // Testing
      TestOptimalGradientMethod(funcND, x0, eps); 
      TestVariableMetricMethod(funcNDMathNet, x0MathNet, eps);
    }
    #endregion

    // FAILED (первый несет чушь)
    // 1) Градиент все время прыгает во все стороны, а xk все наращивается
    // Все потому, что вектор указыает направление наискорейшего возрастания и очень большой
    // xk быстро наращивается
    // 2) yk или Hk или M_yk_t становится слишком мал
    // вследствие этого, на одной из итераций (когда близко к минимуму), мы получаем деление на нуль
    // Поэтому я выставил eps = 0.5 для того, чтобы метод остановился до деления на нуль
    // Вывод: Методы чуть менее чем полностью не приспособлены для минимизации не униминимальных функций 
    #region TEST2_1_1
    static void Test2_1_1()
    {
      // Input data
      FunctionNDByAlex funcND =                 (PointN point) => 
      {
        return SMath.Pow(SMath.Pow(point.At(0), 2) + point.At(1) - 11.0, 2.0) 
             + SMath.Pow(point.At(0) + SMath.Pow(point.At(1), 2) - 7.0, 2.0); 
      };
      FunctionNDMathNet funcNDMathNet = (VectorNMathNet point) =>
      {
        return SMath.Pow(SMath.Pow(point.At(0), 2) + point.At(1) - 11.0, 2.0)
             + SMath.Pow(point.At(0) + SMath.Pow(point.At(1), 2) - 7.0,  2.0);
      };

      PointN x0 = new PointN(3.0, 3.0);
      VectorNMathNet x0MathNet = Helpers.PointNToMathNet(x0);
      const double eps = 0.1; // TODO при слишком малых eps происходит деление на 0 в формуле пересчета Hk

      // Testing
      TestOptimalGradientMethod(funcND, x0, eps);
      TestVariableMetricMethod(funcNDMathNet, x0MathNet, eps);
    }
    #endregion


    #region TestMethods
    static Stopwatch sw = new Stopwatch();
    // Эти функции тестируют одноименные методы
    // И осуществляют форматированный вывод результатов их работы
    // Работает с NDimensionalPrimitives.PointN;
    static void TestOptimalGradientMethod(FunctionNDByAlex funcND, PointN x0, double eps)
    {
      OptimalGradientMethod ogm = new OptimalGradientMethod(funcND, x0);

      sw.Start();
      PointN result = ogm.FindMin(eps);
      sw.Stop();

      Console.Write(String.Join("\n", ogm.Log)); // Выводим все сообщения от этого метода
      Console.WriteLine("\n---------------------------------------------");
      Console.WriteLine("Optimal gradient method: x_min = {0}", result);
      Console.WriteLine("F(x_min) = {0}", funcND(result));
      Console.WriteLine("Time: {0}ms", sw.ElapsedMilliseconds);
      Console.WriteLine("=============================================");
    }

    // Работает с MathNet.Numerics.LinearAlgebra.Vector<double>;
    static void TestVariableMetricMethod(FunctionNDMathNet funcNDMathNet, VectorNMathNet x0, double eps)
    {
      VariableMetricMethod vmm = new VariableMetricMethod(funcNDMathNet, x0);

      sw.Start();
      VectorNMathNet result = vmm.FindMin(eps);
      sw.Stop();

      //Console.Write(String.Join("\n", vmm.Log)); // Выводим все сообщения от этого метода
      Console.WriteLine("Рабочая информация метода VariableMetric лежит в txt файле VariableMetricOutput");
      System.IO.File.WriteAllText(@"D:\Study\OptimizationMethods\3\VariableMetricOutput.txt", String.Join("\n", vmm.Log));
      Console.WriteLine("\n---------------------------------------------");
      Console.WriteLine("Variable metric method: x_min = {{{0} {1}}}", result[0].ToString("N6"), result[1].ToString("N6"));
      Console.WriteLine("F(x_min) = {0}", funcNDMathNet(result));
      Console.WriteLine("Time: {0}ms", sw.ElapsedMilliseconds);
      Console.WriteLine("=============================================");
    }
    #endregion
  }
}
