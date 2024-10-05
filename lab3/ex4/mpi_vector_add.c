/*
    Jose Daniel Gomez Cabrera 21429
    Computacion paralela y distribuida
    Laboratorio 3
    Universidad del Valle de Guatemala
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 100000
#define SCALAR 2.5  // Definimos un escalar constante para la multiplicación

void Allocate_vectors(double** local_x_pp, double** local_y_pp,
      double** local_z_pp, double** local_scalar_x_pp, double** local_scalar_y_pp, int local_n, MPI_Comm comm);
void Generate_vector(double local_a[], int local_n, int n, int my_rank, MPI_Comm comm);
void Print_partial_vector(double local_b[], int local_n, int n, char title[],
      int my_rank, MPI_Comm comm);
void Parallel_vector_sum(double local_x[], double local_y[],
      double local_z[], int local_n);
double Parallel_dot_product(double local_x[], double local_y[], int local_n, MPI_Comm comm);
void Scalar_multiply(double local_x[], double local_result[], int local_n, double scalar);

int main(void) {
   int n = N, local_n;
   int comm_sz, my_rank;
   double *local_x, *local_y, *local_z, *local_scalar_x, *local_scalar_y;
   MPI_Comm comm;
   double start, end, dot_product;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   start = MPI_Wtime(); // Start timing

   local_n = n / comm_sz;

   Allocate_vectors(&local_x, &local_y, &local_z, &local_scalar_x, &local_scalar_y, local_n, comm);

   Generate_vector(local_x, local_n, n, my_rank, comm);
   Generate_vector(local_y, local_n, n, my_rank, comm);

   // Vector sum
   Parallel_vector_sum(local_x, local_y, local_z, local_n);

   // Dot product
   dot_product = Parallel_dot_product(local_x, local_y, local_n, comm);

   // Scalar multiplication
   Scalar_multiply(local_x, local_scalar_x, local_n, SCALAR);
   Scalar_multiply(local_y, local_scalar_y, local_n, SCALAR);

   if (my_rank == 0) {
      printf("Vector operations results:\n");
   }

   Print_partial_vector(local_x, local_n, n, "Vector x:", my_rank, comm);
   Print_partial_vector(local_y, local_n, n, "Vector y:", my_rank, comm);
   Print_partial_vector(local_z, local_n, n, "Sum (x + y):", my_rank, comm);
   if (my_rank == 0) {
      printf("Dot product (x · y): %f\n", dot_product);
   }
   Print_partial_vector(local_scalar_x, local_n, n, "Scalar * x:", my_rank, comm);
   Print_partial_vector(local_scalar_y, local_n, n, "Scalar * y:", my_rank, comm);

   free(local_x);
   free(local_y);
   free(local_z);
   free(local_scalar_x);
   free(local_scalar_y);

   end = MPI_Wtime(); // End timing

   if (my_rank == 0) {
      printf("Total execution time: %f seconds\n", end - start);
   }

   MPI_Finalize();

   return 0;
}

void Allocate_vectors(double** local_x_pp, double** local_y_pp,
      double** local_z_pp, double** local_scalar_x_pp, double** local_scalar_y_pp, int local_n, MPI_Comm comm) {
   *local_x_pp = malloc(local_n*sizeof(double));
   *local_y_pp = malloc(local_n*sizeof(double));
   *local_z_pp = malloc(local_n*sizeof(double));
   *local_scalar_x_pp = malloc(local_n*sizeof(double));
   *local_scalar_y_pp = malloc(local_n*sizeof(double));
   if (*local_x_pp == NULL || *local_y_pp == NULL || *local_z_pp == NULL ||
       *local_scalar_x_pp == NULL || *local_scalar_y_pp == NULL) {
      fprintf(stderr, "Can't allocate local vectors\n");
      MPI_Abort(comm, -1);
   }
}

void Generate_vector(double local_a[], int local_n, int n, int my_rank, MPI_Comm comm) {
   double* a = NULL;
   int i;

   if (my_rank == 0) {
      a = malloc(n*sizeof(double));
      if (a == NULL) {
         fprintf(stderr, "Can't allocate temporary vector\n");
         MPI_Abort(comm, -1);
      }
      srand(time(NULL));
      for (i = 0; i < n; i++)
         a[i] = (double)rand() / RAND_MAX;
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
      free(a);
   } else {
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
   }
}

void Print_partial_vector(double local_b[], int local_n, int n, char title[],
      int my_rank, MPI_Comm comm) {
   double* b = NULL;
   int i;

   if (my_rank == 0) {
      b = malloc(n*sizeof(double));
      if (b == NULL) {
         fprintf(stderr, "Can't allocate temporary vector\n");
         MPI_Abort(comm, -1);
      }
      MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
      printf("%s\n", title);
      for (i = 0; i < 10; i++)
         printf("%f ", b[i]);
      printf("... ");
      for (i = n - 10; i < n; i++)
         printf("%f ", b[i]);
      printf("\n");
      free(b);
   } else {
      MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
   }
}

void Parallel_vector_sum(double local_x[], double local_y[],
      double local_z[], int local_n) {
   int local_i;
   for (local_i = 0; local_i < local_n; local_i++)
      local_z[local_i] = local_x[local_i] + local_y[local_i];
}

double Parallel_dot_product(double local_x[], double local_y[], int local_n, MPI_Comm comm) {
   int local_i;
   double local_dot = 0.0, global_dot;
   for (local_i = 0; local_i < local_n; local_i++)
      local_dot += local_x[local_i] * local_y[local_i];
   
   MPI_Allreduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, comm);
   return global_dot;
}

void Scalar_multiply(double local_x[], double local_result[], int local_n, double scalar) {
   int local_i;
   for (local_i = 0; local_i < local_n; local_i++)
      local_result[local_i] = scalar * local_x[local_i];
}
