#include "Note.h"
#include <qstring.h>
#include "QString_i_o.h"

//Шаблончик для класса Note
Note::Note()
{
	id = -1;
	name = QString("Name of event");
	dateAndTime = QDateTime(QDate(1800, 12, 10), QTime(11, 45));
	timeLength = QTime(11, 11, 00);
	place = QString("Place of event");
	importantly = 1;
	hasPeriodicity = true;
	periodicity = 1;
	numberOfRepeat = 3;
}

Note::Note(const Note & other)
{
	id = other.id;
	name = other.name;
	dateAndTime = other.dateAndTime;
	timeLength = other.timeLength;
	place = other.place;
	importantly = other.importantly;
	hasPeriodicity = other.hasPeriodicity;
	periodicity = other.periodicity;
	numberOfRepeat = other.numberOfRepeat;
}



bool Note::to_file(HANDLE file)
{
	DWORD writed;
	bool success;
	success = WriteFile(file, (void *)&id, sizeof(unsigned int), &writed, NULL) && writed == sizeof(unsigned int);
	success = write_QString(name, file) && success;
	success = write_QDateTime(dateAndTime, file) && success;
	success = WriteFile(file, (void *)&timeLength, sizeof(QTime), &writed, NULL) && writed == sizeof(QTime) && success;
	success = write_QString(place, file) && success;
	success = WriteFile(file, (void *)&importantly, sizeof(int), &writed, NULL) && writed == sizeof(int) && success;

	success = WriteFile(file, (void *)&hasPeriodicity, sizeof(bool), &writed, NULL) && writed == sizeof(bool) && success;
	success = WriteFile(file, (void *)&periodicity, sizeof(int), &writed, NULL) && writed == sizeof(int) && success;
	success = WriteFile(file, (void *)&numberOfRepeat, sizeof(int), &writed, NULL) && writed == sizeof(int) && success;

	return success;
}

bool Note::from_file(HANDLE file)
{
	DWORD readed;
	bool success;
	success = ReadFile(file, (void *)&id, sizeof(unsigned int), &readed, NULL) && readed == sizeof(unsigned int);
	name = read_QString(file);
	dateAndTime = read_QDateTime(file);
	success = ReadFile(file, (void *)&timeLength, sizeof(QTime), &readed, NULL) && readed == sizeof(QTime) && success;
	place = read_QString(file);
	success = ReadFile(file, (void *)&importantly, sizeof(int), &readed, NULL) && readed == sizeof(int) && success;

	success = ReadFile(file, (void *)&hasPeriodicity, sizeof(bool), &readed, NULL) && readed == sizeof(bool) && success;
	success = ReadFile(file, (void *)&periodicity, sizeof(int), &readed, NULL) && readed == sizeof(int) && success;
	success = ReadFile(file, (void *)&numberOfRepeat, sizeof(int), &readed, NULL) && readed == sizeof(int) && success;

	return success;
}

bool Note::send(SOCKET socket)
{
	u_long id_out = htonl((u_long)id);
	if (::send(socket, (char *)&id_out, sizeof(u_long), 0) == SOCKET_ERROR) return false;

	if (send_QString(name, socket) == false) return false;
	if (send_QDateTime(dateAndTime, socket) == false) return false;
	if (::send(socket, (char *)&timeLength, sizeof(QTime), 0) == SOCKET_ERROR) return false;
	if (send_QString(place, socket) == false) return false;

	u_long imp = htonl((u_long)importantly), per = htonl((u_long)periodicity), nor = htonl((u_long)numberOfRepeat);
	if (::send(socket, (char *)&imp, sizeof(u_long), 0) == SOCKET_ERROR) return false;
	if (::send(socket, (char *)&hasPeriodicity, sizeof(bool), 0) == SOCKET_ERROR) return false;
	if (::send(socket, (char *)&per, sizeof(u_long), 0) == SOCKET_ERROR) return false;
	if (::send(socket, (char *)&nor, sizeof(u_long), 0) == SOCKET_ERROR) return false;
	return true;
}

