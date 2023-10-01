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
    // cout << "ANTES DO problema.solve();\n";
    problema.solve();
    problema.exibe_solucao();
    // cout << "DEPOIS DO problema.solve();\n";

}
