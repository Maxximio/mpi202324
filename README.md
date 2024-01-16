"# mpi202324" 

pasos para ejecutar:

sudo apt-get update
sudo apt-get install libopenmpi-dev

mpiexec --version

compilar:
mpicxx ejemplo02.cpp -o ejemplo02

ejecutar:
mpiexec -n 4 ejemplo02