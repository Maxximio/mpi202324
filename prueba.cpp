#include <mpi.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

std::vector<int> read_file() {
    std::fstream fs("path/to/datos.txt", std::ios::in);
    std::string line;
    std::vector<int> ret;
    while (std::getline(fs, line)) {
        ret.push_back(std::stoi(line));
    }
    fs.close();
    return ret;
}

std::vector<int> frecuencias(const std::vector<int>& datos) {
    std::vector<int> cc(101);

    for (int dato : datos) {
        if (dato >= 1 && dato <= 100) {
            cc[dato]++;
        }
    }
    return cc;
}

/*int main(int argc, char** argv) {//v1 frecuencia solo con reduce
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    std::vector<int> datos = read_file();
    std::vector<int> cc = frecuencias(datos);

    std::vector<int> global_cc(101);
    MPI_Reduce(cc.data(), global_cc.data(), 101, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 1; i <= 100; ++i) {
            std::cout << "El número " << i << " aparece " << global_cc[i] << " veces." << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}*/

int main(int argc, char** argv) { //frecuencia
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    std::vector<int> datos;
    if (rank == 0) {
        datos = read_file();
    }

    int num_datos = datos.size();

    MPI_Bcast(datos.data(), num_datos, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> mis_datos(num_datos);
    MPI_Scatter(datos.data(), num_datos / nprocs, MPI_INT, mis_datos.data(), num_datos / nprocs, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> cc_local = frecuencias(mis_datos);

    std::vector<int> cc_global(101);
    MPI_Reduce(cc_local.data(), cc_global.data(), 101, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 1; i <= 100; ++i) {
            std::cout << "número: " << i << " -> " << cc_global[i] << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}

/*int main(int argc, char** argv) {//promedio
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    std::vector<int> data;
    if (rank == 0) {
        data = read_file();
    }

    int n = data.size();
    int local_n = n / nprocs;
    std::vector<int> local_data(local_n);

    MPI_Scatter(data.data(), local_n, MPI_INT, local_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int i = 0; i < local_n; ++i) {
        local_sum += local_data[i];
    }

    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double promedio = static_cast<double>(global_sum) / n;

    if (rank == 0) {
        std::cout << "El promedio es " << promedio << std::endl;
    }

    MPI_Finalize();
    return 0;
}*/

/*int main(int argc, char** argv) {//minimo y maximo
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    std::vector<int> numbers = read_file();

    int local_min = numbers[rank * numbers.size() / nprocs];
    int local_max = numbers[rank * numbers.size() / nprocs];

    for (int i = rank * numbers.size() / nprocs + 1; i < (rank + 1) * numbers.size() / nprocs; ++i) {
        if (numbers[i] < local_min) {
            local_min = numbers[i];
        }
        if (numbers[i] > local_max) {
            local_max = numbers[i];
        }
    }

    int global_min, global_max;
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Minimo: " << global_min << ", Maximo: " << global_max<< std::endl;
    }

    MPI_Finalize();
    return 0;
}*/

/*int main(int argc, char** argv) {//comprobar debug
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == 0) {

        int block_size;
        int real_size;
        int padding=0;

        std::vector<int> data = read_file();

        int n = data.size();

        if(n!=0){

            real_size = std::ceil((int)n/nprocs)*nprocs;
            block_size = real_size/nprocs;
            std::cout << "bloques" << block_size << std::endl;
            padding=real_size-n;
        }
    }
    

    MPI_Finalize();
    return 0;
}*/






