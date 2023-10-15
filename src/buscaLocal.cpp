#include "CVRP.h"

// As duas funções abaixo servem para verificar se o custo da solução diminuiu ou aumentou
double CVRP::epsilon(double a, double b){
    return fabs(a + b) * numeric_limits<double>::epsilon() * 15;
}

bool CVRP::melhorou(double valor1, double valor2){
    return (valor1 - valor2) > epsilon(valor1, valor2);
}


// REINSERTION
int CVRP::calculaCustoReinsertion(Solucao *s, Data *d, int rota, int cliente, int posicao){
    // int anterior = s->get_rotas()[rota][posicao];
    // int posterior = s->get_rotas()[rota][posicao+1];
    // cout << "Tentando reinserir cliente " << s->get_rotas()[rota][cliente] << " na posicao " << posicao << endl;
    // cout << "O cliente " << s->get_rotas()[rota][cliente] << " ficara entre o cliente " << anterior << " e o cliente " << posterior << endl;

    int antes = 0, depois = 0, custo = 0;

    if(posicao == cliente + 1){
        antes = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][cliente]);
        antes+= d->get_custo(s->get_rotas()[rota][cliente], s->get_rotas()[rota][posicao]);

        antes+= d->get_custo(s->get_rotas()[rota][posicao], s->get_rotas()[rota][posicao+1]);

        depois = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][posicao]);
        depois+= d->get_custo(s->get_rotas()[rota][posicao], s->get_rotas()[rota][cliente]);
        depois+= d->get_custo(s->get_rotas()[rota][cliente], s->get_rotas()[rota][posicao+1]);
    }

    else{
        antes = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][cliente]);
        antes+= d->get_custo(s->get_rotas()[rota][cliente], s->get_rotas()[rota][cliente+1]);

        antes+= d->get_custo(s->get_rotas()[rota][posicao], s->get_rotas()[rota][posicao+1]);

        depois = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][cliente+1]);
        depois+= d->get_custo(s->get_rotas()[rota][posicao], s->get_rotas()[rota][cliente]);
        
        depois+= d->get_custo(s->get_rotas()[rota][cliente], s->get_rotas()[rota][posicao+1]);
    }

    custo = depois - antes;
    return custo;
}

bool CVRP::melhorouReinsertion(Solucao *s, Data *d){

    int qtd_rotas = s->get_rotas().size();
    int tamanho_da_rota = 0;
    bool houve_melhora = false;
    // Precisamos de uma rota com no mínimo 2 clientes para tentar fazer a reinserção
    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        // cout << "Verificando reisertion na rota: " << rota_escolhida+1 << endl;
        tamanho_da_rota = s->get_rotas()[rota_escolhida].size();
        if(tamanho_da_rota < 4){
            // cout << "Rota do veiculo " << rota_escolhida+1 << " muito pequena pra aplicar o reinsertion\n";
            continue;
        }

        int melhor_custo = 0;
        int custo = 0;
        int melhor_cliente, melhor_posicao;

        // Se a rota escolhida tiver mais que 2 clientes precisamos testar todas os possíveis lugares em que
        // o cliente pode ser reinserido na rota
        // Vamos tentar reinserir o cliente (i) em todas outras posições (j) da rota
        for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 1; i++){
            for(int j = 0; j < s->get_rotas()[rota_escolhida].size() - 1; j++){
                if(i == j || i == j+1){
                    // Não vamos reinserir ele na posição que ele já está
                    continue;
                }
                custo = calculaCustoReinsertion(s, d, rota_escolhida, i, j);
                if(melhorou(melhor_custo, custo)){
                    // cout << "Da pra fazer reinsertion na rota " << rota_escolhida+1 << endl;
                    melhor_custo = custo;
                    melhor_cliente = i;
                    melhor_posicao = j;
                }
            }
        }
    
        // Se o reisertion melhora a solucao, aplicamos o movimento na solucao atual
        if(melhor_custo < 0){
            // cout << "Antes do reinsertion:" << endl;
            // s->info();
            int elemento = s->get_rotas()[rota_escolhida][melhor_cliente];

            if(melhor_cliente > melhor_posicao){
                s->removeDaRota(rota_escolhida, melhor_cliente);
                s->insereNaRota(rota_escolhida, melhor_posicao, elemento);
            }
            else{
                s->insereNaRota(rota_escolhida, melhor_posicao, elemento);
                s->removeDaRota(rota_escolhida, melhor_cliente);
            }

            // cout << "Vamos tirar o cliente " << elemento << " e o colocar na posicao " << melhor_posicao << endl;

            // cout << "Depois do reinsertion:" << endl;
            // s->info();

            s->atualiza_custo(s->get_custo() + melhor_custo);
            
            // Se houve alguma melhora em qualquer rota retornamos que houve uma melhora na solução
            cout << "Houve melhora no reinsertion" << endl;
            houve_melhora = true;
        }
    }
    
    if(!houve_melhora)
        cout << "Nao houve melhora no reinsertion" << endl;
        
    return houve_melhora;
}


