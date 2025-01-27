//
// Created by rsegerie on 27/01/25.
//


#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include "omp.h"

#define N_MAX 100
#define SEED 478210368

void multiply(int n, double A[N_MAX][N_MAX], double B[N_MAX][N_MAX], double C[N_MAX][N_MAX]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_in_parallel(int n, double A[N_MAX][N_MAX], double B[N_MAX][N_MAX], double C[N_MAX][N_MAX]) {
    int i, j, k;
    #pragma omp parallel for collapse(2) shared(A, B, C) private(i, j, k)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            C[i][j] = 0;
            for (k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Generate a random matrix
void generate_random_matrix(int n, double matrix[100][100]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double)(rand() % 100) / 10.0; // Valeurs entre 0.0 et 9.9
        }
    }
}

// Display a matrix
void print_matrix(int n, double matrix[N_MAX][N_MAX]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <taille de la matrice>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("La taille de la matrice doit être un entier positif.\n");
        return 1;
    }


    double A[100][100], B[100][100], C[100][100];

    srand(SEED);
    generate_random_matrix(n, A);
    generate_random_matrix(n, B);

    printf("Matrice A :\n");
    print_matrix(n, A);
    printf("Matrice B :\n");
    print_matrix(n, B);

    double start_time = omp_get_wtime();
    multiply_in_parallel(n, A, B, C);
    double end_time = omp_get_wtime();

    printf("Matrice C (résultat) :\n");
    print_matrix(n, C);

    printf("Temps d'exécution: %f secondes\n", end_time - start_time);

    return EXIT_SUCCESS;
    /*
  #pragma omp parallel num_threads(4) firstprivate(tid)
  {
    tid=omp_get_thread_num();
    if (!tid)
      printf("In the PARALLEL REGION TID %d:\n There are %d threads in process\n",
             omp_get_thread_num(), omp_get_num_threads());
      printf("Hello World from TID %d / %d on CPU %d of %s!\n",
           tid, omp_get_num_threads(), sched_getcpu(), hostname);
  }
  printf("After PARALLEL REGION TID %d:\n There are %d threads\n" ,
         tid, omp_get_num_threads()) ;
  return EXIT_SUCCESS ; */
}

