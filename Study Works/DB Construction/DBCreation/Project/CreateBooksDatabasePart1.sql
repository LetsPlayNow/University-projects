use DB_BOOKS_GRAPH
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
	