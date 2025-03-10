#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4 // Taille de la matrice et du vecteur

int main(int argc, char *argv[]) {
    int rank, size;
    double A[N][N], X[N], Y[N], local_A[N], local_Y;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = N / size;

    // Initialisation dans le processus 0
    if (rank == 0) {
        printf("Matrice A :\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = i + j; // Exemple de valeurs
                printf("%5.1f ", A[i][j]);
            }
            printf("\n");
        }

        printf("Vecteur X :\n");
        for (int i = 0; i < N; i++) {
            X[i] = i + 1; // Exemple de valeurs
            printf("%5.1f ", X[i]);
        }
        printf("\n");
    }

    // Diffusion du vecteur X à tous les processus
    MPI_Bcast(X, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Distribution des lignes de la matrice A
    MPI_Scatter(A, N * rows_per_process, MPI_DOUBLE, local_A, N * rows_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calcul local de Y
    for (int i = 0; i < rows_per_process; i++) {
        local_Y = 0.0;
        for (int j = 0; j < N; j++) {
            local_Y += local_A[i * N + j] * X[j];
        }
    }

    // Récupération des résultats dans Y
    MPI_Gather(&local_Y, rows_per_process, MPI_DOUBLE, Y, rows_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Affichage du résultat par le processus 0
    if (rank == 0) {
        printf("Résultat Y :\n");
        for (int i = 0; i < N; i++) {
            printf("%5.1f ", Y[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
