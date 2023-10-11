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
            return true;
        }
    }
    cout << "Nao houve melhora no reinsertion" << endl;
    return false;
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

bool CVRP::melhorouSwap(Solucao *s, Data *d){

    int qtd_rotas = s->get_rotas().size();
    int tamanho_da_rota = 0;
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
            return true;
        }
    }
    cout << "Nao houve melhora no swap" << endl;
    return false;
}


// 2-OPT
int CVRP::calculaCusto2opt(Solucao *s, Data *d, int rota, int cliente1, int cliente2){


}

bool CVRP::melhorou2opt(Solucao *s, Data *d){


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


// RVND (Random Variable Neighbourhood Descent, escolhe as estruturas de vizinhança de forma aleatória)
void CVRP::BuscaLocal(Solucao *s, Data *d){
    vector<int> NL = {1, 2, 3};
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
        }
        if(improved)
            NL = {1, 2, 3};
        else
            NL.erase(NL.begin() + n);
    }
}