// SWAP
int CVRP::calculaCustoSwap(Solucao *s, Data *d, int rota, int cliente1, int cliente2){
    int antes = 0, depois = 0, custo = 0;

    if(cliente2 == cliente1 + 1){
        antes = d->get_custo(s->get_rotas()[rota][cliente1-1], s->get_rotas()[rota][cliente1]);
        antes+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente1+1]);

        antes+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente2+1]);

        depois = d->get_custo(s->get_rotas()[rota][cliente1-1], s->get_rotas()[rota][cliente2]);
        depois+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente2+1]);
        depois+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente1]);
    }

    else{
        antes = d->get_custo(s->get_rotas()[rota][cliente1-1], s->get_rotas()[rota][cliente1]);
        antes+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente1+1]);

        antes+= d->get_custo(s->get_rotas()[rota][cliente2-1], s->get_rotas()[rota][cliente2]);
        antes+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente2+1]);

        depois = d->get_custo(s->get_rotas()[rota][cliente1-1], s->get_rotas()[rota][cliente2]);
        depois+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente1+1]);
        
        depois+= d->get_custo(s->get_rotas()[rota][cliente2-1], s->get_rotas()[rota][cliente1]);
        depois+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente2+1]);
    }

    custo = depois - antes;
    return custo;
}

// aplica o melhor swap da melhor melhora
bool CVRP::melhorouSwap(Solucao *s, Data *d){

    int qtd_rotas = s->get_rotas().size();
    int tamanho_da_rota = 0;
    bool houve_melhora = false;
    // Precisamos de uma rota com no mínimo 2 clientes para tentar fazer a troca
    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        // cout << "Verificando swap na rota: " << rota_escolhida+1 << endl;
        tamanho_da_rota = s->get_rotas()[rota_escolhida].size();
        
        if(tamanho_da_rota < 4){
            // cout << "Rota do veiculo " << rota_escolhida+1 << " muito pequena pra aplicar o swap\n";
            continue;
        }

        int melhor_custo = 0;
        int custo = 0;
        int melhor_cliente1, melhor_cliente2;

        // Se a rota escolhida tiver mais que 2 clientes precisamos testar todas combinações possíveis entre os clientes
        for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 2; i++){
            for(int j = i + 1; j < s->get_rotas()[rota_escolhida].size() - 1; j++){
                custo = calculaCustoSwap(s, d, rota_escolhida, i, j);
                if(melhorou(melhor_custo, custo)){
                    // cout << "Da pra fazer swap na rota " << rota_escolhida+1 << endl;
                    melhor_custo = custo;
                    melhor_cliente1 = i;
                    melhor_cliente2 = j;
                }
            }
        }
    
        // Se o swap melhora a solucao, aplicamos o movimento na solucao atual
        if(melhor_custo < 0){
            // cout << endl << "Vamos trocar: " << s->get_rotas()[rota_escolhida][melhor_cliente1] << " com " << s->get_rotas()[rota_escolhida][melhor_cliente2];
            // cout << endl << endl;

            // cout << "Antes do swap:" << endl;
            // s->info();

            int aux = s->get_rotas()[rota_escolhida][melhor_cliente1];
            s->atualizaRota(rota_escolhida, melhor_cliente1, s->get_rotas()[rota_escolhida][melhor_cliente2]);
            s->atualizaRota(rota_escolhida, melhor_cliente2, aux);
            s->atualiza_custo(s->get_custo() + melhor_custo);
            
            // cout << "Depois do swap:" << endl;
            // s->info();

            // Se houve alguma melhora em qualquer rota retornamos que houve uma melhora na solução
            cout << "Houve melhora no swap" << endl;
            houve_melhora = true;
        }
    }

    if(!houve_melhora)
        cout << "Nao houve melhora no swap" << endl;

    return houve_melhora;
}


// 2-OPT
int CVRP::calculaCusto2opt(Solucao *s, Data *d, int rota, int cliente1, int cliente2){

    return 1;
}

