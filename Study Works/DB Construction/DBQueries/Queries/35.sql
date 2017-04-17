DECLARE @PurcasesCounts TABLE(
	Count int
);

INSERT INTO @PurcasesCounts
SELECT Purchases.Amount 
FROM Purchases
WHERE Date_order BETWEEN '01.01.2003' AND '02.01.2015'

SELECT SUM(Count) FROM @PurcasesCounts
