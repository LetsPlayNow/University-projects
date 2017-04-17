#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include <clocale>

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Ru");

	TCHAR server_control_pipename[] = L"\\\\.\\pipe\\diary_server_control";
	WaitNamedPipe(server_control_pipename, NMPWAIT_WAIT_FOREVER);
	HANDLE server_control_pipe = CreateFile(server_control_pipename, GENERIC_WRITE | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD pipe_mode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
	SetNamedPipeHandleState(server_control_pipe, &pipe_mode, NULL, NULL);

	char command;
	DWORD writed;

	puts("Утилита управления сервером \"Diary\"");
	puts("Для передачи коммант серверу, просто введите нужный ключ");
	puts("Ключи программы: q - завершение работы сервера");
	while (true)
	{
		scanf("%c", &command);
		if (command == 'q')
		{
			WriteFile(server_control_pipe, (void *)&command, sizeof(char), &writed, NULL);
			FlushFileBuffers(server_control_pipe);
			DisconnectNamedPipe(server_control_pipe);
			CloseHandle(server_control_pipe);
			return EXIT_SUCCESS;
		}
			
	}

	return EXIT_FAILURE;
}