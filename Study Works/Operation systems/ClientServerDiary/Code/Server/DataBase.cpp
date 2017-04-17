#include "DataBase.h"

DataBase::DataBase()
{
	id = 0;
	lstrcpy(filename, L"Diary.db");
}

DataBase::~DataBase()
{
}


//Возвращает количество записей в базе данных
int DataBase::count() const
{
	return dataBase.size();
}


//Добавляет запись в базу данных
//Генерирует уникальный идентификатор и записывает его в поле Id параметра record
//[in]Добавляемая запись
//[out]Индекс записи в контейнере
index_id DataBase::append(Note rec)
{
	rec.id = id;
	dataBase.insert(id, rec);
	index_id note_info = { getIndex(rec.id), rec.id };

	id++;
	return note_info;
}


//Удалить из базы данных запись с данным идентификатором
//[in] Идентификатор (id)
bool DataBase::remove(u_int id)
{
	return dataBase.remove(id) != 0;
}


//Изменяет запись в базе данных
//[in]Изменяемая запись
//[out]Новая позиция записи с учетом сортировки
int DataBase::update(Note record)
{
	int index = -1;

	if (dataBase.contains(record.id))
	{
		WaitForSingleObject(dataBase[record.id].mutex, INFINITE);
		dataBase[record.id] = record;
		index = getIndex(record.id);
		ReleaseMutex(dataBase[record.id].mutex);
	}

	return index;
}


//Возвращает запись (только для чтения) по заданному идентификатору
Note DataBase::record(u_int id)
{
	Note note;
	if (dataBase.contains(id))
	{
		WaitForSingleObject(dataBase[id].mutex, INFINITE);
		note = dataBase[id];
		ReleaseMutex(dataBase[id].mutex);
	}
		
	return note;
}

//Возвращает вектор записей, которые должны отображаться в браузере с учетом сортировки
//[out]Вектор записей для отображения в QListWidget
QVector<Note> DataBase::records()
{
	//Отсортируем QMAP
	//Записываем все обекты Note из QMap в QVector
	QVector<Note> notes;
	for (QMap <u_int, Note>::const_iterator i = dataBase.constBegin(); i != dataBase.constEnd(); i++)
		notes << i.value();

	//Сортируем содержимое QVector notes
	for(int i = 0; i < notes.size() - 1; i++)
		for(int j = 0; j < notes.size() - i - 1; j++)
			if(notes[j] > notes[j+1])
				{
					Note temp = notes[j];
					notes[j] = notes[j+1];
					notes[j+1] = temp;
				}

	return notes;
}

//Сохранить данные в заданный файл
//[out] bool = false - сохранить данные не удалось
//[out] bool = true - данные успешно сохранены
bool DataBase::write_to_file()
{
	HANDLE file = CreateFile(filename, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	bool success = file != INVALID_HANDLE_VALUE;
	if (!success)
		return false;

	DWORD writed;
	int size = count();
	success = WriteFile(file, (void *)&size, sizeof(int), &writed, NULL) && writed == sizeof(int) && success;

	QVector <Note> notes = records();
	for (int i = 0; i < size; i++)
		success = notes[i].to_file(file) && success;


	//В конце запишем id
	success = WriteFile(file, (void *)&id, sizeof(short int), &writed, NULL) && writed == sizeof(short int) && success;

	success = CloseHandle(file) && success;
	return success;
}
	


//Загрузить данные из заданного файла (!Предыдущие данные уничтожаются!)
//[out] bool = false - загрузить данные не удалось
//[out] bool = true - данные успешно загружены
bool DataBase::read_from_file()
{
	HANDLE file = CreateFile(filename, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	bool success = file != INVALID_HANDLE_VALUE;
	if (!success)
		return false;

	DWORD readed;
	int size = count();
	Note temp;
	success = ReadFile(file, (void *)&size, sizeof(int), &readed, NULL) && sizeof(int) == readed && success;

	for (int i = 0; i < size; i++)
	{
		success = temp.from_file(file) && success;
		append(temp);
	}

	success = ReadFile(file, (void *)&id, sizeof(short int), &readed, NULL) && readed == sizeof(short int) && success;

	success = CloseHandle(file) && success;
	return success;
}

//Уничтожает все данные
void DataBase::clear()
{
	dataBase.clear();
}

/*Выход:
-1:        если в базе данных нет записи с идентификатором id
int index: индекс записи в базе данных
*/
int DataBase::getIndex(u_int id)
{
	if (!dataBase.contains(id))
		return -1;

	Note temp = dataBase[id];
	QMap <u_int, Note>::const_iterator i = dataBase.constBegin();
	int index = 0;
	while (i != dataBase.constEnd())
	{
		if (*i < temp)
			index++;
		i++;
	}

	return index;
}