#include "CVRP.h"

double CVRP::epsilon(double a, double b){
    return fabs(a + b) * numeric_limits<double>::epsilon() * 15;
}

bool CVRP::melhorou(double valor1, double valor2){
    return (valor1 - valor2) > epsilon(valor1, valor2);
}

int CVRP::calculaCustoSwap(Solucao *s, Data *d, int rota, int cliente1, int cliente2){
    int antes = 0, depois = 0, custo = 0;
    // cout << "ENTROU EM calculaCustoSwap" << endl;
    if(cliente2 == cliente1 + 1){
        // cout << "Em cima" << endl;
        antes = d->get_custo(s->get_rotas()[rota][cliente1 - 1], s->get_rotas()[rota][cliente1]) + d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente1]) + d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente2 + 1]);
        depois = d->get_custo(s->get_rotas()[rota][cliente1 - 1], s->get_rotas()[rota][cliente2]) + d->get_custo(s->get_rotas()[rota][cliente2], s->get_rotas()[rota][cliente1]) + d->get_custo(s->get_rotas()[rota][cliente1], s->get_rotas()[rota][cliente2 + 1]);
        
        custo = depois - antes;
        // cout << "Custo depois da troca = " << custo << endl;
    }
    else{
        // cout << "Em baixo" << endl;
        antes = d->get_custo(s->get_rotas()[rota][cliente1 - 1], s->get_rotas()[rota][cliente1]) + d->get_custo(s->get_rotas()[rota][cliente1 + 1], s->get_rotas()[rota][cliente1]) + d->get_custo(s->get_rotas()[rota][cliente2 - 1], s->get_rotas()[rota][cliente2]) + d->get_custo(s->get_rotas()[rota][cliente2 + 1], s->get_rotas()[rota][cliente2]);
        depois = d->get_custo(s->get_rotas()[rota][cliente1 - 1], s->get_rotas()[rota][cliente2]) + d->get_custo(s->get_rotas()[rota][cliente1 + 1], s->get_rotas()[rota][cliente2]) + d->get_custo(s->get_rotas()[rota][cliente2 - 1], s->get_rotas()[rota][cliente1]) + d->get_custo(s->get_rotas()[rota][cliente2 + 1], s->get_rotas()[rota][cliente1]);

        custo = depois - antes;
        // cout << "Custo depois da troca = " << custo << endl;
    }

    return custo;
}

bool CVRP::melhorouSwap(Solucao *s, Data *d){
    // Vamos escolher uma rota aleatória para aplicar o Swap( troca um cliente de lugar)
    int qtd_rotas = s->get_rotas().size();
    int rota_escolhida = 1;

    srand(time(0));
    int tamanho_da_rota = 0;
    // Precisamos de uma rota com no mínimo 2 clientes para tentar fazer a troca
    while(tamanho_da_rota <=1){
        rota_escolhida = (rand() % qtd_rotas);
        cout << endl << "Rota escolhida para aplicar o swap: " << rota_escolhida+1 << endl;
        tamanho_da_rota = s->get_rotas()[rota_escolhida].size();
        cout << "Tamanho da rota escolhida: " << tamanho_da_rota << endl;
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
                cout << endl << endl << "Melhorou a rota, vamos trocar " << s->get_rotas()[rota_escolhida][best_i] << " com " << s->get_rotas()[rota_escolhida][best_j];
                cout << endl << endl;
            }
        }
    }

    // Se o swap melhora a solucao, aplicamos o movimento na solucao atual
    if(melhor_custo < 0){
        int aux = s->get_rotas()[rota_escolhida][best_i];
        s->atualizaRota(rota_escolhida, best_i, s->get_rotas()[rota_escolhida][best_j]);
        s->atualizaRota(rota_escolhida, best_j, aux);
        s->atualiza_custo(s->get_custo() + melhor_custo);
        
        return true;
    }

    return false;
}


void CVRP::BuscaLocal(Solucao *s, Data *d){
    cout << endl;
    cout << endl;
    cout << "Entrou na busca local";
    cout << endl;
    cout << endl;
    melhorouSwap(s, d);
}
