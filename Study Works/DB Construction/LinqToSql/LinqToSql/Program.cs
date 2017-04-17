using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LinqToSql
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.OutputEncoding = new UTF8Encoding();
            LabWork work = new LabWork();
            //work.Task5();
            //work.Task9("ИП");
            //work.Task15(new DateTime(2000, 01, 01), new DateTime(2017, 01, 01));
            //work.Task19(new string[]{ "Питер - Софт", "Альфа", "Наука" });
            //work.Task25(120);
            //work.Task29("Малыш");
            //work.Task35(new DateTime(2000, 01, 01), new DateTime(2017, 01, 01));
            //work.Task39(new DateTime(1996, 01, 01), new DateTime(2040, 01, 01));
            //work.Task55();
            //work.Task59(new DateTime(1996, 01, 01));
            work.Task65();
            Console.Read();
        }
    }
}
