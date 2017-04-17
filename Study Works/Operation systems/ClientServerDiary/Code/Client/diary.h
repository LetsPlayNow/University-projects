#pragma once
//Это файл с описанием класса главного окна "Diary.h"
//Реализация класса находится в "Diary.cpp"
#include <QtWidgets/QMainWindow>
#include "ui_diary.h"
#include "Note.h"
#include "DataBase.h"


class Diary : public QMainWindow
{
	Q_OBJECT

public:
	Diary(QWidget *parent = 0);
	~Diary();
	
	void connecting();

public slots: 
	void addNote();
	void updateNote();
	void deleteCurrentNote();

	void fillData();
	void tcp_connect();
	void checkedButton(bool newValue);

	void showNoteWhichClikedInBrowser();

	void update_browser_list();

	void error_service(int error_code);

public:
	QString makeBrowserString(const Note &);
	void from_file();

private:
	void saveInTemp();
	bool canBeSaved(const Note & note); //Определяет, можно ли сохранить данную запись
	void showNote(unsigned int id);
	void clearTemp(); //Очищаем временную запись от сделанных в ней изменений
	void clearWidgets(); //Очищает виджеты после удаления последней записи из браузера
	
	void prepare_view_forms();
	//База данных
	DataBase DB;

	//Синхронизация заполнения браузера
	HANDLE fill_browser_mutex;

	//Шаблон записи
	Note temp;
	Ui::Diary ui;
};
