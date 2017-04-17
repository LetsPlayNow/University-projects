CREATE PROCEDURE GetAuthorsWithoutBooks
AS
	SELECT Authors.name_author
	FROM Books
	JOIN Authors on Books.Code_author = Authors.Code_author
	WHERE Authors.Code_author NOT IN(SELECT Books.Code_author)
GO