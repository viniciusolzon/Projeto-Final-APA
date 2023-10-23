#include "CVRP.h"

Solucao CVRP::Construcao(Data *dados){
    Solucao solucao_atual(dados->get_Q(), dados->get_demandas(), dados->get_custos_terceirizacao(), dados->get_k()); // cria uma solucao inicial

    // Constrói as rotas iniciais para cada veículo.
    int veiculoAtual = 1;
    int qtd_veiculos = dados->get_k();
    int veiculo_usado = 0;
    vector<bool> clientes_atendidos(dados->get_n(), false);

    for(int veiculoAtual = 1; veiculoAtual <= qtd_veiculos; veiculoAtual++){
        // cout << "Construindo rota para o veículo " << veiculoAtual << endl;
        int capacidadeAtual = dados->get_Q();
        int atual = 0;
        
        while(capacidadeAtual > 0){
            
            int proximo = vizinhoMaisProximo(atual, &solucao_atual, dados, capacidadeAtual, clientes_atendidos);
            int custo_proximo = dados->get_custo(atual, proximo);
            int custo_terceirizacao = dados->get_custos_terceirizacao()[0];


            if(proximo){
                // Verifica se a capacidade não é excedida
                // if (capacidadeAtual - solucao_atual.get_clientes()[proximo - 1]->get_demanda() >= 0){
                // Verifica se a capacidade não é excedida
                if (capacidadeAtual - dados->get_demandas()[proximo - 1] >= 0){
                    
                    // solucao_atual.get_clientes()[proximo - 1]->set_atendido(true);
                    clientes_atendidos[proximo - 1] = true;

                    // capacidadeAtual -= solucao_atual.get_clientes()[proximo -1]->get_demanda();
                    capacidadeAtual -= dados->get_demandas()[proximo - 1];
                
                    // Ao inserir o cliente na rota já podemos calcular o custo dele aqui
                    // Pra não precisar iterar sobre todos os clientes e calcular esse custo dps
                    solucao_atual.pushBack(veiculoAtual-1, proximo);
                    
                    solucao_atual.atualizaCapacidade(veiculoAtual-1, capacidadeAtual);
                    
                    // Calcula o custo do cliente
                    solucao_atual.atualiza_custo(solucao_atual.get_custo() + dados->get_custo(atual, proximo));

                    atual = proximo;
                }
                else{
                    break;  // Se a capacidade for excedida, o veículo retorna ao depósito
                }
            }
            else{
                break;  // Se não houver cliente próximo, o veículo retorna ao depósito
            }
        }

        // Coloca o 0 no final pq ele volta pro depósito
        solucao_atual.pushBack(veiculoAtual-1, 0);
        solucao_atual.atualiza_custo(solucao_atual.get_custo() + dados->get_custo(atual, 0));

        // Se a rota do veiculo atual possui mais de um vértice nela (já começa com o 0 que é o depósito)
        if(solucao_atual.get_rotas()[veiculoAtual-1].size()>2)
            veiculo_usado++;
    }

    // Soma o custo de uso de um veiculo
    solucao_atual.atualiza_custo(solucao_atual.get_custo() + (veiculo_usado * dados->get_r()));

    // Não precisa mais calcular o custo da rota aqui pq ele ta sendo calculado dentro da construção
    return solucao_atual;
}

// Função para encontrar o cliente mais próximo que ainda não foi atendido e que pode ser atendido pelo veículo atual.
int CVRP::vizinhoMaisProximo(int atual, Solucao *solucao, Data *dados, int capacidadeAtual, vector<bool> clientes_atendidos){
    // Inicializa o custo mínimo como o maior valor possível.
    int INT_MAX = std::numeric_limits<int>::max();
    int minCusto = INT_MAX;
    int mais_proximo = 0; // Inicializa o índice do cliente mais próximo como 0.

    // Percorre todos os clientes para encontrar o mais próximo.
    for (int i = 1; i <= dados->get_n(); i++){
        
        // Pra não pegar a distância dele com ele mesmo, sempre vai ser a menor (0)
        if(i == atual)
            continue;

        // Verifica se o cliente atual ainda não foi atendido, se sua demanda pode
        // ser atendida e se o custo para atendê-lo é menor que o custo mínimo
        // encontrado até agora.
        if(!(clientes_atendidos[i-1]) && dados->get_demandas()[i-1] <= capacidadeAtual && dados->get_custo(atual, i) < minCusto){
            minCusto = dados->get_custo(atual, i);
            mais_proximo = i;
        }
    }

    return mais_proximo; // Retorna o índice do cliente mais próximo ou -1 se nenhum cliente puder ser atendido.
}
