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
        vector<int> capacidadeRotas; // vector que guarda a capacidade das rotas, vc pega pelo índice
    public:
        Solucao(int capacidades, vector<int> demandas, vector<int> custos_terceirizacao, int k);
        Solucao(){}
        ~Solucao(){}

        vector<Cliente*> get_clientes(){return clientes;}
        vector<vector<int>> get_rotas(){return rotas;}
        void pushBack(int rota, int vertice);
        void atualizaRota(int rota, int index, int novo_vertice);
        void insereNaRota(int rota, int index, int vertice);
        void removeDaRota(int rota, int index);

        int get_capacidadeRota(int rota){return capacidadeRotas[rota];}
        void atualizaCapacidade(int rota, int capacidade){capacidadeRotas[rota] = capacidade;}

        int get_custo(){return custo;}
        void atualiza_custo(int valor){this->custo = valor;}

        void info();
};

#endif