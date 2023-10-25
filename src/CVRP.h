#ifndef CVRP_H
#define CVRP_H

#include "Data.h"
#include "Solucao.h"

// Capacitaded Vehicle Routing Problem
class CVRP{
    private:
        int maxIter;
        int maxIterILS;
        Data dados;
        Solucao melhor_solucao;

    public:
        // Construtor da classe
        CVRP(Data dados){
            this->dados = dados;
            this->maxIter = 50;
            this->maxIterILS = dados.get_n() >= 150 ? dados.get_n() / 2 : dados.get_n();
        }
        ~CVRP(){} // Destrutor da classe

        Solucao get_solution(){return melhor_solucao;}

        // Funções para a construção
        Solucao Construcao(Data *d); // A heurística construtiva utilizada será a do 'vizinho mais pŕoximo'
        int vizinhoMaisProximo(int atual, Solucao *solucao, Data *d, int capacidadeAtual, vector<bool> clientes_atendidos);

        // Funções para a busca local
        double epsilon(double a, double b);
        bool melhorou(double valor1, double valor2);
        void BuscaLocal(Solucao *s, Data *d);

        // Reisertion mesma rota
        int calculaCustoReinsertionIntra(Solucao *s, Data *d, int rota, int cliente, int posicao);
        bool melhorouReinsertionIntra(Solucao *s, Data *d);

        // Reisertion rota diferente
        int calculaCustoReinsertionEntre(Solucao *s, Data *d, int rota_inicial, int rota_destino, int cliente, int posicao);
        bool verificaCapacidadeRota(Solucao *s, Data *d, int rota, int cliente_entrando);
        bool melhorouReinsertionEntre(Solucao *s, Data *d);

        // Swap
        int calculaCustoSwap(Solucao *s, Data *d, int rota, int cliente1, int cliente2);
        bool melhorouSwap(Solucao *s, Data *d);

        // 2-opt
        int calculaCusto2opt(Solucao *s, Data *d, int rota, int cliente1, int cliente2);
        bool melhorou2opt(Solucao *s, Data *d);
        
        // Shift
        int calculaCustoShift(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2);
        bool melhorouShift(Solucao *s, Data *d);
        bool verificaCapacidadeRotas(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2);

        // Terceirização
        bool verificaLimiteL(Solucao *s, Data *d);
        int calculaCustoTerceirizacao(Solucao *s, Data *d, int rota, int cliente);
        bool melhorouTerceirizacao(Solucao *s, Data *d);
        // Desterceirização
        bool verificaCapacidade(Solucao *s, Data *d, int rota, int cliente);
        int calculaCustoDesterceirizacao(Solucao *s, Data *d, int rota, int posicao, int cliente);
        bool melhorouDesterceirizacao(Solucao *s, Data *d);

        // Funções para a perturbação
        Solucao Perturbacao(Solucao *s, Data *d); // A heurística construtiva utilizada será a do 'vizinho mais pŕoximo'


        void solve();
        void solveILS();
        void gera_output();
        void calculaTudo(Solucao s);
};

#endif