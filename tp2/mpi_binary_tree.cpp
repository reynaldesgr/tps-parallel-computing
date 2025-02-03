#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rang, nbprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);

    char message[1000];
    sprintf(message, "P%d", rang);

    /**
    * Si un processus est divisible par 2 * step, il attend un message venant du processus rang + step.
    * Si rang % (2 * step) != 0, alors le processus doit envoyer son message au processus parent (rang - step).
    */
    int step = 1;
    while (step < nbprocs) {
        if (rang % (2 * step) == 0) {
            if (rang + step < nbprocs) {
                char buffer[1000];
                MPI_Recv(buffer, 1000, MPI_CHAR, rang + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                strcat(message, ", ");
                strcat(message, buffer);
            }
        } else {
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, rang - step, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    if (rang == 0) {
        printf("%s!\n", message);
    }

    MPI_Finalize();
    return 0;
}
