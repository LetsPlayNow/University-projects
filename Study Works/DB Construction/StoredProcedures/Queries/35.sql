ALTER PROCEDURE GetPurchasesByDateRange @DateFrom AS Date,
										 @DateTo   AS Date
AS
	DECLARE @PurcasesCounts TABLE(
		Count int
	);

	INSERT INTO @PurcasesCounts
	SELECT Purchases.Amount 
	FROM Purchases
	WHERE Date_order BETWEEN @DateFrom AND @DateTo
	
	SELECT SUM(Count) FROM @PurcasesCounts 
GO