#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "Cliente.h"
#include <vector>
#include <limits>
#include <iostream>
#include <iomanip>

using namespace std;

class Solucao{
    private:
        vector<vector<int>> rotas; // vector de vector de rotas, cada vector vai conter uma rota
        vector<Cliente> clientes;
        double custo; // custo total da solucao
   
    public:
        Solucao(vector<int> demandas, vector<int> custos_terceirizacao, int k);
        Solucao(){}
        ~Solucao(){}

        vector<Cliente> get_clientes(){return clientes;}
        vector<vector<int>> get_rotas(){return rotas;}
        int get_custo(){return custo;}

        void info();
};

#endif