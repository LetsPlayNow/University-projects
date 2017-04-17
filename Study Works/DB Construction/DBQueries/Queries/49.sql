DECLARE CompaniesNamesCursor CURSOR FOR (select Deliveries.Name_company FROM Deliveries)
DECLARE @CompanyName CHAR(20)

OPEN CompaniesNamesCursor
WHILE @@FETCH_STATUS = 0
BEGIN
	FETCH NEXT FROM CompaniesNamesCursor INTO @CompanyName
	PRINT @CompanyName
END
CLOSE CompaniesNamesCursor
DEALLOCATE CompaniesNamesCursor