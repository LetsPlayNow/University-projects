CREATE PROC AuthorInfoByPagesCountAndPublishCode @PagesCountMin AS INT, 
												 @PublishCodeMin AS INT,
												 @PublishCodeMax AS INT
AS
	SELECT Authors.Code_author, Authors.name_author
	FROM Authors
	JOIN Books on Books.Code_author = Authors.Code_author
	WHERE Books.Pages > @PagesCountMin AND Books.Code_publish BETWEEN @PublishCodeMin AND @PublishCodeMax
GO