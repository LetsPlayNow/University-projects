#pragma once
#include <Windows.h>
#include <qstring.h>
#include <QDateTime>
QString read_QString(HANDLE file_l);
bool write_QString(QString obj, HANDLE file_s);
QDateTime read_QDateTime(HANDLE file_l);
bool write_QDateTime(QDateTime obj, HANDLE file_s);
QDateTime recv_QDateTime(SOCKET socket);
bool send_QDateTime(QDateTime obj, SOCKET socket);
QString recv_QString(SOCKET socket);
bool send_QString(QString obj, SOCKET socket);




static QString read_QString(HANDLE file_l)
{
	QString obj;
	int size;
	DWORD bytes_read;

	ReadFile(file_l, (void *)&size, sizeof(int), &bytes_read, NULL);
	obj.resize(size);
	ReadFile(file_l, (void *)obj.data(), sizeof(TCHAR) * (size + 1), &bytes_read, NULL);  //+1 на /0
	return obj;
}

static bool write_QString(QString obj, HANDLE file_s)
{
	int size = obj.size();
	DWORD bytes_write;
	bool success;

	success = WriteFile(file_s, (void *)&size, sizeof(int), &bytes_write, NULL) && bytes_write == sizeof(int);
	success = WriteFile(file_s, (void *)obj.data(), sizeof(TCHAR)*(size + 1), &bytes_write, NULL) && bytes_write == sizeof(TCHAR)*(size + 1) && success;

	return success;
}

static QDateTime read_QDateTime(HANDLE file_l)
{
	DWORD bytes_read;
	unsigned int date_time;
	ReadFile(file_l, (void *)&date_time, sizeof(unsigned int), &bytes_read, NULL);
	return QDateTime().fromTime_t(date_time); //Что это???
}


static bool write_QDateTime(QDateTime obj, HANDLE file_s)
{
	DWORD bytes_write;
	bool success;

	unsigned int time = obj.toTime_t();
	success = WriteFile(file_s, (void *)&time, sizeof(unsigned int), &bytes_write, NULL) && bytes_write == sizeof(unsigned int);

	return success;
}


static QDateTime recv_QDateTime(SOCKET socket)
{
	QString date_time = recv_QString(socket);
	return QDateTime::fromString(date_time);
}

static bool send_QDateTime(QDateTime obj, SOCKET socket)
{
	QString date_time = obj.toString();
	return send_QString(date_time, socket);
}


static QString recv_QString(SOCKET socket)
{
	u_long size;
	//<-u_long size
	::recv(socket, (char*)&size, sizeof(u_long), 0);
	size = ntohl(size);

	//<-QString
	QString str(size, '\0');
	::recv(socket, (char*)str.data(), sizeof(QChar)*(size + 1), 0);

	return str;
}

static bool send_QString(QString obj, SOCKET socket)
{
	u_long size = htonl((u_long)obj.size());
	//->u_long size
	::send(socket, (char*)&size, sizeof(u_long), 0);
	//->QString
	::send(socket, (char *)obj.data(), sizeof(QChar)*(obj.size() + 1), 0);

	return true;
}