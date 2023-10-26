#include "CVRP.h"
#include <fstream>

void CVRP::calculaTudo(Solucao s){
    int custo_total = 0;
    for(int i = 0; i < s.get_rotas().size(); i++){
        if(s.get_rotas()[i].size() >2)
            custo_total+=dados.get_r();

        for(int j = 0; j < s.get_rotas()[i].size()-1; j++){
                custo_total+= dados.get_custo(s.get_rotas()[i][j], s.get_rotas()[i][j+1]);
            }
        }

    for (int i = 0; i < s.get_clientes_terceirizados().size(); i++)
        custo_total+= dados.get_custos_terceirizacao()[s.get_clientes_terceirizados()[i]-1];

    if(s.get_num_clientes() + s.get_num_clientesTerceirizados() != dados.get_n()){
        cout << "Nem todos clientes foram atendidos" << endl;
    }

    cout << "\nCUSTO TOTAL RECALCULADO: " << custo_total << endl;

}

void CVRP::solve(){

    srand(time(NULL)); // Para conseguir gerar números aleatórios
    Solucao melhor_de_todas(dados.get_Q(), dados.get_demandas(), dados.get_custos_terceirizacao(), dados.get_k()); // Cria a solução que guardará a melhor solução possível

    auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro

    melhor_de_todas = Construcao(&dados); // Cria a solução atual
    auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nTempo de execucao construcao:  " << float_ms.count() / 1000.0000000000000 << " segundos" << "\n";

    melhor_de_todas.valorConstrucao = melhor_de_todas.get_custo();
    melhor_de_todas.tempoConstrucao = float_ms.count() / 1000.0000000000000;


    start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro

    BuscaLocal(&melhor_de_todas, &dados); // Tenta melhorar o máximo possível a solução
    end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nTempo de execucao RVND:  " << float_ms.count() / 1000.0000000000000 << " segundos" << "\n";

    melhor_de_todas.valorRVND = melhor_de_todas.get_custo();
    melhor_de_todas.tempoRVND = float_ms.count() / 1000.0000000000000;


    // Atribui a solução gerada aqui em cima ("melhor_de_todas") a Solucao ("melhor_solucao") da classe CVRP
    melhor_solucao = melhor_de_todas;
    melhor_solucao.info();
}


void CVRP::solveILS(){
    auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro
    srand(time(NULL)); // Para conseguir gerar números aleatórios
    Solucao melhor_de_todas(dados.get_Q(), dados.get_demandas(), dados.get_custos_terceirizacao(), dados.get_k()); // Cria a solução que guardará a melhor solução possível

    for(int i = 0; i < maxIter; i++){
        
        Solucao atual = Construcao(&dados);

        Solucao melhor = atual;
        if(i == 0)
            melhor_de_todas = atual;

        int iterIls = 0;
        while(iterIls <= maxIterILS){
            BuscaLocal(&atual, &dados);
            if(melhorou(melhor.get_custo(), atual.get_custo())){
                melhor = atual;
                iterIls = 0;
            }
            atual = Perturbacao(&melhor, &dados);
            iterIls++;
        }
        if(melhorou(melhor_de_todas.get_custo(), melhor.get_custo()))
            // cout << "Solucao melhorou de " << melhor_de_todas.get_custo() << " para " << melhor.get_custo() << endl;
            melhor_de_todas = melhor;
    }

    auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nTempo de execucao:  " << float_ms.count() / 1000.0000000000000 << " segundos" << "\n";

    melhor_solucao = melhor_de_todas;
    melhor_solucao.tempoILS = float_ms.count() / 1000.0000000000000;
    melhor_solucao.info();
    calculaTudo(melhor_solucao);
}

void CVRP::gera_output(){

    // Criando o caminho e nome do output
    string output_instancia = this->dados.get_nome_instancia();
    output_instancia.erase(output_instancia.length() - 4);
    string caminho = "outputs";
    output_instancia.replace(0, 9, caminho);

    std::ofstream outfile(output_instancia + ".txt");

    // Custo
    outfile << this->melhor_solucao.get_custo() << "\n";

    // Rotas
    int custo_roteamento = 0;
    int veiculos_utilizados = 0;
    for (int i = 0; i < this->melhor_solucao.get_rotas().size(); i++){
        if(this->melhor_solucao.get_rotas()[i].size() > 2)
            veiculos_utilizados++;
        for (int j = 0; j < this->melhor_solucao.get_rotas()[i].size() - 1; j++){
            custo_roteamento += this->dados.get_custo(this->melhor_solucao.get_rotas()[i][j],this->melhor_solucao.get_rotas()[i][j+1]);
        }
    }
    
    // Custo do roteamento
    outfile << custo_roteamento << "\n";
    // Custo por veiculos utilizados
    outfile << veiculos_utilizados * dados.get_r() << "\n";

    // Clientes terceirizados
    int custo_terceirizacao = 0;
    for(int i = 0; i < this->melhor_solucao.get_clientes_terceirizados().size(); i++)
        custo_terceirizacao+= dados.get_custos_terceirizacao()[this->melhor_solucao.get_clientes_terceirizados()[i]-1];

    // Custo de terceirizacoes
    outfile << custo_terceirizacao << "\n\n";

    // Clientes terceirizados
    for(int i = 0; i < this->melhor_solucao.get_clientes_terceirizados().size(); i++)
        outfile << this->melhor_solucao.get_clientes_terceirizados()[i] << " ";
    outfile << "\n\n";

    // Rotas
    int qtd_rotas = 0;
    for (int i = 0; i < this->melhor_solucao.get_rotas().size(); i++){
        if(this->melhor_solucao.get_rotas()[i].size() > 2)
            qtd_rotas+=1;
    }

    // Quantidade de rotas não vazias e seus clientes
    outfile << qtd_rotas << "\n";
    for (int i = 0; i < this->melhor_solucao.get_rotas().size(); i++){
        if(this->melhor_solucao.get_rotas()[i].size() >2){
            for (int j = 0; j < this->melhor_solucao.get_rotas()[i].size(); j++){
                outfile << this->melhor_solucao.get_rotas()[i][j] << " ";
            }
        outfile << "\n";
        }
    }

    outfile.close();
}
