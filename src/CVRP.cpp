#include "CVRP.h"

void CVRP::solve(){
    auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro

    srand(time(NULL)); // Para conseguir gerar números aleatórios
    Solucao melhor_de_todas(dados.get_demandas(), dados.get_custos_terceirizacao(), dados.get_k()); // Cria a solução que guardará a melhor solução possível

    // cout << "ANTES DA CONSTRUCAO\n";
    melhor_de_todas = Construcao(&dados); // Cria a solução atual
    // cout << "DEPOIS DA CONSTRUCAO\n";
    melhor_de_todas.info(); // Mostra as informações da solução, como rotas, custo, etc.

    auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nExecution time:  " << float_ms.count() / 1000.0000000000000 << " seconds" << "\n";
}

void CVRP::exibe_solucao(){
    cout << "\nSolucao final:\n";
    // Rotas
    for (int i = 0; i < this->melhor_solucao.get_rotas().size(); i++){
        cout << "Rota " << i+1 << ": ";
        for (int j = 0; j < this->melhor_solucao.get_rotas()[i].size(); j++){
            cout << this->melhor_solucao.get_rotas()[i][j] << " ";
        }
        cout << "\n\n";
    }

    // Clientes
    for(int i = 0; i < this->melhor_solucao.get_clientes().size(); i++){
        cout << "Cliente " << i+1 << ":\n";
        cout << "   Demanda = " << this->melhor_solucao.get_clientes()[i].get_demanda() << "\n";
        cout << "   Custo de Terceirizacao = " << this->melhor_solucao.get_clientes()[i].get_custoTerceirizacao() << "\n";
        if(this->melhor_solucao.get_clientes()[i].get_atendido())
            cout << "   Foi atendido\n";
        else
            cout << "   Não foi atendido\n";
    }

    cout << "Custo:  " << setprecision(2) << fixed << this->melhor_solucao.get_custo() << "\n";
}
