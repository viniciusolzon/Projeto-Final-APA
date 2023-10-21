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

    int custo = 0;
    int melhor_custo = 0;
    int melhor_cliente = 0;
    int melhor_rota = 0;
    int melhor_posicao = 0;

    // Precisamos de uma rota com no mínimo 2 clientes para tentar fazer a reinserção
    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        tamanho_da_rota = s->get_rotas()[rota_escolhida].size();

        if(tamanho_da_rota < 4)
            continue;

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
                    melhor_rota = rota_escolhida;
                    melhor_custo = custo;
                    melhor_cliente = i;
                    melhor_posicao = j;
                }
            }
        }
    }

    // Se o reisertion melhora a solucao, aplicamos o movimento na solucao atual
    if(melhor_custo < 0){

        int elemento = s->get_rotas()[melhor_rota][melhor_cliente];

        if(melhor_cliente > melhor_posicao){
            s->removeDaRota(melhor_rota, melhor_cliente);
            s->insereNaRota(melhor_rota, melhor_posicao, elemento);
        }
        else{
            s->insereNaRota(melhor_rota, melhor_posicao, elemento);
            s->removeDaRota(melhor_rota, melhor_cliente);
        }

        s->atualiza_custo(s->get_custo() + melhor_custo);
        
        // Se houve alguma melhora em qualquer rota retornamos que houve uma melhora na solução
        houve_melhora = true;
    }
    
    if(houve_melhora){
        // cout << "Houve melhora no reinsertion" << endl;
    }
    else{
        // cout << "Nao houve melhora no reinsertion" << endl;
    }

    return houve_melhora;
}


// SWAP
int CVRP::calculaCustoSwap(Solucao *s, Data *d, int rota, int cliente1, int cliente2){
    int antes = 0, depois = 0, custo = 0;

    // cout << "Verificando swap do cliente " << s->get_rotas()[rota][cliente1] << " com o cliente " << s->get_rotas()[rota][cliente2] << endl;
    // Para a perturbação, pode pegar um i maior que o j lá
    if(cliente1 == cliente2 + 1){
        antes = d->get_custo(s->get_rotas()[rota][cliente2-1], s->get_rotas()[rota][cliente2]);
        antes+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente2+1]);
        antes+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente1+1]);

        depois = d->get_custo(s->get_rotas()[rota][cliente2-1], s->get_rotas()[rota][cliente1]);
        depois+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente2]);
        depois+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente1+1]);
    }

    else{
        if(cliente2 == cliente1 + 1){
            antes = d->get_custo(s->get_rotas()[rota][cliente1-1], s->get_rotas()[rota][cliente1]);
            antes+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente1+1]);
            antes+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente2+1]);

            depois = d->get_custo(s->get_rotas()[rota][cliente1-1], s->get_rotas()[rota][cliente2]);
            depois+= d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente1]);
            depois+= d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente2+1]);
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
    }

    custo = depois - antes;
    return custo;
}

// aplica o melhor swap da melhor melhora
bool CVRP::melhorouSwap(Solucao *s, Data *d){

    int qtd_rotas = s->get_rotas().size();
    int tamanho_da_rota = 0;
    bool houve_melhora = false;

    int custo = 0;
    int melhor_custo = 0;
    int melhor_cliente1, melhor_cliente2 = 0;;
    int melhor_rota = 0;

    // Precisamos de uma rota com no mínimo 2 clientes para tentar fazer a troca
    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        tamanho_da_rota = s->get_rotas()[rota_escolhida].size();
        
        if(tamanho_da_rota < 4)
            continue;
        
        // Se a rota escolhida tiver mais que 2 clientes precisamos testar todas combinações possíveis entre os clientes
        for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 2; i++){
            for(int j = i + 1; j < s->get_rotas()[rota_escolhida].size() - 1; j++){
                custo = calculaCustoSwap(s, d, rota_escolhida, i, j);

                int teste = calculaCustoSwap(s, d, rota_escolhida, j, i);
                if(teste != custo){
                    cout << "CUSTOS DIFEREM DO SWAP ENTRE (i,j) E (j,i)" << endl;
                    cout << "Custo de swap entre i e j: " << custo << endl;
                    cout << "Custo de swap entre j e i: " << teste << endl;
                    getchar();
                }

                if(melhorou(melhor_custo, custo)){
                    melhor_custo = custo;
                    melhor_rota = rota_escolhida;
                    melhor_cliente1 = i;
                    melhor_cliente2 = j;
                }
            }
        }
    }

    // Se o swap melhora a solucao, aplicamos o movimento na solucao atual
    if(melhor_custo < 0){

        int aux = s->get_rotas()[melhor_rota][melhor_cliente1];
        s->atualizaRota(melhor_rota, melhor_cliente1, s->get_rotas()[melhor_rota][melhor_cliente2]);
        s->atualizaRota(melhor_rota, melhor_cliente2, aux);
        s->atualiza_custo(s->get_custo() + melhor_custo);
        
        // Se houve alguma melhora em qualquer rota retornamos que houve uma melhora na solução
        houve_melhora = true;
    }

    if(houve_melhora){
        // cout << "Houve melhora no swap" << endl;
    }
    else{
        // cout << "Nao houve melhora no swap" << endl;
    }

    return houve_melhora;
}


