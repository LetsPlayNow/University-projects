using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using NDimensionalPrimitives;
namespace MethodHookJivs
{
  public static class HookJivs
  {
    public static PointN MethodHookJivs(F function, PointN b1, VectorN h, double eps, double z = 0.1)
    {
      PointN x;
      do
      {
        do
        {
          PointN xk = b1;
          PointN b2 = HookJivsHelper.ExploringSearch(function, xk, h);
          Console.WriteLine("> Exploring search ( xk{0} ) = b2{1}", xk.ToString(), b2.ToString());
          
          do
          {
            Console.WriteLine("> Doing step xk = b1 + (b2 - b1) * 2, will give xk{0}", xk.ToString());
            xk = b1 + (b2 - b1) * 2;
            x = HookJivsHelper.ExploringSearch(function, xk, h);
            Console.WriteLine("> Exploring search ( xk{0} ) = x{1}", xk.ToString(), x.ToString());
            b1 = b2;
            b2 = x;
          } while (function.Value(x) < function.Value(b1));

        } while (function.Value(x) > function.Value(b1));


        if (h.Length <= eps)
          break;
        
        h = h * z;
      }while(true);

      return b1;
    }
  }

  class HookJivsHelper
  {
    public static PointN ExploringSearch(F function, PointN basisPoint, VectorN h)
    {
      double fb = function.Value(basisPoint);
      int dimensionsCount = basisPoint.Coordinates.Count;
      for (int i = 0; i < dimensionsCount; i++)
      {
        // hi * ei
        PointN hi_ei = new PointN(dimensionsCount);
        hi_ei.Coordinates[i] = h.Components[i];
        double f = function.Value(basisPoint + hi_ei);

        if (f < fb)
        {
          basisPoint = basisPoint + hi_ei;
          fb = f;
        }
        else
        {
          f = function.Value(basisPoint - hi_ei);
          if (f < fb)
          {
            basisPoint = basisPoint - hi_ei;
            fb = f;
          }
        }
      }

      return basisPoint;
    }
  }
}
