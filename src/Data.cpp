#include "Data.h"

void Data::readData(int argc, char **argv){

    string nomeArquivo = argv[1]; // Nome do arquivo de entrada
    ifstream arquivo(nomeArquivo);

    // Abrindo o arquivo
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return;
    }

    // Lendo as informações da instância e guardando na classe CVRP
    arquivo >> this->n;
    arquivo >> this->k;
    arquivo >> this->Q;
    arquivo >> this->L;
    arquivo >> this->r;

    // Atribuindo as respectivas demandas aos clientes
    int demanda;
    for (int i = 0; i < n; i++) {
        arquivo >> demanda;
        this->demandas.push_back(demanda);
    }

    // Atribuindo as respectivas demandas aos clientes
    int custo_terceirizacao;
    for (int i = 0; i < n; i++) {
        arquivo >> custo_terceirizacao;
        this->custos_terceirizacao.push_back(custo_terceirizacao);
    }

    // Aqui embaixo é n+1 pq tem o depósito além dos clientes
    // vector<vector<int>> matrizAdj(n+1, vector<int>(n+1));

    // Aloca memória p matriz
    matrizAdj = new int*[n+1];

    for (int i = 0; i < n+1; i++) {
        matrizAdj[i] = new int[n+1];
    }

    for (int i = 0; i < n+1; i++){
        for (int j = 0; j < n+1; j++){
            arquivo >> matrizAdj[i][j];
        }
    }
}

void Data::info(){
    // Valores
    cout << this->n << "\n";
    cout << this->k << "\n";
    cout << this->Q << "\n";
    cout << this->L << "\n";
    cout << this->r << "\n";
    cout << "\n";

    // Demandas
    for (int i = 0; i < n; i++) {
        cout << this->demandas[i] << ' ';
    }
    cout << "\n";
    cout << "\n";

    // Custos de terceirizacao
    for (int i = 0; i < n; i++) {
    cout << this->custos_terceirizacao[i] << ' ';
    }
    cout << "\n";
    cout << "\n";

    // Matriz de adjacencia
    for (int i = 0; i < n+1; i++){
        for (int j = 0; j < n+1; j++){
            cout << matrizAdj[i][j] << " ";
        }
        cout << '\n';
    }
}
