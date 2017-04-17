#include "mpi.h"
#include <iostream>
#include <string> 
#include <vector> 

using namespace std;
void Task1();
void Task3();
void Task4();
void Task5();
void Task6();
void Task7();
void IndividualTask();

void PutDelimiter(std::string taskName, int procRank)
{
    if (procRank != 0) 
        return;

    cout << taskName << endl;
    for (int i = 0; i < 20; i++)
        cout << '=';
    cout << endl;
}


typedef void (*Task)(void);
int main(int argc, char * argv[])
{
    std::vector<Task> tasks { Task1, Task3,  Task5, Task6, Task7, IndividualTask };
    std::vector<std::string> taskNames{ "Task1", "Task3",  "Task5", "Task6", "Task7", "IndividualTask" };

    MPI_Init(&argc, &argv);
    double start, end;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    for (int i = 0; i < tasks.size(); i++) 
    {
        PutDelimiter(taskNames[i], procRank);

        start = MPI_Wtime();  
        tasks[i]();
        end = MPI_Wtime();

        MPI_Barrier(MPI_COMM_WORLD);

        if (procRank == 0)
            cout << "Time: " << end - start << endl << endl;
    }
    
    MPI_Finalize();
}

// Print proc size
void Task1() 
{
    int procSize, procRank;
    
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    if (procRank == 0)
        cout << "Size is " << procSize << endl;

    cout << procRank << endl;
}

// Send number in a circle
void Task3()
{
    int procSize, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    bool isMain = procRank == 0;
    if (isMain)
    {
        int buffer = 15;
        int recieverRank = (procRank + 1) % procSize;
        MPI_Send(&buffer, 1, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);
        int lastRank = procSize - 1;

        // Other procs work
        MPI_Status recvStatus;
        int gotFromLast;
        MPI_Recv(&gotFromLast, 1, MPI_INT, lastRank, MPI_ANY_TAG, MPI_COMM_WORLD, &recvStatus);
        cout << "Got " << gotFromLast << endl;
    }
    else 
    {
        int buffer;
        MPI_Status recvStatus;
        int senderRank = procRank - 1;
        MPI_Recv(&buffer, 1, MPI_INT, senderRank, MPI_ANY_TAG, MPI_COMM_WORLD, &recvStatus);
        cout << "Proc " << procRank << " got " << buffer << " from " << senderRank << endl;

        int recieverRank = (procRank + 1) % procSize;
        MPI_Send(&buffer, 1, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);
    }
}


