#include "Server.h"
Mutex Server::global_locks[] = { Mutex(L"AD"), Semaphore(L"COUNT"), Semaphore(L"RES_TOFILE"), Semaphore(L"REC"), Mutex(L"UPDATE") };
DataBase Server::server_DB;
int Server::errors_limit = 1;
int Server::operations_count = 0;
int Server::autosave_period = 10;
int Server::clients_count = 0;
map <SOCKET, SOCKET> Server::clients;
vector <HANDLE> Server::threads;
Server *Server::to_use_signals;

Server::Server()
{
	//Программные настройки
	setlocale(LC_ALL, "RUSSIAN");
	to_use_signals = this;
	port_service = htons(50000);
	port_update  = htons(50001);
};


Server::~Server()
{
	//Закрываем все сокеты
	for (map <SOCKET, SOCKET>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		closesocket((*i).first);  //Закрываем сокет обслуживания клиента
		closesocket((*i).second); //Закрываем сокет обновления клиента
		clients.erase((*i).first);
	}

	//Завершаем все потоки
	for (vector <HANDLE>::iterator j = threads.begin(); j != threads.end(); j++)
		TerminateThread(*j, EXIT_SUCCESS);


	to_file();
}

DWORD WINAPI Server::service_client(LPVOID p)
{
	SOCKET socket = (SOCKET)p;
	mediator connector(socket, server_DB);
	requests message;
	int errors_count = 0;
	/*
	 Дождаться снятия проверяемых блокировок
	 Установить устанавливаемые блокировки
	 Выполнить операцию
	 Отправить результат клиенту
	 Снять устанавливаемые блокировки
	*/

	do
	{
		if (::recv(socket, (char *)&message, sizeof(requests), 0) == SOCKET_ERROR); //{shutdown((HANDLE)socket); puts("Ошибка приема соообщения"); return EXIT_FAILURE; } 

		switch (message)
		{
		case COUNT:	  count(connector, errors_count);    break;
		case APPEND:  append(connector, errors_count);   break;
		case REMOVE:  remove(connector, errors_count);   break;
		case UPDATE:  update(connector, errors_count);   break;
		case RECORD:  record(connector, errors_count);   break;
		case RECORDS: records(connector, errors_count);  break;
		case EXIT: break;
		default:      
			puts("Неверный код операции");	
			shutdown((HANDLE)socket);
			return EXIT_FAILURE;
			break;
		}

		if (errors_count > errors_limit)
		{
			//Произошло слишком много ошибок в этом потоке, необходимо закрыть его
			puts("[Ошибка] отправка результатов/обработка запроса");
			shutdown((HANDLE)socket);
			return EXIT_FAILURE;
		}

		if (message == APPEND || message == REMOVE || message == UPDATE)
			to_use_signals->update_clients(socket);
		++operations_count;
	} while (message != EXIT);

	shutdown((HANDLE)socket);
	return true;
}

