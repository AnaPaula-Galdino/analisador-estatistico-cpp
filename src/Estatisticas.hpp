#ifndef ESTATISTICAS_HPP
#define ESTATISTICAS_HPP

#include <vector>

// Funções estatísticas reutilizáveis sobre um vetor de valores.
// Autora: Ana Paula Galdino
namespace estat {
    double soma(const std::vector<double>& v);
    double media(const std::vector<double>& v);
    double mediana(std::vector<double> v);          // recebe cópia (vai ordenar)
    double desvioPadrao(const std::vector<double>& v);
    double minimo(const std::vector<double>& v);
    double maximo(const std::vector<double>& v);
}

#endif
