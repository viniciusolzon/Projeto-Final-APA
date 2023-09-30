#ifndef DATA_H
#define DATA_H


#include <vector>
#include <iostream>
#include <fstream>
#include <string>

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
        void readData(int argc, char **argv);
        void info();
};

#endif