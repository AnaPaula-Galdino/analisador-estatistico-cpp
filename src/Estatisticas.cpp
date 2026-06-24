#include "Estatisticas.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>

namespace estat {

double soma(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0);
}

double media(const std::vector<double>& v) {
    return v.empty() ? 0.0 : soma(v) / v.size();
}

double mediana(std::vector<double> v) {
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    size_t n = v.size();
    return (n % 2 == 0) ? (v[n/2 - 1] + v[n/2]) / 2.0 : v[n/2];
}

double desvioPadrao(const std::vector<double>& v) {
    if (v.size() < 2) return 0.0;
    double m = media(v), acc = 0.0;
    for (double x : v) acc += (x - m) * (x - m);
    return std::sqrt(acc / (v.size() - 1));   // desvio amostral
}

double minimo(const std::vector<double>& v) {
    return *std::min_element(v.begin(), v.end());
}

double maximo(const std::vector<double>& v) {
    return *std::max_element(v.begin(), v.end());
}

} // namespace estat
