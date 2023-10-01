#include "Cliente.h"

Cliente::Cliente(int demanda, int custs_terceirizacao){
    this->demanda = demanda;
    this->custoTerceirizacao = custs_terceirizacao;
    this->atendido = false;
}

