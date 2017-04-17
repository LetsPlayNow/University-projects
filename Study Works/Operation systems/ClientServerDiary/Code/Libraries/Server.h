#pragma once
#include <Windows.h>
#include <Windows.h>
#include "mediator.h"
#include "qlocale.h"
#include "server_requests.h"
#include "Semaphore.h"
class Server
{
public:
	Server();
	~Server();

	static::DWORD WINAPI service_client(LPVOID p);
	static::DWORD WINAPI server_control(LPVOID p);

	void operating_cycle();

private:
	void connect_new_client();
	void shutdown_client(HANDLE pipe);

	TCHAR channel_name[30];
	DataBase server_DB;

	int clients_count;
	int errors_limit;
	int autosave_period;

	int locks_count;
	enum global_locks
	{
		AD,
		Count,
		Records_and_toFile,
		Record,
		Update
	};

	Mutex * global_locks;
};