void SendTest() {
    int procSize, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    // send big data and watch for time of execution (in 2 different programms)
    double start = MPI_Wtime();
    bool isMain = procRank == 0;
    if (isMain)
    {
        int data[1000] = { 0 };
        MPI_Send(data, 1000, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Status status;
        int data[1000];
        MPI_Recv(data, 1000, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    double finish = MPI_Wtime();

    if (isMain)
        cout << "Send time is " << finish - start << endl;
}

void SsendTest() 
{
    int procSize, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    double start = MPI_Wtime();
    bool isMain = procRank == 0;
    if (isMain)
    {
        int data[1000] = { 0 };
        MPI_Ssend(data, 1000, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Status status;
        int data[1000];
        MPI_Recv(data, 1000, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // если эту строку закомментировать, то программа зависнет
    }
    double finish = MPI_Wtime();

    if (isMain)
        cout << "Ssend time is " << finish - start << endl;
}


// Sync send big data
// Or thread not recieve sync send package
void Task4()
{
    SendTest();
    SsendTest();
}


template <class T>
void PrintArray(T * arr, int size) 
{
    for (int i = 0l; i < size; i++)
        cout << arr[i] << ' ';

    cout << endl;
}


// BSend with buffer attachment
// Recv array, send array
void Task5() 
{
    const int dataSize = 10;
    int procSize, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    bool isMain = procRank == 0;
    if (isMain)
    {
        int data[dataSize] = { 0 };
        int buffSize = dataSize * sizeof(int) + MPI_BSEND_OVERHEAD;
        char * buffer = (char *) malloc(buffSize);
        MPI_Buffer_attach(buffer, buffSize);

        int recieverRank = (procRank + 1) % procSize;
        MPI_Bsend(&data, dataSize, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);
        int lastRank = procSize - 1;

        // Other procs work

        MPI_Status recvStatus;
        MPI_Recv(&data, dataSize, MPI_INT, lastRank, MPI_ANY_TAG, MPI_COMM_WORLD, &recvStatus);

        MPI_Buffer_detach(buffer, &buffSize);
        free(buffer);

        PrintArray(data, dataSize);
    }
    else
    {
        int data[dataSize];
        
        MPI_Status recvStatus;
        int senderRank = procRank - 1;
        MPI_Recv(&data, dataSize, MPI_INT, senderRank, MPI_ANY_TAG, MPI_COMM_WORLD, &recvStatus);
        cout << "Proc " << procRank << " got from " << senderRank << endl;

        int recieverRank = (procRank + 1) % procSize;

        int buffSize = dataSize * sizeof(int) + MPI_BSEND_OVERHEAD;
        char * buffer = (char *)malloc(buffSize);
        MPI_Buffer_attach(buffer, buffSize);

        MPI_Bsend(&data, dataSize, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);

        MPI_Buffer_detach(buffer, &buffSize);
        free(buffer);
    }
}


// Send to all
// Reciecieve from anyone withour knowledge 
// about count of elements or source
void Task6()
{
    int procSize, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    bool isMain = procRank == 0;
    if (isMain)
    {
        char data[100] = "I love cheese. I need cheese!";
        MPI_Send(data, strlen(data) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    }
    else if (procRank == 1)
    {
        MPI_Status recvStatus;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &recvStatus);
        int source = recvStatus.MPI_SOURCE;
        int count = recvStatus.count;

        char *data = new char[count];
        MPI_Recv(data, count, MPI_CHAR, source, 0, MPI_COMM_WORLD, &recvStatus);
        cout << "Proc " << procRank << " got from " << source << " message with count " << count << endl;
        PrintArray<char>(data, count);
        delete[] data;
    }
}


#include <Windows.h>
// http://rsusu1.rnd.runnet.ru/tutor/method/m2/page07.html
// Reciever waits and performs calculations
// And writes message
void Task7()
{
    int procSize, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    bool isMain = procRank == 0;
    if (isMain)
    {
        const int bufferSize = 100;
        int buffer[bufferSize] = {1, 2, 3, 4, 5, 6};

        // Send
        int recieverRank = (procRank + 1) % procSize;
        MPI_Request sendRequest;
        MPI_Status sendStatus;
        MPI_Isend(&buffer, bufferSize, MPI_INT, recieverRank, 0, MPI_COMM_WORLD, &sendRequest);
        MPI_Wait(&sendRequest, &sendStatus);

        // Recv
        MPI_Status recvStatus;
        MPI_Request recvRequest;
        int lastRank = procSize - 1;
        int inBuffer[bufferSize] = {};

        MPI_Irecv(&buffer, bufferSize, MPI_INT, lastRank, MPI_ANY_TAG, MPI_COMM_WORLD, &recvRequest);
        MPI_Wait(&recvRequest, &recvStatus);

        PrintArray(inBuffer, bufferSize);
    }
    else
    {
        // Recv
        const int bufferSize = 100;
        int buffer[bufferSize];
        
        MPI_Request recvRequest;
        int senderRank = procRank - 1;
        MPI_Irecv(&buffer, bufferSize, MPI_INT, senderRank, MPI_ANY_TAG, MPI_COMM_WORLD, &recvRequest);
     
        MPI_Status recvStatus;
        int recvFinished;
        do { 
            cout << "Proc " << procRank << " waiting for recieve from " << senderRank << endl;
            // While recv is performing, we can do other things here
            Sleep(100); // Calculations
            MPI_Test(&recvRequest, &recvFinished, &recvStatus);
        } while (!recvFinished);


        // Send
        int recieverRank = (procRank + 1) % procSize;
        MPI_Request sendRequest;
        MPI_Status sendStatus;
        MPI_Isend(&buffer, bufferSize, MPI_INT, recieverRank, 0, MPI_COMM_WORLD, &sendRequest);
        MPI_Wait(&sendRequest, &sendStatus);
    }
}




#include <vector>
using std::vector;
typedef double(*matrFunc)(double); // math func
void IndividualTask()
{
    int procSize, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    
    const int rangeLength = 20;   
    vector<matrFunc> functions(procSize, sin); // many sinuses

    bool isMain = procRank == 0;
    if (isMain)
    {
        // Create Range
        double range[rangeLength] = {};
        double start = 0.0;
        double finish = 150.0;
        double step = abs(start - finish) / rangeLength;
        for (int i = 0; i < rangeLength; i++)
            range[i] = start + i*step;

        MPI_Bcast(range, rangeLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        double * functionValues = new double[rangeLength];
        for (int i = 1; i < procSize; i++) 
        {
            MPI_Status recvStatus;
            MPI_Recv(functionValues, rangeLength, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &recvStatus);
            cout << "Function " << recvStatus.MPI_SOURCE << " calculated" << endl;
            PrintArray(functionValues, rangeLength);
        }
    }
    else
    {
        double * range = new double[rangeLength];
        MPI_Bcast(range, rangeLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        

        double *functionValues = new double[rangeLength];

        //Calculations
        for (int i = 0; i < rangeLength; i++)
            functionValues[i] = functions[procRank](range[i]);

        MPI_Send(functionValues, rangeLength, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        
        delete[] range;
        delete[] functionValues;
    }
}