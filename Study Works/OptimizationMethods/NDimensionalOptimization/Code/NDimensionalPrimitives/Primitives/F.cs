using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NDimensionalPrimitives
{
  /// <summary>
  /// N - dimensional function at which we find the minimum 
  /// </summary>
  public class F
  {
    public F(Func<PointN, double> function)
    {
      m_function = function;
    }

    public double Value(PointN point)
    {
      return m_function(point);
    }
   
    protected Func<PointN, double> m_function;
  }

  public class FunctionN : F
  {
    public FunctionN(Func<PointN, double> function) : base(function)
    {}
  }
}


