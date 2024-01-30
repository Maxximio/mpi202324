#include <iostream>
#include <vector>
#include <mpi.h>

#define MAX_ELEMENTOS 1024

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> input(MAX_ELEMENTOS);

    if (rank == 0) {
        for (int i = 0; i < MAX_ELEMENTOS; i++) {
            input[i] = i + 1;
        }
    }

    // Broadcast input vector to all processes
    MPI_Bcast(input.data(), MAX_ELEMENTOS, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;

    // Calculate local sum
    for (int i = rank; i < MAX_ELEMENTOS; i += size) {
        local_sum += input[i];
    }

    // Reduce local sums to get the global sum
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::printf("La suma es: %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}