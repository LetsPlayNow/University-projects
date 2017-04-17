SELECT Books.Title_book, Purchases.Amount
FROM Books 
JOIN Purchases ON Purchases.Code_book = Books.Code_book
WHERE Purchases.Date_order BETWEEN '03.12.2000' AND '06.15.2030'