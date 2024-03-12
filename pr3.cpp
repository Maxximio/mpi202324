#include<mpi.h>

#include<vector>
#include<cmath>
#include<stdio.h>

#define WIDTH 720
#define HEIGHT 480

std::vector<int> generar_imagen(){
    
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

    std::vector<int> imagen, imagen_local, imagen_res;

    if(rank==0){
        imagen= generar_imagen();

        elementos=imagen.size();
        
        real_size=std::ceil((double)elementos/nprocs)*nprocs;
        padding=real_size-elementos;
        block_size = real_size/ nprocs;

        imagen_res.resize(real_size);

        MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
    
        MPI_Send( &padding , 1 , MPI_INT , nprocs-1 , 0 , MPI_COMM_WORLD);

    }else{

        MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);

        if(rank==nprocs-1){
            MPI_Recv( &padding , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        }

    }

    imagen_local.resize(block_size);

    MPI_Scatter( imagen.data() , block_size , MPI_INT , imagen_local.data() , block_size , MPI_INT , 0 , MPI_COMM_WORLD);//envia el valor a pedazos a todos y al rank 0 tambien

    //procesar la imagen 
    int block_size_tmp = rank == nprocs-1 ? block_size - padding: block_size;

    for(int i =0;i<block_size_tmp;i++){
        if(i % 4 == 0){
            imagen_local[i]=0;
        }
    }

    //unir el resultado
    MPI_Gather( imagen_local.data() , block_size , MPI_INT , imagen_res.data() , block_size , MPI_INT , 0 , MPI_COMM_WORLD);

    imagen_res.resize(elementos);

    if(rank==0){
        for(int i=0;i<16;i++){
            std::printf("%d, ", imagen_res[i]);
        }
    }

    MPI_Finalize();

    return 0;
}