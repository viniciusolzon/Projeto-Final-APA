#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>

#include "CVRP.h"

using namespace std;

void gera_tabela(){

    vector<string> instancias = {"n9k5_A", "n9k5_B", "n9k5_C", "n9k5_D",
                                "n14k5_A", "n14k5_B", "n14k5_C", "n14k5_D",
                                "n22k3_A", "n22k3_B", "n22k3_C", "n22k3_D"};

    // Preparar o arquivo CSV onde os resultados serão salvos.
    std::ofstream arquivoCSV;
    arquivoCSV.open("resultados.csv"); // Abre para escrita, descartando o conteúdo anterior.

    for(int i = 0; i < instancias.size(); i++){
        
        Data dados;
        dados.readData("instances/" + instancias[i] + ".txt");

        CVRP problema(dados);
        cout << "Rodando a instância " << instancias[i] + ".txt" << endl;
        problema.solve();
        double tempo_construcao = problema.get_solution().tempoConstrucao;
        int solucao_construcao = problema.get_solution().valorConstrucao;
        double tempo_RVND = problema.get_solution().tempoRVND;
        int solucao_RVND = problema.get_solution().valorRVND;

        cout << "Rodando o ILS na instância " << instancias[i] + ".txt" << endl;
        problema.solveILS();

        // Mapeia os valores ótimos de cada instância.
        vector<int> valores_otimos = {428,506,559,408,471,565,569,471,605,777,777,605};

        // Escreve o cabeçalho do CSV. Note que adicionamos cabeçalhos para os novos dados.
        if(i == 0)
            arquivoCSV << "Instancia,Valor Otimo,Solucao Construcao,Tempo Construcao,GAP Construcao,Solucao RVND,Tempo RVND,GAP RVND,Solucao ILS,Tempo ILS,GAP ILS\n";

        double gapConstrucao = ( (solucao_construcao - valores_otimos[i]) * 100) / valores_otimos[i];
        double gapRVND = ( (solucao_RVND - valores_otimos[i]) * 100) / valores_otimos[i];
        double gapILS = ( (problema.get_solution().get_custo() - valores_otimos[i]) * 100) / valores_otimos[i];
        
        string instancia = dados.get_nome_instancia();
        instancia.erase(0,10);
        // Escrever os resultados no arquivo CSV.
        arquivoCSV << instancia << "," << valores_otimos[i] << "," << solucao_construcao << "," 
                << tempo_construcao << "," << gapConstrucao << "%," << solucao_RVND << ","
                << tempo_RVND << "," << gapRVND << "%," << problema.get_solution().get_custo() << ","
                << problema.get_solution().tempoILS << "," << gapILS << "%\n";

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
