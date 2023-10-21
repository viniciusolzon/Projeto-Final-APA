#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>

#include "CVRP.h"

using namespace std;

int main(int argc, char *argv[]){

    Data dados;
    dados.readData(argc, argv);

    CVRP problema(dados);
    if(argc == 2)
        problema.solve();
    else
        problema.solveILS();
        
    problema.get_solution().info();

}
