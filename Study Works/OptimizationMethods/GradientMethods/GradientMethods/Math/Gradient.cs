using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NDimensionalPrimitives;
using MathNet.Numerics.LinearAlgebra;

namespace GradientMethods.Math
{
  using VectorNMathNet = Vector<double>;
  using PointNMathNet  = Vector<double>;

  class Gradient
  {
    // Центральная разностная схема
    //        f(x+h) - f(x-h)
    // f(x) = ----------------
    //               2h
    public static VectorN Calculate(Func<PointN, double> f, PointN point, double h = 0.01)
    {
      int dimensionsCount = point.Coordinates.Count;
      VectorN gradient = new VectorN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
      {
        PointN xMinusH = new PointN(point);
        xMinusH.Coordinates[i] -= h;
        PointN xPlusH  = new PointN(point);
        xPlusH.Coordinates[i]  += h;
        gradient.Components[i] = (f(xPlusH) - f(xMinusH)) / (2 * h);
      }

      return gradient;
    }

    public static VectorNMathNet Calculate(Func<PointNMathNet, double> f, PointNMathNet point, double h = 0.1)
    {
      int n = point.Count;
      VectorNMathNet gradient = VectorNMathNet.Build.Dense(n);

      for (int i = 0; i < n; i++)
      {
        PointNMathNet xMinusH = point.Clone();
        xMinusH[i] -= h;
        PointNMathNet xPlusH = point.Clone();
        xPlusH[i]  += h;
        gradient[i] = (f(xPlusH) - f(xMinusH)) / (2 * h);
      }

      return gradient;
    }
  }
}
