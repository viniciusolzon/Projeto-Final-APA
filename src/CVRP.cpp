#include "CVRP.h"

void CVRP::solve(){
    auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro

    srand(time(NULL)); // Para conseguir gerar números aleatórios
    Solucao melhor_de_todas(dados.get_Q(), dados.get_demandas(), dados.get_custos_terceirizacao(), dados.get_k()); // Cria a solução que guardará a melhor solução possível

    cout << endl;
    cout << endl;
    cout << "Entrou na construcao";
    cout << endl;
    cout << endl;
    melhor_de_todas = Construcao(&dados); // Cria a solução atual
    cout << endl;
    cout << "Saiu da construcao";
    cout << endl;
    cout << endl;
    melhor_de_todas.info();

    cout << endl;
    cout << endl;
    cout << "Entrou na busca local";
    cout << endl;
    cout << endl;BuscaLocal(&melhor_de_todas, &dados); // Tenta melhorar o máximo possível a solução
    cout << endl;
    cout << "Saiu da busca local";
    cout << endl;
    cout << endl;
    melhor_de_todas.info();

    auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nTempo de execucao:  " << float_ms.count() / 1000.0000000000000 << " segundos" << "\n";

    // Atribui a solução gerada aqui em cima ("melhor_de_todas") a Solucao ("melhor_solucao") da classe CVRP
    melhor_solucao = melhor_de_todas;
}

