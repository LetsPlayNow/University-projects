#include <QtCore/QCoreApplication>
#define NOMINMAX
#include <Windows.h>
#include "Server.h"

int main(int argc, char *argv[])
{
	WSADATA something;
	WSAStartup(0x0202, &something);

	Server server;
	server.operating_cycle();

	WSACleanup();
	return 0;
}