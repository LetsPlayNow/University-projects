#pragma once
#include "windows.h"
#include "Mutex.h"
class Semaphore : public Mutex
{
public:
	Semaphore(LPCWSTR semaphore_name);
	~Semaphore();

	void enter(); //Захватить устанавливаемый семафор
	void leave(); //Отпустить устанавливаемый семафор

	void lock();
	void unlock(); //Спорный момент
private:
	int count; //Число потоков, захвативших этот семафор
};

