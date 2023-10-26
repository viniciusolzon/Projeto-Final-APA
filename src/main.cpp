#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>

#include "CVRP.h"

using namespace std;

void gera_tabela(){

    // Todas instâncias.
    vector<string> instancias = {"n9k5_A", "n9k5_B", "n9k5_C", "n9k5_D",
                                "n14k5_A", "n14k5_B", "n14k5_C", "n14k5_D",
                                "n22k3_A", "n22k3_B", "n22k3_C", "n22k3_D",
                                "n31k5_A", "n31k5_B", "n31k5_C", "n31k5_D",
                                "n43k6_A", "n43k6_B", "n43k6_C", "n43k6_D",
                                "n64k9_A", "n64k9_B", "n64k9_C", "n64k9_D",
                                "n120k7_A", "n120k7_B", "n120k7_C", "n120k7_D"};
    

    // Valores ótimos de cada instância.
    vector<int> valores_otimos = {428,506,559,408,471,565,569,471,605,777,777,605,650,933,939,656,801,1203,1208,802,934,1503,1510,932,1029,2052,2040,1046};

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
        problema.gera_output();
    }

}
