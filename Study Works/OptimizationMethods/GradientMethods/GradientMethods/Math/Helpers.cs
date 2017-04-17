using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NDimensionalPrimitives;
using MathNet.Numerics.LinearAlgebra;

namespace GradientMethods.Math
{
  class Helpers
  {
    public static Vector<double> PointNToMathNet(PointN point)
    {
      double[] coordinates = new double[point.Coordinates.Count];
      point.Coordinates.CopyTo(coordinates);
      return Vector<double>.Build.DenseOfArray(coordinates);
    }

    public static Vector<double> VectorNToMathNet(VectorN point)
    {
      double[] components = new double[point.Components.Count];
      point.Components.CopyTo(components);
      return Vector<double>.Build.DenseOfArray(components);
    }
  }
}
