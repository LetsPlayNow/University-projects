CREATE PROCEDURE GetBookTitleByDateOrderRange @DateFrom AS DATE,
											  @DateTo   AS DATE
AS
	SELECT Books.Title_book, SUM(Purchases.Cost * Purchases.Amount) as Itogo
	FROM Books
	INNER JOIN Purchases on Purchases.Code_book = Books.Code_book
	WHERE Purchases.Date_order BETWEEN @DateFrom AND @DateTo
	GROUP BY Books.Title_book
GO