use DB_BOOKS_GRAPH
drop table Publishing_house

create table Publishing_house(
	Code_publish INT PRIMARY KEY,
	Publish CHAR(30),
	City CHAR(20))
	
create table Books(
	Code_book INT PRIMARY KEY,
	Title_book CHAR(40),
	Code_author INT FOREIGN KEY REFERENCES Author(Code_author),
	Pages INT,
	Code_publish INT FOREIGN KEY REFERENCES Publishing_house(Code_publish))
	
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