// 2-OPT
int CVRP::calculaCusto2opt(Solucao *s, Data *d, int rota, int cliente1, int cliente2){
    // falta fazer
    return 1;
}

bool CVRP::melhorou2opt(Solucao *s, Data *d){
    // falta fazer
    return true;
}


// SHIFT
int CVRP::calculaCustoShift(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2){ // Verificado ta certo
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

bool CVRP::verificaCapacidadeRotas(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2){ // Verificado ta certo
    int capacidade1 = s->get_capacidadeRota(rota1);
    int capacidade2 = s->get_capacidadeRota(rota2);

    cliente1 = s->get_rotas()[rota1][cliente1];
    cliente2 = s->get_rotas()[rota2][cliente2];
    int demanda1 = s->get_clientes()[cliente1-1]->get_demanda();
    int demanda2 = s->get_clientes()[cliente2-1]->get_demanda();

    // Tira o cliente da rota dele e coloca o outro cliente da outra rota e verifica se a capacidade n será excedida
    if((capacidade1 + demanda1 - demanda2) < 0){
        // Não dá pra aplicar o shift
        return true;
    }
    if((capacidade2 + demanda2 - demanda1) < 0){
        // Não dá pra aplicar o shift
        return true;
    }
    
    // Dá pra aplicar o shift
    return false;
}

bool CVRP::melhorouShift(Solucao *s, Data *d){

    int qtd_rotas = s->get_rotas().size();

    int melhor_custo = 0;
    int custo = 0;
    int melhor_cliente1, melhor_cliente2;
    int melhor_rota1, melhor_rota2;
    bool capacidade_excedida = false;
    bool houve_melhora = false;

    // Passamos por todas as rotas pra tentar o shift
    for(int rota1_escolhida = 0; rota1_escolhida < qtd_rotas-1; rota1_escolhida++){

        for(int rota2_escolhida = rota1_escolhida + 1; rota2_escolhida < qtd_rotas; rota2_escolhida++){

            // Testa o shift com todos os clientes da rota1 com todos os clientes da rota2
            for(int i = 1; i < s->get_rotas()[rota1_escolhida].size() - 1; i++){
                
                for(int j = 1; j < s->get_rotas()[rota2_escolhida].size() - 1; j++){
                    // Antes de ver se vale a pena trocar os clientes entre rotas, é preciso primeiro
                    // verificar se a capacidade de nenhuma das rota será excedida ao trocar os clientes
                    capacidade_excedida = verificaCapacidadeRotas(s, d, rota1_escolhida, rota2_escolhida, i, j);
                    // Se a capacidade de alguma das rotas for excedida, tentamos trocar outra troca entre clientes
                    if(capacidade_excedida){
                        continue;
                    }

                    // Se não, verificamos se vale a pena trocar eles baseado no custo das rotas
                    custo = calculaCustoShift(s, d, rota1_escolhida, rota2_escolhida, i, j);
                    if(melhorou(melhor_custo, custo)){
                        melhor_custo = custo;
                        melhor_rota1 = rota1_escolhida;
                        melhor_rota2 = rota2_escolhida;
                        melhor_cliente1 = i;
                        melhor_cliente2 = j;
                    }
                }
            }
        }
    }

    // Se o shift melhora a solucao, aplicamos o movimento na solucao atual
    if(melhor_custo < 0){

        int cliente1 = s->get_rotas()[melhor_rota1][melhor_cliente1];
        int cliente2 = s->get_rotas()[melhor_rota2][melhor_cliente2];
        int demanda_cliente1 = s->get_clientes()[cliente1-1]->get_demanda();
        int demanda_cliente2 = s->get_clientes()[cliente2-1]->get_demanda();
        int capacidade_rota1 = s->get_capacidadeRota(melhor_rota1);
        int capacidade_rota2 = s->get_capacidadeRota(melhor_rota2);
        // retirar o cliente 1 da rota 1 e colocar o 2 no lugar dele
        int nova_capacidade_rota1 = capacidade_rota1 + demanda_cliente1 - demanda_cliente2;
        // retirar o cliente 2 da rota 2 e colocar o 1 no lugar dele
        int nova_capacidade_rota2 = capacidade_rota2 + demanda_cliente2 - demanda_cliente1;

        // atualiza a capacidade das 2 rotas após a alteração
        s->atualizaCapacidade(melhor_rota1, nova_capacidade_rota1);
        s->atualizaCapacidade(melhor_rota2, nova_capacidade_rota2);

        int aux = s->get_rotas()[melhor_rota1][melhor_cliente1];
        s->atualizaRota(melhor_rota1, melhor_cliente1, s->get_rotas()[melhor_rota2][melhor_cliente2]);
        s->atualizaRota(melhor_rota2, melhor_cliente2, aux);
        s->atualiza_custo(s->get_custo() + melhor_custo);

        houve_melhora = true;
    }

    if(houve_melhora){
        // cout << "Houve melhora no shift" << endl;
    }
    else{
        // cout << "Nao houve melhora no shift" << endl;
    }

    return houve_melhora;
}


// TERCEIRIZAÇÃO
bool CVRP::verificaLimiteL(Solucao *s, Data *d){ // Verificado ta certo

    // A quantidade de clientes não terceirizados tem que ser maior do que o limite L
    // Tem q ser MAIOR pq se for igual ao terceirizar a qtd de clientes não terceirizados
    // vai ser menos do que o limite L
    // cout << "Numero de clientes: " << d->get_n() << endl;
    // cout << "Numero de clientes terceirizados: " << s->get_num_clientesTerceirizados() << endl;
    // cout << "Quantidade minima de entregas nao terceirizadas: " << d->get_L() << endl;
    if(d->get_n() - s->get_num_clientesTerceirizados() > d->get_L()){
        // cout << "Da pra terceirizar" << endl;
        return true;
    }
    
    // cout << "Nao da pra terceirizar" << endl;
    return false;
}

int CVRP::calculaCustoTerceirizacao(Solucao *s, Data *d, int rota, int cliente){ // Verificado ta certo
    int antes = 0, depois = 0, custo = 0;

    // Vamos tirar ele da rota
    // Remover o custos de suas arestas e adicionar os custos das novas arestas e o de terceirizacao
    antes = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][cliente]);
    antes+= d->get_custo(s->get_rotas()[rota][cliente], s->get_rotas()[rota][cliente+1]);

    depois = d->get_custo(s->get_rotas()[rota][cliente-1], s->get_rotas()[rota][cliente+1]);
    depois+= d->get_custos_terceirizacao()[s->get_rotas()[rota][cliente]-1]; // custo de terceirizar aquele cliente


    custo = depois - antes;
    return custo;
}

