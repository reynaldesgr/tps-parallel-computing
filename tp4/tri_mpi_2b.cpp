#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define N 16 // Taille du tableau
#define P 4  // Nombre de processus

void merge(std::vector<int> &arr) {
    std::sort(arr.begin(), arr.end());
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int m = N / P;
    std::vector<int> local_array(m);
    srand(time(NULL) + rank);

    for (int &val : local_array) {
        val = rand() % 100;
    }
    merge(local_array);

    for (int phase = 0; phase < size; phase++) {
        int partner;
        if (phase % 2 == 0) {
            partner = (rank % 2 == 0) ? rank + 1 : rank - 1;
        } else {
            partner = (rank % 2 == 1) ? rank + 1 : rank - 1;
        }

        if (partner >= 0 && partner < size) {
            std::vector<int> recv_array(m);
            MPI_Sendrecv(local_array.data(), m, MPI_INT, partner, 0,
                         recv_array.data(), m, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::vector<int> combined(2 * m);
            std::merge(local_array.begin(), local_array.end(),
                       recv_array.begin(), recv_array.end(),
                       combined.begin());

            if (rank < partner) {
                std::copy(combined.begin(), combined.begin() + m, local_array.begin());
            } else {
                std::copy(combined.begin() + m, combined.end(), local_array.begin());
            }
        }
    }

    std::vector<int> sorted_array;
    if (rank == 0) sorted_array.resize(N);
    MPI_Gather(local_array.data(), m, MPI_INT, sorted_array.data(), m, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Sorted array: ";
        for (int val : sorted_array) std::cout << val << " ";
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
