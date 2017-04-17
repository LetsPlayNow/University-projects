#include <QtWidgets/QApplication>
#include "diary.h"
#include <winsock.h>
int main(int argc, char *argv[])
{
	WSADATA something;
	WSAStartup(0x0202, &something);

	QApplication a(argc, argv);
	Diary w;
	w.show();
	a.exec();
	
	WSACleanup();
	return EXIT_SUCCESS;
}
