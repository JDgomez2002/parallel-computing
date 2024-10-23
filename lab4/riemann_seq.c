
// Universidad del Valle de Guatemala
// Programación paralela y distribuida
// Laboratorio 4
// Daniel Gomez 21429

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double f(double x) {
    return x * x;
    // return sin(x); //test de la funcion seno
}

int main() {
    double a = 0.0;
    double b = 1.0;
    long n = 1000000;
    double h = (b - a) / n;
    double sum = 0.0;
    
    clock_t start = clock();
    
    // Calcular la suma de Riemann
    for (long i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    sum *= h;
    
    clock_t end = clock();
    double execution_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("\nResultados (Versión Secuencial):\n");
    printf("--------------------------------\n");
    printf("Aproximación de la integral: %f\n", sum);
    double exact_value = (pow(b, 3) - pow(a, 3)) / 3.0;
    printf("Valor exacto: %f\n", exact_value);
    printf("Error absoluto: %e\n", fabs(exact_value - sum));
    printf("Tiempo de ejecución: %f segundos\n", execution_time);
    
    return 0;
}