#!/bin/bash

# Log file
LOGFILE="mpi_test_results.log"
echo "===== MPI Test Execution Started =====" > $LOGFILE
echo "Date: $(date)" >> $LOGFILE
echo "" >> $LOGFILE

echo "Compiling.." | tee -a $LOGFILE
mpic++ -o tri_mpi_2a tri_mpi_2a.cpp
mpic++ -o tri_mpi_2b tri_mpi_2b.cpp
echo "Compilation completed." | tee -a $LOGFILE
echo "" >> $LOGFILE

# Number of processes to test
PROCESS_COUNTS=(4 8 16 32)

# Testing case 2.a (p elements on p processes)
echo "===== Running test for case 2.a =====" | tee -a $LOGFILE
for p in "${PROCESS_COUNTS[@]}"; do
    echo "Testing with $p processes..." | tee -a $LOGFILE
    { time mpirun -np $p ./tri_mpi_2a ; } 2>> $LOGFILE
    echo "" >> $LOGFILE
done

# Testing case 2.b (n = m*p elements on p processes)
echo "===== Running test for case 2.b =====" | tee -a $LOGFILE
for p in "${PROCESS_COUNTS[@]}"; do
    echo "Testing with $p processes..." | tee -a $LOGFILE
    { time mpirun -np $p ./tri_mpi_2b ; } 2>> $LOGFILE
    echo "" >> $LOGFILE
done

echo "===== MPI Test Execution Completed =====" | tee -a $LOGFILE
echo "Results are saved in $LOGFILE."
