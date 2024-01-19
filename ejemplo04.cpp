#include<iostream>
#include<mpi.h>

int sumar(int* tmp, int n){
    int suma=0;
    for(int i=0;i<n;i++){
          suma+=tmp[i];
    }
    return suma;
}

int main(int argc, char ** argv){

    MPI_Init(&argc,&argv);
    int rank, nprocs;
 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   
    int num_datos = 200;  // Valor predeterminado
    if (argc > 1) {
        num_datos = std::atoi(argv[1]);
    }

    int* data = new int[num_datos];
   
    if(rank == 0){
        std::printf("Total de ranks: %d\n", nprocs);
        std::printf("Total de datos: %d\n", num_datos);
 
        for(int i=0;i<num_datos;i++){
            data[i]=i;
        }

        int* suma_ranks = new int[nprocs];
        suma_ranks[0] = sumar(data, num_datos);
        
        for(int rank_id=1;rank_id<nprocs; rank_id++){
            std::printf("rank 0 enviando datos a rank %d\n",rank_id);  
            int start = rank_id * (num_datos / nprocs);
            int chunk_size = (rank_id == nprocs - 1) ? (num_datos - start) : (num_datos / nprocs);
            MPI_Send(&data[start],chunk_size, MPI_INT,rank_id,0,MPI_COMM_WORLD ); 
        }

        for(int rank_id=1; rank_id<nprocs;rank_id++){
            MPI_Recv(&suma_ranks[rank_id],1,MPI_INT,rank_id,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

        std::printf("sumas parciales:");
        for (int i = 0; i < nprocs; i++) {
            std::printf(" %d", suma_ranks[i]);
        }
        std::printf("\n");
        
        int suma_total = sumar(suma_ranks, nprocs);
        std::printf("SUMA TOTAL: %d\n", suma_total);

        delete[] suma_ranks;
    
    }else {

        int chunk_size = (rank == nprocs - 1) ? (num_datos - (rank * (num_datos / nprocs))) : (num_datos / nprocs);
        std::printf("rank %d Recibiendo datos\n ",rank);
        
        MPI_Recv(data, chunk_size, MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        int suma_parcial = sumar(data,chunk_size);
        std::printf("rank %d enviando suma parcial %d\n",rank,suma_parcial);
        MPI_Send(&suma_parcial,1,MPI_INT,0,0,MPI_COMM_WORLD);

    }
 
    delete[] data;
    MPI_Finalize();
 
    return 0;
}