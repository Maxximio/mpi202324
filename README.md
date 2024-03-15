"# mpi202324" 

pasos para ejecutar:

sudo apt-get update
sudo apt-get install libopenmpi-dev

mpiexec --version

compilar:
mpicxx fim.cpp -o fim

ejecutar:
mpiexec -n 4 fim

mpiexec -n 4 ./fim

///////////////////////////////////////////Funciones MPI/////////////////////////////////////////////////

MPI_Init:
MPI_Init(int *argc, char ***argv);

MPI_Finalize:
MPI_Finalize();

int rank, nprocs;

MPI_Comm_size:
MPI_Comm_size(MPI_Comm comm, int *size);

MPI_Comm_rank:
MPI_Comm_rank(MPI_Comm comm, int *nprocs);

MPI_Send:
MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);

MPI_Recv:
MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);

MPI_Barrier:
MPI_Barrier(MPI_Comm comm);

MPI_Bcast:
MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);

MPI_Reduce:
MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);

MPI_Scatter:
MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);

MPI_Gather:
MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);

MPI_Allreduce:
MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);

MPI_Allgather:
MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);