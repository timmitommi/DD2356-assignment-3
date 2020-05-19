#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int count = 0;
    double x, y, z, pi, t1, t2;
    int rank, size, i, provided;
    
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    t1 = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int iter_per_proc = NUM_ITER/size;

    srand(SEED*rank); // Important: Multiply SEED by "rank" when you introduce MPI!
    
    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < iter_per_proc; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            count++;
        }
    }
    
    if (rank != 0) {
      MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
      int recv_count;
      MPI_Status status;
      for (int i = 0; i < size-1; i++) {
        MPI_Recv(&recv_count, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        count += recv_count;
      }
      // Estimate Pi and display the result
      pi = ((double)count / (double)NUM_ITER) * 4.0;
      t2 = MPI_Wtime();
      printf("The result is %f\nt = %f\n", pi, t2-t1);
    }

    MPI_Finalize();
    return 0;
}
