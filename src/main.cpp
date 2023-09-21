#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>

// ILS

// auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro
//     srand(time(NULL)); // Para conseguir gerar números aleatórios
//     Solucao bestOfAll; // Cria a solução que guardará a melhor solução possível

//     for(int i = 0; i < maxIter; i++){
//         Solucao current = Construcao(&dist); // Cria a solução atual ( Muita gula e pouca aleatoriedade )
//         calcularcost(&current, &dist); // Calcula o custo dessa solução com base na matriz de adjacência
//         Solucao best = current; // Cria a solução que guardará a melhor solução dessa iteração
//         if(i == 0)
//             bestOfAll = current; // Se for a primeira solução criada ela já é atribuída para a melhor solução possível

//         int iterIls = 0;
//         while(iterIls <= maxIterILS){ // Enquanto houver melhoras nessa solução, reinicia-se o processo de melhora
//             BuscaLocal(&current, &dist); // Tenta melhorar o máximo possível a solução atual
//             if(improve(best.cost, current.cost)){ // Se a solução atual foi melhorada a melhor solução atual é atualizada pela atual
//                 best = current;
//                 iterIls = 0;
//             }
//             current = Pertubacao(&best, &dist); // Pertuba de forma aleatória a solução atual pra ver se ao alterá-la é possível melhorá-la
//             iterIls++;
//         }

//         if(improve(bestOfAll.cost, best.cost)) // Se a melhor solução atual for melhor que a melhor solução possível, ela se torna a melhor solução possível
//             bestOfAll = best;
//     }

//     auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
//     std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
//     cout << "\nExecution time:  " << float_ms.count() / 1000.0000000000000 << " seconds" << "\n";

//     best_solution = bestOfAll;

int main(){

    std::cout << "TESTE\n";
}
