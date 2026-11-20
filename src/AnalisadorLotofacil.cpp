#include "AnalisadorLotofacil.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <string>

AnalisadorLotofacil::AnalisadorLotofacil(const std::vector<Concurso>& concursos)
    : concursos_(concursos) {}

std::vector<std::pair<int, int>> AnalisadorLotofacil::calcularFrequencias(int indiceInicio, int indiceFim) const {
    std::array<int, 26> contagem{};
    for (int i = indiceInicio; i <= indiceFim; ++i) {
        for (int dezena : concursos_.at(i).getDezenas()) {
            contagem.at(dezena)++;
        }
    }

    std::vector<std::pair<int, int>> frequencias;
    frequencias.reserve(25);
    for (int dez = 1; dez <= 25; ++dez) {
        frequencias.emplace_back(contagem.at(dez), dez);
    }

    std::sort(frequencias.rbegin(), frequencias.rend());
    return frequencias;
}

bool AnalisadorLotofacil::exibirGrafico(int quantidadeContagem) const {
    if (concursos_.size() < 2) {
        std::cerr << "Erro: A planilha precisa conter pelo menos 2 concursos validos.\n";
        return false;
    }

    const Concurso& ultimoConcurso = concursos_.back();
    int totalHistorico = static_cast<int>(concursos_.size()) - 1;

    if (quantidadeContagem <= 0 || quantidadeContagem > totalHistorico) {
        quantidadeContagem = totalHistorico;
    }

    int inicioContagem = totalHistorico - quantidadeContagem;
    int fimContagem = totalHistorico - 1;

    auto frequencias = calcularFrequencias(inicioContagem, fimContagem);

    std::cout << "Historico analisado : Concurso " << concursos_.at(inicioContagem).getNumero()
              << " ao " << concursos_.at(fimContagem).getNumero()
              << " (" << quantidadeContagem << " concursos)\n";
    std::cout << "Comparado com       : Concurso " << ultimoConcurso.getNumero()
              << " (* = dezena sorteada no ultimo concurso)\n\n";

    int acertosNoTopo15 = 0;
    int posicao = 0;

    for (const auto& [qtd, dezena] : frequencias) {
        posicao++;
        double porCento = static_cast<double>(qtd) / quantidadeContagem;
        bool saiuNoUltimo = ultimoConcurso.contemDezena(dezena);

        if (saiuNoUltimo && posicao <= 15) {
            acertosNoTopo15++;
        }

        std::cout << (saiuNoUltimo ? " * " : "   ")
                  << std::setw(2) << std::setfill('0') << dezena << ' '
                  << std::string(static_cast<int>(50 * porCento), '=') << ' '
                  << qtd << ' '
                  << std::fixed << std::setprecision(2)
                  << '(' << (porCento * 100.0) << "%)\n";
    }

    std::cout << "\nResumo: " << acertosNoTopo15
              << " das 15 dezenas mais frequentes sairam no concurso "
              << ultimoConcurso.getNumero() << ".\n";

    return true;
}

