PRINT 'Before Costs'
SELECT Cost FROM Purchases

UPDATE Purchases
SET Cost=Cost*1.2
WHERE Date_order > '01.01.2010'

PRINT 'After Costs'
Select Cost FROM Purchases
