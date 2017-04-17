#pragma once
#include "Note.h"
#include <qmap.h>
#include <qstringlist.h>
#include <Windows.h>
#include <qdatetime.h>
#include <qvector.h>
#include "request.h"
#include "error_codes.h"
#include <process.h>
#include "D:\Projects\VS_2013\OS_3\Server\index_id.h"

typedef unsigned int u_int;
class DataBase : public QObject
{
	Q_OBJECT

public:
	HANDLE t_update_client; //Дескриптор потока обновления клиента
	struct update_info      //Структура, передаваемая в функцию потока обновления
	{
		SOCKET s_update;
		DataBase * db;
	};
	static DataBase *to_use_signals;
	DataBase();
	~DataBase(void);

	bool reconnect(char * inet_address);

	//Отправляет значение переменной request серверу
	bool send_request(requests request);

	int count();

	//Функции изменения записей в базе данных
	index_id append(Note record);
	void remove(u_int id);
	int update(Note record);

	//Функции получения записей от сервера
	Note record(u_int id);
	QVector<Note> records();
	
	static DWORD WINAPI update_cathcer(LPVOID p);
signals:
	void need_update();
	void error_exist(int);
	

private:
	SOCKET client_service, client_update;
};