bool AnalisadorLotofacil::simularEstrategia(int janelaHistorico) const {
    if (concursos_.size() < 10) {
        std::cerr << "Erro: Poucos concursos para realizar simulacao.\n";
        return false;
    }

    int totalConcursos = static_cast<int>(concursos_.size());
    int inicioSimulacao = (janelaHistorico > 0 && janelaHistorico < totalConcursos) ? janelaHistorico : 1;

    std::array<int, 16> distribuicaoAcertos{};
    int concursosTestados = 0;

    for (int alvo = inicioSimulacao; alvo < totalConcursos; ++alvo) {
        int inicioJanela = (janelaHistorico > 0) ? std::max(0, alvo - janelaHistorico) : 0;
        int fimJanela = alvo - 1;

        auto frequencias = calcularFrequencias(inicioJanela, fimJanela);

        std::array<bool, 26> palpite{};
        for (int i = 0; i < 15; ++i) {
            palpite.at(frequencias.at(i).second) = true;
        }

        int acertos = 0;
        for (int dezSorteada : concursos_.at(alvo).getDezenas()) {
            if (palpite.at(dezSorteada)) {
                acertos++;
            }
        }

        distribuicaoAcertos.at(acertos)++;
        concursosTestados++;
    }

    std::cout << "=== SIMULACAO HISTORICA (TOP 15 MAIS FREQUENTES) ===\n";
    std::cout << "Base de calculo   : "
              << (janelaHistorico > 0 ? "Ultimos " + std::to_string(janelaHistorico) + " concursos moveis"
                                      : "Todo o historico acumulado") << "\n";
    std::cout << "Concursos testados: " << concursosTestados
              << " (Concurso " << concursos_.at(inicioSimulacao).getNumero()
              << " ao " << concursos_.back().getNumero() << ")\n\n";

    int totalPremiados = 0;
    for (int pts = 15; pts >= 11; --pts) {
        totalPremiados += distribuicaoAcertos.at(pts);
        double pct = (static_cast<double>(distribuicaoAcertos.at(pts)) / concursosTestados) * 100.0;
        std::cout << pts << " acertos: " << std::setw(4) << std::setfill(' ') << distribuicaoAcertos.at(pts)
                  << " vezes (" << std::fixed << std::setprecision(2) << pct << "%)\n";
    }

    double pctPremiados = (static_cast<double>(totalPremiados) / concursosTestados) * 100.0;
    std::cout << "\nTotal de concursos com premiacao (11 a 15 pts): "
              << totalPremiados << " (" << std::fixed << std::setprecision(2) << pctPremiados << "%)\n";

    return true;
}
bool AnalisadorLotofacil::gerarPalpite(int janelaHistorico) const {
    if (concursos_.empty()) {
        std::cerr << "Erro: Nenhum concurso disponivel na planilha.\n";
        return false;
    }

    int totalConcursos = static_cast<int>(concursos_.size());
    if (janelaHistorico <= 0 || janelaHistorico > totalConcursos) {
        janelaHistorico = totalConcursos;
    }

    int inicio = totalConcursos - janelaHistorico;
    int fim = totalConcursos - 1; // Inclui até o último concurso

    auto frequencias = calcularFrequencias(inicio, fim);

    // Pega as 15 mais frequentes e ordena em ordem crescente (estilo volante)
    std::array<int, 15> jogoSugerido{};
    std::array<bool, 26> mapaPalpite{}; // Facilita conferência rápida O(1)

    for (int i = 0; i < 15; ++i) {
        int dezena = frequencias.at(i).second;
        jogoSugerido.at(i) = dezena;
        mapaPalpite.at(dezena) = true;
    }
    std::sort(jogoSugerido.begin(), jogoSugerido.end());

    // Calcula estatísticas de equilíbrio do volante
    const Concurso& ultimoConcurso = concursos_.back();
    int pares = 0;
    int impares = 0;
    int soma = 0;
    int repetidas = 0;

    for (int dez : jogoSugerido) {
        soma += dez;
        if (dez % 2 == 0) pares++;
        else impares++;

        if (ultimoConcurso.contemDezena(dez)) {
            repetidas++;
        }
    }

    // Confere quantas vezes esse palpite específico pontuou em TODO o histórico
    std::array<int, 16> historicoAcertos{};
    for (const auto& concurso : concursos_) {
        int acertos = 0;
        for (int dezSorteada : concurso.getDezenas()) {
            if (mapaPalpite.at(dezSorteada)) {
                acertos++;
            }
        }
        historicoAcertos.at(acertos)++;
    }

    std::cout << "=== SUGESTAO DE PALPITE (TOP 15 MAIS FREQUENTES) ===\n";
    std::cout << "Base de calculo : Concurso " << concursos_.at(inicio).getNumero()
              << " ao " << ultimoConcurso.getNumero()
              << " (" << janelaHistorico << " concursos)\n\n";

    std::cout << "Dezenas sugeridas:\n";
    for (int i = 0; i < 15; ++i) {
        std::cout << std::setw(2) << std::setfill('0') << jogoSugerido.at(i);
        if (i < 14) std::cout << " - ";
    }

    std::cout << "\n\nAnalise do volante:\n"
              << "- Pares / Impares       : " << pares << " pares e " << impares << " impares\n"
              << "- Soma das dezenas      : " << soma << "\n"
              << "- Repetidas do anterior : " << repetidas << " dezenas do concurso "
              << ultimoConcurso.getNumero() << "\n";

    std::cout << "\nDesempenho deste palpite fixo em todo o historico ("
              << totalConcursos << " concursos):\n";

    int totalPremiados = 0;
    for (int pts = 15; pts >= 11; --pts) {
        totalPremiados += historicoAcertos.at(pts);
        double pct = (static_cast<double>(historicoAcertos.at(pts)) / totalConcursos) * 100.0;
        std::cout << "- " << pts << " acertos: " << std::setw(4) << std::setfill(' ')
                  << historicoAcertos.at(pts) << " vezes ("
                  << std::fixed << std::setprecision(2) << pct << "%)\n";
    }

    double pctTotal = (static_cast<double>(totalPremiados) / totalConcursos) * 100.0;
    std::cout << "- Total premiado (11 a 15 pts): " << totalPremiados
              << " vezes (" << std::fixed << std::setprecision(2) << pctTotal << "%)\n";

    return true;
}
