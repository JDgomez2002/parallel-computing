/*
    Jose Daniel Gomez Cabrera 21429
    Computacion paralela y distribuida
    Laboratorio 3
    Universidad del Valle de Guatemala
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000

void Allocate_vectors(double** x_pp, double** y_pp, double** z_pp, int n);
void Generate_vector(double a[], int n);
void Print_partial_vector(double b[], int n, char title[]);
void Vector_sum(double x[], double y[], double z[], int n);

int main(void) {
   double *x, *y, *z;
   clock_t start, end;
   double cpu_time_used;

   start = clock(); // Start timing

   srand(time(NULL));  // Initialize random seed

   Allocate_vectors(&x, &y, &z, N);
   
   Generate_vector(x, N);
   Generate_vector(y, N);
   
   Vector_sum(x, y, z, N);

   Print_partial_vector(x, N, "Vector x:");
   Print_partial_vector(y, N, "Vector y:");
   Print_partial_vector(z, N, "The sum is:");

   free(x);
   free(y);
   free(z);

   end = clock(); // End timing
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   printf("Total execution time: %f seconds\n", cpu_time_used);

   return 0;
}

void Allocate_vectors(double** x_pp, double** y_pp, double** z_pp, int n) {
   *x_pp = malloc(n*sizeof(double));
   *y_pp = malloc(n*sizeof(double));
   *z_pp = malloc(n*sizeof(double));
   if (*x_pp == NULL || *y_pp == NULL || *z_pp == NULL) {
      fprintf(stderr, "Can't allocate vectors\n");
      exit(-1);
   }
}

void Generate_vector(double a[], int n) {
   for (int i = 0; i < n; i++)
      a[i] = (double)rand() / RAND_MAX;
}

void Print_partial_vector(double b[], int n, char title[]) {
   printf("%s\n", title);
   for (int i = 0; i < 10; i++)
      printf("%f ", b[i]);
   printf("... ");
   for (int i = n - 10; i < n; i++)
      printf("%f ", b[i]);
   printf("\n");
}

void Vector_sum(double x[], double y[], double z[], int n) {
   for (int i = 0; i < n; i++)
      z[i] = x[i] + y[i];
}