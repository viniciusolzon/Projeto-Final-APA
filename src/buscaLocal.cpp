#include "CVRP.h"

double CVRP::epsilon(double a, double b){
    return fabs(a + b) * numeric_limits<double>::epsilon() * 15;
}

bool CVRP::melhorou(double valor1, double valor2){
    return (valor1 - valor2) > epsilon(valor1, valor2);
}

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

    bool houve_melhora = false;

    int qtd_rotas = s->get_rotas().size();
    int tamanho_da_rota = 0;
    // Precisamos de uma rota com no mínimo 2 clientes para tentar fazer a troca
    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        // cout << "Verificando swap na rota: " << rota_escolhida+1 << endl;
        tamanho_da_rota = s->get_rotas()[rota_escolhida].size();
        
        if(tamanho_da_rota < 4){
            cout << "Rota muito pequena\n";
            continue;
        }

        int melhor_custo = 0;
        int custo = 0;
        int best_i, best_j;

        // Se a rota escolhida tiver mais que 2 clientes precisamos testar todas combinações possíveis entre os clientes
        for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 2; i++){
            for(int j = i + 1; j < s->get_rotas()[rota_escolhida].size() - 1; j++){
                custo = calculaCustoSwap(s, d, rota_escolhida, i, j);
                if(melhorou(melhor_custo, custo)){
                    melhor_custo = custo;
                    best_i = i;
                    best_j = j;

                }
            }
        }
    
        // Se o swap melhora a solucao, aplicamos o movimento na solucao atual
        if(melhor_custo < 0){
            cout << endl << "Vamos trocar: " << s->get_rotas()[rota_escolhida][best_i] << " com " << s->get_rotas()[rota_escolhida][best_j];
            cout << endl << endl;
            
            // Se houve alguma melhora em qualquer rota retornamos que houve uma melhora na solução
            houve_melhora = true;
        }
    }
    return houve_melhora;
}

int CVRP::calculaCustoShift(Solucao *s, Data *d, int rota1, int rota2, int cliente1, int cliente2){

}

bool CVRP::melhorouShift(Solucao *s, Data *d){
    
    bool houve_melhora = false;

    int qtd_rotas = s->get_rotas().size();
    int tamanho_da_rota = 0;
    // Precisamos de uma rota com no mínimo 2 clientes para tentar fazer a troca
    for(int rota_escolhida = 0; rota_escolhida < qtd_rotas; rota_escolhida++){
        // cout << "Verificando swap na rota: " << rota_escolhida+1 << endl;
        tamanho_da_rota = s->get_rotas()[rota_escolhida].size();
        
        if(tamanho_da_rota < 4){
            cout << "Rota muito pequena\n";
            continue;
        }

        int melhor_custo = 0;
        int custo = 0;
        int best_i, best_j;

        // Se a rota escolhida tiver mais que 2 clientes precisamos testar todas combinações possíveis entre os clientes
        for(int i = 1; i < s->get_rotas()[rota_escolhida].size() - 2; i++){
            for(int j = i + 1; j < s->get_rotas()[rota_escolhida].size() - 1; j++){
                custo = calculaCustoSwap(s, d, rota_escolhida, i, j);
                if(melhorou(melhor_custo, custo)){
                    melhor_custo = custo;
                    best_i = i;
                    best_j = j;

                }
            }
        }
    
        // Se o swap melhora a solucao, aplicamos o movimento na solucao atual
        if(melhor_custo < 0){
            cout << endl << "Vamos trocar: " << s->get_rotas()[rota_escolhida][best_i] << " com " << s->get_rotas()[rota_escolhida][best_j];
            cout << endl << endl;
            
            // Se houve alguma melhora em qualquer rota retornamos que houve uma melhora na solução
            houve_melhora = true;
        }
    }
    return houve_melhora;
}


// RVND (Escolhe as estruturas de vizinhança de forma aleatória)
void CVRP::BuscaLocal(Solucao *s, Data *d){
    if(melhorouSwap(s, d))
        cout << "Houve melhora no swap" << endl;
    else
        cout << "Nao houve melhora no swap" << endl;

    if(melhorouShift(s, d))
        cout << "Houve melhora no shift" << endl;
    else
        cout << "Nao houve melhora no shift" << endl;

}
