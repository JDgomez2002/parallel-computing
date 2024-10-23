# Laboratorio 4 🧪
### Implementación de sumas de Riemann usando openMPI 🧑🏻‍💻
- José Daniel Gómez Cabrera 21429

##### Para poder compilar y ejecutar el archivo **riemann_mpi.c**
```
mpicc -o riemann_mpi riemann_mpi.c -lm
```
```
mpirun -np 4 ./riemann_mpi
```

##### Para compilar y ejecutar el archivo **riemann_mp.c**
```
gcc -o riemann_omp riemann_omp.c -fopenmp -lm
```
```
./riemann_omp
```

##### Para compilar y ejecutar el archivo **riemann_seq.c**

```
gcc -o riemann_seq riemann_seq.c -lm
```
```
./riemann_seq
```
