#include <climits>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// Estrutura para representar um cliente.
struct Cliente {
  int demanda;            // Demanda do cliente.
  int custoTerceirizacao; // Custo para terceirizar a entrega para o cliente.
  bool atendido;          // Indica se o cliente foi atendido ou não.
};

// Função para encontrar o cliente mais próximo que ainda não foi atendido e que
// pode ser atendido pelo veículo atual.
int clienteMaisProximo(int atual, const vector<Cliente> &clientes,
                       const vector<vector<int>> &matrizCustos,
                       int capacidadeAtual) {
  // Inicializa o custo mínimo como o maior valor possível.
  int minCusto = INT_MAX;
  int idx = -1; // Inicializa o índice do cliente mais próximo como -1.

  // Percorre todos os clientes para encontrar o mais próximo.
  for (int i = 0; i < clientes.size(); i++) {
    // Verifica se o cliente atual ainda não foi atendido, se sua demanda pode
    // ser atendida e se o custo para atendê-lo é menor que o custo mínimo
    // encontrado até agora.
    if (!clientes[i].atendido && clientes[i].demanda <= capacidadeAtual &&
        matrizCustos[atual][i] < minCusto) {
      minCusto = matrizCustos[atual][i];
      idx = i;
    }
  }
  return idx; // Retorna o índice do cliente mais próximo ou -1 se nenhum
              // cliente puder ser atendido.
}

// Função para calcular a capacidade usada em uma rota.
int calcularCapacidadeUsada(const vector<int> &rota,
                            const vector<Cliente> &clientes) {
  int capacidadeUsada = 0;
  for (int clienteId : rota) {
    capacidadeUsada += clientes[clienteId].demanda;
  }
  return capacidadeUsada;
}

// Estrutura de Vizinhança 1: Swap.
// Realiza trocas entre clientes de uma mesma rota para tentar melhorar o custo.
bool swapSingleRoute(vector<int> &rota,
                     const vector<vector<int>> &matrizCustos) {
  bool melhorou = false;
  int n = rota.size();
  for (int i = 1; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      // Calcula o custo antes e depois da troca.
      int custoAntigo = matrizCustos[rota[i - 1]][rota[i]] +
                        matrizCustos[rota[i]][rota[i + 1]] +
                        matrizCustos[rota[j - 1]][rota[j]] +
                        matrizCustos[rota[j]][rota[j + 1]];
      int custoNovo = matrizCustos[rota[i - 1]][rota[j]] +
                      matrizCustos[rota[j]][rota[i + 1]] +
                      matrizCustos[rota[j - 1]][rota[i]] +
                      matrizCustos[rota[i]][rota[j + 1]];
      // Se o custo após a troca for menor, realiza a troca.
      if (custoNovo < custoAntigo) {
        swap(rota[i], rota[j]);
        melhorou = true;
      }
    }
  }
  return melhorou;
}

// Estrutura de Vizinhança 2: Or-opt-2.
// Tenta mover um bloco de 2 clientes de uma rota para outra para melhorar o
// custo.
bool orOpt2MultipleRoutes(vector<vector<int>> &rotas,
                          const vector<vector<int>> &matrizCustos,
                          const vector<Cliente> &clientes, int Q) {
  bool melhorou = false;
  for (int rotaOrigem = 0; rotaOrigem < rotas.size(); rotaOrigem++) {
    for (int rotaDestino = 0; rotaDestino < rotas.size(); rotaDestino++) {
      for (int i = 1; i < rotas[rotaOrigem].size() - 2; i++) {
        vector<int> bloco = {rotas[rotaOrigem][i], rotas[rotaOrigem][i + 1]};
        int demandaBloco =
            clientes[bloco[0]].demanda + clientes[bloco[1]].demanda;
        for (int j = 1; j < rotas[rotaDestino].size(); j++) {
          int capacidadeUsadaDestino =
              calcularCapacidadeUsada(rotas[rotaDestino], clientes);
          if (capacidadeUsadaDestino + demandaBloco > Q)
            continue;
          // Calcula o custo de remover o bloco da rota de origem e inserir na
          // rota de destino.
          int custoRemocao =
              matrizCustos[rotas[rotaOrigem][i - 1]][rotas[rotaOrigem][i]] +
              matrizCustos[rotas[rotaOrigem][i + 1]][rotas[rotaOrigem][i + 2]] -
              matrizCustos[rotas[rotaOrigem][i - 1]][rotas[rotaOrigem][i + 2]];
          int custoInsercao =
              matrizCustos[rotas[rotaDestino][j - 1]][bloco[0]] +
              matrizCustos[bloco[1]][rotas[rotaDestino][j]] -
              matrizCustos[rotas[rotaDestino][j - 1]][rotas[rotaDestino][j]];
          // Se o custo total for menor após a movimentação, realiza a
          // movimentação.
          if (custoInsercao + custoRemocao < 0) {
            rotas[rotaOrigem].erase(rotas[rotaOrigem].begin() + i,
                                    rotas[rotaOrigem].begin() + i + 2);
            rotas[rotaDestino].insert(rotas[rotaDestino].begin() + j,
                                      bloco.begin(), bloco.end());
            melhorou = true;
          }
        }
      }
    }
  }
  return melhorou;
}

