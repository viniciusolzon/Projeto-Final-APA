#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>

#include "CVRP.h"

using namespace std;

int main(int argc, char *argv[]){

    Data dados;
    dados.readData(argc, argv);
    // dados.info();

    CVRP problema(dados);
    problema.solve();
    // problema.exibe_solucao();

}
