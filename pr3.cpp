#include<mpi.h>

#include<vector>
#include<cmath>
#include<stdio.h>

#define WIDTH 720
#define HEIGHT 480

sdt::vector<int> generar_imagen(){
    
    std::vector<int> imagen(WIDTH*HEIGHT*4);

    for(int i =0; i<WIDTH*HEIGHT*4;i++){
        imagen[i]=255;
    }

    return imagen;

}

int main(int argc, char** argv) {

    MPI_Init( &argc , &argv);

    int rank,nprocs;

    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD , &nprocs);

    int block_size; //elementos a procesar por cada rank
    int padding; //real_size-elementos
    int elementos; //procesar
    int real_size; //procesar + los que hagan falta

    

    MPI_Finalize();

    return 0;
}