#ifndef CVRP_H
#define CVRP_H

#include "Data.h"
#include "Solucao.h"

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

        // Funções para a heurística construtiva da solução
        Solucao Construcao(Data *dados); // A heurística construtiva utilizada será a do 'Vizinho mais pŕoximo'
        int vizinhoMaisProximo(int atual, Solucao *solucao, Data *dados, int capacidadeAtual);
        int calcularCapacidadeUsada(vector<int> &rota, vector<Cliente*> clientes);
        void calcularCustoTotal(Solucao *s, Data d);

        // Funções para a busca local
        // void BuscaLocal(Data *dados);

        void solve();
        void exibe_solucao();
};

#endif