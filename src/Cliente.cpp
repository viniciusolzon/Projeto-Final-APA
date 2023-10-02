#include "Cliente.h"

Cliente::Cliente(int demanda, int custo_terceirizacao){
    this->demanda = demanda;
    this->custoTerceirizacao = custo_terceirizacao;
    this->atendido = false;
}

