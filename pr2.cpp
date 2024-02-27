#include <vector>
#include <iostream>
#include <mpi.h>
#include <ctime>

using namespace std;

vector<float> multiplication(const vector<float>& A, const vector<float>& B) {
    int n_rows = A.size() / B.size();
    vector<float> local_result(n_rows, 0);

    int indexA = 0;
    for (int i = 0; i < n_rows; i++) {
        float result = 0;
        for (int j = 0; j < B.size(); j++) {
            result += A[indexA] * B[j];
            indexA += 1;
        }
        local_result[i] = result;
    }
    return local_result;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank_id;
    int nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int matrix_size = 32;
    int total_elements = matrix_size * matrix_size;
    int elements_per_process = total_elements / nprocs;

    vector<float> A(total_elements, 0);
    vector<float> B(matrix_size, 0);
    vector<float> C(matrix_size, 0);

    if (rank_id == 0) {
        srand(time(0));

        for (int i = 0; i < total_elements; i++) {
            A[i] = std::rand() % 10 + 1;
        }

        for (int i = 0; i < matrix_size; i++) {
            B[i] = std::rand() % 10;
        }

        for (int i = 1; i < nprocs; i++) {
            MPI_Send(B.data(), matrix_size, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(B.data(), matrix_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        A.resize(elements_per_process, 0);
    }

    MPI_Scatter(A.data(), elements_per_process, MPI_FLOAT, A.data(), elements_per_process, MPI_FLOAT, 0, MPI_COMM_WORLD);

    C = multiplication(A, B);

    MPI_Gather(C.data(), elements_per_process, MPI_FLOAT, C.data(), elements_per_process, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank_id == 0) {
        cout << "Multiplication A * B = C" << endl;
        for (int i = 0; i < C.size(); ++i) {
            cout << "C[" << i << "]=" << C[i] << endl;
        }
    }

    MPI_Finalize();
    return 0;
}