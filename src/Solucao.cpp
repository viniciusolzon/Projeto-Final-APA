#include "Solucao.h"

Solucao :: Solucao(vector<int> demandas, vector<int> custos_terceirizacao, int k){
    int INT_MAX = std::numeric_limits<int>::max();

    this->custo = INT_MAX;
    // Inicializa os clientes
    for(int i = 0; i < demandas.size(); i++){
        Cliente novo_cliente(demandas[i], custos_terceirizacao[i]);
        clientes.push_back(novo_cliente);
    }

    // Inicializa as rotas
    vector<vector<int>> rotas_zeradas(k);
    this->rotas = rotas_zeradas;
}

void Solucao::info(){
    // Rotas
    for (int i = 0; i < this->rotas.size(); i++){
        cout << "Rota " << i+1 << ": ";
        for (int j = 0; j < this->rotas[i].size(); j++){
            cout << this->rotas[i][j] << " ";
        }
        cout << "\n\n";
    }

    // Clientes
    for(int i = 0; i < this->clientes.size(); i++){
        cout << "Cliente " << i+1 << ":\n";
        cout << "   Demanda = " << clientes[i].get_demanda() << "\n";
        cout << "   Custo de Terceirizacao = " << clientes[i].get_custoTerceirizacao() << "\n";
        if(clientes[i].get_atendido())
            cout << "   Foi atendido\n";
        else
            cout << "   Não foi atendido\n";
    }
    cout << "Custo:  " << setprecision(2) << fixed << custo << "\n";
}

