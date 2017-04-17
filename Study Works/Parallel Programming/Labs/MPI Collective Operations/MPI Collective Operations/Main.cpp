#include "LabWork4.h"
#include "Practice4.h"
#include <mpi.h>



int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);
    Practice::Task3(2);
    MPI_Finalize();
}