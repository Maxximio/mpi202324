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

    int elementos;
    int block_size;
    int padding;
    int real_size;

    uint8_t *imagen,*imagen_local,*resultado;

    int width=6016;
    int height=4016;
    int channels=3;

    if(rank==0){
        //leer imagen en formato RGB

        imagen =stbi_load("imgjpgg", &width, &height, &channels, STBI_rgb);

        elementos = width*height*channels;

        real_size=std::ceil((double)elementos/nprocs)*nprocs;
        padding=real_size-elementos;
        block_size = real_size/ nprocs;

        resultado = new uint8_t [real_size];//para reescalar
        
        MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
    
        MPI_Send( &padding , 1 , MPI_INT , nprocs-1 , 0 , MPI_COMM_WORLD);

    }else{
        MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);

        if(rank==nprocs-1){
            MPI_Recv( &padding , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        }
    }

    imagen_local =new uint8_t [block_size];

    MPI_Scatter( imagen , block_size , MPI_UINT8_T , imagen_local , block_size , MPI_UINT8_T , 0 , MPI_COMM_WORLD);

    int block_size_tmp = rank == nprocs-1 ? block_size - padding: block_size;

    for(int i =0;i<block_size_tmp;i++){
        if(i % 3 == 2){//B
            imagen_local[i]=imagen_local[i]*0.7;
        }else if(i % 3 == 1){//G
            imagen_local[i]=imagen_local[i]*0.72;
        }else if(i %3 == 0){//R
             imagen_local[i]=imagen_local[i]*0.21;
        }
    }

    resultado =new uint8_t [block_size];

    //unir el resultado
    MPI_Gather( imagen_local , block_size , MPI_UINT8_T , resultado , block_size , MPI_UINT8_T , 0 , MPI_COMM_WORLD);

    if(rank==0){
        for(int i=0;i<16;i++){
            std::printf("%d, ", resultado[i]);
        }
    }

    stbi_write_png( "img-gris.png", width, height,STBI_grey, resultado, width );

    MPI_Finalize();
    return 0;
}