#include <mpi.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

std::vector<int> read_file() {
    std::fstream fs("C:/Users/moise/Downloads/datos.txt", std::ios::in);
    std::string line;
    std::vector<int> ret;
    while (std::getline(fs, line)) {
        ret.push_back(std::stoi(line));
    }
    fs.close();
    return ret;
}

void frecuencias (std::vector<int> datos,std::vector<int>&contador){
    for(int i=0;i<datos.size();i++){
        contador[datos[i]]=contador[i]+1;
    }
}

int main(int argc, char** argv) {

    std::vector<int> datos=read_file();

    std::vector<int> contador(101,0);

    //frecuencias(datos,contador);

    /*for(int i=0;i<101;i++){
        std::cout << "número: " << i << " -> " << contador[i] << std::endl;
    }*/

    for(int i=0;i<10;i++){
        std::cout << "número: " << datos[i] << std::endl;
    }

    return 0;

}