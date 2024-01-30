"# mpi202324" 

pasos para ejecutar:

sudo apt-get update
sudo apt-get install libopenmpi-dev

mpiexec --version

compilar:
mpicxx ejemplo02.cpp -o ejemplo02

ejecutar:
mpiexec -n 4 ejemplo02

///////////////////////////////////////////Funciones MPI/////////////////////////////////////////////////

MPI_Init:
int MPI_Init(int *argc, char ***argv);

MPI_Finalize:
int MPI_Finalize();

MPI_Comm_size:
int MPI_Comm_size(MPI_Comm comm, int *size);

MPI_Comm_rank:
int MPI_Comm_rank(MPI_Comm comm, int *rank);

MPI_Send:
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);

MPI_Recv:
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);

MPI_Barrier:
int MPI_Barrier(MPI_Comm comm);

MPI_Bcast:
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);

MPI_Reduce:
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);

MPI_Scatter:
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);

MPI_Gather:
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);

MPI_Allreduce:
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);

MPI_Allgather:
int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);