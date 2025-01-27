//
// Created by rsegerie on 27/01/25.
//


#include <ctime>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include "omp.h"

#define N_MAX 100
#define SEED 478210368

using namespace std;

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

void matrix_multiply_parallel(int n, double A[][100], double B[][100], double C[][100], const char* schedule_type) {
#pragma omp parallel for schedule(dynamic) collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            int thread_num = omp_get_thread_num();
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
#pragma omp critical
            cout << "Thread " << thread_num << " a calculé C[" << i << "][" << j << "] avec " << schedule_type << endl;
        }
    }
}


// Generate a random matrix
void generate_random_matrix(int n, double matrix[N_MAX][N_MAX]) {
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

// Verify results between sequential & parallel execution
bool check_results(int n, double C1[N_MAX][N_MAX], double C2[N_MAX][N_MAX]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(C1[i][j] - C2[i][j]) > 1e-6) {
                return false;
            }
        }
    }
    return true;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <taille de la matrice>" << endl;
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0 || n > 100) {
        cerr << "La taille de la matrice doit être un entier positif et <= 100." << endl;
        return 1;
    }

    double A[N_MAX][N_MAX], B[N_MAX][N_MAX], C_seq[N_MAX][N_MAX], C_par[N_MAX][N_MAX];

    srand(time(nullptr));
    generate_random_matrix(n, A);
    generate_random_matrix(n, B);


    cout << "Matrice A :" << endl;
    print_matrix(n, A);
    cout << "Matrice B :" << endl;
    print_matrix(n, B);


    // Time
    double start_seq = omp_get_wtime();
    multiply(n, A, B, C_seq);
    double end_seq = omp_get_wtime();
    double time_seq = end_seq - start_seq;
    cout << "Temps d'exécution séquentiel: " << (end_seq - start_seq) << " secondes" << endl;

    double start_par = omp_get_wtime();
    matrix_multiply_parallel(n, A, B, C_par, "dynamic");
    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;
    cout << "Temps d'exécution parallèle (dynamic): " << time_par << " secondes" << endl;

    start_par = omp_get_wtime();
    matrix_multiply_parallel(n, A, B, C_par, "static");
    end_par = omp_get_wtime();
    cout << "Temps d'exécution parallèle (static): " << time_par << " secondes" << endl;

    cout << "* Accélération obtenue: " << time_seq / time_par << "x \n" << endl;

    // Results
    cout << "Matrice C (résultat séquentielle) :" << endl;
    print_matrix(n, C_seq);

    cout << "Matrice C (résultat parallèle) :" << endl;
    print_matrix(n, C_par);


    if (check_results(n, C_seq, C_par)) {
        cout << "Les résultats sont corrects." << endl;
    } else {
        cout << "Les résultats sont incorrects." << endl;
    }

    return 0;
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