bool CVRP::melhorouTerceirizacao(Solucao *s, Data *d){ // Verificado ta certo

    int melhor_rota;
    int melhor_terceirizacao;
    bool houve_melhora = false;
    bool limite_respeitado = false;
    int melhor_custo = 0;
    int custo = 0;

    int qtd_rotas = s->get_rotas().size();


    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        // Testa se vale a pena terceirizar algum cliente da rota atual

        for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 1; i++){

            // Antes de calcular o custo da terceirização do cliente, é preciso verificar
            // se dá pra terceirizar respeitando o limite mínimo L de entregas não terceirizadas
            limite_respeitado = verificaLimiteL(s, d);
            if(!limite_respeitado)
                continue;

            custo = calculaCustoTerceirizacao(s, d, rota_escolhida, i);
            if(melhorou(melhor_custo, custo)){
                melhor_rota = rota_escolhida;
                melhor_custo = custo;
                melhor_terceirizacao = i;
            }
        }
    }

    // Se terceirizar algum cliente na rota atual é melhor para a solucao, já terceirizamos aquele cliente
    if(melhor_custo < 0){

        int cliente = s->get_rotas()[melhor_rota][melhor_terceirizacao];
        int demanda_cliente = s->get_clientes()[cliente-1]->get_demanda();
        int capacidade_rota = s->get_capacidadeRota(melhor_rota);
        // retirar o cliente da rota 
        int nova_capacidade_rota = capacidade_rota + demanda_cliente;


        // atualiza a capacidade da rota após a alteração
        s->atualizaCapacidade(melhor_rota, nova_capacidade_rota);

        s->terceirizaCliente(cliente);

        s->removeDaRota(melhor_rota, melhor_terceirizacao);
        s->atualiza_custo(s->get_custo() + melhor_custo);

        // Se o segundo 'cliente' da rota agora for um zero, quer dizer que a rota está vazia,
        // então diminuimos a quantidade de veiculos utilizados
        if(s->get_rotas()[melhor_rota][1] == 0){
            s->atualiza_custo(s->get_custo() - d->get_r());
        }

        houve_melhora = true;
    }

    if(houve_melhora){
        // cout << "Houve melhora na terceirizacao" << endl;
    }
    else{
        // cout << "Nao houve melhora na terceirizacao" << endl;
    }

    return houve_melhora;
}


