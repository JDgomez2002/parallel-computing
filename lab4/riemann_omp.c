
// Universidad del Valle de Guatemala
// Programación paralela y distribuida
// Laboratorio 4
// Daniel Gomez 21429

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Función a integrar: ejemplo f(x) = x^2
double f(double x) {
    return x * x;  // Podemos cambiar esta función según necesitemos
    // return sin(x);  // Ejemplo de función seno
}

int main() {
    double a = 0.0;          // Límite inferior
    double b = 1.0;          // Límite superior
    long n = 1000000;        // Número de subintervalos
    double h = (b - a) / n;  // Ancho de cada subintervalo
    double total_sum = 0.0;  // Resultado final
    int num_threads = 4;     // Definimos el número de hilos que queremos usar
    
    // Establecer el número de hilos antes de la región paralela
    omp_set_num_threads(num_threads);
    
    // Verificar que se estableció correctamente el número de hilos
    printf("Número de hilos solicitados: %d\n", num_threads);
    printf("Máximo número de hilos disponibles: %d\n", omp_get_max_threads());
    
    // Obtener el tiempo inicial
    double start_time = omp_get_wtime();
    
    // Verificar el número real de hilos en uso
    #pragma omp parallel
    {
        #pragma omp master
        {
            printf("Número real de hilos en uso: %d\n", omp_get_num_threads());
            printf("Número de subintervalos: %ld\n", n);
        }
    }

    // Calcular la suma de Riemann en paralelo
    #pragma omp parallel reduction(+:total_sum)
    {
        // Obtener información del hilo actual
        int thread_id = omp_get_thread_num();
        
        #pragma omp for
        for (long i = 0; i < n; i++) {
            double x = a + (i + 0.5) * h;  // Punto medio del subintervalo
            total_sum += f(x);
        }
        
        // Imprimir información por hilo (opcional)
        #pragma omp critical
        {
            printf("Hilo %d completó su parte del cálculo\n", thread_id);
        }
    }
    
    // Multiplicar por el ancho del subintervalo
    total_sum *= h;
    
    // Obtener el tiempo final
    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;
    
    // Calcular el valor exacto para comparar (para x^2, la integral es x^3/3)
    double exact_value = (pow(b, 3) - pow(a, 3)) / 3.0;
    
    // Imprimir resultados y estadísticas
    printf("\nResultados:\n");
    printf("------------\n");
    printf("Aproximación de la integral de f(x) = x^2 en [%.1f, %.1f]: %f\n", a, b, total_sum);
    printf("Valor exacto: %f\n", exact_value);
    printf("Error absoluto: %e\n", fabs(exact_value - total_sum));
    
    printf("\nEstadísticas de rendimiento:\n");
    printf("---------------------------\n");
    printf("Tiempo de ejecución: %f segundos\n", execution_time);
    printf("Número de hilos utilizados: %d\n", num_threads);
    printf("Subintervalos por hilo (aproximado): %ld\n", n / num_threads);
    
    return 0;
}