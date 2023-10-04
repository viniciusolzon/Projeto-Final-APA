#include "CVRP.h"

Solucao CVRP::Construcao(Data *dados){
    Solucao solucao_atual(dados->get_demandas(), dados->get_custos_terceirizacao(), dados->get_k()); // cria uma solucao inicial

    // Constrói as rotas iniciais para cada veículo.
    int veiculoAtual = 1;
    int qtd_veiculos = dados->get_k();
    for(int veiculoAtual = 1; veiculoAtual <= qtd_veiculos; veiculoAtual++){
        cout << "Construindo rota para o veículo " << veiculoAtual << endl;
        int capacidadeAtual = dados->get_Q();
        int atual = 0;
        
        while(capacidadeAtual > 0){
            int proximo = vizinhoMaisProximo(atual, &solucao_atual, dados, capacidadeAtual);
            cout << "   Cliente mais próximo do " << atual << " é " << proximo << " com custo " <<
            dados->get_custo(atual, proximo) << " e demanda " <<  solucao_atual.get_clientes()[proximo- 1]->get_demanda() << endl;
            if(proximo){
                if (capacidadeAtual - solucao_atual.get_clientes()[proximo - 1]->get_demanda() >= 0){  // Verifica se a capacidade não é excedida
                    
                    solucao_atual.get_clientes()[proximo - 1]->set_atendido(true);

                    cout << "   Capacidade atual do veículo: " << capacidadeAtual << endl;
                    capacidadeAtual -= solucao_atual.get_clientes()[proximo -1]->get_demanda();
                
                    // Ao inserir o cliente na rota já podemos calcular o custo dele aqui
                    // Pra não precisar iterar sobre todos os clientes e calcular esse custo dps
                    solucao_atual.insereNaRota(veiculoAtual-1, proximo);
                    // Calcula o custo do cliente
                    cout << "   Custo da solucao aumenta de  " << solucao_atual.get_custo();
                    cout << " para " << solucao_atual.get_custo()+ dados->get_custo(atual, proximo) << endl << endl;
                    solucao_atual.atualiza_custo(solucao_atual.get_custo() + dados->get_custo(atual, proximo));

                    atual = proximo;
                }
                else{
                    cout << "   Capacidade do veiculo atual excedida, retorna ao deposito\n\n";
                    break;  // Se a capacidade for excedida, o veículo retorna ao depósito
                }
            }
            else{
                cout << "   Sem cliente proximo, o veiculo retorna ao deposito\n\n";
                break;  // Se não houver cliente próximo, o veículo retorna ao depósito
            }
        }

        // Coloca o 0 no final pq ele volta pro depósito
        solucao_atual.insereNaRota(veiculoAtual-1, 0);

        cout << "   Ao voltar pro depósito o custo da solucao aumenta de  " << solucao_atual.get_custo();
        cout << " para " << solucao_atual.get_custo()+ dados->get_custo(atual, 0) << endl << endl;
        solucao_atual.atualiza_custo(solucao_atual.get_custo() + dados->get_custo(atual, 0));

        // Precisa disso?
        // if(veiculoAtual+1 > dados->get_k()){
        //     solucao_atual.get_rotas().push_back({});
        // }
    }
    
    // Não precisa mais calcular o custo da rota aqui pq ele ta sendo calculado dentro da construção
    // calcularCustoTotal(&solucao_atual, *dados);
    return solucao_atual;
}

// Função para calcular a capacidade usada em uma rota.
int CVRP::calcularCapacidadeUsada(vector<int> &rota, vector<Cliente*> clientes){
    int capacidadeUsada = 0;
    for (int clienteId : rota){
        capacidadeUsada += clientes[clienteId]->get_demanda();
    }
    return capacidadeUsada;
}

// Função para encontrar o cliente mais próximo que ainda não foi atendido e que pode ser atendido pelo veículo atual.
int CVRP::vizinhoMaisProximo(int atual, Solucao *solucao, Data *dados, int capacidadeAtual){
    // Inicializa o custo mínimo como o maior valor possível.
    int INT_MAX = std::numeric_limits<int>::max();
    int minCusto = INT_MAX;
    int mais_proximo = 0; // Inicializa o índice do cliente mais próximo como 0.


    // Percorre todos os clientes para encontrar o mais próximo.
    for (int i = 1; i <= solucao->get_clientes().size(); i++){
        
        // Pra não pegar a distância dele com ele mesmo, sempre vai ser a menor (0)
        if(i == atual)
            continue;

        // Verifica se o cliente atual ainda não foi atendido, se sua demanda pode
        // ser atendida e se o custo para atendê-lo é menor que o custo mínimo
        // encontrado até agora.

        if(!(solucao->get_clientes()[i-1]->get_atendido()) && solucao->get_clientes()[i-1]->get_demanda() <= capacidadeAtual && dados->get_custo(atual, i) < minCusto){
            minCusto = dados->get_custo(atual, i);
            mais_proximo = i;
        }
    }

    return mais_proximo; // Retorna o índice do cliente mais próximo ou -1 se nenhum cliente puder ser atendido.
}

// Função para calcular o custo total da solução.
// int calcularCustoTotal(vector<vector<int>> rotas, vector<Cliente> clientes, vector<vector<int>> &matrizCustos, int r){
void CVRP::calcularCustoTotal(Solucao *s, Data d){
    int custoTotal = 0;
    int veiculosUsados = 0;

    // Calcula o custo das rotas.
    for (auto rota : s->get_rotas()){
        if (!rota.empty()){
            veiculosUsados++;
            for (int i = 0; i < rota.size() - 1; i++){
                custoTotal += d.get_custo(rota[i], rota[i+1]);
            }
        }
    }

    // Adiciona o custo fixo dos veículos utilizados.
    custoTotal += d.get_r() * veiculosUsados;

    // Adiciona o custo de terceirização.
    for (auto cliente : s->get_clientes()) {
        if (!cliente->get_atendido()) {
            custoTotal += cliente->get_custoTerceirizacao();
        }
    }

    s->atualiza_custo(custoTotal);
}
