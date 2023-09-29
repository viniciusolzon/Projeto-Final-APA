#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>


using namespace std;

int INT_MAX = 100;


struct Cliente {
    int demanda;
    int custoTerceirizacao;
    bool atendido;
    
};

int clienteMaisProximo(int atual, const vector<Cliente>& clientes, const vector<vector<int>>& matrizCustos, int capacidadeAtual) {
    int minCusto = INT_MAX;
    int idx = -1;
    for (int i = 0; i < clientes.size(); i++) {
        if (!clientes[i].atendido && clientes[i].demanda <= capacidadeAtual && matrizCustos[atual][i] < minCusto) {
            minCusto = matrizCustos[atual][i];
            idx = i;
        }
    }
    return idx;
}

int main(int argc, char *argv[]) {
    string nomeArquivo = argv[1]; // Nome do arquivo de entrada
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return 1;
    }

    int n, k, Q, L, r;
    arquivo >> n >> k >> Q >> L >> r;

    vector<Cliente> clientes(n);
    cout << "Demanda dos clientes:\n";
    for (int i = 0; i < n; i++) {
        arquivo >> clientes[i].demanda;
        cout << clientes[i].demanda << '\n';
    }

    cout << "Custo de terceirização dos clientes:\n";
    for (int i = 0; i < n; i++) {
        arquivo >> clientes[i].custoTerceirizacao;
        cout << clientes[i].custoTerceirizacao << '\n';
    }

    // Assumindo que a matriz de custos é nxn
    vector<vector<int>> matrizCustos(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arquivo >> matrizCustos[i][j];
        }
    }

    arquivo.close();

    for (int i = 0; i < k; i++) { // para cada veículo
        int capacidadeAtual = Q;
        int atual = 0; // começa no depósito
        while (capacidadeAtual > 0) {
            int proximo = clienteMaisProximo(atual, clientes, matrizCustos, capacidadeAtual);
            if (proximo == -1) break; // nenhum cliente viável encontrado
            clientes[proximo].atendido = true;
            capacidadeAtual -= clientes[proximo].demanda;
            atual = proximo;
        }
    }

    return 0;
}
