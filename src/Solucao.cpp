#include "Solucao.h"

Solucao::Solucao(int capacidades, vector<int> demandas, vector<int> custos_terceirizacao, int k){

    this->custo = 0;
    // Inicializa os clientes
    for(int i = 0; i < demandas.size(); i++){
        Cliente *novo_cliente = new Cliente(demandas[i], custos_terceirizacao[i]);
        this->clientes.push_back(novo_cliente);
    }

    // Inicializa as rotas
    vector<vector<int>> rotas_zeradas(k);
    vector<int> capacidades_iniciais(k);
    this->rotas = rotas_zeradas;
    this->capacidadeRotas = capacidades_iniciais;

    // Coloca o 0 no começo pq ele sai do depósito
    int capacidade_veiculos = capacidades;
    for(int i = 0; i < rotas_zeradas.size(); i++){
        this->rotas[i].push_back(0);
        this->atualizaCapacidade(i, capacidade_veiculos);
    }
}


void Solucao::pushBack(int rota, int vertice){
    this->rotas[rota].push_back(vertice);
}

void Solucao::atualizaRota(int rota, int index, int novo_vertice){
    this->rotas[rota][index] = novo_vertice;
}

void Solucao::insereNaRota(int rota, int index, int vertice){
    this->rotas[rota].insert(this->rotas[rota].begin() + index+1, vertice);
}

void Solucao::removeDaRota(int rota, int index){
    this->rotas[rota].erase(this->rotas[rota].begin() + index);
}


void Solucao::info(){
    // Rotas
    for (int i = 0; i < this->rotas.size(); i++){
        cout << "Rota do veiculo " << i+1 << ": ";
        for (int j = 0; j < this->rotas[i].size(); j++){
            cout << this->rotas[i][j] << " ";
        }
        cout << "\n";
    }

    // Clientes
    int todos_atendidos = true;
    for(int i = 0; i < this->clientes.size(); i++){
        // cout << "Cliente " << i+1;
        // cout << "   Demanda = " << clientes[i]->get_demanda() << "\n";
        // cout << "   Custo de Terceirizacao = " << clientes[i]->get_custoTerceirizacao() << "\n";
        if(!clientes[i]->get_atendido())
            todos_atendidos = false;
    }

    if(todos_atendidos){
        cout << "Todos clientes foram atendidos" << endl;
    }
    else{
        for(int i = 0; i < this->clientes.size(); i++){
            if(!clientes[i]->get_atendido())
                cout << "Cliente " << i+1 << " nao foi atendido" << endl;
        }
    }
    
    cout << "\nCusto:  " << setprecision(2) << fixed << custo << endl;
}

