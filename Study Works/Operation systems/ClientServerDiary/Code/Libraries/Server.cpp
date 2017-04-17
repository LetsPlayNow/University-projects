#include "Server.h"


Server::Server()
{

	lstrcpy(channel_name, L"\\\\.\\pipe\\diary_channel");

	//Настройка блокировок
	locks_count = 5;
	global_locks = new Mutex[locks_count];
	global_locks[0] = Mutex(L"AD");
	global_locks[1] = Semaphore(L"COUNT");
	global_locks[2] = Semaphore(L"RES_TOFILE");
	global_locks[3] = Semaphore(L"REC");
	global_locks[4] = Mutex(L"UPDATE");

	clients_count = 0;
	errors_limit = 1;
	autosave_period = 100;
};


Server::~Server()
{
}

DWORD WINAPI Server::service_client(LPVOID p)
{
	HANDLE pipe = (HANDLE)p;
	mediator connector(pipe, server_DB);
	requests message;

	BOOL success;
	int errors_count = 0;
	int operations_count = 0;

	/*
	 Дождаться снятия проверяемых блокировок
	 Установить устанавливаемые блокировки
	 Выполнить операцию
	 Отправить результат клиенту
	 Снять устанавливаемые блокировки
	*/

	do
	{
		DWORD readed;
		success = ReadFile(pipe, (void *)&message, sizeof(message), &readed, NULL) && sizeof(message) == readed && success;

		switch (message)
		{
		case COUNT:
			//=============================================
			global_locks[AD].lock();
			//=============================================
			global_locks[Count].enter();
			//=============================================
			global_locks[AD].unlock();
			//=============================================
			puts("[COUNT] started");
			if (!connector.count()) errors_count++;
			puts("[COUNT] finished");
			//=============================================
			global_locks[Count].leave();
			break;
			//=============================================

		case APPEND:
			//=============================================
			for (int i = Count; i < locks_count; i++)
				global_locks[i].lock();
			//=============================================
			global_locks[AD].lock();
			//=============================================
			for (int i = Update; i >= Count; i--)
				global_locks[i].unlock();
			//=============================================
			puts("[APPEND] started  ");
			if (!connector.append()) errors_count++;
			puts("[APPEND] finished");
			//=============================================
			global_locks[AD].unlock();
			break;
			//=============================================

		case REMOVE:
			//=============================================
			for (int i = Count; i < locks_count; i++)
				global_locks[i].lock();
			//=============================================
			global_locks[AD].lock();
			//=============================================
			for (int i = Update; i >= Count; i--)
				global_locks[i].unlock();
			//=============================================
			puts("[REMOVE] started  ");
			if (!connector.remove()) errors_count++;
			puts("[REMOVE] finished");
			//=============================================
			global_locks[AD].unlock();
			break;
			//=============================================

		case UPDATE:
			//=============================================
			global_locks[AD].lock();
			global_locks[Records_and_toFile].lock();
			//=============================================
			global_locks[Update].lock();
			//=============================================
			global_locks[Records_and_toFile].unlock();
			global_locks[AD].unlock();
			//=============================================
			puts("[UPDATE] started  ");
			if (!connector.update()) errors_count++;
			puts("[UPDATE] finished");
			//=============================================
			global_locks[Update].unlock();
			break;
			//=============================================

		case RECORD:
			//=============================================
			global_locks[AD].lock();
			//=============================================
			global_locks[Record].enter();
			//=============================================
			global_locks[AD].unlock();
			//=============================================
			puts("[RECORD] started  ");
			//Sleep(5000);
			if (!connector.record()) errors_count++;
			puts("[RECORD] finished");
			//=============================================
			global_locks[Record].leave();
			break;
			//=============================================

		case RECORDS:
			//=============================================
			global_locks[AD].lock();
			global_locks[Update].lock();
			//=============================================
			global_locks[Records_and_toFile].enter();
			//=============================================
			global_locks[Update].unlock();
			global_locks[AD].unlock();
			//=============================================
			puts("[RECORDS] started ");
			if (!connector.records()) errors_count++;
			puts("[RECORDS] finished");
			//=============================================
			global_locks[Records_and_toFile].leave();
			break;
			//=============================================
		default:
			break;
		}

		if (errors_count > errors_limit)
		{
			//Произошло слишком много ошибок
			puts("[Ошибка] отправка результатов/обработка запроса");
			shutdown(pipe);
			return EXIT_FAILURE;
		}


		//Автосохранение
		if (++operations_count >= autosave_period)
		{
			//=============================================
			//Сохранение в базу данных
			global_locks[AD].lock();
			global_locks[Update].lock();
			//=============================================
			global_locks[Records_and_toFile].enter();
			//=============================================
			global_locks[Update].unlock();
			global_locks[AD].unlock();
			//=============================================
			puts("[TO_FILE] started ");
			if (!server_DB.write_to_file()) errors_count++;
			puts("[TO_FILE] finished");
			//=============================================
			global_locks[Records_and_toFile].leave();
			//=============================================
			operations_count = 0;
			//=============================================
		}
		//printf("Число записей в базе данных %d\n", connector.DB.count());
	} while (message != EXIT);

	shutdown(pipe);
	return success;
}

DWORD WINAPI Server::server_control(LPVOID p)
{

}

void Server::operating_cycle()
{

}

void Server::connect_new_client()
{
	//Создаем канал
	HANDLE pipe = CreateNamedPipe(channel_name,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		2048,
		2048, //Следует расширить, иначе необходимо будет делать цикл, но не до этого сейчас
		3000,
		NULL);

	(pipe == INVALID_HANDLE_VALUE) ? puts("CreateNamedPipe : ERROR") : puts("CreateNamedPipe : SUCCESS");

	//Ожидаем подключения клиента
	(ConnectNamedPipe(pipe, NULL) == FALSE) ? puts("ConnectNamedPipe : ERROR") : puts("ConnectNamedPipe : SUCCESS");

	clients_count++;

	//Передаем канал потоковой функции
	HANDLE t_handle = CreateThread(NULL, 0, service_client, (LPVOID)pipe, NULL, NULL);
	if (t_handle == INVALID_HANDLE_VALUE)
		puts("CreateThread : ERROR");
	else
	{
		puts("CreateThread : SUCCESS");
	}

	printf("Подключено клиентов:%d\n", clients_count);
}

void shutdown_client(HANDLE pipe)
{

}
