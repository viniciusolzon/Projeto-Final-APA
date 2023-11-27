#include "Data.h"

int Data::get_n(){
    return n;
}

int Data::get_k(){
    return k;
}

int Data::get_Q(){
    return Q;
}

int Data::get_L(){
    return L;
}

int Data::get_r(){
    return r;
}

vector<int> Data::get_demandas(){
    return demandas;
}

vector<int> Data::get_custos_terceirizacao(){
    return custos_terceirizacao;
}

int Data::get_custo(int i, int j){
    return matrizAdj[i][j];
}


void Data::readData(string filename){
    
    string nomeArquivo = filename; // Nome do arquivo de entrada
    
    this->nome_instancia = nomeArquivo;

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
