#include "Mutex.h"


Mutex::Mutex(LPCWSTR mutex_name = L"mutex")
{
	mutex = CreateMutex(NULL, FALSE, mutex_name);
}

Mutex::Mutex()
{
	mutex = CreateMutex(NULL, FALSE, L"Mutex");
}

Mutex::~Mutex()
{
}

void Mutex::lock()
{
	WaitForSingleObject(mutex, INFINITE);
}

void Mutex::unlock()
{
	ReleaseMutex(mutex);
}


void Mutex::enter()
{

}

void Mutex::leave()
{

}