bool CVRP::melhorou2opt(Solucao *s, Data *d){

    return true;
}


// SHIFT
int CVRP::calculaCustoShift(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2){
    int antes = 0, depois = 0, custo = 0;

    antes = d->get_custo(s->get_rotas()[rota1][cliente1-1], s->get_rotas()[rota1][cliente1]);
    antes+= d->get_custo(s->get_rotas()[rota1][cliente1], s->get_rotas()[rota1][cliente1+1]);

    antes+= d->get_custo(s->get_rotas()[rota2][cliente2-1], s->get_rotas()[rota2][cliente2]);
    antes+= d->get_custo(s->get_rotas()[rota2][cliente2], s->get_rotas()[rota2][cliente2+1]);

    depois = d->get_custo(s->get_rotas()[rota1][cliente1-1], s->get_rotas()[rota2][cliente2]);
    depois+= d->get_custo(s->get_rotas()[rota2][cliente2], s->get_rotas()[rota1][cliente1+1]);
    
    depois+= d->get_custo(s->get_rotas()[rota2][cliente2-1], s->get_rotas()[rota1][cliente1]);
    depois+= d->get_custo(s->get_rotas()[rota1][cliente1], s->get_rotas()[rota2][cliente2+1]);

    custo = depois - antes;
    return custo;
}

bool CVRP::verificaCapacidadeRotas(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2){
    int capacidade1 = s->get_capacidadeRota(rota1);
    int capacidade2 = s->get_capacidadeRota(rota2);

    cliente1 = s->get_rotas()[rota1][cliente1];
    cliente2 = s->get_rotas()[rota2][cliente2];
    int demanda1 = s->get_clientes()[cliente1-1]->get_demanda();
    int demanda2 = s->get_clientes()[cliente2-1]->get_demanda();

    // Tira o cliente da rota dele e coloca o outro cliente da outra rota e verifica se a capacidade n será excedida
    if ((capacidade1 + demanda1 - demanda2) < 0 || (capacidade2 + demanda2 - demanda1) < 0){
        // cout << "Nao da pra aplicar o shift com os clientes " <<  cliente1 << " e " << cliente2 <<endl;
        // Não dá pra aplicar o shift
        return true;
    }
    // Dá pra aplicar o shift
    // cout << "Da pra aplicar o shift com os clientes " <<  cliente1 << " e " << cliente2 <<endl;
    return false;
}

bool CVRP::melhorouShift(Solucao *s, Data *d){

    int qtd_rotas = s->get_rotas().size();

    int melhor_custo = 0;
    int custo = 0;
    int melhor_cliente1, melhor_cliente2;
    int melhor_rota1, melhor_rota2;
    bool capacidade_excedida = false;
    
    // Passamos por todas as rotas pra tentar o shift
    for(int rota1_escolhida = 0; rota1_escolhida < qtd_rotas; rota1_escolhida++){

        for(int rota2_escolhida = rota1_escolhida + 1; rota2_escolhida < qtd_rotas; rota2_escolhida++){

            // Testa o shift com todos os clientes da rota1 com todos os clientes da rota2
            for(int i = 1; i < s->get_rotas()[rota1_escolhida].size() - 1; i++){
                for(int j = 1; j < s->get_rotas()[rota2_escolhida].size() - 1; j++){
                    // Antes de ver se ale a pena trocar os clientes entre rotas, é preciso primeiro
                    // verificar se a capacidade de nenhuma das rota será excedida ao trocar os clientes
                    capacidade_excedida = verificaCapacidadeRotas(s, d, rota1_escolhida, rota2_escolhida, i, j);
                    // Se a capacidade de alguma das rotas for excedida, tentamos trocar outra troca entre clientes
                    if(capacidade_excedida)
                        continue;

                    // Se não, verificamos se vale a pena trocar eles baseado no custo das rotas
                    custo = calculaCustoShift(s, d, rota1_escolhida, rota2_escolhida, i, j);
                    if(melhorou(melhor_custo, custo)){
                        melhor_custo = custo;
                        melhor_rota1 = rota1_escolhida;
                        melhor_rota2 = rota2_escolhida;
                        // cout << "Da pra fazer shift na rota " << melhor_rota1+1 << " com a rota " << melhor_rota2+1 << endl;
                        melhor_cliente1 = i;
                        melhor_cliente2 = j;
                    }
                }
            }
        
            // Se o shift melhora a solucao, aplicamos o movimento na solucao atual
            if(melhor_custo < 0){
                // cout << "Antes do shift:" << endl;
                // s->info();

                int aux = s->get_rotas()[melhor_rota1][melhor_cliente1];
                s->atualizaRota(melhor_rota1, melhor_cliente1, s->get_rotas()[melhor_rota2][melhor_cliente2]);
                s->atualizaRota(melhor_rota2, melhor_cliente2, aux);
                s->atualiza_custo(s->get_custo() + melhor_custo);
                
                // cout << "Depois do shift:" << endl;
                // s->info();

                cout << "Houve melhora no shift" << endl;
                return true;
            }
        }
    }

    cout << "Nao houve melhora no shift" << endl;
    return false;
}


