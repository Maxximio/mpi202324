#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define N 6 // Número de vectores en A
#define M 3 // Número de vectores en C

double calcularDistancia(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificar que el número de procesos sea igual al número de vectores en A
    if (size != N) {
        if (rank == 0) {
            fprintf(stderr, "El número de procesos debe ser igual a %d\n", N);
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    double A[N][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0},
                      {10.0, 11.0, 12.0}, {13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}};

    double C[M][3] = {{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {2.0, 2.0, 2.0}};

    int indices[N]; // Almacenará el índice del vector más cercano en C para cada vector en A

    for (int i = 0; i < N; ++i) {
        double distanciaMinima = INFINITY;
        int indiceMinimo = -1;

        // Calcular la distancia entre el vector actual en A y todos los vectores en C
        for (int j = 0; j < M; ++j) {
            double distancia = calcularDistancia(A[i][0], A[i][1], A[i][2], C[j][0], C[j][1], C[j][2]);
            if (distancia < distanciaMinima) {
                distanciaMinima = distancia;
                indiceMinimo = j;
            }
        }

        // Almacenar el índice del vector más cercano en C para el vector actual en A
        indices[i] = indiceMinimo;
    }

    // Imprimir resultados
    if (rank == 0) {
        printf("Resultado:\n");
        for (int i = 0; i < N; ++i) {
            printf("Vector A[%d] está más cercano al vector C[%d]\n", i, indices[i]);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}