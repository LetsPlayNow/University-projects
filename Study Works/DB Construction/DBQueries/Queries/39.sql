SELECT Books.Title_book, SUM(Purchases.Cost * Purchases.Amount) as Itogo
FROM Books
INNER JOIN Purchases on Purchases.Code_book = Books.Code_book
WHERE Purchases.Date_order BETWEEN '01.01.2000' AND '06.01.2015'
GROUP BY Books.Title_book
