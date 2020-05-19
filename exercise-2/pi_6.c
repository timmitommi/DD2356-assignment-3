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
    int count;
    double x, y, z, pi, t1, t2;
    int rank, size, i, provided;
    
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    t1 = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int iter_per_proc = NUM_ITER/size;

    int N = size * sizeof(int);

    int *counts = malloc(N);

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
    
    MPI_Win win;
    MPI_Win_allocate(N, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &counts, &win);

    MPI_Win_fence(0, win);
    {
      MPI_Put(&count, 1, MPI_INT, 0, rank, N, MPI_INT, win);
    }
    MPI_Win_fence(0, win);

    if (rank == 0) {
      for (i = 1; i < size; i++) {
        count += counts[i];
      }
      pi = ((double) count / (double)NUM_ITER) * 4.0;
      t2 = MPI_Wtime();
      printf("The result is %f\nt = %f\n", pi, t2-t1);
    }
   
    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}
