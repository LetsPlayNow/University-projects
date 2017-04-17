#include "mediator.h"

mediator::mediator(SOCKET s_client, DataBase & server_DB) 
:socket(s_client),
 DB(server_DB)
{}


mediator::~mediator()
{}


bool mediator::count()
{
	//Обработка
	short int count = htons(DB.count());

	//-> int count
	if (::send(socket, (char *)&count, sizeof(short int), 0) == SOCKET_ERROR) return false;

	return true;
}

bool mediator::append()
{
	Note record;

	//<- Note record
	if (record.recv(socket) == false) return false;

	//Обработка
	index_id note_info = DB.append(record);

	//-> index_id note_info
	if (::send(socket, (char *)&note_info, sizeof(index_id), 0) == SOCKET_ERROR) return false;

	return true;
}

bool mediator::remove()
{
	short int id;

	//<- short int id
	if (::recv(socket, (char *)&id, sizeof(short int), 0) == SOCKET_ERROR) return false;
	id = ntohs(id);

	//Обрабатка
	bool remove_success = DB.remove(id);

	return true;
}

bool mediator::update()
{
	Note record;

	//<- Note
	if (record.recv(socket) == false) return false;

	//Обработка
	short int index = ::htons(DB.update(record));

	//-> index
	if (::send(socket, (char *)&index, sizeof(short int), 0) == SOCKET_ERROR) return false;

	return true;
}

bool mediator::record()
{
	short int id;

	//<- short int id
	if (::recv(socket, (char*)&id, sizeof(short int), 0) == SOCKET_ERROR) return false;
	id = ntohs(id);

	//Обработка
	Note record = DB.record(id);

	//-> Note record
	if (record.send(socket) == false) return false;
	
	return true;
}

bool mediator::records()
{
	//-> short int size
	short int size = ::htons(DB.count());
	if (::send(socket, (char *)&size, sizeof(short int), 0) == SOCKET_ERROR) return false;

	//-> QVector <Note> records
	QVector <Note> temp = DB.records();
	for (QVector <Note>::iterator i = temp.begin(); i != temp.constEnd(); i++)
		if ((*i).send(socket) == false) return false;
		
	return true;
}