// TERCEIRIZAÇÃO
bool CVRP::verificaLimiteL(Solucao *s, Data *d){

    // A quantidade de clientes não terceirizados tem que ser maior do que o limite L
    // Tem q ser MAIOR pq se for igual ao terceirizar a qtd de clientes não terceirizados
    // vai ser menos do que o limite L
    if(d->get_n() - s->get_num_clientesTerceirizados() > d->get_L())
        return true;
    
    // cout << "Limite L nao deixa terceirizar" << endl;
    return false;
}

int CVRP::calculaCustoTerceirizacao(Solucao *s, Data *d, int rota, int cliente){
    int antes = 0, depois = 0, custo = 0;

    // Vamos tirar ele da rota
    // Remover o custos de suas arestas e adicionar os custos das novas arestas e o de terceirizacao
    antes = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][cliente]);
    antes+= d->get_custo(s->get_rotas()[rota][cliente], s->get_rotas()[rota][cliente+1]);

    depois = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][cliente+1]);
    depois+= d->get_custos_terceirizacao()[cliente]; // custo de terceirizar aquele cliente

    custo = depois - antes;
    // cout << "Custo de terceirizar o cliente " << s->get_rotas()[rota][cliente] << " = " << custo << endl;
    // cout << "Custo antes " << antes << endl;
    // cout << "Custo depois " << depois << endl;
    return custo;
}

bool CVRP::melhorouTerceirizacao(Solucao *s, Data *d){

    // int melhor_custo = 0;
    // int custo = 0;
    int melhor_terceirizacao;
    bool houve_melhora = false;
    bool limite_respeitado = false;

    int qtd_rotas = s->get_rotas().size();

    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        // Testa se vale a pena terceirizar algum cliente da rota atual
        int melhor_custo = 0;
        int custo = 0;

        for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 1; i++){

            // Antes de calcular o custo da terceirização do cliente, é preciso verificar
            // se dá pra terceirizar respeitando o limite mínimo L de entregas não terceirizadas
            limite_respeitado = verificaLimiteL(s, d);
            if(!limite_respeitado)
                continue;

            custo = calculaCustoTerceirizacao(s, d, rota_escolhida, i);
            if(melhorou(melhor_custo, custo)){
                melhor_custo = custo;
                melhor_terceirizacao = i;
            }
        }

        // Se terceirizar algum cliente na rota atual é melhor para a solucao, já terceirizamos aquele cliente
        if(melhor_custo < 0){
            // cout << "Terceirizando o cliente de indice " << melhor_terceirizacao << " da rota "  << rota_escolhida+1 << endl;
            // cout << "Antes da terceirizacao:" << endl;
            // s->info();

            s->get_clientes()[s->get_rotas()[rota_escolhida][melhor_terceirizacao]-1]->set_terceirizado(true);
            s->terceirizaCliente(melhor_terceirizacao);

            s->removeDaRota(rota_escolhida, melhor_terceirizacao);
            s->atualiza_custo(s->get_custo() + melhor_custo);

            s->atualiza_num_clientesTerceirizados(s->get_num_clientesTerceirizados() + 1);


            // cout << "Depois da terceirizacao:" << endl;
            // s->info();

            houve_melhora = true;
        }
    }

    if(houve_melhora)
        cout << "Houve melhora na terceirizacao" << endl;
    else
        cout << "Nao houve melhora na terceirizacao" << endl;
        
    return houve_melhora;
}


// DESTERCEIRIZACAO
bool CVRP::verificaCapacidade(Solucao *s, Data *d, int rota, int cliente_terceirizado){

    int demanda_cliente = d->get_demandas()[s->get_clientes_terceirizados()[cliente_terceirizado-1]];
    int capacidade_restante = s->get_capacidadeRota(rota);

    if(demanda_cliente > capacidade_restante)
        return false;

    return true;
}

