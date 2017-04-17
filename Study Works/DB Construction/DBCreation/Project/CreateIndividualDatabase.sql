-- Created by Titov Alex
-- My own original work
use IndividualTask
create table Employees(
	EmployeeId Int Primary Key,
	FirstName Char(40),
	SecondName Char(40),
	Patronimyc Char(40),
	Position Char(40),
	Subdivision Char(40),
	HireDate DateTime
)

create table Children(
	ChildrenId Int Primary Key,
	Name Char(40),
	DateOfBirth DateTime,
	EmployeeId Int Foreign Key References Employees(EmployeeId)
)

create table Gift(
	GiftId Int Primary Key,
	Cost Money,
	IssueDate DateTime,
	ChildrenId Int Foreign Key References Children(ChildrenId)
)