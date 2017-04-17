using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NDimensionalPrimitives
{
  public class PointN
  {
    public PointN(int size)
    {
      Coordinates = new List<double>(size);
      for (int i = 0; i < size; i++)
        Coordinates.Add(0.0);
    }

    public PointN(int size, double value)
    {
      Coordinates = new List<double>(size);
      for (int i = 0; i < size; i++)
        Coordinates.Add(value);
    }

    public PointN(List<double> coordinates)
    {
      Coordinates = coordinates;
    }

    public PointN(params double[] coordinates)
    {
      int dimensionsCount = coordinates.Length;
      Coordinates = new List<double>(coordinates.Length);
      for (int i = 0; i < dimensionsCount; i++)
        Coordinates.Add(coordinates[i]);
    }

    public PointN(PointN other)
    {
      int dimensionsCount = other.Coordinates.Count;
      Coordinates = new List<double>(dimensionsCount);
      for (int i = 0; i < dimensionsCount; i++)
        Coordinates.Add(other.Coordinates[i]);
    }

    public List<double> Coordinates { get; set; }
    public double At(int index)
    {
      return Coordinates[index];
    }

    public static PointN operator +(PointN a, PointN b)
    {
      int dimensionsCount = a.Coordinates.Count;
      PointN c = new PointN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
        c.Coordinates[i] = a.Coordinates[i] + b.Coordinates[i];

      return c;
    }

    public static PointN operator -(PointN a, PointN b)
    {
      int dimensionsCount = a.Coordinates.Count;
      PointN c = new PointN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
        c.Coordinates[i] = a.Coordinates[i] - b.Coordinates[i];

      return c;
    }

    public static PointN operator *(PointN multiplicand, double multiplier)
    {
      int dimensionsCount = multiplicand.Coordinates.Count;
      PointN composition = new PointN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
        composition.Coordinates[i] = multiplicand.Coordinates[i] * multiplier;

      return composition;
    }

    public static PointN operator +(PointN point, VectorN vector)
    {
      int dimensionsCount = point.Coordinates.Count;
      PointN resultPoint = new PointN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
        resultPoint.Coordinates[i] = point.Coordinates[i] + vector.Components[i];

      return resultPoint;
    }

    public static PointN operator -(PointN point, VectorN vector)
    {
      int dimensionsCount = point.Coordinates.Count;
      PointN resultPoint = new PointN(dimensionsCount);

      for (int i = 0; i < dimensionsCount; i++)
        resultPoint.Coordinates[i] = point.Coordinates[i] - vector.Components[i];

      return resultPoint;
    }

    override public string ToString()
    {
      string point = "{ ";
      foreach (var coordinate in Coordinates)
        point += coordinate.ToString("N6") + ' ';

      point += "}";
      return point;
    }
  }
}
