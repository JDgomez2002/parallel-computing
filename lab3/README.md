# Laboratorio 3
### MPI
- Jose Daniel Gomez Cabrera 21429

Para ejecutar vector_add.c

```

gcc -o vector_add vector_add.c
./vector_add

```

Para ejecutar mpi_vector_add.c

```

mpicc -o mpi_vector_add mpi_vector_add.c
mpirun -n <número_de_procesos> ./mpi_vector_add

```
