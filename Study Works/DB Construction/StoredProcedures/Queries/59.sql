CREATE PROCEDURE IncreasePurchasesByDateOrder @DateOrder as Date
AS
	PRINT 'Before Costs'
	SELECT Cost FROM Purchases

	UPDATE Purchases
	SET Cost=Cost*1.2
	WHERE Date_order > @DateOrder

	PRINT 'After Costs'
	Select Cost FROM Purchases
GO