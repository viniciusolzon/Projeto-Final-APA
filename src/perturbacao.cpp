#include "CVRP.h"

// Perturbação
Solucao CVRP::Perturbacao(Solucao *s, Data *d){
    Solucao s_copy;
    s_copy = *s;

    // cout << endl << "ENTROU NA PERTURBACAO" << endl << endl;
    // s_copy.info();
    // for(int z=0; z< s_copy.get_rotas().size();z++){
    //     cout << "Capacidade da rota " << z+1 << " = " << s_copy.get_capacidadeRota(z) << endl;
    // }

    int qtd_rotas = s_copy.get_rotas().size();
    int rota_escolhida = 0;
    int tamanho_da_rota = 0;
    int custo = 0;
    int i = 0;
    int j = 0;
    int n = 0;
    

    // APLICA N SWAPS ALEATÓRIOS
    // Escolhe uma rota aleatoŕia não vazia pra aplicar uma sequência de (n) swaps aleatórios
    // (n) é um número aleatório entre 1 e 3
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
        
        // se ele faz um swap que melhore a solução na primeira iteração ta errado, pq a busca local deveria ter achado isso
        if(custo < 0 && t == 0){
            cout << "BUSCA LOCAL N ACHOU ESSA MELHORIA DO SWAP" << endl;
            cout << "rota escolhida = " << rota_escolhida+1 << endl;
            cout << "i escolhido = " << i << endl;
            cout << "j escolhido = " << j << endl;
            getchar();
        }
        
        // Aplica o swap aleatório com a possibilidade dele piorar a solução
        int aux = s_copy.get_rotas()[rota_escolhida][i];
        s_copy.atualizaRota(rota_escolhida, i, s_copy.get_rotas()[rota_escolhida][j]);
        s_copy.atualizaRota(rota_escolhida, j, aux);
        s_copy.atualiza_custo(s_copy.get_custo() + custo);
        
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
        // (n) é um número aleatório entre 1 e 3

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
            int demanda_cliente1 = s_copy.get_clientes()[cliente1-1]->get_demanda();
            int demanda_cliente2 = s_copy.get_clientes()[cliente2-1]->get_demanda();
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



    // cout << endl << "SAIU DA PERTURBACAO" << endl << endl;
    // s_copy.info();
    // for(int z=0; z< s_copy.get_rotas().size();z++){
    //     cout << "Capacidade da rota " << z+1 << " = " << s_copy.get_capacidadeRota(z) << endl;
    // }


    return s_copy;
}
