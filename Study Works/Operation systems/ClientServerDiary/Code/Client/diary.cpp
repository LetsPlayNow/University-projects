//Реализация класса Diary
#include "diary.h"
#include "Validators.h"
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qmessagebox.h>
#include <stdlib.h>
#include <QTimer>
#include <qlistwidget.h>


Diary::Diary(QWidget *parent)
	:QMainWindow(parent)
{
	ui.setupUi(this);
	prepare_view_forms();
	connecting();
	update_browser_list();

	//Валидатор
	QValidator *ev = new ExampleValidator(this);
	connect(ui.txtPlace, SIGNAL(textEdited()), this, SLOT(hasAcceptableInput()));
	ui.txtPlace->setValidator(ev);
	fill_browser_mutex = CreateMutex(NULL, FALSE, L"fill_browser_mutex");
}


Diary::~Diary()
{}

void Diary::prepare_view_forms()
{
	QStringList ImportantlyLst, PeriodicityLst;
	ImportantlyLst << "Unimportant" << "Important" << "Very important";
	PeriodicityLst << "Every day" << "After day" << "After 2 days" << "After 3 days" << "Every week" << "Monthly (28 days)" << "Every quarter" << "Every six months" << "Yearly";
	ui.lstImportant->insertItems(0, ImportantlyLst);
	ui.lstPeriodicity->insertItems(0, PeriodicityLst);
}


//События
void Diary::connecting()
{
	//Кнопки взаимодейсвия с записями
	connect(ui.btnCreate,   SIGNAL(clicked()), this, SLOT(addNote()));
	connect(ui.btnSave,     SIGNAL(clicked()), this, SLOT(updateNote()));
	connect(ui.btnDelete,   SIGNAL(clicked()), this, SLOT(deleteCurrentNote()));
	connect(ui.btnFillList, SIGNAL(clicked()), this, SLOT(fillData()));

	//Взаимодействие с виджетами редактирования
	connect(ui.cbHasPeriodicity, SIGNAL(clicked(bool)), this, SLOT(checkedButton(bool)));

	//Взаимодействие с браузером
	connect(ui.lstList, SIGNAL(itemSelectionChanged()), this, SLOT(showNoteWhichClikedInBrowser())); //Обработка взаимодействий со списком

	connect(&DB, SIGNAL(error_exist(int)), SLOT(error_service(int)));
	connect(&DB, SIGNAL(need_update()), this, SLOT(update_browser_list()));
	connect(ui.btn_connect, SIGNAL(clicked()), this, SLOT(tcp_connect()));
}

void Diary::tcp_connect()
{
	QString ip_address = ui.leIP->text();
	//Здесь должен быть валидатор

	int size = ip_address.size();
	char * ip_address_char;
	if (size > 0)
	{
		//Приводим QString к типу char *
		ip_address_char = new char[size + 1];
		for (int i = 0; i < size + 1; i++)
			ip_address_char[i] = ip_address[i].cell();
		if (!DB.reconnect(ip_address_char)) { QMessageBox::information(this, "[Error]Connect", "Can't connect to server"); ui.lstList->clear(); }
		else update_browser_list();
		delete[] ip_address_char;
	}

	else
		QMessageBox::information(this, "[Error]Connect", "Invalid server adress");
}


