#ifndef CVRP_H
#define CVRP_H

#include "Data.h"
#include <chrono>

class Solucao{
    private:
        vector<vector<int>> rotas; // vector de vector de rotas, cada vector vai conter uma rota
        double custo; // custo total da solucao
   
    public:
        void info();
};

// Capacitaded Vehicle Routing Problem
class CVRP{
    private:
        // int maxIterILS; // caso decidirmos utilizar a meta-heurística ILS (Iterated Local Search), tenho ela fácil implementada já
        int maxIter;
        Data dados;
        Solucao melhor_solucao;

    public:
        CVRP(Data dados){this->dados = dados;} // Construtor da classe
        ~CVRP(){} // Destrutor da classe

        Solucao construcao(Data *d); // A heurística construtiva utilizada será a do 'Vizinho mais pŕoximo'
        int vizinhoMaisProximo();

        void solve();
        void show_solution();
};

#endif