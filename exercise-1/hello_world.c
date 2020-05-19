#include <stdio.h>
#include <mpi.h>

int main(int argc, char *arv[]) {
  
  int rank, size, i, provided;

  MPI_Init_thread(&argc, &arv, MPI_THREAD_SINGLE, &provided);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Hello World from rank %d from %d processes\n", rank, size);

  MPI_Finalize();
}



