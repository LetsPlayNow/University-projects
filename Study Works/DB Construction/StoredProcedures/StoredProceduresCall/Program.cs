using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StoredProceduresCall
{
    class Program
    {
        static void Main(string[] args)
        {
            // создаем и открываем подключение к базе данных
            using (SqlConnection conn = new SqlConnection(GetDatabaseConnection()))
            {
                conn.Open();
                CallProcedureWithReturnValue(conn);
                CallProcedureWithReturnTable(conn);
                conn.Close();
                Console.ReadKey();
            }
        }


        // 35
        private static void CallProcedureWithReturnValue(SqlConnection conn)
        {
            SqlCommand procedure = new SqlCommand("GetPurchasesByDateRange", conn);
            procedure.CommandType = CommandType.StoredProcedure;
            string date_from = "01.01.2003";
            string date_to = "01.02.2016";
            procedure.Parameters.AddWithValue("@DateFrom", date_from);
            procedure.Parameters.AddWithValue("@DateTo", date_to);
            var count_of_purchases = procedure.ExecuteScalar();
            Console.WriteLine("Count of purchases from {0} to {1} is {2}", date_from, date_to, (int) count_of_purchases);
        }

        // 39
        private static void CallProcedureWithReturnTable(SqlConnection conn)
        {
            SqlCommand procedure = new SqlCommand("GetBookTitleByDateOrderRange", conn);
            procedure.CommandType = CommandType.StoredProcedure;
            string date_from = "01.01.2003";
            string date_to = "01.02.2016";
            procedure.Parameters.AddWithValue("@DateFrom", date_from);
            procedure.Parameters.AddWithValue("@DateTo", date_to);
            SqlDataReader reader = procedure.ExecuteReader();

            Console.WriteLine("Book title and summ of selling for books published from {0} to {1}", date_from, date_to);
            while (reader.Read())
            {
                for (int i = 0; i < reader.FieldCount; i++)
                {
                    Console.Write(reader[i]);
                    Console.Write(" | ");
                }
                Console.WriteLine();
            }

            reader.Close();
        }

        private static string GetDatabaseConnection()
        {
            return "server=.\\SQLEXPRESS;" +
                "integrated security=SSPI;" +
                "database=DB_BOOKS";
        }
    }
}