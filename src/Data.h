#ifndef DATA_H
#define DATA_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

class Data{
    private:
        int n, k, Q, L, r;
        vector<int> demandas;
        vector<int> custos_terceirizacao;
        int **matrizAdj; // matriz de adjacencia da instancia (não é simétrica)
        // vector<vector<int>> matrizAdj;
        int num_vertices; // qtd de vertices na instancia

    public:
        int get_n(){return n;}
        int get_k(){return k;}
        int get_Q(){return Q;}
        int get_L(){return L;}
        int get_r(){return r;}
        vector<int> get_demandas(){return demandas;}
        vector<int> get_custos_terceirizacao(){return custos_terceirizacao;}
        int **get_matrizAdj(){return matrizAdj;}
        int get_custo(int i, int j){return matrizAdj[i][j];}
        
        void readData(int argc, char **argv);
        void info();
        
};

#endif