#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define MPI_MAX_MSG_LENGTH 1000

int main(int argc, char **argv) {
    int rang, nbprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);

    char message[MPI_MAX_MSG_LENGTH];

    if (rang == nbprocs - 1) {
        // PN-1 initialise son message
        sprintf(message, "Bonjour de la part de P%d", rang);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, rang - 1, 0, MPI_COMM_WORLD);
    } else {
        // Réception du message
        MPI_Recv(message, MPI_MAX_MSG_LENGTH, MPI_CHAR, rang + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Concaténer le message
        char buffer[MPI_MAX_MSG_LENGTH];
        sprintf(buffer, "%s, P%d", message, rang);
        strcpy(message, buffer);

        // Envoyer au processus précédent (sauf P0 qui affiche)
        if (rang > 0) {
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, rang - 1, 0, MPI_COMM_WORLD);
        } else {
            // P0 affiche le message final
            printf("Message final : %s !\n", message);
        }
    }

    MPI_Finalize();
    return 0;
}
