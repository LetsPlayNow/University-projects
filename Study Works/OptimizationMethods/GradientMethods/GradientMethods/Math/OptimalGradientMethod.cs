using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NDimensionalPrimitives;


namespace GradientMethods.Math
{
  using Function1D = Func<double, double>;
  using FunctionND = Func<PointN, double>;

  class OptimalGradientMethod
  {
    private FunctionND funcND;
    private PointN x0;
    public List<string> Log;
    public OptimalGradientMethod(FunctionND funcND, PointN x0)
    {
      this.funcND = funcND;
      this.x0 = new PointN(x0);

      Log = new List<string>();
    }

    public PointN FindMin(double eps)
    {
      int dimensionsCount = x0.Coordinates.Count;
      int k = 0;
      double ak = 0.0;
      PointN xk = x0;
      while(true)
      {
        VectorN gradFxk = Gradient.Calculate(funcND, xk); // Градиент все время прыгает во все стороны, а xk все наращивается
        Log.Add(String.Format("Grad(F({0, -23})) = {1, -25}", xk, gradFxk));

        if (gradFxk.Length <= eps)
          return xk;

        Function1D func1D = (double alpha) => { return funcND(xk - gradFxk * alpha); }; // TODO alpha >= 0
        ak = OneDimensionalMinimization.FindMin(func1D, ak, eps); // TODO негибкая стратегия выбора eps
        Log.Add(String.Format("ak = {0}", ak));

        xk = xk - gradFxk * ak;
        k++;
        Log.Add("==================Next Iteration====================");
      }
    }
  }
}
