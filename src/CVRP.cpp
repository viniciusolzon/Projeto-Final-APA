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
    cout << endl;
    BuscaLocal(&melhor_de_todas, &dados); // Tenta melhorar o máximo possível a solução
    cout << endl;
    cout << "Saiu da busca local";
    cout << endl;
    cout << endl;
    melhor_de_todas.info();

    // cout << endl;
    // cout << endl;
    // cout << "Entrou na perturbacao";
    // cout << endl;
    // cout << endl;
    // Solucao melhor_de_todas_teste(dados.get_Q(), dados.get_demandas(), dados.get_custos_terceirizacao(), dados.get_k()); // Cria a solução que guardará a melhor solução possível
    // melhor_de_todas_teste = Perturbacao(&melhor_de_todas, &dados); // Tenta melhorar o máximo possível a solução
    // cout << endl;
    // cout << "Saiu da perturbacao";
    // cout << endl;
    // cout << endl;
    // melhor_de_todas_teste.info();

    // cout << endl;
    // cout << endl;
    // cout << "Entrou na busca local";
    // cout << endl;
    // cout << endl;
    // BuscaLocal(&melhor_de_todas_teste, &dados); // Tenta melhorar o máximo possível a solução
    // cout << endl;
    // cout << "Saiu da busca local";
    // cout << endl;
    // cout << endl;
    // melhor_de_todas_teste.info();

    auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nTempo de execucao:  " << float_ms.count() / 1000.0000000000000 << " segundos" << "\n";

    // Atribui a solução gerada aqui em cima ("melhor_de_todas") a Solucao ("melhor_solucao") da classe CVRP
    melhor_solucao = melhor_de_todas;
}


void CVRP::solveILS(){
    auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro
    srand(time(NULL)); // Para conseguir gerar números aleatórios
    Solucao melhor_de_todas(dados.get_Q(), dados.get_demandas(), dados.get_custos_terceirizacao(), dados.get_k()); // Cria a solução que guardará a melhor solução possível


    for(int i = 0; i < maxIter; i++){
        Solucao current = Construcao(&dados);
        
        Solucao best = current;
        if(i == 0)
            melhor_de_todas = current;

        int iterIls = 0;
        while(iterIls <= maxIterILS){
            BuscaLocal(&current, &dados);
            if(melhorou(best.get_custo(), current.get_custo())){
                // cout << "Solucao melhorou de " << best.get_custo() << " para " << current.get_custo() << endl;
                best = current;
                iterIls = 0;
            }
            current = Perturbacao(&best, &dados);
            iterIls++;
        }

        if(melhorou(melhor_de_todas.get_custo(), best.get_custo()))
            melhor_de_todas = best;
    }

    auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nTempo de execucao:  " << float_ms.count() / 1000.0000000000000 << " segundos" << "\n";

    melhor_solucao = melhor_de_todas;
    calculaTudo();
}

void CVRP::calculaTudo(){
    int custo_total = 0;
    // Rotas
    for (int i = 0; i < this->get_solution().get_rotas().size(); i++){
        if(this->get_solution().get_rotas()[i].size() >2)
            custo_total+=dados.get_r();
        
        for (int j = 0; j < this->get_solution().get_rotas()[i].size()-1; j++){
            custo_total+= dados.get_custo(this->get_solution().get_rotas()[i][j], this->get_solution().get_rotas()[i][j+1]);
        }
    }

    for (int i = 0; i < this->get_solution().get_clientes_terceirizados().size(); i++){
        // cout << "Cliente " << this->get_solution().get_clientes_terceirizados()[i] << endl;
        // cout << "Custo de terceirizacao: " << dados.get_custos_terceirizacao()[this->get_solution().get_clientes_terceirizados()[i]-1] << endl;
        custo_total+= dados.get_custos_terceirizacao()[this->get_solution().get_clientes_terceirizados()[i]-1];
    }

    cout << "CUSTO TOTAL RECALCULADO: " << custo_total << endl;
}
