#include "CVRP.h"

void Solucao::info(){
    for (int i = 0; i < this->rotas.size(); i++){
        cout << "Rota " << i+1 << ": ";
        for (int j = 0; j < this->rotas[i].size(); j++){
            cout << this->rotas[i][j] << " ";
        }
        cout << endl;
    }
}

void CVRP::solve(){

    auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro
    srand(time(NULL)); // Para conseguir gerar números aleatórios
    Solucao melhor_de_todas; // Cria a solução que guardará a melhor solução possível

    //melhor_de_todas = construcao(&dist); // Cria a solução atual

    melhor_de_todas.info(); // Mostra as informações da solução, como rotas, custo, etc.
}


Solucao CVRP::construcao(Data *d){
    Solucao atual; // cria uma solucao vazia

    return atual;
}

int CVRP::vizinhoMaisProximo(){
    int mais_proximo = 0;

    return mais_proximo;
}
