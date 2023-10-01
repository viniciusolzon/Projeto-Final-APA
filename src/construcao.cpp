#include "CVRP.h"

Solucao CVRP::Construcao(Data *dados){
    Solucao solucao_atual(dados->get_demandas(), dados->get_custos_terceirizacao(), dados->get_k()); // cria uma solucao inicial
    // solucao_atual.info();

    // Constrói as rotas iniciais para cada veículo.
    for (int i = 0; i < dados->get_k(); i++) {
        int capacidadeAtual = dados->get_Q();
        int atual = 0; // Começa no depósito.
        while (capacidadeAtual > 0){
            int proximo = vizinhoMaisProximo(atual, &solucao_atual, dados, capacidadeAtual);
            if (proximo == -1)
                cout << "Nenhum cliente viável encontrado.\n";
                break; // Nenhum cliente viável encontrado.
            solucao_atual.get_clientes()[proximo].set_atendido(true);
            capacidadeAtual -= solucao_atual.get_clientes()[proximo].get_demanda();
            solucao_atual.get_rotas()[i].push_back(proximo);
            atual = proximo;
        }
    }

    return solucao_atual;
}

// Função para encontrar o cliente mais próximo que ainda não foi atendido e que pode ser atendido pelo veículo atual.
int CVRP::vizinhoMaisProximo(int atual, Solucao *solucao, Data *dados, int capacidadeAtual){
    // Inicializa o custo mínimo como o maior valor possível.
    int INT_MAX = std::numeric_limits<int>::max();
    int minCusto = INT_MAX;
    int mais_proximo = -1; // Inicializa o índice do cliente mais próximo como -1.

    // Percorre todos os clientes para encontrar o mais próximo.
    for (int i = 0; i < solucao->get_clientes().size(); i++){
        // Verifica se o cliente atual ainda não foi atendido, se sua demanda pode
        // ser atendida e se o custo para atendê-lo é menor que o custo mínimo
        // encontrado até agora.
        if(!solucao->get_clientes()[i].get_atendido() && solucao->get_clientes()[i].get_demanda() <= capacidadeAtual && dados->get_custo(atual, i) < minCusto){
            minCusto = dados->get_custo(atual, i);
            mais_proximo = i;
        }
    }

    return mais_proximo; // Retorna o índice do cliente mais próximo ou -1 se nenhum cliente puder ser atendido.
}
