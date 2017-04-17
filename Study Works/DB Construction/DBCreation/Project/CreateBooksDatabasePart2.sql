create table Deliveries(
	Code_delivery INT PRIMARY KEY,
	Name_delivery CHAR(40),
	Name_company CHAR(40),
	Delivery_address CHAR(40),
	Phone NUMERIC,
	INN CHAR(40)
)

create table Purchases(
	Code_purchase INT PRIMARY KEY,
	Code_book INT FOREIGN KEY REFERENCES Books(Code_book),
	Date_order DATETIME,
	Code_delivery INT FOREIGN KEY REFERENCES Deliveries(Code_delivery),
	Type_purchase BIT,
	Cost MONEY,
	Amount INT
)