// DESTERCEIRIZACAO
bool CVRP::verificaCapacidade(Solucao *s, Data *d, int rota, int cliente_terceirizado){ // Verificado ta certo

    int demanda_cliente = d->get_demandas()[cliente_terceirizado-1];
    int capacidade_restante = s->get_capacidadeRota(rota);

    if(demanda_cliente > capacidade_restante){
        return false;
    }

    return true;
}

int CVRP::calculaCustoDesterceirizacao(Solucao *s, Data *d, int rota, int posicao, int cliente_terceirizado){ // Verificado ta certo
    int antes = 0, depois = 0, custo = 0;

    // Vamos colocar ele da rota
    // Adicionar o custos de suas novas arestas e remover os custos das antigas arestas e o de terceirizacao
    antes = d->get_custo(s->get_rotas()[rota][posicao], s->get_rotas()[rota][posicao+1]);
    antes+= d->get_custos_terceirizacao()[cliente_terceirizado-1]; // custo de terceirizar aquele cliente

    depois = d->get_custo(s->get_rotas()[rota][posicao], cliente_terceirizado);
    depois+= d->get_custo(cliente_terceirizado, s->get_rotas()[rota][posicao+1]);

    custo = depois - antes;
    return custo;
}

bool CVRP::melhorouDesterceirizacao(Solucao *s, Data *d){ // Verificado ta certo

    // Primeiro verifica se tem algum cliente terceirizado
    if(s->get_num_clientesTerceirizados() == 0){
        return false;
    }

    int melhor_rota;
    int melhor_lugar;
    bool houve_melhora = false;
    bool capacidade_respeitada = false;
    int cliente_terceirizado = 0;
    int melhor_cliente = 0;
    int index_melhor_cliente = 0;

    int melhor_custo = 0;
    int custo = 0;

    int qtd_rotas = s->get_rotas().size();

    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){

        for(int index = 0; index < s->get_num_clientesTerceirizados(); index++){

            cliente_terceirizado = s->get_clientes_terceirizados()[index];
            
            // Antes de calcular o custo da desterceirização do cliente, é preciso verificar
            // se dá pra desterceirizar esse cliente na rota atual respeitando o limite do veiculo
            capacidade_respeitada = verificaCapacidade(s, d, rota_escolhida, cliente_terceirizado);
            if(!capacidade_respeitada)
                continue;

            // Testa se vale a pena desterceirizar algum cliente da rota atual
            for(int i = 0; i < s->get_rotas()[rota_escolhida].size() - 1; i++){

                custo = calculaCustoDesterceirizacao(s, d, rota_escolhida, i, cliente_terceirizado);
                if(melhorou(melhor_custo, custo)){
                    melhor_rota = rota_escolhida;
                    melhor_cliente = cliente_terceirizado;
                    index_melhor_cliente = index;

                    melhor_custo = custo;
                    melhor_lugar = i;
                }
            }
        }
    }

    // Se desterceirizar algum cliente na rota atual é melhor para a solucao, já desterceirizamos aquele cliente
    if(melhor_custo < 0){

        int demanda_cliente = s->get_clientes()[melhor_cliente-1]->get_demanda();
        int capacidade_rota = s->get_capacidadeRota(melhor_rota);
        // retirar o cliente da rota 
        int nova_capacidade_rota = capacidade_rota - demanda_cliente;

        // atualiza a capacidade da rota após a alteração
        s->atualizaCapacidade(melhor_rota, nova_capacidade_rota);

        s->desterceirizaCliente(index_melhor_cliente);

        // Se o segundo 'cliente' da rota iremos inserir for um zero, quer dizer que a rota está vazia,
        //  então aumentamos a quantidade de veiculos utilizados
        if(s->get_rotas()[melhor_rota][1] == 0){
            s->atualiza_custo(s->get_custo() + d->get_r());
        }

        s->insereNaRota(melhor_rota, melhor_lugar, melhor_cliente);

        s->atualiza_custo(s->get_custo() + melhor_custo);

        houve_melhora = true;
    }

    if(houve_melhora){
        // cout << "Houve melhora na desterceirizacao" << endl;
    }
    else{
        // cout << "Nao houve melhora na desterceirizacao" << endl;
    }
        
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
