#include "Solucao.h"

Solucao::Solucao(int capacidade, vector<int> demandas, vector<int> custos_terceirizacao, int qtd_veiculos){

    // Inicializa custo
    this->custo = 0;

    // Inicializa clientes terceirizados e não terceirizados
    this->num_clientes_terceirizados = 0;
    this->num_clientes = 0;

    // Inicializa as rotas
    vector<vector<int>> rotas_zeradas(qtd_veiculos);
    vector<int> capacidades_iniciais(qtd_veiculos);
    this->rotas = rotas_zeradas;
    this->capacidadeRotas = capacidades_iniciais;

    // Coloca o 0 no começo pq ele sai do depósito
    int capacidade_veiculos = capacidade;
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

    cout << "\nCusto:  " << custo << endl;

    // Rotas
    for (int i = 0; i < this->rotas.size(); i++){
        if(this->rotas[i].size() <=2)
            continue;
        cout << "Rota " << i+1 << ": ";
        for (int j = 0; j < this->rotas[i].size(); j++){
            cout << this->rotas[i][j] << " ";
        }
        cout << "\n";
    }

    // Clientes
    if(this->num_clientes_terceirizados == 0){
        cout << "Nenhum cliente foi terceirizado\n";
    }
    else{
        // cout << "num_clientesTerceirizados = " << this->num_clientesTerceirizados << "\n";
        cout << "Terceirizados: ";
        for(int i = 0; i < this->clientes_terceirizados.size(); i++)
            cout << this->clientes_terceirizados[i] << " ";
        cout << "\n";
    }

    // if(this->num_clientes_terceirizados + this->num_clientes == dados->get_n()){
    //     cout << "Todos clientes foram atendidos\n\n";
    // }
    // else{
    //     cout << "Nem todos clientes foram atendidos\n\n";
    // }
}
