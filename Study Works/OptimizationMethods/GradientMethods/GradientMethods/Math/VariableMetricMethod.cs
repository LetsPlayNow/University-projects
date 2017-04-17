using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MathNet.Numerics;
using MathNet.Numerics.LinearAlgebra;

namespace GradientMethods.Math
{
  using PointNMathNet  = Vector<double>;
  using VectorNMathNet = Vector<double>;
  using FunctionND = Func<Vector<double>, double>;
  using Function1D = Func<double, double>;
  
  class VariableMetricMethod
  {
    private FunctionND funcND;
    private PointNMathNet x0;
    int n; // Размерность задачи
    Matrix<double> H0; // Единичная матрица (nxn)

    public List<string> Log;
    public VariableMetricMethod(FunctionND funcND, PointNMathNet x0)
    {
      this.funcND = funcND;
      this.x0 = x0.Clone();
      n = x0.Count;
      H0 = Matrix<double>.Build.DenseDiagonal(n, 1.0);

      Log = new List<string>();
    }

    // Это хорошая идея - выносить параметры вроде eps сюда
    // Можно добавить список строк List<String>
    // И выводить в него информацию по алгоритму
    // Затем, в output мы берем его и выводим на экран (по желанию) 
    public PointNMathNet FindMin(double eps)
    {
      int k = 0;
      PointNMathNet xk   = x0; // x(k)
      PointNMathNet xk_1 = x0; // x(k-1)
      Matrix<double> Hk = H0.Clone(); // Квазиньютоновская матрицы
      double ak = 0.0;

      // Вспомогательные вектора
      VectorNMathNet sk = VectorNMathNet.Build.Dense(n);
      VectorNMathNet yk = VectorNMathNet.Build.Dense(n);
      while (true)
      {
        VectorNMathNet gradient = Gradient.Calculate(funcND, xk);
        Log.Add(String.Format("Grad(F(\n{0, -23})) = \n{1, -25}", xk, gradient)); // TODO: некрасивый вывод
        if (gradient.L2Norm() <= eps) // градиент слишком мал, максимум очень близко
          return xk;

        if (k != 0)
        {
          sk = xk - xk_1;
          //Log.Add(String.Format("sk:\n{0}", sk));
          Matrix<double> m_sk = sk.ToColumnMatrix();
          Matrix<double> m_sk_t = m_sk.Transpose();

          yk = Gradient.Calculate(funcND, xk) - Gradient.Calculate(funcND, xk_1); // TODO Один из членов этого вектора становится слишком мал (или Hk или M_yk_t)
          //Log.Add(String.Format("yk:\n{0}", yk));
          Matrix<double> m_yk   = yk.ToColumnMatrix();
          Matrix<double> m_yk_t = m_yk.Transpose();

          Hk = Hk - ((Hk * m_yk * m_yk_t * Hk) / (m_yk_t * Hk * m_yk)[0,0]) + ((m_sk * m_sk_t)[0,0] / (m_yk_t * m_sk)[0,0]);
          Log.Add(String.Format("Hk:\n{0}", Hk));
        }

        VectorNMathNet pk = -Hk * gradient;
        Function1D func1D = (double alpha) => { return funcND((xk + pk * alpha)); };
        ak = OneDimensionalMinimization.FindMin(func1D, ak, eps);
        Log.Add(String.Format("ak = {0}", ak));

        // Инкремент
        xk_1 = xk;
        xk = xk + ak * pk;
        k++;
        Log.Add("==================Next Iteration====================");
      }
    }
  }
}