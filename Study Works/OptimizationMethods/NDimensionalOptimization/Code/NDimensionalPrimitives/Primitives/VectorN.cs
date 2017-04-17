using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NDimensionalPrimitives
{
  public class VectorN
  {
    public VectorN(int size)
    {
      Components = new List<double>(size);
      for (int i = 0; i < size; i++)
        Components.Add(0.0);
    }

    public VectorN(int size, double value)
    {
      Components = new List<double>(size);
      for (int i = 0; i < size; i++)
        Components.Add(value);
    }

    public VectorN(List<double> components)
    {
      Components = components;
    }

    public VectorN(params double[] components)
    {
      int dimensionsCount = components.Length;
      Components = new List<double>(components.Length);
      for (int i = 0; i < dimensionsCount; i++)
        Components.Add(components[i]);
    }

    public List<double> Components { get; set; }
    public double At(int index)
    {
      return Components[index];
    }

    public double Length
    {
      get
      {
        double summOfSquares = 0.0;
        foreach (var c in Components)
          summOfSquares += Math.Pow(c, 2);

        return Math.Sqrt(summOfSquares);
      }
    }

    public static VectorN operator *(VectorN multiplicand, double multiplier)
    {
      int dimensionsCount = multiplicand.Components.Count;
      VectorN composition = new VectorN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
        composition.Components[i] = multiplicand.Components[i] * multiplier;

      return composition;
    }

    public static VectorN operator /(VectorN dividend, double divider)
    {
      int dimensionsCount = dividend.Components.Count;
      VectorN quotient = new VectorN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
        quotient.Components[i] = dividend.Components[i] / divider;

      return quotient;
    }

    override public string ToString()
    {
      string point = "{ ";
      foreach (var component in Components)
        point += component.ToString("N6") + ' ';

      point += "}";
      return point;
    }
  }
}
