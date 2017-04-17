#include "Semaphore.h"


Semaphore::Semaphore(LPCWSTR semaphore_name = L"Semaphore") :Mutex(semaphore_name)
{
	count = 0;
}


Semaphore::~Semaphore()
{
}

void Semaphore::lock()
{
	for (WaitForSingleObject(mutex, INFINITE); count > 0;
		WaitForSingleObject(mutex, INFINITE))
		ReleaseMutex(mutex);
}

void Semaphore::unlock()
{
	ReleaseMutex(mutex);
}

void Semaphore::enter()
{
	WaitForSingleObject(mutex, INFINITE);
	++count;
	ReleaseMutex(mutex);
}

void Semaphore::leave()
{
	WaitForSingleObject(mutex, INFINITE);
	--count;
	ReleaseMutex(mutex);
}