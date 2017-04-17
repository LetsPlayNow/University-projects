using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LinqToSql
{
    class LabWork
    {
        private DB_BOOKSDataContext _dc;
        public LabWork()
        {
            _dc = new DB_BOOKSDataContext();
        }

        private void PrintQueryResult(dynamic query)
        {
            foreach (var item in query)
            {
                Console.WriteLine(item);
            }
        }

        public void Task5()
        {
            var query = from houses in _dc.Publishing_houses
                        select new
                        {
                            Publish = houses.Publish,
                            City = houses.City,
                            Code_publish = houses.Code_publish
                        };

            PrintQueryResult(query);
        }


        public void Task9(string company_name)
        {
            var query = from d in _dc.Deliveries
                        where d.Name_company.Contains(company_name)
                        select new { d.Name_company, d.Phone, d.INN };

            PrintQueryResult(query);
        }

        public void Task15(DateTime a, DateTime b)
        {
            var query = from book in _dc.Books
                        join purchase in _dc.Purchases
                        on book.Code_book equals purchase.Code_book
                        where purchase.Date_order > a && purchase.Date_order < b
                        select new { book.Title_book, purchase.Amount };

            PrintQueryResult(query);
        }

        public void Task19(string[] publish_houses)
        {
            var query = from book in _dc.Books
                        join publishing_house in _dc.Publishing_houses
                        on book.Code_publish equals publishing_house.Code_publish
                        where publish_houses.Contains<string>(publishing_house.Publish)
                        select new { book.Title_book};

            PrintQueryResult(query);
        }

        public void Task25(int pages_min_count)
        {
            var query = from book in _dc.Books
                        join author in _dc.Authors
                        on book.Code_author equals author.Code_author
                        where book.Pages > pages_min_count
                        select new { author.Code_author, author.name_author };

            PrintQueryResult(query);
        }

        public void Task29(string publish)
        {
            var query = from deliveries in _dc.Deliveries
                        join purchases in _dc.Purchases on deliveries.Code_delivery equals purchases.Code_delivery
                        join books in _dc.Books on purchases.Code_book equals books.Code_book
                        join publishing_house in _dc.Publishing_houses on books.Code_publish equals publishing_house.Code_publish
                        where publishing_house.Publish.Contains(publish)
                        select new { deliveries.Name_company };

            PrintQueryResult(query);
        }

        public void Task35(DateTime a, DateTime b)
        {
            var query = from purchases in _dc.Purchases
                        where purchases.Date_order > a && purchases.Date_order < b
                        select new { purchases.Amount };
            Console.WriteLine("Summ is {0}", query.Sum(x => x.Amount));
            PrintQueryResult(query);
        }

        public void Task39(DateTime a, DateTime b)
        {
            var query = from books in _dc.Books
                        join purchases in _dc.Purchases on books.Code_book equals purchases.Code_book
                        where purchases.Date_order > a && purchases.Date_order < b
                        group purchases by purchases.Book.Title_book into unique_purchases

                        select new
                        {
                            BookName = unique_purchases.Key,
                            Itogo = unique_purchases.Sum(p => p.Amount * p.Cost)
                        };
            PrintQueryResult(query);
            
        }

        public void Task45()
        {
            throw new NotImplementedException();
        }

        public void Task55()
        {
            var query = from books in _dc.Books
                        join authors in _dc.Authors on books.Code_author equals authors.Code_author
                        where !(from books in _dc.Books select books.Code_author).Contains(authors.Code_author)
                        select new { authors.Code_author };
            PrintQueryResult(query);
        }

        public void Task59(DateTime date_order)
        {
            var query = from purchases in _dc.Purchases
                        where purchases.Date_order > date_order
                        select purchases;

            foreach (var item in query)
            {
                item.Cost *= 1.2;
            }

            _dc.SubmitChanges();
            PrintQueryResult(query);
        }

        public void Task65()
        {
            var query = from deliveries in _dc.Deliveries
                        where deliveries.INN == null
                        select deliveries;

            foreach (var item in query)
            {
                _dc.Deliveries.DeleteOnSubmit(item);
            }

            _dc.SubmitChanges();
        }
    }
}
