#include "CVRP.h"

// Perturbação
Solucao Perturbacao(Solucao *s, Data *d){
    Solucao s_copy;
    s_copy = *s;

    // a que eu fazia era fazer uma sequência de n shifts 1 aleatorios
    // onde n é um número aleatório entre 1 e 3
    // eu então de 1 a 3 vezes eu pegava um cliente aleatório e jogava
    // ele em outra rota aleatoriamente, numa posição também aleatória
    // aí pra garantir que a solução é viável bota um while true até ele fazer um shift viável
    // eu também fazia a mesma coisa com swap (faz 1 a 3 swap aleatórios no mesmo esquema)

    return s_copy;
}
