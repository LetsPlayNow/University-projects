#include "Test.h"
#include <fstream>
using std::ofstream;

int main(int argc, char * argv[])
{
    string arg = argv[1];
    bool testSerial = false, testOMP = false, testMPI = false;
    if (arg == "serial")
        testSerial = true;
    else if (arg == "omp")
        testOMP = true;
    else if (arg == "mpi")
        testMPI = true;
    else
        testSerial = true;

    srand(1);
    const int testsCount = 10;
    int ns[testsCount] = { 10, 25, 50, 100, 200, 400, 800, 1000, 1300, 1600 };
    
    ofstream serialFile, ompFile, mpiFile;
    if (testSerial) serialFile = ofstream("serial.csv");
    if (testOMP)    ompFile = ofstream("omp.csv");
    if (testMPI)    mpiFile = ofstream("mpi.csv");

    if (testMPI)
        MPI_Init(&argc, &argv);

    for (int i = 0; i < testsCount; i++)
    {
        if (testSerial)
        {
            double serialTime = TestSerial(ns[i]);
            serialFile << ns[i] << ',' << serialTime << endl;
        }
        if (testOMP)
        {
            double ompTime = TestOMP(ns[i]);
            ompFile << ns[i] << ',' << ompTime << endl;
        }
        if (testMPI)
        {
            double mpiTime = TestMPI(ns[i]);
            int procRank;
            MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
            
            bool isMain = procRank == 0;
            if (isMain)
                mpiFile << ns[i] << ',' << mpiTime << endl;
        }  
    }

    if (testMPI)
        MPI_Finalize();

    if (testSerial) serialFile.close();
    if (testOMP)    ompFile.close();
    if (testMPI)    mpiFile.close();
}




