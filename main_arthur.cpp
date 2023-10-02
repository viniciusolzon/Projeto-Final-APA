// Função para encontrar o cliente mais próximo que ainda não foi atendido e que
// pode ser atendido pelo veículo atual.
pair<int, bool> clienteMaisProximo(int atual, const vector<Cliente> &clientes, const vector<vector<int>> &matrizCustos, int capacidadeAtual) {
    int minCusto = INT_MAX;
    int mais_proximo = -1;
    for (int i = 1; i < clientes.size(); i++) {  // Percorre todos os clientes
        if (!clientes[i].atendido && clientes[i].demanda <= capacidadeAtual &&
            matrizCustos[atual][i] < minCusto) {
            minCusto = matrizCustos[atual][i];
            mais_proximo = i; 
        }
    }
    // Retorna o índice do cliente mais próximo e um booleano indicando se ele foi encontrado.
    if (mais_proximo != -1) {
        cout << "Cliente mais próximo do " << atual << " é " << mais_proximo << " com custo " << minCusto << " e demanda " << clientes[mais_proximo].demanda << endl;
        return {mais_proximo, true};
    } else {
        cout << "Não foi encontrado cliente próximo do " << atual << " que possa ser atendido." << endl;
        return {mais_proximo, false};
    }
}

// Função para calcular a capacidade usada em uma rota.
int calcularCapacidadeUsada(const vector<int> &rota, const vector<Cliente> &clientes) {
    int capacidadeUsada = 0;
    for (int clienteId : rota) {
        capacidadeUsada += clientes[clienteId].demanda;
    }
    return capacidadeUsada;
}

// Função para calcular o custo total da solução.
int calcularCustoTotal(const vector<vector<int>> &rotas,
                       const vector<Cliente> &clientes,
                       const vector<vector<int>> &matrizCustos, int r) {
    int custoTotal = 0;
    int veiculosUsados = 0;

    // Calcula o custo das rotas.
    for (const auto &rota : rotas) {
        if (!rota.empty()) {
            veiculosUsados++;
            for (int i = 0; i < rota.size() - 1; i++) {
                custoTotal += matrizCustos[rota[i]][rota[i + 1]];
            }
        }
    }

    // Adiciona o custo fixo dos veículos utilizados.
    custoTotal += r * veiculosUsados;

    // Adiciona o custo de terceirização.
    for (const auto &cliente : clientes) {
        if (!cliente.atendido) {
            custoTotal += cliente.custoTerceirizacao;
        }
    }

    return custoTotal;
}

int main() {
    // Abre o arquivo de entrada.
    string nomeArquivo = "C:\\Users\\arthu\\Desktop\\entrada.txt";
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return 1;
    }

    // Lê os parâmetros iniciais do problema.
    int n, k, Q, L, r;
    arquivo >> n >> k >> Q >> L >> r;
    cout << "n = " << n << ", k = " << k << ", Q = " << Q << ", L = " << L << ", r = " << r << endl;

    // Lê as informações dos clientes.
    vector<Cliente> clientes(n);
    for (int i = 1; i <= n; i++) {
        arquivo >> clientes[i].demanda;
        cout << "Cliente " << i << " - demanda: " << clientes[i].demanda << endl;
    }
    for (int i = 1; i <= n; i++) {
        arquivo >> clientes[i].custoTerceirizacao;
        cout << "Cliente " << i << " - custoTerceirizacao: " << clientes[i].custoTerceirizacao << endl;
    }

    // Lê a matriz de custos.
    vector<vector<int>> matrizCustos(n+1, vector<int>(n+1));
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            arquivo >> matrizCustos[i][j];
            cout << "matrizCustos[" << i << "][" << j << "] = " << matrizCustos[i][j] << endl;
        }
    }
    arquivo.close();

    // Inicializa as rotas para cada veículo.
    vector<vector<int>> rotas(k);

    // Constrói as rotas iniciais para cada veículo.
    int veiculoAtual = 1;
    while (veiculoAtual <= k || calcularCapacidadeUsada(rotas.back(), clientes) != 0) {
        int capacidadeAtual = Q;
        int atual = 0;
        cout << "Construindo rota para o veículo " << veiculoAtual << endl;
        while (capacidadeAtual > 0) {
            pair<int, bool> resultado = clienteMaisProximo(atual, clientes, matrizCustos, capacidadeAtual);
            int proximo = resultado.first;
            if (resultado.second) {
                if (capacidadeAtual - clientes[proximo].demanda >= 0) {  // Verifica se a capacidade não é excedida
                    clientes[proximo].atendido = true;
                    capacidadeAtual -= clientes[proximo].demanda;
                    rotas[veiculoAtual-1].push_back(proximo);
                    atual = proximo;
                } else {
                    break;  // Se a capacidade for excedida, o veículo retorna ao depósito
                }
            } else {
                break;  // Se não houver cliente próximo, o veículo retorna ao depósito
            }
        }
        cout << "Rota do veículo " << veiculoAtual << ": ";
        for (int j : rotas[veiculoAtual-1]) {
            cout << j << " ";
        }
        cout << endl;
        veiculoAtual++;
        if (veiculoAtual > k) {
            rotas.push_back({});
        }
    }
    // Imprime os clientes atendidos e não atendidos.
    for (int i = 1; i <= n; i++) {
        if (clientes[i].atendido) {
            cout << "Cliente " << i << " foi atendido." << endl;
        } else {
            cout << "Cliente " << i << " não foi atendido." << endl;
        }
    }

    // Calcula e imprime o custo total da solução.
    int custoTotal = calcularCustoTotal(rotas, clientes, matrizCustos, r);
    cout << "Custo total da solução: " << custoTotal << endl;

    return 0;
}