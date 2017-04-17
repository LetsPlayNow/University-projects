using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

using MethodRandomSearch;
using MethodHookJivs;
using NDimensionalPrimitives;

namespace Test
{
  class Program
  {
    static void Main(string[] args)
    {
      TestRandomSearch();
      //TestHookJivs();
    }

    public static void TestHookJivs()
    {
      Console.WriteLine("Method Hook Jivs");
      PointN basicPoint = new PointN(100.0, 100.0);
      VectorN h = new VectorN(1.0, 1.0);

      const double eps = 0.001;
      F minimizedFunction = new F((point) => 4 * Math.Pow(point.Coordinates[0] - 5.123, 2) + Math.Pow(point.Coordinates[1] - 6.789, 2));

      PointN calculatedMinimum = MethodHookJivs.HookJivs.MethodHookJivs(minimizedFunction, basicPoint, h, eps);
      PointN expectedMinimum = new PointN(new List<double> { 1.0, 1.0 });

      Console.WriteLine("=================================================================");
      Console.WriteLine("Minimum Point: {0} ", calculatedMinimum.ToString());
      Console.WriteLine("F({0}) = {1}",  calculatedMinimum.ToString(), minimizedFunction.Value(calculatedMinimum));
      Console.WriteLine("Press enter to close...");
      Console.ReadLine();
    }

    public static void TestRandomSearch()
    {
      Console.WriteLine("Method Random Search");
      PointN basicPoint = new PointN(8.0, 9.0);

      const double alpha = 1.618,
                   betta = 0.618,
                   t0 = 1,
                   R = 0.8;
      const int N = 10,
                M = 3;
      F minimizedFunction = new F((point) => 4 * Math.Pow(point.Coordinates[0] - 5, 2) + Math.Pow(point.Coordinates[1] - 6, 2));

      PointN calculatedMinimum = MethodRandomSearch.RandomSearch.MethodRandomSearch(minimizedFunction, basicPoint, alpha, betta, t0, R, N, M);
      PointN expectedMinimum = new PointN(new List<double> { 1.0, 1.0 });

      Console.WriteLine("=================================================================");
      Console.WriteLine("Minimum Point: {0} ", calculatedMinimum.ToString());
      Console.WriteLine("F({0}) = {1}", calculatedMinimum.ToString(), minimizedFunction.Value(calculatedMinimum));
      Console.WriteLine("Press enter to close...");
      Console.ReadLine();
    }
  }
}
