#pragma once
#include <Windows.h>
#include <qstring.h>
#include <QDateTime>

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

static void write_QString(QString obj, HANDLE file_s)
{
	int size = obj.size();
	DWORD bytes_write;

	WriteFile(file_s, (void *)&size, sizeof(int), &bytes_write, NULL);
	WriteFile(file_s, (void *)obj.data(), sizeof(TCHAR)*(size + 1), &bytes_write, NULL);
}

static QDateTime read_QDateTime(HANDLE file_l)
{
	DWORD bytes_read;
	unsigned int date_time;
	ReadFile(file_l, (void *)&date_time, sizeof(unsigned int), &bytes_read, NULL);
	return QDateTime().fromTime_t(date_time); //Что это???
}


static void write_QDateTime(QDateTime obj, HANDLE file_s)
{
	DWORD bytes_write;
	unsigned int time = obj.toTime_t();
	WriteFile(file_s, (void *)&time, sizeof(unsigned int), &bytes_write, NULL);
}