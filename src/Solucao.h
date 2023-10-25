#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

class Solucao{
    private:
        double custo; // custo total da solucao
        vector<vector<int>> rotas; // vector de vector de rotas, cada vector vai conter uma rota
        vector<int> capacidadeRotas; // vector que guarda a capacidade das rotas, vc pega pelo índice
        int num_veiculos_utilizados;

        int num_clientes;
        vector<int> clientes_terceirizados;
        int num_clientes_terceirizados;
    public:
        int valorConstrucao;
        double tempoConstrucao;
        int valorRVND;
        double tempoRVND;

        double tempoILS;

        Solucao(int capacidades, vector<int> demandas, vector<int> custos_terceirizacao, int qtd_veiculos);
        Solucao(){}
        ~Solucao(){}

        // Custo
        int get_custo(){return custo;}
        void atualiza_custo(int valor){this->custo = valor;}

        // Rotas
        vector<vector<int>> get_rotas(){return rotas;}
        int get_capacidadeRota(int rota){return capacidadeRotas[rota];}
        void atualizaCapacidade(int rota, int capacidade){capacidadeRotas[rota] = capacidade;}
        void pushBack(int rota, int vertice);
        void atualizaRota(int rota, int index, int novo_vertice);
        void insereNaRota(int rota, int index, int vertice);
        void removeDaRota(int rota, int index);
        void inverteRota(int rota, int inicio, int fim){reverse(this->rotas[rota].begin() + inicio, this->rotas[rota].begin() + fim + 1);}

        // Clientes
        int get_num_clientes(){return num_clientes;}
        void mais_um_cliente(){this->num_clientes+=1;}
        void menos_um_cliente(){this->num_clientes-=1;}

        // Clientes terceirizados
        vector<int> get_clientes_terceirizados(){return clientes_terceirizados;}
        int get_num_clientesTerceirizados(){return num_clientes_terceirizados;}
        void atualiza_num_clientesTerceirizados(int val){this->num_clientes_terceirizados = val;}
        void terceirizaCliente(int cliente){
            this->clientes_terceirizados.push_back(cliente);
            this->num_clientes_terceirizados+=1;
        }
        void desterceirizaCliente(int cliente_index){
                this->clientes_terceirizados.erase(this->clientes_terceirizados.begin() + cliente_index);
                this->num_clientes_terceirizados-=1;
        }

        // Informações da solução
        void info();
};

#endif