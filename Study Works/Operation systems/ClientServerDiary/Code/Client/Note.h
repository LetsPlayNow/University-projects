//Это файл с описанием класса Note
//Реализация класса находится в "Note.cpp"
#pragma once
#undef min
#include <QDateTime>
#include <QStringList.h> //для поддержки объектов типа QStringList
#include <qstring.h> //для поддержки localAwareCompare
#include <qdatastream.h>
#include <Windows.h>

class Note
{
public:
	Note(const Note & other);
	Note(); //Определяет шаблон мероприятия, который потом можно изменить

	unsigned int id;
	QString name; //Наименование мероприятия
	QDateTime dateAndTime; //Дата и время мероприятия
	QTime timeLength; //Протяженность мероприятия
	QString place; //Место проведения мероприятия
	int importantly; //Важность мероприятия

	/*_____Периодичность_____*/
	bool hasPeriodicity; //Имеется ли периодичность?
	int periodicity; //Период повторения (Выбираем один из вариантов)
	int numberOfRepeat; //Количество повторений


	bool to_file(HANDLE file);
	bool from_file(HANDLE file);

	bool send(SOCKET socket);
	bool recv(SOCKET socket);
	
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