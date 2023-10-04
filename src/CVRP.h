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
        Solucao Construcao(Data *d); // A heurística construtiva utilizada será a do 'vizinho mais pŕoximo'
        int vizinhoMaisProximo(int atual, Solucao *solucao, Data *d, int capacidadeAtual);
        int calcularCapacidadeUsada(vector<int> &rota, vector<Cliente*> clientes);
        void calcularCustoTotal(Solucao *s, Data d);

        // Funções para a busca local
        double epsilon(double a, double b);
        bool melhorou(double valor1, double valor2);
        void BuscaLocal(Solucao *s, Data *d);
        int calculaCustoSwap(Solucao *s, Data *d, int rota, int cliente1, int cliente2);
        bool melhorouSwap(Solucao *s, Data *d);

        int calculaCustoShift(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2);
        bool melhorouShift(Solucao *s, Data *d);

        void solve();
};

#endif