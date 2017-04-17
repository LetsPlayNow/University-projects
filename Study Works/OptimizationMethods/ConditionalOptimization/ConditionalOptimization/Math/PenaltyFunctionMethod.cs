using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using MethodHookJivs; // Метод многомерной минимизации Хука - Дживса 
using NDimensionalPrimitives;

// TODO Что такое функции ограничения в методе это f(i)?
namespace ConditionalOptimization.Math
{
  using FunctionND = System.Func<PointN, double>;
  public class PenaltyFunctionMethod
  {
    // Input data
    private FunctionND f;
    private List<FunctionND> limitFunctions;
    private PointN x0;
    private double eps;
    private int m;
    private int p;
    private double r0;
    private double z;

    public List<string> Log;
    /// <summary>
    /// Метод последовательной безусловной оптимизации
    /// Метод штрафных функций
    /// </summary>
    /// <param name="f">Минимизируемая функция</param>
    /// <param name="fi"> Список функций ограничений fi  от 1   до m</param>
    /// <param name="_fi">Список функций ограничений _fi от m+1 до p</param>
    /// <param name="x0"> Начальная точка</param>
    /// <param name="eps">Параметр точности поиска (>0)</param>
    /// <param name="m">Число функций ограничений _fi</param>
    /// <param name="p">(p - m) Число функций ограничений fi</param>
    /// <param name="r0">Начальное значение параметра штрафа</param>
    /// <param name="z">Коэффициент умножения параметра штрафа</param>
    public PenaltyFunctionMethod(FunctionND f, List<FunctionND> limitFunctions, PointN x0, int m, int p, double r0 = 0.1, double z = 6)
    {
      this.f = f;
      this.limitFunctions = limitFunctions;
      this.x0 = x0;
      this.r0 = r0;
      this.z = z;
      this.m = m;
      this.p = p;

      Log = new List<string>(); // TODO: здесь концепция Log не работает
    }

    public PointN FindMin(double eps)
    {
      int k = 0;
      int dimensionsCount = x0.Coordinates.Count;
      
      // Промежуточные данные
      PointN xk = new PointN(x0);
      PointN x_rk;
      double rk = r0;

      do
      {
        // Поиск минимума F(x, r)
        F Fxr = new F((PointN point) => { return F(point, rk); });
        Console.WriteLine(">>> ++++++++++++++++++++HookJivs started++++++++++++++++++++");
        x_rk = HookJivs.MethodHookJivs(Fxr, xk, new VectorN(dimensionsCount, 1.0), eps);
        Console.WriteLine(">>> ++++++++++++++++++++HookJivs finished++++++++++++++++++++");

        // Значение штрафной функции в xrk
        double P_x_rk = P(x_rk, rk); // TODO в данных условиях не имеет смысла, т.к. нигде далее не используется и пересчитывается заново
        Console.WriteLine("P(xk, rk) = {0}", P(xk, rk));  //Log.Add(String.Format("P(xrk, rk) = {0}", P_x_rk));
        Console.WriteLine("f(xk) = {0}", f(xk));
        Console.WriteLine("F(xk, rk)  = {0}", F(xk, rk));

        // Инкремент цикла
        rk = rk * z;
        xk = x_rk;
        k++;
        Console.WriteLine("==================Next Iteration====================");  //Log.Add("==================Next Iteration====================");
      } while (P(x_rk, rk) >= eps);

      return x_rk;
    }

    // Вспомогательная функция
    private double F(PointN x, double rk) {  return P(x, rk) + f(x);  }


    // Штрафная функция (Penalty function)
    private double P(PointN x, double rk)
    {
      double sumFi  = 0;
      double sum_Fi = 0;

      // Цикл по всем функциям органичений типа F(x) >= 0
      for (int i = 0; i < m; i++)
        sumFi += _PFi(x, i);

      // Цикл по всем функциям органичений типа F(x) = 0
      for (int i = m; i < p; i++)
        sum_Fi += PFi(x, i);

      return rk * (sumFi + sum_Fi);
    }


    // Схема с параболами
    private double _PFi(PointN x, int index) {  return System.Math.Pow(System.Math.Min(0, limitFunctions[index](x)), 2);  }
    private double PFi(PointN x, int index)  {  return System.Math.Pow(limitFunctions[index](x), 2);  }
  }  
}