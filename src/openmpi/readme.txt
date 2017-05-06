required packages:
* mpich

compile:
mpicc <program>.c -o <program>
e.g.: mpicc -lm mpi_wave.c -o mpi_wave

run:
mpirun -np <Thread number> ./<program>
e.g.: mpirun -np 2 ./mpi_wave