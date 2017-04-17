CREATE PROC DeleteDeliveriesWithEmptyINN
AS
	DELETE FROM Deliveries
	WHERE INN = NULL
GO