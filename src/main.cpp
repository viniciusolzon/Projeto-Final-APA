#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>

#include "CVRP.h"

using namespace std;

// n9k5_A = 428
// n9k5_B = 506
// n9k5_C = 559
// n9k5_D = 408
// n14k5_A = 471
// n14k5_B = 565
// n14k5_C = 569
// n14k5_D = 471
// n22k3_A = 605
// n22k3_B = 777
// n22k3_C = 777
// n22k3_D = 605
// n31k5_A = 650
// n31k5_B = 933
// n31k5_C = 939
// n31k5_D = 656
// n43k6_A = 801
// n43k6_B = 1203
// n43k6_C = 1208
// n43k6_D = 802
// n64k9_A = 934
// n64k9_B = 1503
// n64k9_C = 1510
// n64k9_D = 932
// n120k7_A = 1029
// n120k7_B = 2052
// n120k7_C = 2040
// n120k7_D = 1046
// n199k17_A = 1672
// n199k17_B = 3302
// n199k17_C = 3301
// n199k17_D = 1672

void gera_tabela(){

    // Todas instâncias.
    vector<string> instancias = {"n9k5_A", "n9k5_B", "n9k5_C", "n9k5_D",
                                "n14k5_A", "n14k5_B", "n14k5_C", "n14k5_D",
                                "n22k3_A", "n22k3_B", "n22k3_C", "n22k3_D"};

    // Valores ótimos de cada instância.
    vector<int> valores_otimos = {428,506,559,408,471,565,569,471,605,777,777,605};

    // Preparar o arquivo CSV onde os resultados serão salvos.
    std::ofstream arquivoCSV;
    arquivoCSV.open("resultados.csv"); // Abre para escrita, descartando o conteúdo anterior.

    for(int i = 0; i < instancias.size(); i++){
        
        Data dados;
        dados.readData("instances/" + instancias[i] + ".txt");
        CVRP problema(dados);

        // Para as informações sobre o a construção e o RVND na tabela
        cout << "Rodando a instância " << instancias[i] + ".txt\n";
        problema.solve();
        double tempo_construcao = problema.get_solution().tempoConstrucao;
        int solucao_construcao = problema.get_solution().valorConstrucao;
        double tempo_RVND = problema.get_solution().tempoRVND;
        int solucao_RVND = problema.get_solution().valorRVND;

        // Para as informações sobre o ILS na tabela
        int solucao_media_ILS = 0;
        int solucao_melhor_ILS = 0;
        double tempo_medio_ILS = 0;
        double tempo_melhor_ILS = 0;
        double gap_medio_ILS = 0;
        double gap_melhor_ILS = 0;
        // Para o ILS precisamos de uma média, então iremos rodar 10 vezes e pegar a média e melhor execução de cada instância
        int n = 10;
        for(int j = 0; j < n; j++){
            cout << "Rodando o ILS na instância " << instancias[i] + ".txt pela " << j+1 << "º vez\n";
            problema.solveILS();

            if(j==0){
                solucao_melhor_ILS = problema.get_solution().get_custo();
                tempo_melhor_ILS = problema.get_solution().tempoILS;
                gap_melhor_ILS = ( (problema.get_solution().get_custo() - valores_otimos[i]) * 100) / valores_otimos[i];
            }

            // Se acharmos uma melhor solução 'overall' guardamos seu valor, seu tempo e gap do ótimo.
            if(problema.get_solution().get_custo() < solucao_melhor_ILS){
                solucao_melhor_ILS = problema.get_solution().get_custo();
                tempo_melhor_ILS = problema.get_solution().tempoILS;
                gap_melhor_ILS = ( (problema.get_solution().get_custo() - valores_otimos[i]) * 100) / valores_otimos[i];
            }

            // Soma pra calcular os valores médios depois
            solucao_media_ILS += problema.get_solution().get_custo();
            tempo_medio_ILS += problema.get_solution().tempoILS;
            gap_medio_ILS += ( (problema.get_solution().get_custo() - valores_otimos[i]) * 100) / valores_otimos[i];
        }

        solucao_media_ILS /=n;
        tempo_medio_ILS /=n;
        gap_medio_ILS /=n;


        // Escreve o cabeçalho do CSV. Note que adicionamos cabeçalhos para os novos dados.
        if(i == 0)
            arquivoCSV << "Instancia,Valor Otimo,Solucao Construcao,Tempo Construcao,GAP Construcao,Solucao RVND,Tempo RVND,GAP RVND,"
             << "Media Solucao ILS,Melhor Solucao ILS,Tempo Medio ILS,Tempo Melhor ILS,GAP Medio ILS,GAP Melhor ILS\n";

        double gapConstrucao = ( (solucao_construcao - valores_otimos[i]) * 100) / valores_otimos[i];
        double gapRVND = ( (solucao_RVND - valores_otimos[i]) * 100) / valores_otimos[i];

        double gapILS = ( (problema.get_solution().get_custo() - valores_otimos[i]) * 100) / valores_otimos[i];
        
        string instancia = dados.get_nome_instancia();
        instancia.erase(0,10);
        // Escrever os resultados no arquivo CSV.
        arquivoCSV << instancia << "," << valores_otimos[i] << "," << solucao_construcao << ","
                << tempo_construcao << "," << gapConstrucao << "%," << solucao_RVND << ","
                << tempo_RVND << "," << gapRVND << "%," << solucao_media_ILS << ","
                << solucao_melhor_ILS << "," << tempo_medio_ILS << ","  << tempo_melhor_ILS << ","
                << gap_medio_ILS  << "%," << gap_melhor_ILS << "%\n";

    }
    // Fechar o arquivo CSV.
    arquivoCSV.close();
}


int main(int argc, char *argv[]){

    if(argc >=4){
        cout << "Muitos argumentos.\nAbortando..." << endl;
        return 0;
    }


    // Roda todas as instâncias listadas e cria a tabela com as informações obre cada instância rodada
    if(argc == 1) // ./cvrp
        gera_tabela();

    // Roda a instância selecionada apenas com a heurística construtita e com o RVND e gera o output dessa instância
    if(argc == 2){ // ./cvrp instances/instancia_desejada
        Data dados;
        dados.readData(argv[1]);

        CVRP problema(dados);
        problema.solve();
        problema.gera_output();
    }

    // Roda a instância selecionada com a heurística construtita, com o RVND e com a perturbação aplicados ao ILS
    // e gera o output dessa instância
    if(argc == 3){ // ./cvrp instances/instancia_desejada ils
        Data dados;
        dados.readData(argv[1]);

        CVRP problema(dados);
        problema.solveILS();
    }

}
