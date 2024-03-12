#include<mpi.h>

#include<vector>
#include<cmath>
#include<stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank==0){
        //leer imagen en formato RGB
        int width, height, channels;

        uint8_t* imagen =stbi_load("imgjpgg", &width, &height, &channels, STBI_rgb);

        int elementos = width*height*channels;
        int block_size;
        int padding;
        int real_size;

        

        MPI_Bcast(&imagen, 1, MPI_Datatype datatype, int root, MPI_Comm comm);

    }

    MPI_Finalize();
    return 0;
}