DWORD WINAPI Server::server_control(LPVOID p)
{
	Server * server = (Server *)p;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	CreateProcess(L"D:\\Projects\\VS_2013\\OS_3\\Server_control\\Server_control\\Debug\\Server_control.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);

	HANDLE server_c = CreateNamedPipe(L"\\\\.\\pipe\\diary_server_control",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,
		1,
		1,
		1000,
		NULL);

	ConnectNamedPipe(server_c, NULL);
	puts("Программа управления сервером запущена");

	char command;
	DWORD readed;
	while (true)
	{
		ReadFile(server_c, (void *)&command, sizeof(char), &readed, NULL);
		if (command == 'q')
		{
			puts("Поступил сигнал выключения сервера. Завершение работы");
			system("pause");

			server->~Server();

			DisconnectNamedPipe(server_c);
			CloseHandle(server_c);
			exit(EXIT_SUCCESS);
		}
	}
	
	return EXIT_SUCCESS;
}

DWORD WINAPI Server::autosave(LPVOID p)
{
	while (true)
	{
		Sleep(1000);
		if (operations_count > autosave_period)
		{
			to_file();
			operations_count = 0;
		}
	}
}
void Server::operating_cycle()
{
	//Инициализация
	puts("Запуск сервера...");
	if (server_DB.read_from_file() == false) puts("[Ошибка] server_DB.read_from_file()");

	//Запуск основных потоков
	CreateThread(NULL, 0, server_control, this, NULL, NULL);
	CreateThread(NULL, 0, autosave, NULL, NULL, NULL);


	//Создаем TCP сокеты
	SOCKET server_service = socket(AF_INET, SOCK_STREAM, 0);
	SOCKET server_update  = socket(AF_INET, SOCK_STREAM, 0);
	
	//Настройка и привязыка TCP сокетов
	sockaddr_in server_service_address;
	server_service_address.sin_family = AF_INET;
	server_service_address.sin_port = port_service;
	server_service_address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(server_service, (struct sockaddr *)(&server_service_address), sizeof(server_service_address)) == SOCKET_ERROR) puts("[Ошибка] bind(server_service)");


	sockaddr_in server_update_address;
	server_update_address.sin_family = AF_INET;
	server_update_address.sin_port = port_update;
	server_update_address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(server_update, (struct sockaddr *)(&server_update_address), sizeof(server_update_address)) == SOCKET_ERROR)   puts("[Ошибка] bind(server_update)");
	
	//Перевод сокетов в пассивный режим
	if (listen(server_service, 1) == SOCKET_ERROR) puts("[Ошибка] listen (server_service)");
	if (listen(server_update, 1)  == SOCKET_ERROR)  puts("[Ошибка] listen (server_update)");

	//Цикл подключения клиентов
	sockaddr_in client_service_adress, client_update_adress;
	int sock_addr_size = sizeof(sockaddr_in);

	SOCKET client_service, client_update;
	while (true)
	{
		client_service = accept(server_service, (sockaddr *)&client_service_adress, &sock_addr_size);
		client_update  = accept(server_update,  (sockaddr *)&client_update_adress,  &sock_addr_size);
		HANDLE t_handle = CreateThread(NULL, 0, service_client, (LPVOID)client_service, NULL, NULL);
		
		//Добавляем сведения о сокетах в контейнер
		clients.insert(clients.begin(), pair <SOCKET, SOCKET>(client_service, client_update));
		threads.push_back(t_handle);
		printf("Подключено клиентов : %d\n", ++clients_count);
	}
}

void Server::update_clients(SOCKET client_updater)
{
	bool updating = true;
	for (map <SOCKET, SOCKET>::const_iterator i = clients.cbegin(); i != clients.cend(); i++)
		if ((*i).first != client_updater)
			send((*i).second, (char *)&updating, sizeof(bool), 0);
}

DWORD WINAPI Server::shutdown(HANDLE pipe)
{
	SOCKET client_service_socket = (SOCKET)pipe;
	if (client_service_socket != INVALID_SOCKET)
	{
		closesocket(clients[client_service_socket]); //Закрываем сокет обновления клиента
		closesocket(client_service_socket);		   //Закрываем сокет обслуживания клиента
		clients.erase(client_service_socket);
	}

	//Вывод сервисных сообщений и сохранение данных в файл
	puts("Клиент отключился");
	to_file();
	clients_count--;

	return EXIT_SUCCESS;
}


void Server::count(mediator & connector, int & errors_count)
{
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
}

void Server::append(mediator & connector, int & errors_count)
{
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
}

void Server::remove(mediator & connector, int & errors_count)
{
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
}

void Server::update(mediator & connector, int & errors_count)
{
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
}

void Server::record(mediator & connector, int & errors_count)
{
	//=============================================
	global_locks[AD].lock();
	//=============================================
	global_locks[Record].enter();
	//=============================================
	global_locks[AD].unlock();
	//=============================================
	puts("[RECORD] started  ");
	if (!connector.record()) errors_count++;
	puts("[RECORD] finished");
	//=============================================
	global_locks[Record].leave();
}

void Server::records(mediator & connector, int & errors_count)
{
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
}

void Server::to_file()
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
	server_DB.write_to_file();
	puts("[TO_FILE] finished");
	//=============================================
	global_locks[Records_and_toFile].leave();
}