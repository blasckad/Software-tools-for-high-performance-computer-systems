#include <iostream>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello: rank %d\n",rank);
    MPI_Finalize();
    
}
