#include "Solucao.h"

Solucao::Solucao(int capacidade, vector<int> demandas, vector<int> custos_terceirizacao, int qtd_veiculos){

    // Inicializa custo
    this->custo = 0;

    // Inicializa clientes terceirizados e não terceirizados
    this->num_clientes_terceirizados = 0;
    this->num_clientes = 0;

    // Inicializa as rotas
    this->rotas = vector<vector<int>> (qtd_veiculos);

    // Inicializa as capacidades das rotas
    this->capacidadeRotas = vector<int> (qtd_veiculos);

    // Coloca o 0 no começo de todas as rotas e as deixam com capacidade mínima
    for(int i = 0; i < this->rotas.size(); i++){
        this->rotas[i].push_back(0);
        this->atualizaCapacidade(i, capacidade);
    }
}

Solucao::Solucao(){}
Solucao::~Solucao(){}

int Solucao::get_custo(){
    return this->custo;
}

void Solucao::atualiza_custo(int valor){
    this->custo = valor;
}

vector<vector<int>> Solucao::get_rotas(){
    return this->rotas;
}

int Solucao::get_capacidadeRota(int rota){
    return this->capacidadeRotas[rota];
}

void Solucao::atualizaCapacidade(int rota, int capacidade){
    capacidadeRotas[rota] = capacidade;
}

void Solucao::pushBack(int rota, int vertice){
    this->rotas[rota].push_back(vertice);
}

void Solucao::atualizaRota(int rota, int index, int novo_vertice){
    this->rotas[rota][index] = novo_vertice;
}

void Solucao::insereNaRota(int rota, int index, int vertice){
    this->rotas[rota].insert(this->rotas[rota].begin() + index+1, vertice);
}

void Solucao::removeDaRota(int rota, int index){
    this->rotas[rota].erase(this->rotas[rota].begin() + index);
}

void Solucao::inverteRota(int rota, int inicio, int fim){
    reverse(this->rotas[rota].begin() + inicio, this->rotas[rota].begin() + fim + 1);
}

int Solucao::get_num_clientes(){
    return this->num_clientes;
}

void Solucao::mais_um_cliente(){
    this->num_clientes+=1;
}

void Solucao::menos_um_cliente(){
    this->num_clientes-=1;
}

vector<int> Solucao::get_clientes_terceirizados(){
    return this->clientes_terceirizados;
}

int Solucao::get_num_clientesTerceirizados(){
    return this->num_clientes_terceirizados;
}

void Solucao::atualiza_num_clientesTerceirizados(int val){
    this->num_clientes_terceirizados = val;
}

void Solucao::terceirizaCliente(int cliente){
    this->clientes_terceirizados.push_back(cliente);
    this->num_clientes_terceirizados+=1;
}

void Solucao::desterceirizaCliente(int cliente_index){
        this->clientes_terceirizados.erase(this->clientes_terceirizados.begin() + cliente_index);
        this->num_clientes_terceirizados-=1;
}

void Solucao::info(){

    cout << "\nCusto:  " << custo << endl;

    // Rotas
    for (int i = 0; i < this->rotas.size(); i++){
        if(this->rotas[i].size() <=2)
            continue;
        cout << "Rota " << i+1 << ": ";
        for (int j = 0; j < this->rotas[i].size(); j++){
            cout << this->rotas[i][j] << " ";
        }
        cout << "\n";
    }

    // Clientes
    if(this->num_clientes_terceirizados == 0){
        cout << "Nenhum cliente foi terceirizado\n";
    }
    else{
        // cout << "num_clientesTerceirizados = " << this->num_clientesTerceirizados << "\n";
        cout << "Terceirizados: ";
        for(int i = 0; i < this->clientes_terceirizados.size(); i++)
            cout << this->clientes_terceirizados[i] << " ";
        cout << "\n";
    }
}
