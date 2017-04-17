//Реализация класса "ExampleValidator"
#include "Validators.h"

ExampleValidator::ExampleValidator(QObject *parent)
	: QValidator(parent)
{}

ExampleValidator::~ExampleValidator()
{}

//Валидатор для второй строки
QValidator::State ExampleValidator::validate(QString & input, int &) const
{
	bool acceptable = 1;
	bool intermediate = input.isEmpty();
	//Не более 100 символов, состоит из букв, двойных кавычек, пробелов и знаков препинания , . !
	//Первый критерий проверется в QTDesigner
	//Все остальные проверим мы
	for (int i = 0; i<= input.size() - 1; i++)
	{
		acceptable = acceptable && (input[i].isLetter() || input[i] == ' ' || input[i] == ',' || input[i] == '.' || input[i] == '!');
	}

	//Блок выбора
	if (acceptable || intermediate)
	{return QValidator::Intermediate;}

	else 
	{return QValidator::Invalid;}
}