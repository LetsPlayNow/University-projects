CREATE PROC BooksInfoByDateRange @DateFrom AS DATE, @DateTo AS DATE
AS
	SELECT Books.Title_book, Purchases.Amount
	FROM Books 
	JOIN Purchases ON Purchases.Code_book = Books.Code_book
	WHERE Purchases.Date_order BETWEEN @DateFrom AND @DateTo
GO