// Estrutura de Vizinhança 3: Terceirização.
// Tenta terceirizar entregas ou retirar da terceirização para melhorar o custo.
bool terceirizacaoMovimento(vector<vector<int>> &rotas,
                            vector<int> &terceirizados,
                            const vector<vector<int>> &matrizCustos,
                            vector<Cliente> &clientes, int Q) {
  bool melhorou = false;
  // 1. Tenta terceirizar clientes que ainda não foram atendidos.
  for (int i = 0; i < clientes.size(); i++) {
    if (!clientes[i].atendido &&
        clientes[i].custoTerceirizacao < matrizCustos[0][i]) {
      terceirizados.push_back(i);
      clientes[i].atendido = true;
      melhorou = true;
    }
  }
  // 2. Tenta retirar clientes da terceirização e inserir em uma rota.
  for (int i = 0; i < terceirizados.size(); i++) {
    int clienteId = terceirizados[i];
    for (vector<int> &rota : rotas) {
      int capacidadeUsada = calcularCapacidadeUsada(rota, clientes);
      if (capacidadeUsada + clientes[clienteId].demanda <= Q &&
          clientes[clienteId].custoTerceirizacao > matrizCustos[0][clienteId]) {
        rota.push_back(clienteId);
        terceirizados.erase(terceirizados.begin() + i);
        clientes[clienteId].atendido = true;
        melhorou = true;
        break;
      }
    }
  }
  return melhorou;
}

// Função para calcular o custo total da solução.
int calcularCustoTotal(const vector<vector<int>> &rotas,
                       const vector<int> &terceirizados,
                       const vector<Cliente> &clientes,
                       const vector<vector<int>> &matrizCustos) {
  int custoTotal = 0;
  // Calcula o custo das rotas.
  for (const auto &rota : rotas) {
    for (int i = 0; i < rota.size() - 1; i++) {
      custoTotal += matrizCustos[rota[i]][rota[i + 1]];
    }
  }
  // Adiciona o custo da terceirização.
  for (int clienteId : terceirizados) {
    custoTotal += clientes[clienteId].custoTerceirizacao;
  }
  return custoTotal;
}

int main() {
  string nomeArquivo = "entrada.txt"; // Nome do arquivo de entrada.
  ifstream arquivo(nomeArquivo);
  if (!arquivo.is_open()) {
    cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
    return 1;
  }

  int n, k, Q, L, r;
  arquivo >> n >> k >> Q >> L >> r;

  vector<Cliente> clientes(n);
  for (int i = 0; i < n; i++) {
    arquivo >> clientes[i].demanda;
    clientes[i].atendido =
        false; // Todos os clientes começam como não atendidos.
  }
  for (int i = 0; i < n; i++) {
    arquivo >> clientes[i].custoTerceirizacao;
  }

  // Lê a matriz de custos.
  vector<vector<int>> matrizCustos(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      arquivo >> matrizCustos[i][j];
    }
  }
  arquivo.close();

  vector<vector<int>> rotas(k); // Cada veículo tem sua própria rota.

  // Constrói as rotas iniciais para cada veículo.
  for (int i = 0; i < k; i++) {
    int capacidadeAtual = Q;
    int atual = 0; // Começa no depósito.
    while (capacidadeAtual > 0) {
      int proximo =
          clienteMaisProximo(atual, clientes, matrizCustos, capacidadeAtual);
      if (proximo == -1)
        break; // Nenhum cliente viável encontrado.
      clientes[proximo].atendido = true;
      capacidadeAtual -= clientes[proximo].demanda;
      rotas[i].push_back(proximo);
      atual = proximo;
    }
  }

  // Imprime os clientes atendidos e não atendidos.
  for (int i = 0; i < n; i++) {
    if (clientes[i].atendido) {
      cout << "Cliente " << i << " foi atendido." << endl;
    } else {
      cout << "Cliente " << i << " não foi atendido." << endl;
    }
  }

  // Aplica estruturas de vizinhança até que nenhuma melhoria seja encontrada.
  vector<int> terceirizados;
  bool melhorou;
  do {
    melhorou = false;
    for (auto &rota : rotas) {
      if (swapSingleRoute(rota, matrizCustos)) {
        melhorou = true;
      }
    }
    if (orOpt2MultipleRoutes(rotas, matrizCustos, clientes, Q)) {
      melhorou = true;
    }
    if (terceirizacaoMovimento(rotas, terceirizados, matrizCustos, clientes,
                               Q)) {
      melhorou = true;
    }
  } while (melhorou);

  // Terceiriza os clientes que ainda não foram atendidos.
  for (int i = 0; i < clientes.size(); i++) {
    if (!clientes[i].atendido) {
      terceirizados.push_back(i);
    }
  }

  // Calcula e imprime o custo total da solução.
  int custoTotal =
      calcularCustoTotal(rotas, terceirizados, clientes, matrizCustos);
  cout << "Custo total da solução: " << custoTotal << endl;

  return 0;
}