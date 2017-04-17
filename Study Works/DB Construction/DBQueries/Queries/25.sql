SELECT Authors.Code_author, Authors.name_author
FROM Authors
JOIN Books on Books.Code_author = Authors.Code_author
WHERE Books.Pages > 120 AND Books.Code_publish BETWEEN 5 AND 25