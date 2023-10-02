#include "Solucao.h"

Solucao :: Solucao(vector<int> demandas, vector<int> custos_terceirizacao, int k){
    int INT_MAX = std::numeric_limits<int>::max();

    this->custo = INT_MAX;
    // Inicializa os clientes
    for(int i = 0; i < demandas.size(); i++){
        Cliente *novo_cliente = new Cliente(demandas[i], custos_terceirizacao[i]);
        this->clientes.push_back(novo_cliente);
    }

    // Inicializa as rotas
    vector<vector<int>> rotas_zeradas(k);
    this->rotas = rotas_zeradas;

    // Coloca o 0 no começo pq ele sai do depósito
    for(int i = 0; i < rotas_zeradas.size(); i++){
        this->rotas[i].push_back(0);
    }
}

void Solucao :: insereNaRota(int rota, int vertice){
    this->rotas[rota].push_back(vertice);
}

void Solucao :: atualizaRota(int rota, int index, int novo_vertice){
    this->rotas[rota][index] = novo_vertice;
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
    for(int i = 0; i < this->clientes.size(); i++){
        cout << "Cliente " << i+1;
        // cout << "   Demanda = " << clientes[i]->get_demanda() << "\n";
        // cout << "   Custo de Terceirizacao = " << clientes[i]->get_custoTerceirizacao() << "\n";
        if(clientes[i]->get_atendido())
            cout << " foi atendido\n";
        else
            cout << " nao foi atendido\n";
    }
    cout << "\nCusto:  " << setprecision(2) << fixed << custo << "";
}

