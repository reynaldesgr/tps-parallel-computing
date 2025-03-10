#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[]) {
    int rank, size, local_value, neighbor_value;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    srand(time(NULL) + rank);
    local_value = rand() % 100;
    printf("Process %d a généré : %d\n", rank, local_value);
    
    for (int phase = 0; phase < size; phase++) {
        int partner;
        if (phase % 2 == 0) {
            if (rank % 2 == 0 && rank + 1 < size) partner = rank + 1;
            else if (rank % 2 == 1) partner = rank - 1;
            else continue;
        } else {
            if (rank % 2 == 1 && rank + 1 < size) partner = rank + 1;
            else if (rank % 2 == 0 && rank > 0) partner = rank - 1;
            else continue;
        }
        
        MPI_Sendrecv(&local_value, 1, MPI_INT, partner, 0,
                     &neighbor_value, 1, MPI_INT, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        if ((rank < partner && local_value > neighbor_value) ||
            (rank > partner && local_value < neighbor_value)) {
            swap(&local_value, &neighbor_value);
        }
    }
    
    int *sorted_array = NULL;
    if (rank == 0) sorted_array = (int *) malloc(size * sizeof(int));
    MPI_Gather(&local_value, 1, MPI_INT, sorted_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Tableau trié : ");
        for (int i = 0; i < size; i++) printf("%d ", sorted_array[i]);
        printf("\n");
        free(sorted_array);
    }
    
    MPI_Finalize();
    return 0;
}
