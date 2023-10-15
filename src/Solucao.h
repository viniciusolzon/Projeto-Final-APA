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
        vector<vector<int>> rotas; // vector de vector de rotas, cada vector vai conter uma rota
        vector<int> capacidadeRotas; // vector que guarda a capacidade das rotas, vc pega pelo índice
        vector<Cliente*> clientes;
        // vector<Cliente*> clientes_terceirizados;
        vector<int> clientes_terceirizados;
        int num_clientesTerceirizados;
    public:
        Solucao(int capacidades, vector<int> demandas, vector<int> custos_terceirizacao, int k);
        Solucao(){}
        ~Solucao(){}

        int get_custo(){return custo;}
        void atualiza_custo(int valor){this->custo = valor;}

        vector<vector<int>> get_rotas(){return rotas;}
        int get_capacidadeRota(int rota){return capacidadeRotas[rota];}
        void atualizaCapacidade(int rota, int capacidade){capacidadeRotas[rota] = capacidade;}
        void pushBack(int rota, int vertice);
        void atualizaRota(int rota, int index, int novo_vertice);
        void insereNaRota(int rota, int index, int vertice);
        void removeDaRota(int rota, int index);

        vector<Cliente*> get_clientes(){return clientes;}

        vector<int> get_clientes_terceirizados(){return clientes_terceirizados;}
        void terceirizaCliente(int cliente){this->clientes_terceirizados.push_back(cliente);}
        void desterceirizaCliente(int cliente_index){this->clientes_terceirizados.erase(this->clientes_terceirizados.begin() + cliente_index);}
        int get_num_clientesTerceirizados(){return num_clientesTerceirizados;}
        void atualiza_num_clientesTerceirizados(int val){this->num_clientesTerceirizados = val;}

        void info();
};

#endif