void Diary::error_service(int error_code)
{
	TerminateThread(DB.t_update_client, EXIT_FAILURE);
	close();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Добавить запись
void Diary::addNote()
{
	saveInTemp();
	//Ошибки клиента
	if (!canBeSaved(temp)){ QMessageBox::information(this, "[Error]Add", "Some fields are empty"); return;}

	index_id index_and_id = DB.append(temp);
	//Ошибки сервера
	if (index_and_id.id < 0 || index_and_id.index < 0 || index_and_id.index > ui.lstList->count())	{QMessageBox::information(this, "[Error]Add", "Invalid server answer"); return;}
	
	QString noteInBrowser = makeBrowserString(temp);
	clearTemp();
	
	ui.lstList->insertItem(index_and_id.index, noteInBrowser);
	ui.lstList->item(index_and_id.index)->setData(Qt::UserRole, index_and_id.id);

	ui.lstList->setCurrentRow(index_and_id.index);
}

//Сохранить изменения в выбранной в данный момент записи
void Diary::updateNote()
{
	saveInTemp();
	bool incorrect = ui.lstList->currentRow() == -1 || !canBeSaved(temp);
	if (incorrect){ QMessageBox::information(this, "[Error] Update", "Some fields are empty"); return; }

	unsigned int id = ui.lstList->currentItem()->data(Qt::UserRole).toUInt();
	temp.id = id;

	//Отлов ошибок
	int index = DB.update(temp);
	bool error = (index < 0 || index > ui.lstList->count() - 1);
	if (error) { QMessageBox::information(this, "[Error]Update", "Note was deleted"); return; }

	QString noteInBrowser = makeBrowserString(temp);

	clearTemp();

	//Обновляем запись в браузере
	ui.lstList->takeItem(ui.lstList->currentRow());
	ui.lstList->insertItem(index, noteInBrowser);
	ui.lstList->item(index)->setData(Qt::UserRole, id);

	ui.lstList->setCurrentRow(index);
}

//Удалить выбранную в данный момент запись
void Diary::deleteCurrentNote()
{
	bool incorrect = ui.lstList->currentRow() == -1;
	if (incorrect) { QMessageBox::information(this, "[Error]Delete", "Removal note is not selected"); return; }

	//Удалить запись
	QListWidgetItem * current_item = ui.lstList->takeItem(ui.lstList->currentRow()); //Из браузера
	DB.remove(current_item->data(Qt::UserRole).toUInt());                            //Из базы данных
}


void Diary::clearTemp()
{
	temp = Note();
}

//Сохранить текущее содержимое полей виджетов во временную запись
void Diary::saveInTemp()
{
	//Можно привертеть сохранение id из строку браузера сюда
	temp.name = ui.txtName->text();
	temp.dateAndTime = ui.dteDateAndTime->dateTime();
	temp.timeLength = ui.teTimeLength->time();
	temp.place = ui.txtPlace->text();
	temp.importantly = ui.lstImportant->currentRow(); 
	temp.hasPeriodicity = (ui.cbHasPeriodicity->checkState() == Qt::Checked) ? 1 : 0;
	temp.periodicity = ui.lstPeriodicity->currentRow(); 
	temp.numberOfRepeat = ui.spbNumberOfRepeat->value();
}


void Diary::showNoteWhichClikedInBrowser()
{
	if (ui.lstList->currentRow() == -1)
	{
		clearWidgets();
		return;
	}
		
	showNote(ui.lstList->currentItem()->data(Qt::UserRole).toUInt());
}

//Вывести на экран запись с идентификатором id (Ready)
void Diary::showNote(unsigned int id)
{
	//Достаем из базы данных запись с индексом number и редактируем ее
	const Note s = DB.record(id);
	if (s.id == -1)
		return;
	//Устанавливам все поля в правильное значение
	if (!ui.txtName->hasFocus())          ui.txtName->setText(s.name);
	if (!ui.dteDateAndTime->hasFocus())   ui.dteDateAndTime->setDateTime(s.dateAndTime);
	if (!ui.teTimeLength->hasFocus())     ui.teTimeLength->setTime(s.timeLength);
	if (!ui.txtPlace->hasFocus())	      ui.txtPlace->setText(s.place);
	if (!ui.lstImportant->hasFocus())     ui.lstImportant->setCurrentRow(s.importantly);
	if (!ui.cbHasPeriodicity->hasFocus()) ui.cbHasPeriodicity->setCheckState(s.hasPeriodicity ? Qt::Checked : Qt::Unchecked);

	//Выставление значений в составном блоке "Периодичность"
	if (!ui.spbNumberOfRepeat->hasFocus()) ui.spbNumberOfRepeat->setValue(s.numberOfRepeat);
	if (!ui.lstPeriodicity->hasFocus())    ui.lstPeriodicity->setCurrentRow(s.periodicity);

	//Видимость блока "Периодичность"
	bool checkState = (s.hasPeriodicity ? Qt::Checked : Qt::Unchecked);
	if (!ui.lstPeriodicity->hasFocus())    ui.lstPeriodicity->setEnabled(checkState);
	if (!ui.spbNumberOfRepeat->hasFocus()) ui.spbNumberOfRepeat->setEnabled(checkState);

	//Выводим номер просматриваемой в данный момент записи
	ui.lcdNoteId->display(ui.lstList->currentRow() + 1);
}

//Изменяет активность составного поля "Периодичность" (Tested)
void Diary::checkedButton(bool newValue)
{
	ui.lstPeriodicity->setEnabled(newValue);
	ui.spbNumberOfRepeat->setEnabled(newValue);
}

//Заполняет запись с индексом i совокупностью значений, соответствующей индексу i
void Diary::fillData()
{
	WaitForSingleObject(fill_browser_mutex, INFINITE);
	QStringList names;
	names << "Base of Volgograd" << "Invasion of mice" << "Watering plants" << "Labour Day" << "Tornenado" << "Drawing a circle" << "Erasing a circle" << "Alpha - team" << "Alpha - mission" << "Laboratory work";
	QDateTime datesAndTimes[10] = {QDateTime(QDate(2000,9,11), QTime(9,45)),
								  QDateTime(QDate(2100,8,11), QTime(4,21)),
								  QDateTime(QDate(3300,9,13), QTime(3,42)),
								  QDateTime(QDate(2009,5,21), QTime(10,43)),
								  QDateTime(QDate(1791,11,7), QTime(11,47)),
								  QDateTime(QDate(3881,10,9), QTime(12,35)),
								  QDateTime(QDate(3802,12,9), QTime(11,45)),
								  QDateTime(QDate(3099,12,10), QTime(11,53)),
								  QDateTime(QDate(3456,1,10), QTime(10,12)),
								  QDateTime(QDate(3089,12,10), QTime(7,13))};

	QTime times[10] = {QTime(9,15),
					   QTime(4,21),
					   QTime(3,42),
					   QTime(10,43),
					   QTime(11,47),
					   QTime(11,47),
					   QTime(11,45),
					   QTime(10,12),
					   QTime(11,13),
					   QTime(11,13)};

	QStringList places;
	places << "Somewhere" << "There" << "Mars" << "de_dust2" << "Great Ustyug" << "North" << "Arland" << "Meston" << "Lablandia" << "Austria";

	bool hasPeriodicity[10] = {1,1,0,0,0,0,0,1,1,1};
	int importantly[10] = {1,2,2,2,2,1,1,2,2,1};
	int periodicity[10] = {3,4,2,1,4,3,2,1,5,6};
	int numberOfRepeat[10] = {363,212,33,8,55,46,7,48,92,9};

	for (int i = 0; i < 10; i++)
	{
		temp.name = names[i];
		temp.dateAndTime = datesAndTimes[i];
		temp.timeLength = times[i];
		temp.place = places[i];
		temp.importantly = importantly[i];
		temp.hasPeriodicity = hasPeriodicity[i];
		temp.periodicity = periodicity[i];
		temp.numberOfRepeat = numberOfRepeat[i];

		//Добавляем запись в базу данных
		index_id ind_id = DB.append(temp);
		if (ind_id.id < 0 || ind_id.index < 0 || ind_id.index > ui.lstList->count()) { QMessageBox::information(this, "[Error]Fill_browser", "Invalid server answer"); return; }
		QString noteInBrowser = makeBrowserString(temp);
	
		//Вставляем строку и прописываем id
		ui.lstList->insertItem(ind_id.index, noteInBrowser);
		ui.lstList->item(ind_id.index)->setData(Qt::UserRole, ind_id.id);
	}

	//--==-=-=-=--==-=-=-=
	if (ui.lstList->count() > 0) ui.lstList->setCurrentRow(0);
	//Очищаем шаблон
	clearTemp();
	ReleaseMutex(fill_browser_mutex);
}

//Может ли быть сохранена запись с данными значениями полей
bool Diary::canBeSaved(const Note & note)
{
	bool canBeSaved = !note.name.isEmpty()
		              //&& !note.dateAndTime.isNull()
					  //&& !note.timeLength.isNull()
					  && !note.place.isEmpty()
					  && note.importantly >= 0
					  && (note.hasPeriodicity == true && note.periodicity >= 0 || note.hasPeriodicity == false);
	return canBeSaved;
}

//Делает для записи браузерную строку 
QString Diary::makeBrowserString(const Note & record)
{
	QStringList importantlyList;
	importantlyList << "Unimportant" << "Important" << "Very important";
	QString noteToBrowser = record.name + "|" + record.dateAndTime.toString(Qt::ISODate) + "|" + importantlyList[record.importantly];
	return noteToBrowser;
}

//Очищает виджеты редктирования
void Diary::clearWidgets()
{
	ui.txtName -> clear();
	ui.dteDateAndTime -> setDateTime(QDateTime(QDate(), QTime()));
	ui.teTimeLength -> setTime(QTime());
	ui.txtPlace -> clear();
	ui.lstImportant ->setCurrentRow(-1);
	ui.cbHasPeriodicity->setCheckState(Qt::Unchecked);

	//Выставление значений в составном блоке "Периодичность"
	checkedButton(false);
	ui.spbNumberOfRepeat->setValue(0);
	ui.lstPeriodicity->setCurrentRow(-1);
	ui.lcdNoteId->display("#");
}


//Кривой код здесь, разберись с этим
void Diary::update_browser_list()
{
	WaitForSingleObject(fill_browser_mutex, INFINITE);
	ui.lstList->clear();
	QVector <Note> to_browser = DB.records();
	for (int i = 0; i < to_browser.size(); i++)
	{
		QString noteInBrowser = makeBrowserString(to_browser[i]);
		ui.lstList->insertItem(i, noteInBrowser);
		ui.lstList->item(i)->setData(Qt::UserRole, to_browser[i].id);
	}

	if (ui.lstList->count() > 0)
		ui.lstList->setCurrentRow(0);
	else if (ui.lstList->currentRow() == -1)
		clearWidgets();

	ReleaseMutex(fill_browser_mutex);
}