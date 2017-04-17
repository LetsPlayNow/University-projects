DECLARE @CompaniesNames TABLE(
	CompanyName char(20),
	TitleBook char(20)
);

INSERT @CompaniesNames
SELECT Deliveries.Name_company, Books.Title_book
FROM Deliveries
JOIN Purchases on Deliveries.Code_delivery = Purchases.Code_delivery
JOIN Books on Purchases.Code_book = Books.Code_book

select *
FROM @CompaniesNames