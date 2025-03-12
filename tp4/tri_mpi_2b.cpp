#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 16  // size of the array
#define P 4   // number of processes

int cmpfunc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// function to merge and split data between two processes
void merge_split(int *local_array, int *recv_array, int m, int rank, int partner) {
    int combined[2 * m];
    
    // merge the two sorted arrays
    memcpy(combined, local_array, m * sizeof(int));
    memcpy(combined + m, recv_array, m * sizeof(int));
    qsort(combined, 2 * m, sizeof(int), cmpfunc);

    // split the data
    if (rank < partner) {
        memcpy(local_array, combined, m * sizeof(int));
    } else {
        memcpy(local_array, combined + m, m * sizeof(int));
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // ensure N is a multiple of P
    if (N % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "ERROR: N must be a multiple of P!\n");
        }
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int m = N / P;
    int local_array[m];

    srand(time(NULL) + rank);

    // generate random numbers
    for (int i = 0; i < m; i++) {
        local_array[i] = rand() % 100;
    }

    // sort local array
    qsort(local_array, m, sizeof(int), cmpfunc);

    // odd-even sorting algorithm
    for (int phase = 0; phase < size; phase++) {
        int partner;
        if (phase % 2 == 0) {
            partner = (rank % 2 == 0) ? rank + 1 : rank - 1;
        } else {
            partner = (rank % 2 == 1) ? rank + 1 : rank - 1;
        }

        if (partner >= 0 && partner < size) {
            int recv_array[m];
            MPI_Status status;
            MPI_Sendrecv(local_array, m, MPI_INT, partner, 0,
                         recv_array, m, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, &status);

            int received_count;
            MPI_Get_count(&status, MPI_INT, &received_count);
            if (received_count != m) {
                fprintf(stderr, "ERROR: Process %d received %d instead of %d\n", rank, received_count, m);
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }

            merge_split(local_array, recv_array, m, rank, partner);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    int sorted_array[N];
    MPI_Gather(local_array, m, MPI_INT, sorted_array, m, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sorted array: ");
        for (int i = 0; i < N; i++) {
            printf("%d ", sorted_array[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
