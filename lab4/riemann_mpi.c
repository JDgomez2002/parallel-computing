
// Universidad del Valle de Guatemala
// Programación paralela y distribuida
// Laboratorio 4
// Daniel Gomez 21429

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Función a integrar: ejemplo f(x) = x^2
double f(double x) {
    return x * x;
    // return sin(x); //test de la funcion seno
}

int main(int argc, char** argv) {
    int rank, size;
    double a, b;           // Límites de integración
    long n;                // Número total de subintervalos
    double h;              // Ancho de cada subintervalo
    double local_sum = 0;  // Suma parcial para cada proceso
    double total_sum;      // Suma total (resultado final)
    double start_time, end_time; // Variables para medir tiempo
    
    // Inicializar MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Iniciar el cronómetro
    start_time = MPI_Wtime();

    // El proceso 0 establece los parámetros iniciales
    if (rank == 0) {
        // Definir los límites de integración y número de subintervalos
        a = 0.0;          // Límite inferior
        b = 1.0;          // Límite superior
        n = 1000000;      // Número de subintervalos
        h = (b - a) / n;  // Ancho de cada subintervalo
        
        printf("Iniciando cálculo con %d procesos...\n", size);
        printf("Número de subintervalos: %ld\n", n);
        
        // Enviar parámetros a todos los demás procesos
        for (int i = 1; i < size; i++) {
            MPI_Send(&a, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            MPI_Send(&n, 1, MPI_LONG, i, 2, MPI_COMM_WORLD);
            MPI_Send(&h, 1, MPI_DOUBLE, i, 3, MPI_COMM_WORLD);
        }
    } else {
        // Los demás procesos reciben los parámetros
        MPI_Recv(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&b, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&n, 1, MPI_LONG, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&h, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Calcular los subintervalos para cada proceso
    long local_n = n / size;  // Número de subintervalos por proceso
    long start = rank * local_n;
    long end = (rank == size - 1) ? n : start + local_n;

    // Calcular la suma parcial de Riemann para este proceso
    for (long i = start; i < end; i++) {
        double x = a + (i + 0.5) * h;  // Punto medio del subintervalo
        local_sum += f(x);
    }
    local_sum *= h;

    // Recolección de resultados usando comunicación punto a punto
    if (rank != 0) {
        // Los procesos no-0 envían su suma parcial al proceso 0
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);
    } else {
        // El proceso 0 recibe y suma los resultados parciales
        total_sum = local_sum;  // Incluye su propia suma
        double partial_sum;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&partial_sum, 1, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += partial_sum;
        }
        
        // Detener el cronómetro
        end_time = MPI_Wtime();
        double execution_time = end_time - start_time;
        
        // Imprimir resultados y estadísticas
        printf("\nResultados:\n");
        printf("------------\n");
        printf("Aproximación de la integral de f(x) = x^2 en [%.1f, %.1f]: %f\n", a, b, total_sum);
        
        // Calcular el valor exacto para comparar (para x^2, la integral es x^3/3)
        double exact_value = (pow(b, 3) - pow(a, 3)) / 3.0;
        printf("Valor exacto: %f\n", exact_value);
        printf("Error absoluto: %e\n", fabs(exact_value - total_sum));
        
        printf("\nEstadísticas de rendimiento:\n");
        printf("---------------------------\n");
        printf("Tiempo de ejecución: %f segundos\n", execution_time);
        printf("Número de procesos: %d\n", size);
        printf("Subintervalos por proceso: %ld\n", local_n);
    }

    MPI_Finalize();
    return 0;
}