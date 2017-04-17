#pragma once
#include "windows.h"
class Mutex
{
public:
	Mutex(LPCWSTR mutex_name);
	Mutex();
	virtual ~Mutex();

	virtual void lock();
	virtual void unlock();

	//Костыльные методы для поддержания полиморфизма
	virtual void enter();
	virtual void leave();

protected:
	HANDLE mutex;
};