int CVRP::calculaCustoDesterceirizacao(Solucao *s, Data *d, int rota, int posicao, int cliente_terceirizado){
    int antes = 0, depois = 0, custo = 0;

    // Vamos colocar ele da rota
    // Adicionar o custos de suas novas arestas e remover os custos das antigas arestas e o de terceirizacao
    antes = d->get_custo(s->get_rotas()[rota][posicao-1], s->get_rotas()[rota][posicao]);
    antes+= d->get_custo(s->get_rotas()[rota][posicao], s->get_rotas()[rota][posicao+1]);
    antes+= d->get_custos_terceirizacao()[cliente_terceirizado]; // custo de terceirizar aquele cliente

    depois = d->get_custo(s->get_rotas()[rota][posicao-1], s->get_rotas()[rota][cliente_terceirizado]);
    depois+= d->get_custo(s->get_rotas()[rota][cliente_terceirizado], s->get_rotas()[rota][posicao+1]);

    custo = depois - antes;

    cout << "Custo pra desterceirizar o cliente " << s->get_clientes_terceirizados()[cliente_terceirizado];
    cout << " = " << custo << endl;
    return custo;
}

bool CVRP::melhorouDesterceirizacao(Solucao *s, Data *d){

    // Primeiro verifica se tem algum cliente terceirizado
    if(s->get_num_clientesTerceirizados() == 0){
        cout << "Nao da pra desterceirizar" << endl;
        return false;
    }

    // int melhor_custo = 0;
    // int custo = 0;
    int melhor_lugar;
    bool houve_melhora = false;
    bool capacidade_respeitada = false;

    int qtd_rotas = s->get_rotas().size();

    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        int melhor_custo = 0;
        int custo = 0;

        for(int cliente_terceirizado = 0; cliente_terceirizado < s->get_num_clientesTerceirizados(); cliente_terceirizado++){

            // Testa se vale a pena desterceirizar algum cliente da rota atual
            for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 1; i++){
                
                // Antes de calcular o custo da desterceirização do cliente, é preciso verificar
                // se dá pra desterceirizar respeitando o limite Q da capacidade dos veículos de entregas
                capacidade_respeitada = verificaCapacidade(s, d, rota_escolhida, cliente_terceirizado);
                if(!capacidade_respeitada)
                    continue;

                custo = calculaCustoDesterceirizacao(s, d, rota_escolhida, i, cliente_terceirizado);
                if(melhorou(melhor_custo, custo)){
                    melhor_custo = custo;
                    melhor_lugar = i;
                }
            }

            // Se desterceirizar algum cliente na rota atual é melhor para a solucao, já desterceirizamos aquele cliente
            if(melhor_custo < 0){
                // cout << "Desterceirizando o cliente de indice " << melhor_terceirizacao << " da rota "  << rota_escolhida+1 << endl;
                // cout << "Antes da desterceirizacao:" << endl;
                // s->info();

                s->get_clientes()[s->get_rotas()[rota_escolhida][cliente_terceirizado]-1]->set_terceirizado(false);
                s->desterceirizaCliente(cliente_terceirizado);

                s->insereNaRota(rota_escolhida, melhor_lugar, s->get_clientes_terceirizados()[cliente_terceirizado]);

                s->atualiza_custo(s->get_custo() + melhor_custo);


                // cout << "Depois da desterceirizacao:" << endl;
                // s->info();

                houve_melhora = true;
            }
        }
    }

    if(houve_melhora)
        cout << "Houve melhora na desterceirizacao" << endl;
    else
        cout << "Nao houve melhora na desterceirizacao" << endl;
        
    return houve_melhora;
}



// RVND (Random Variable Neighbourhood Descent, escolhe as estruturas de vizinhança de forma aleatória)
void CVRP::BuscaLocal(Solucao *s, Data *d){
    vector<int> NL = {1, 2, 3, 4, 5}; // alterar a estrutura de dados para maior eficiência
    bool improved = false;
    while(!NL.empty()){
        int n = rand() % NL.size();
        switch(NL[n]){
            case 1:
                improved = melhorouReinsertion(s, d);
                break;
            case 2:
                improved = melhorouSwap(s, d);
                break;
            case 3:
                improved = melhorouShift(s, d);
                break;
            case 4:
                improved = melhorouTerceirizacao(s, d);
                break;
            case 5:
                improved = melhorouDesterceirizacao(s, d);
                break;
        }
        if(improved)
            NL = {1, 2, 3, 4, 5};
        else
            NL.erase(NL.begin() + n);
    }
}
