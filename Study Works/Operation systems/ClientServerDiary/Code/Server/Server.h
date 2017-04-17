#pragma once
#include "mediator.h"
#include "server_requests.h"
#include "Semaphore.h"
#include <qobject.h>
#include <map>

using std::map;
using std::vector;
using std::pair;

class Server : public QObject
{
	Q_OBJECT
public:
	static Server * to_use_signals;
	Server();
	~Server();

	//Основные серверные функции
	static DWORD WINAPI service_client(LPVOID p);
	static DWORD WINAPI server_control(LPVOID p);
	static DWORD WINAPI shutdown(LPVOID p);
	static DWORD WINAPI autosave(LPVOID p);


	//Функции обслуживания клиента
	static void count(mediator &, int &);
	static void append(mediator &, int &);
	static void remove(mediator &, int &);
	static void update(mediator &, int &);
	static void record(mediator &, int &);
	static void records(mediator &, int &);
	static void to_file();

	
	void operating_cycle();    //Запуск сервера
private:
	short int port_service;
	short int port_update;
	static DataBase server_DB;

	//Серверные константы 
	//============================================
	static int clients_count;    //Число подключенных клиентов
	static int errors_limit;     //Лимит числа ошибок (до отключения)
	static int operations_count; //Число совершенных операций над базой данных
	static int autosave_period;  //Через какое число операций сервер должен обновляться
	static map <SOCKET, SOCKET> clients; //Первый Хэндл - ключ - для канала свзи с клиентом, второй - от канала автообновления
	static vector <HANDLE> threads;
	//Блокировки
	//=====================================
	static const int locks_count = 5;
	enum global_locks
	{
		AD,
		Count,
		Records_and_toFile,
		Record,
		Update
	};

	static Mutex global_locks[locks_count];

	//Обновление клиентов
	static void update_clients(SOCKET);
};

