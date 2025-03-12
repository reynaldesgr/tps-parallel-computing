#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int main(int argc, char *argv[]) {
    int rank, size, local_value, neighbor_value;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);
    local_value = rand() % 100;
    std::cout << "Process " << rank << " generated: " << local_value << std::endl;

    for (int phase = 0; phase < size; phase++) {
        int partner;
        if (phase % 2 == 0) {
            partner = (rank % 2 == 0) ? rank + 1 : rank - 1;
        } else {
            partner = (rank % 2 == 1) ? rank + 1 : rank - 1;
        }

        if (partner >= 0 && partner < size) {
            MPI_Sendrecv(&local_value, 1, MPI_INT, partner, 0,
                         &neighbor_value, 1, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if ((rank < partner && local_value > neighbor_value) || 
                (rank > partner && local_value < neighbor_value)) {
                swap(local_value, neighbor_value);
            }
        }
    }

    int *sorted_array = nullptr;
    if (rank == 0) sorted_array = new int[size];
    MPI_Gather(&local_value, 1, MPI_INT, sorted_array, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Sorted array: ";
        for (int i = 0; i < size; i++) std::cout << sorted_array[i] << " ";
        std::cout << std::endl;
        delete[] sorted_array;
    }

    MPI_Finalize();
    return 0;
}
