#include "AplicacaoCLI.hpp"
#include "LeitorPlanilha.hpp"
#include "AnalisadorLotofacil.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>

AplicacaoCLI::AplicacaoCLI(int argc, char* argv[])
    : args_(argv, argv + argc) {}

void AplicacaoCLI::exibirAjuda() const {
    std::cout << "lot-jogar v" << VERSAO << "\n"
              << "Uso: " << args_.at(0) << " [OPCAO]\n\n"
              << "Opcoes:\n"
              << "  --grafico       Analisa todos os concursos ate o penultimo e compara com o ultimo.\n"
              << "  --grafico <N>   Analisa os ultimos <N> concursos antes do ultimo sorteio.\n"
              << "  --simular       Simula o desempenho das 15 mais frequentes em todo o historico.\n"
              << "  --simular <N>   Simula usando janela movel dos ultimos <N> concursos.\n"
              << "  --palpite       Gera um palpite com as 15 mais frequentes de todo o historico.\n"
              << "  --palpite <N>   Gera um palpite baseado nos ultimos <N> concursos.\n"
              << "  --analisar      Compara 5 estrategias diferentes (5 jogos cada) em todo o historico.\n"
              << "  --analisar <N>  Compara as 5 estrategias usando janela movel de <N> concursos.\n"
              << "  --version, -v   Exibe a versao atual do aplicativo.\n"
              << "  --help, -h      Exibe esta mensagem de ajuda.\n";
}

bool AplicacaoCLI::converterInteiroPositivo(const std::string& texto, int& valorSaida) {
    if (texto.empty() || !std::all_of(texto.begin(), texto.end(), [](unsigned char c) {
        return std::isdigit(c);
    })) {
        return false;
    }

    try {
        valorSaida = std::stoi(texto);
        return valorSaida > 0;
    } catch (...) {
        return false;
    }
}

int AplicacaoCLI::executar() const {
    if (args_.size() < 2 || args_.size() > 3) {
        exibirAjuda();
        return 1;
    }

    const std::string& comando = args_.at(1);

    if (comando == "--help" || comando == "-h") {
        exibirAjuda();
        return 0;
    }

    if (comando == "--version" || comando == "-v") {
        std::cout << "lot-jogar v" << VERSAO << "\n";
        return 0;
    }


    int parametroNumerico = 0;
    if (args_.size() == 3) {
        if (!converterInteiroPositivo(args_.at(2), parametroNumerico)) {
            std::cerr << "Erro: O argumento '" << args_.at(2) << "' deve ser um numero inteiro maior que zero.\n\n";
            exibirAjuda();
            return 1;
        }
    }

    if (comando != "--grafico" && comando != "--simular" &&
        comando != "--palpite" && comando != "--analisar") {
        std::cerr << "Erro: Opcao desconhecida '" << comando << "'.\n\n";
        exibirAjuda();
        return 1;
    }

    LeitorPlanilha leitor;
    std::vector<Concurso> concursos;
    if (!leitor.carregar(concursos)) {
        return 1;
    }

    AnalisadorLotofacil analisador(concursos);

    if (comando == "--grafico") {
        return analisador.exibirGrafico(parametroNumerico) ? 0 : 1;
    }

    if (comando == "--simular") {
        return analisador.simularEstrategia(parametroNumerico) ? 0 : 1;
    }

    if (comando == "--palpite") {
        return analisador.gerarPalpite(parametroNumerico) ? 0 : 1;
    }

    if (comando == "--analisar") {
        return analisador.analisarEstrategias(parametroNumerico) ? 0 : 1;
    }

    return 0;
}