// Analisador Estatístico de Transações — C++
// Lê um CSV de transações, calcula estatísticas descritivas (geral e por
// categoria), desenha um histograma em ASCII e exporta os resumos.
// Autora: Ana Paula Galdino

#include "Estatisticas.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include <cmath>

struct Transacao { int id; std::string categoria; double valor; };

static std::vector<Transacao> lerCSV(const std::string& caminho) {
    std::vector<Transacao> dados;
    std::ifstream arq(caminho);
    if (!arq.is_open()) { std::cerr << "Erro ao abrir " << caminho << "\n"; return dados; }
    std::string linha;
    std::getline(arq, linha); // cabeçalho
    while (std::getline(arq, linha)) {
        if (linha.empty()) continue;
        std::stringstream ss(linha);
        std::string id, cat, valor;
        std::getline(ss, id, ',');
        std::getline(ss, cat, ',');
        std::getline(ss, valor, ',');
        dados.push_back({std::stoi(id), cat, std::stod(valor)});
    }
    return dados;
}

static void histograma(const std::vector<double>& v, int bins = 10) {
    double mn = estat::minimo(v), mx = estat::maximo(v), w = (mx - mn) / bins;
    std::vector<int> cont(bins, 0);
    for (double x : v) {
        int b = std::min(bins - 1, (int)((x - mn) / w));
        cont[b]++;
    }
    int maxc = *std::max_element(cont.begin(), cont.end());
    std::cout << "\nDistribuicao dos valores (histograma):\n";
    for (int i = 0; i < bins; ++i) {
        int barras = (int)std::round(40.0 * cont[i] / maxc);
        std::printf("  %8.0f - %8.0f | %s %d\n",
                    mn + i * w, mn + (i + 1) * w, std::string(barras, '#').c_str(), cont[i]);
    }
}

int main(int argc, char* argv[]) {
    std::string caminho = (argc > 1) ? argv[1] : "dados/transacoes.csv";
    std::vector<Transacao> dados = lerCSV(caminho);
    if (dados.empty()) return 1;

    std::vector<double> valores;
    std::map<std::string, std::vector<double>> porCategoria;
    for (const auto& t : dados) {
        valores.push_back(t.valor);
        porCategoria[t.categoria].push_back(t.valor);
    }

    std::cout << std::string(46, '=') << "\n";
    std::cout << "   ANALISE ESTATISTICA DE TRANSACOES\n";
    std::cout << std::string(46, '=') << "\n";
    std::printf("Registros        : %zu\n", valores.size());
    std::printf("Soma total       : R$ %.2f\n", estat::soma(valores));
    std::printf("Media            : R$ %.2f\n", estat::media(valores));
    std::printf("Mediana          : R$ %.2f\n", estat::mediana(valores));
    std::printf("Desvio padrao    : R$ %.2f\n", estat::desvioPadrao(valores));
    std::printf("Minimo / Maximo  : R$ %.2f / R$ %.2f\n",
                estat::minimo(valores), estat::maximo(valores));

    std::cout << "\nPor categoria (n | media | total):\n";
    // ordena categorias por total desc
    std::vector<std::pair<std::string, double>> totais;
    for (auto& kv : porCategoria) totais.push_back({kv.first, estat::soma(kv.second)});
    std::sort(totais.begin(), totais.end(),
              [](auto& a, auto& b) { return a.second > b.second; });
    for (auto& kv : totais) {
        auto& vec = porCategoria[kv.first];
        std::printf("  %-13s %5zu | R$ %8.2f | R$ %11.2f\n",
                    kv.first.c_str(), vec.size(), estat::media(vec), kv.second);
    }

    histograma(valores);

    // exporta resumos para gerar os graficos
    std::ofstream fc("saida/resumo_categoria.csv");
    fc << "categoria,total,media,n\n";
    for (auto& kv : totais) {
        auto& vec = porCategoria[kv.first];
        fc << kv.first << "," << kv.second << "," << estat::media(vec) << "," << vec.size() << "\n";
    }
    std::cout << "\nResumo exportado para saida/resumo_categoria.csv\n";
    return 0;
}
