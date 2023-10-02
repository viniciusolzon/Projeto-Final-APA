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
        double custo; // custo total da solucao
        vector<Cliente*> clientes;
        vector<vector<int>> rotas; // vector de vector de rotas, cada vector vai conter uma rota
    public:
        Solucao(vector<int> demandas, vector<int> custos_terceirizacao, int k);
        Solucao(){}
        ~Solucao(){}

        vector<Cliente*> get_clientes(){return clientes;}
        vector<vector<int>> get_rotas(){return rotas;}
        void insereNaRota(int rota, int vertice);
        void atualizaRota(int rota, int index, int vertice);

        int get_custo(){return custo;}
        void atualiza_custo(int valor){this->custo = valor;}

        void info();
};

#endif