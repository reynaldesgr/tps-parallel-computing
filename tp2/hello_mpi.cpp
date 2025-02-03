#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sched.h>

int main(int argc, char **argv)
{
    int rang = -1, nbprocs = 0;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int namelen;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    MPI_Get_processor_name(processor_name, &namelen);  // Récupérer le nom du noeud

    int cpu_id = sched_getcpu(); // Récupération du core sur lequel s'exécute le processus

    printf("Hello from process %d of %d running on node %s, core %d\n",
           rang, nbprocs, processor_name, cpu_id);

    MPI_Finalize();
    return 0;
}