bool Note::recv(SOCKET socket)
{
	u_long id_in;
	if (::recv(socket, (char *)&id_in, sizeof(u_long), 0) == SOCKET_ERROR) return false;
	id = ntohl(id_in);

	name = recv_QString(socket);
	dateAndTime = recv_QDateTime(socket);
	if (::recv(socket, (char *)&timeLength, sizeof(QTime), 0) == SOCKET_ERROR) return false;
	place = recv_QString(socket);

	u_long imp, per, nor;
	if (::recv(socket, (char *)&imp, sizeof(u_long), 0) == SOCKET_ERROR) return false;
	importantly = ntohl(imp);
	if (::recv(socket, (char *)&hasPeriodicity, sizeof(bool), 0) == SOCKET_ERROR) return false;
	if (::recv(socket, (char *)&per, sizeof(u_long), 0) == SOCKET_ERROR) return false;
	periodicity = ntohl(per);
	if (::recv(socket, (char *)&nor, sizeof(u_long), 0) == SOCKET_ERROR) return false;
	numberOfRepeat = ntohl(nor);
	return true;
}

void Note::operator=(const Note &other)
{
	id = other.id;
	name = other.name;
	dateAndTime = other.dateAndTime;
	timeLength = other.timeLength;
	place = other.place;
	importantly = other.importantly;
	hasPeriodicity = other.hasPeriodicity;
	periodicity = other.periodicity;
	numberOfRepeat = other.numberOfRepeat;
}

bool Note::operator>(const Note & other) const
{
	if (name.localeAwareCompare(other.name) == 0)
	{
		if (dateAndTime == other.dateAndTime)
		{
			if (importantly == other.importantly)
			{
				return false;
			}

			else if (importantly > other.importantly)
				return true;
			else
				return false;
		}

		else if (dateAndTime > other.dateAndTime)
			return true;
		else
			return false;

	}

	else if (name.localeAwareCompare(other.name) > 0)
		return true;
	else
		return false;
}

bool Note::operator>=(const Note & other) const
{
	if (name.localeAwareCompare(other.name) == 0)
	{
		if (dateAndTime == other.dateAndTime)
		{
			if (importantly == other.importantly)
			{
				return true;
			}
			else if (importantly > other.importantly)
				return true;
			else
				return false;
		}

		else if (dateAndTime > other.dateAndTime)
			return true;
		else
			return false;

	}

	else if (name.localeAwareCompare(other.name) > 0)
		return true;
	else
		return false;
}


bool Note::operator<(const Note & other) const
{
	if (name.localeAwareCompare(other.name) == 0)
	{
		if (dateAndTime == other.dateAndTime)
		{
			if (importantly == other.importantly)
			{
				return false;
			}
			else if (importantly < other.importantly)
				return true;
			else
				return false;
		}

		else if (dateAndTime < other.dateAndTime)
			return true;
		else
			return false;

	}

	else if (name.localeAwareCompare(other.name) < 0)
		return true;
	else
		return false;
}


bool Note::operator<=(const Note & other) const
{
	if (name.localeAwareCompare(other.name) == 0)
	{
		if (dateAndTime == other.dateAndTime)
		{
			if (importantly == other.importantly)
			{
				return true;
			}
			else if (importantly < other.importantly)
				return true;
			else
				return false;
		}

		else if (dateAndTime < other.dateAndTime)
			return true;
		else
			return false;

	}

	else if (name.localeAwareCompare(other.name) < 0)
		return true;
	else
		return false;
}

bool Note::operator!=(const Note & other) const
{
	bool notEqually;
	notEqually = name != other.name || dateAndTime != other.dateAndTime
		|| importantly != other.importantly;
	return notEqually;
}

bool Note::operator==(const Note & other) const
{
	bool equally;
	equally = name == other.name;
	equally = equally && dateAndTime == other.dateAndTime;
	equally = equally && importantly == other.importantly;
	return equally;
}

//Операторы
QDataStream & operator<<(QDataStream & out, const Note & data)
{
	out << data.id
		<< data.name
		<< data.dateAndTime
		<< data.place
		<< data.importantly
		<< data.hasPeriodicity
		<< data.periodicity
		<< data.numberOfRepeat;
	return out;
}


QDataStream & operator>>(QDataStream & in, Note & data)
{
	in >> data.id
		>> data.name
		>> data.dateAndTime
		>> data.place
		>> data.importantly
		>> data.hasPeriodicity
		>> data.periodicity
		>> data.numberOfRepeat;
	return in;
}