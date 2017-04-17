#include "DataBase.h"
#include <qapplication.h>
#include <stdio.h>
#include <Windows.h>
DataBase * DataBase::to_use_signals;
bool DataBase::reconnect(char * inet_address)
{
	//Закрыть сокет, если он был открыт
	//То же касается и слотов и всякой другой всячины
	send_request(EXIT);
	Sleep(100); //Ждем, пока отправляется запрос
	closesocket(client_service);
	closesocket(client_update);
	TerminateThread(t_update_client, EXIT_SUCCESS);

	char adress[16];
	strcpy(adress, inet_address);

	short int port_service = htons(50000); //Возможно для обеих каналов разные порты должны быть
	short int port_update  = htons(50001);

	//Настройка TCP соединения
	client_service = socket(AF_INET, SOCK_STREAM, 0);
	client_update  = socket(AF_INET, SOCK_STREAM, 0);

	//Настройка свойств сокетов
	sockaddr_in client_service_adress;
	client_service_adress.sin_family = AF_INET;
	client_service_adress.sin_port = port_service;
	client_service_adress.sin_addr.S_un.S_addr = inet_addr(adress);

	sockaddr_in client_update_adress;
	client_update_adress.sin_family = AF_INET;
	client_update_adress.sin_port = port_update;
	client_update_adress.sin_addr.S_un.S_addr = inet_addr(adress);

	//Соединение
	int sock_addr_size_service = sizeof(client_service_adress);
	int sock_addr_size_update = sizeof(client_update_adress);
	
	if (::connect(client_service, (sockaddr *)&client_service_adress, sock_addr_size_service) == SOCKET_ERROR) return false;
	if (::connect(client_update, (sockaddr *)&client_update_adress, sock_addr_size_update) == SOCKET_ERROR)    return false;

	//Закрываем предыдущий поток обновления клиента, если он существовал
	t_update_client = CreateThread(NULL, 0, update_cathcer, (LPVOID)client_update, NULL, NULL);
	return true;
}

DataBase::DataBase()
{
	to_use_signals = this;
	freopen("error.log", "w", stderr);
	short int port_service = htons(50000); //Возможно для обеих каналов разные порты должны быть
	short int port_update = htons(50001);

	//Настройка TCP соединения
	client_service = socket(AF_INET, SOCK_STREAM, 0);
	client_update = socket(AF_INET, SOCK_STREAM, 0);

	//Настройка свойств сокетов
	sockaddr_in client_service_adress;
	client_service_adress.sin_family = AF_INET;
	client_service_adress.sin_port = port_service;
	client_service_adress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	sockaddr_in client_update_adress;
	client_update_adress.sin_family = AF_INET;
	client_update_adress.sin_port = port_update;
	client_update_adress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	//Соединение
	int sock_addr_size_service = sizeof(client_service_adress);
	int sock_addr_size_update = sizeof(client_update_adress);

	if (::connect(client_service, (sockaddr *)&client_service_adress, sock_addr_size_service) == SOCKET_ERROR) return;
	if (::connect(client_update, (sockaddr *)&client_update_adress, sock_addr_size_update) == SOCKET_ERROR)    return;

	//update_info info = { client_update, this };
	//Здесь создать поток ловителя обновлений
	t_update_client = CreateThread(NULL, 0, update_cathcer, (LPVOID)client_update, NULL, NULL);
}

DWORD WINAPI DataBase::update_cathcer(LPVOID p)
{
	SOCKET info = (SOCKET)p;
	bool update = false;
	while (true)
	{
		if (recv(info, (char *)&update, sizeof(bool), 0) == SOCKET_ERROR)
			perror("DataBase::update_catcher");
		if (update) 
			emit to_use_signals->need_update();
		update = false;
	}
	return EXIT_SUCCESS;
}

DataBase::~DataBase()
{
	send_request(EXIT);
	closesocket(client_service);
	closesocket(client_update);
}


int DataBase::count()
{
	send_request(COUNT);

	//<- short int count
	short int count;
	recv(client_service, (char *)&count, sizeof(short int), 0);
	count = ntohs(count);

	return count;
}


index_id DataBase::append(Note record)
{
	send_request(APPEND);

	//-> Note record
	record.send(client_service);

	//<- index_id note_info
	index_id note_info;
	recv(client_service, (char *)&note_info, sizeof(index_id), 0);
	
	return note_info;
}


void DataBase::remove(u_int id)
{
	send_request(REMOVE);

	//-> short int id
	short int out_id = htons(id);
	send(client_service, (char *)&out_id, sizeof(short int), 0);
}


int DataBase::update(Note record)
{
	send_request(UPDATE);

	//-> Note record
	record.send(client_service);

	//<- short int index
	short int index;
	recv(client_service, (char *)&index, sizeof(short int), 0);
	index = ntohs(index);

	return index;
}

/*В случае неудачи возвращает запись с id == -1*/
Note DataBase::record(u_int id)
{
	send_request(RECORD);

	//-> short int id
	short unsigned int in_id = htons((short unsigned int)id);
	send(client_service, (char *)&in_id, sizeof(short unsigned int), 0);

	//<- Note record
	Note record;
	record.recv(client_service);

	return record;
}


QVector<Note> DataBase::records()
{
	send_request(RECORDS);
	short int count;

	//<- short int size
	recv(client_service, (char *)&count, sizeof(short int), 0);
	count = ntohs(count);

	QVector<Note> records;
	Note next_note;
	for (int i = 0; i < count; i++)
	{
		next_note.recv(client_service);
		records.push_back(next_note);
	}

	return records;
}


bool DataBase::send_request(requests request)
{
	if (send(client_service, (char *)&request, sizeof(requests), 0) == SOCKET_ERROR) return false;
	else return true;
}