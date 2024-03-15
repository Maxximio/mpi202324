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

    uint8_t *imagen, *imagen_local,*rojo_local,*verde_local,*azul_local,*result_rojo,*result_verde,*result_azul;

    int width=6016;
    int height=4016;
    int channels=3;

    if(rank==0){
        //leer imagen en formato RGB

        imagen =stbi_load("image01.jpg", &width, &height, &channels, STBI_rgb);

        elementos = width*height*channels;

        real_size=std::ceil((double)elementos/nprocs)*nprocs;
        padding=real_size-elementos;
        block_size = real_size/ nprocs;

        result_rojo = new uint8_t [real_size];//para reescalar
        result_verde = new uint8_t [real_size];
        result_azul = new uint8_t [real_size];
        
        MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
    
        MPI_Send( &padding , 1 , MPI_INT , nprocs-1 , 0 , MPI_COMM_WORLD);

    }else{
        MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);

        if(rank==nprocs-1){
            MPI_Recv( &padding , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        }
    }


    rojo_local =new uint8_t [block_size];
    verde_local =new uint8_t [block_size];
    azul_local =new uint8_t [block_size];
    imagen_local=new uint8_t [block_size];

    MPI_Scatter( imagen , block_size , MPI_UINT8_T , imagen_local , block_size , MPI_UINT8_T , 0 , MPI_COMM_WORLD);

    int block_size_tmp = rank == nprocs-1 ? block_size - padding: block_size;

    for(int i =0;i<block_size_tmp;i++){

        int canal = i % 3;

        if (canal == 0) {//rojo
            rojo_local[i] = imagen_local[i];
            verde_local[i] =0;
            azul_local[i]=0;
        } else if (canal == 1) {//verde
            rojo_local[i]=0;
            verde_local[i]= imagen_local[i];
            azul_local[i]=0;
        } else if (canal == 2) {//azul
            rojo_local[i]=0;
            verde_local[i] =0;
            azul_local[i]= imagen_local[i];
        }

    }

    result_rojo =new uint8_t [block_size];
    result_verde =new uint8_t [block_size];
    result_azul =new uint8_t [block_size];

    //unir el resultado
    MPI_Gather( rojo_local , block_size , MPI_UINT8_T , result_rojo , block_size , MPI_UINT8_T , 0 , MPI_COMM_WORLD);
    MPI_Gather( verde_local , block_size , MPI_UINT8_T , result_verde , block_size , MPI_UINT8_T , 0 , MPI_COMM_WORLD);
    MPI_Gather( azul_local , block_size , MPI_UINT8_T , result_azul , block_size , MPI_UINT8_T , 0 , MPI_COMM_WORLD);

    if(rank==0){
        stbi_write_png( "result_rojo.png", width, height,STBI_grey, result_rojo, width );
        stbi_write_png( "result_verde.png", width, height,STBI_grey, result_verde, width );
        stbi_write_png( "result_azul.png", width, height,STBI_grey, result_azul, width );
    }

    MPI_Finalize();
    return 0;
}