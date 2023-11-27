#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

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
        Solucao();
        ~Solucao();

        // Custo
        int get_custo();
        void atualiza_custo(int valor);

        // Rotas
        vector<vector<int>> get_rotas();
        int get_capacidadeRota(int rota);
        void atualizaCapacidade(int rota, int capacidade);
        void pushBack(int rota, int vertice);
        void atualizaRota(int rota, int index, int novo_vertice);
        void insereNaRota(int rota, int index, int vertice);
        void removeDaRota(int rota, int index);
        void inverteRota(int rota, int inicio, int fim);

        // Clientes atendidos
        int get_num_clientes();
        void mais_um_cliente();
        void menos_um_cliente();

        // Clientes terceirizados
        vector<int> get_clientes_terceirizados();
        int get_num_clientesTerceirizados();
        void atualiza_num_clientesTerceirizados(int val);
        void terceirizaCliente(int cliente);
        void desterceirizaCliente(int cliente_index);

        // Informações da solução
        void info();
};

#endif