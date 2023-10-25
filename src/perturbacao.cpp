#include "CVRP.h"

// Perturbação
Solucao CVRP::Perturbacao(Solucao *s, Data *d){
    Solucao s_copy;
    s_copy = *s;

    int qtd_rotas = s_copy.get_rotas().size();
    int rota_escolhida = 0;
    int tamanho_da_rota = 0;
    int custo = 0;
    int i = 0;
    int j = 0;
    int n = 0;
    
    // Primeiro precisa verificar se tem 2 clientes não terceirizados em uma rota só pra aplicar o swap aleatório
    bool viavel = false;
    for(int rota = 0; rota < s_copy.get_rotas().size(); rota++){
        tamanho_da_rota = s_copy.get_rotas()[rota].size();

        // Pra aplicar o swap a rota precisa ter no mínimo 2 clientes, (4 pq tem a ida e volta do depósito)
        if(tamanho_da_rota >= 4)
            viavel = true;
    }

    
    if(viavel){
            
        // APLICA N SWAPS ALEATÓRIOS
        // Escolhe uma rota aleatoŕia não vazia pra aplicar uma sequência de (n) swaps aleatórios
        n = (rand() % 3) + 1;

        for(int t = 0; t < n; t++){

            // Escolha da rota aleatória
            while(true){
                rota_escolhida = rand() % qtd_rotas;
                tamanho_da_rota = s_copy.get_rotas()[rota_escolhida].size();
                

                // Pra aplicar o swap a rota precisa ter no mínimo 2 clientes, (4 pq tem a ida e volta do depósito)
                if(tamanho_da_rota >= 4)
                    break;
            }

            // Escolha do primeiro cliente aleatório
            i = std::max(1, rand() % tamanho_da_rota-1);

            // Escolha do segundo cliente aleatório
            while(true){

                j = std::max(1, rand() % tamanho_da_rota-1);
                if(j != i)
                    break;
            }

            // Calcula o custo do swap aleatório
            custo = 0;
            custo = calculaCustoSwap(&s_copy, d, rota_escolhida, i, j);
            
            // Aplica o swap aleatório com a possibilidade dele piorar a solução
            int aux = s_copy.get_rotas()[rota_escolhida][i];
            s_copy.atualizaRota(rota_escolhida, i, s_copy.get_rotas()[rota_escolhida][j]);
            s_copy.atualizaRota(rota_escolhida, j, aux);
            s_copy.atualiza_custo(s_copy.get_custo() + custo);
            
        }
    }
    

    // Precisa ter ao menos 2 rotas pra aplicar o shift
    int qtd_rotas_nao_vazias = 0;
    for(int z = 0; z < qtd_rotas; z++){
        if(s_copy.get_rotas()[z].size() > 2)
            qtd_rotas_nao_vazias++;
    }

    if(qtd_rotas_nao_vazias >= 2){
        // APLICA N SHIFTS 1 ALEATÓRIOS
        // Escolhe uma rota aleatoŕia não vazia pra aplicar uma sequência de (n) shifts 1 aleatórios
        n = (rand() % 3) + 1;

        int rota_escolhida1 = 0;
        int rota_escolhida2 = 0;
        int tamanho_da_rota1 = 0;
        int tamanho_da_rota2 = 0;
        custo = 0;
        i = 0;
        j = 0;

        for(int t = 0; t < n; t++){

            // Escolha da primeira rota aleatória
            while(true){
                rota_escolhida1 = rand() % qtd_rotas;
                tamanho_da_rota1 = s_copy.get_rotas()[rota_escolhida1].size();
                
                // Pra aplicar o shift 1 a rota precisa ter no mínimo 1 clientes, (3 pq tem a ida e volta do depósito)
                if(tamanho_da_rota1 >= 3)
                    break;
            }

            // Escolha da segunda rota aleatória
            while(true){
                
                // Pra segunda rota ser diferente da primeira
                while(true){
                    rota_escolhida2 = rand() % qtd_rotas;
                    if(rota_escolhida2 != rota_escolhida1){
                        break;
                    }
                }

                tamanho_da_rota2 = s_copy.get_rotas()[rota_escolhida2].size();

                // Pra aplicar o shift 1 a rota precisa ter no mínimo 1 clientes, (3 pq tem a ida e volta do depósito)
                if(tamanho_da_rota2 >= 3){
                    break;
                }
            }


            // Escolha do primeiro cliente aleatório
            i = std::max(1, rand() % tamanho_da_rota1-1);

            // Escolha do segundo cliente aleatório
            j = std::max(1, rand() % tamanho_da_rota2-1);

            // Antes de ver se vale a pena trocar os clientes entre rotas, é preciso primeiro
            // verificar se a capacidade de nenhuma das rota será excedida ao trocar os clientes
            bool capacidade_excedida = verificaCapacidadeRotas(&s_copy, d, rota_escolhida1, rota_escolhida2, i, j);
            // Se a capacidade de alguma das rotas for excedida, tentamos outra troca entre clientes
            if(capacidade_excedida){
                continue;
            }

            int cliente1 = s_copy.get_rotas()[rota_escolhida1][i];
            int cliente2 = s_copy.get_rotas()[rota_escolhida2][j];
            // int demanda_cliente1 = s_copy.get_clientes()[cliente1-1]->get_demanda();
            int demanda_cliente1 = d->get_demandas()[cliente1-1];
            // int demanda_cliente2 = s_copy.get_clientes()[cliente2-1]->get_demanda();
            int demanda_cliente2 = d->get_demandas()[cliente2-1];
            int capacidade_rota1 = s_copy.get_capacidadeRota(rota_escolhida1);
            int capacidade_rota2 = s_copy.get_capacidadeRota(rota_escolhida2);
            // retirar o cliente 1 da rota 1 e colocar o 2 no lugar dele
            int nova_capacidade_rota1 = capacidade_rota1 + demanda_cliente1 - demanda_cliente2;
            // retirar o cliente 2 da rota 2 e colocar o 1 no lugar dele
            int nova_capacidade_rota2 = capacidade_rota2 + demanda_cliente2 - demanda_cliente1;

            // atualiza a capacidade das 2 rotas após a alteração
            s_copy.atualizaCapacidade(rota_escolhida1, nova_capacidade_rota1);
            s_copy.atualizaCapacidade(rota_escolhida2, nova_capacidade_rota2);

            // Calcula o custo do shift aleatório
            custo = calculaCustoShift(&s_copy, d, rota_escolhida1, rota_escolhida2, i, j);
            
            // Aplica o shift aleatório com a possibilidade dele piorar a solução
            int aux = s_copy.get_rotas()[rota_escolhida1][i];
            s_copy.atualizaRota(rota_escolhida1, i, s_copy.get_rotas()[rota_escolhida2][j]);
            s_copy.atualizaRota(rota_escolhida2, j, aux);
            s_copy.atualiza_custo(s_copy.get_custo() + custo);
        }
    }


    // APLICA N TERCEIRIZAÇÕES ALEATÓRIAS
    // Escolhe uma rota aleatoŕia não vazia pra aplicar uma sequência de (n) terceirizações aleatórios
    n = (rand() % 3) + 1;

    for(int t = 0; t < n; t++){
        
        // Se não existe cliente pra ser terceirizado já abortamos essa perturbação
        if(s_copy.get_num_clientes() == 0){
            getchar();
            break;
        }
        
        // Antes de calcular o custo da terceirização do cliente, é preciso verificar
        // se dá pra terceirizar respeitando o limite mínimo L de entregas não terceirizadas
        bool limite_respeitado = false;
        limite_respeitado = verificaLimiteL(&s_copy, d);
        if(!limite_respeitado){
            continue;
        }

        // Escolha da rota aleatória
        while(true){
            rota_escolhida = rand() % qtd_rotas;
            tamanho_da_rota = s_copy.get_rotas()[rota_escolhida].size();
            
            // Pra aplicar o terceirização a rota precisa ter no mínimo 1 cliente, (3 pq tem a ida e volta do depósito)
            if(tamanho_da_rota >= 3)
                break;
        }

        // Escolha do cliente aleatório
        i = std::max(1, rand() % tamanho_da_rota-1);

        // Calcula o custo da terceirização aleatória
        custo = 0;
        custo = calculaCustoTerceirizacao(&s_copy, d, rota_escolhida, i);
        
        // Aplica a terceirizacao aleatória com a possibilidade dela piorar a solução
        int cliente = s_copy.get_rotas()[rota_escolhida][i];
        // int demanda_cliente = s_copy.get_clientes()[cliente-1]->get_demanda();
        int demanda_cliente = d->get_demandas()[cliente-1];
        int capacidade_rota = s_copy.get_capacidadeRota(rota_escolhida);
        // retirar o cliente da rota 
        int nova_capacidade_rota = capacidade_rota + demanda_cliente;

        // atualiza a capacidade da rota após a alteração
        s_copy.atualizaCapacidade(rota_escolhida, nova_capacidade_rota);

        s_copy.terceirizaCliente(cliente);
        s_copy.menos_um_cliente();

        s_copy.removeDaRota(rota_escolhida, i);
        s_copy.atualiza_custo(s_copy.get_custo() + custo);

        // Se o segundo 'cliente' da rota agora for um zero, quer dizer que a rota está vazia,
        // então diminuimos a quantidade de veiculos utilizados
        if(s_copy.get_rotas()[rota_escolhida][1] == 0){
            s_copy.atualiza_custo(s_copy.get_custo() - d->get_r());
        }

    }

    // cout << "Depois da nova perturbacao:" << endl;
    // s_copy.info();
    // cout << "quantidade de clientes = " << s_copy.get_num_clientes() << endl;
    // cout << "quantidade de clientes terceirizados = " << s_copy.get_num_clientesTerceirizados() << endl;
    // getchar();

    return s_copy;
}
