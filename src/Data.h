#ifndef DATA_H
#define DATA_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <cmath>

using namespace std;

class Data{
    private:
        string nome_instancia;
        int n, k, Q, L, r;
        vector<int> demandas;
        vector<int> custos_terceirizacao;
        int **matrizAdj; // matriz de adjacencia da instancia (não é simétrica)
        int num_vertices; // qtd de vertices na instancia

    public:
        string get_nome_instancia(){return nome_instancia;}

        int get_n(){return n;} // Número de entregas
        int get_k(){return k;} // Quantidade de veículos
        int get_Q(){return Q;} // Capacidade de cada veículo
        int get_L(){return L;} // Pelo menos L entregas devem ser feitas sem terceirização
        int get_r(){return r;} // Custo de uso de cada veículo
        vector<int> get_demandas(){return demandas;}
        vector<int> get_custos_terceirizacao(){return custos_terceirizacao;}
        int get_custo(int i, int j){return matrizAdj[i][j];}
        
        void readData(string filename);
        void info();
        
};

#endif