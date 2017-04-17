#pragma once
#define NOMINMAX
#include <Windows.h>
#include <QDateTime>
#include <qstring.h>
#include <qdatastream.h>
#include <qstringlist.h>


class Note
{
public:
	Note();
	Note(const Note & other);
	~Note();

	unsigned int id;
	QString name; //Наименование мероприятия
	QDateTime dateAndTime; //Дата и время мероприятия
	QTime timeLength; //Протяженность мероприятия
	QString place; //Место проведения мероприятия
	int importantly; //Важность мероприятия

	bool hasPeriodicity; //Имеется ли периодичность?
	int periodicity; //Период повторения (Выбираем один из вариантов)
	int numberOfRepeat; //Количество повторений

	bool to_file(HANDLE file);
	bool from_file(HANDLE file);

	bool send(SOCKET socket);
	bool recv(SOCKET socket);

	HANDLE mutex;


	//Операторы
	friend QDataStream & operator<<(QDataStream & out, const Note & data);
	friend QDataStream & operator>>(QDataStream & in, Note & data);


	void operator=(const Note &);
	bool operator>(const Note &) const;
	bool operator>=(const Note &) const;
	bool operator<(const Note &) const;
	bool operator<=(const Note &) const;
	bool operator!=(const Note &) const;
	bool operator==(const Note &) const;
};