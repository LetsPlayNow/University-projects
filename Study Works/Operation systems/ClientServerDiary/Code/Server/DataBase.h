#pragma once
#include "Note.h"
#include <qmap.h>
#include <qstringlist.h>
#include <qdatetime.h>
#include "request.h"
#include <qvector.h>
#include "index_id.h"

typedef unsigned int u_int;

class DataBase
{
public:
	DataBase(void);
	~DataBase(void);

	int count() const;

	index_id append(Note record);
	bool remove(u_int id);
	int update(Note record);

	Note record(u_int id);
	QVector<Note> records();

	bool write_to_file();
	bool read_from_file();

	void clear();

private:
	int getIndex(u_int id);   //¬озвращает индекс записи с учетом сортировки

	QMap < u_int, Note > dataBase;
	u_int id;

	wchar_t filename[100];
};

