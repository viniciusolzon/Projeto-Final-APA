#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>

using namespace std;

// ILS

// class ILS{
//     int maxIter;
//     int maxIterILS;
//     Data dist;
//     Solucao best_solution;

// public:
//     ILS(Data d); // CONSTRUTOR
//     ~ILS(); // DESCONSTRUTOR

//     Solucao Construcao(Data *d);

//     void BuscaLocal(Solucao *s, Data *d);

//     Solucao Pertubacao(Solucao *s, Data *d);

//     void solve();
//     void show_solution();
// };

// auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro
//     srand(time(NULL)); // Para conseguir gerar números aleatórios
//     Solucao bestOfAll; // Cria a solução que guardará a melhor solução possível

//     for(int i = 0; i < maxIter; i++){
//         Solucao atual = Construcao(&dist); // Cria a solução atual ( Muita gula e pouca aleatoriedade )
//         calcularcost(&atual, &dist); // Calcula o custo dessa solução com base na matriz de adjacência
//         Solucao best = atual; // Cria a solução que guardará a melhor solução dessa iteração
//         if(i == 0)
//             bestOfAll = atual; // Se for a primeira solução criada ela já é atribuída para a melhor solução possível

//         int iterIls = 0;
//         while(iterIls <= maxIterILS){ // Enquanto houver melhoras nessa solução, reinicia-se o processo de melhora
//             BuscaLocal(&atual, &dist); // Tenta melhorar o máximo possível a solução atual
//             if(improve(best.cost, atual.cost)){ // Se a solução atual foi melhorada a melhor solução atual é atualizada pela atual
//                 best = atual;
//                 iterIls = 0;
//             }
//             atual = Pertubacao(&best, &dist); // Pertuba de forma aleatória a solução atual pra ver se ao alterá-la é possível melhorá-la
//             iterIls++;
//         }

//         if(improve(bestOfAll.cost, best.cost)) // Se a melhor solução atual for melhor que a melhor solução possível, ela se torna a melhor solução possível
//             bestOfAll = best;
//     }

//     auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
//     std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
//     cout << "\nExecution time:  " << float_ms.count() / 1000.0000000000000 << " seconds" << "\n";

//     best_solution = bestOfAll;


class Cliente{
    private:
        int demanda;
        int custoTerceirizacao;
        bool atendido;

    public:
        Cliente(int demanda, int custoTerceirizacao, bool atendido); // Construtor
        ~Cliente(); // Destrutor

        // Métodos gets e sets para os atributos da classe Cliente
        int get_demanda(){
            return this->demanda;
        }
        void set_demanda(int d){
            this->demanda = d;
        }

        int get_custoTerceirizacao(){
            return this->custoTerceirizacao;
        }
        void set_custoTerceirizacao(int c){
            this->custoTerceirizacao = c;
        }

        bool foiAtendido(){
            return this->atendido;
        }
        void set_atendido(bool a){
            this->atendido = a;
        }

};

// Capacitaded Vehicle Routing Problem
class CVRP{
    private:
        // int maxIterILS; // caso decidirmos utilizar a meta-heurística ILS (Iterated Local Search), tenho ela fácil implementada já
        int maxIter;
        Data dados;
        Solucao melhor_solucao;

    public:
        CVRP(Data d); // Construtor da classe
        ~CVRP(); // Destrutor da classe

        Solucao construcao(Data *d); // A heurística construtiva utilizada será a do 'Vizinho mais pŕoximo'

        void solve();
        void show_solution();
};


struct Data{
    double **matrizAdj; // matriz de adjacencia da instancia (não é simétrica)
    int num_vertices; // qtd de vertices na instancia
};

struct Solucao{
    vector<vector<int>> rotas; // vector de vector de rotas, cada vector vai conter uma rota
    double custo; // custo total da solucao
};

void CVRP :: solve(){

    for (int i = 0; i < k; i++) { // para cada veículo
        int capacidadeAtual = Q;
        int atual = 0; // começa no depósito
        while (capacidadeAtual > 0) {
            int proximo = clienteMaisProximo(atual, clientes, matrizCustos, capacidadeAtual);
            if (proximo == -1) break; // nenhum cliente viável encontrado
            clientes[proximo].foiAtendido(true);
            capacidadeAtual -= clientes[proximo].get_demanda();
            atual = proximo;
        }
    }
}


Solucao CVRP :: construcao(Data *d){
    Solucao atual; // cria uma solucao vazia



    return atual
}

int CVRP :: vizinhoMaisProximo(){
    mais_proximo = 0

    return mais_proximo
}

// Solucao CVRP :: Construcao(Data *d){
//     Solucao atual;
//     vector<int> CL;

//     for (int i = 2; i <= d->vertices; i++){
//         CL.push_back(i);
//     }

//     atual.sequence = {1};

//     random_shuffle(CL.begin(), CL.end());
//     for(int i = 0; i < 2; i++){
//         atual.sequence.push_back(CL[i]);
//         CL.erase(remove(CL.begin(), CL.end(), CL[i]), CL.end());
//     }

//     atual.sequence.push_back(1);
     
//     while(!CL.empty()){
//         vector<InsertionInfo> insertioncost;
//         insertioncost = calcularCustoInsercao(&atual, d, CL);

//         sort(insertioncost.begin(), insertioncost.end(), compare);

//         double alpha = (double) rand() / RAND_MAX;
//         int selecionado = rand() % ((int) ceil(alpha * insertioncost.size()));

//         atual.sequence.insert(atual.sequence.begin() + insertioncost[selecionado].remover + 1, insertioncost[selecionado].inserir);

//         CL.erase(remove(CL.begin(), CL.end(), insertioncost[selecionado].inserir), CL.end());
//     }
//     // calcularcost(&atual, d);

//     return atual;
// }
