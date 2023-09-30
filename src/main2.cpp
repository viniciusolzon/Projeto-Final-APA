#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>

#include "CVRP.cpp"
#include "Data.cpp"
// #include "Cliente.cpp"

using namespace std;


int main(int argc, char *argv[]){

    Data dados;
    dados.readData(argc, argv);
    dados.info();

    CVRP solucao(dados);
    // solucao.solve();

}
