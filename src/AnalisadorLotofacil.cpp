#include "AnalisadorLotofacil.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <string>
#include <functional>

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

std::vector<std::pair<int, int>> AnalisadorLotofacil::calcularAtrasos(int indiceFim) const {
    std::array<int, 26> atraso{};
    for (int dez = 1; dez <= 25; ++dez) {
        int cont = 0;
        for (int i = indiceFim; i >= 0; --i) {
            if (concursos_.at(i).contemDezena(dez)) {
                break;
            }
            cont++;
        }
        atraso.at(dez) = cont;
    }

    std::vector<std::pair<int, int>> listaAtrasos;
    listaAtrasos.reserve(25);
    for (int dez = 1; dez <= 25; ++dez) {
        listaAtrasos.emplace_back(atraso.at(dez), dez);
    }

    std::sort(listaAtrasos.rbegin(), listaAtrasos.rend());
    return listaAtrasos;
}

// Estratégia 1: Gera até 5 jogos combinando o topo das mais sorteadas
std::vector<std::array<int, 15>> AnalisadorLotofacil::estrategiaQuentes(
    int indiceInicio, int indiceFim, int qtdPalpites
) const {
    auto freq = calcularFrequencias(indiceInicio, indiceFim);
    std::vector<std::array<int, 15>> palpites;

    for (int p = 0; p < qtdPalpites; ++p) {
        std::array<int, 15> jogo{};
        for (int i = 0; i < 15; ++i) {
            // Desloca levemente a janela do ranking para variar os 5 bilhetes
            int idx = (i + p) % 20;
            jogo.at(i) = freq.at(idx).second;
        }
        std::sort(jogo.begin(), jogo.end());
        palpites.push_back(jogo);
    }
    return palpites;
}

// Estratégia 2: Gera até 5 jogos apostando nas menos sorteadas do período
std::vector<std::array<int, 15>> AnalisadorLotofacil::estrategiaFrias(
    int indiceInicio, int indiceFim, int qtdPalpites
) const {
    auto freq = calcularFrequencias(indiceInicio, indiceFim);
    std::reverse(freq.begin(), freq.end()); // Menos frequentes primeiro
    std::vector<std::array<int, 15>> palpites;

    for (int p = 0; p < qtdPalpites; ++p) {
        std::array<int, 15> jogo{};
        for (int i = 0; i < 15; ++i) {
            int idx = (i + p) % 20;
            jogo.at(i) = freq.at(idx).second;
        }
        std::sort(jogo.begin(), jogo.end());
        palpites.push_back(jogo);
    }
    return palpites;
}

// Estratégia 3: Gera até 5 jogos priorizando as dezenas há mais tempo sem sair
std::vector<std::array<int, 15>> AnalisadorLotofacil::estrategiaAtrasadas(
    int indiceFim, int qtdPalpites
) const {
    auto atrasos = calcularAtrasos(indiceFim);
    std::vector<std::array<int, 15>> palpites;

    for (int p = 0; p < qtdPalpites; ++p) {
        std::array<int, 15> jogo{};
        for (int i = 0; i < 15; ++i) {
            int idx = (i + p) % 20;
            jogo.at(i) = atrasos.at(idx).second;
        }
        std::sort(jogo.begin(), jogo.end());
        palpites.push_back(jogo);
    }
    return palpites;
}

// Estratégia 4: Mantém 9 dezenas do último sorteio + 6 mais frequentes fora dele
std::vector<std::array<int, 15>> AnalisadorLotofacil::estrategiaRepeticaoAnterior(
    int indiceInicio, int indiceFim, int qtdPalpites
) const {
    auto freq = calcularFrequencias(indiceInicio, indiceFim);
    const auto& anterior = concursos_.at(indiceFim).getDezenas();

    std::vector<int> doAnteriorOrdenado;
    std::vector<int> foraAnteriorOrdenado;

    for (const auto& [qtd, dez] : freq) {
        if (std::find(anterior.begin(), anterior.end(), dez) != anterior.end()) {
            doAnteriorOrdenado.push_back(dez);
        } else {
            foraAnteriorOrdenado.push_back(dez);
        }
    }

    std::vector<std::array<int, 15>> palpites;
    for (int p = 0; p < qtdPalpites; ++p) {
        std::array<int, 15> jogo{};
        int pos = 0;
        // 9 dezenas do concurso anterior
        for (int i = 0; i < 9; ++i) {
            jogo.at(pos++) = doAnteriorOrdenado.at((i + p) % doAnteriorOrdenado.size());
        }
        // 6 dezenas de fora do concurso anterior
        for (int i = 0; i < 6; ++i) {
            jogo.at(pos++) = foraAnteriorOrdenado.at((i + p) % foraAnteriorOrdenado.size());
        }
        std::sort(jogo.begin(), jogo.end());
        palpites.push_back(jogo);
    }
    return palpites;
}

// Estratégia 5: Equilíbrio clássico de 8 ímpares e 7 pares entre as mais frequentes
std::vector<std::array<int, 15>> AnalisadorLotofacil::estrategiaEquilibrada(
    int indiceInicio, int indiceFim, int qtdPalpites
) const {
    auto freq = calcularFrequencias(indiceInicio, indiceFim);
    std::vector<int> impares;
    std::vector<int> pares;

    for (const auto& [qtd, dez] : freq) {
        if (dez % 2 != 0) impares.push_back(dez);
        else pares.push_back(dez);
    }

    std::vector<std::array<int, 15>> palpites;
    for (int p = 0; p < qtdPalpites; ++p) {
        std::array<int, 15> jogo{};
        int pos = 0;
        for (int i = 0; i < 8; ++i) {
            jogo.at(pos++) = impares.at((i + p) % impares.size());
        }
        for (int i = 0; i < 7; ++i) {
            jogo.at(pos++) = pares.at((i + p) % pares.size());
        }
        std::sort(jogo.begin(), jogo.end());
        palpites.push_back(jogo);
    }
    return palpites;
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
int fim = totalConcursos - 1;
auto frequencias = calcularFrequencias(inicio, fim);
std::array<int, 15> jogoSugerido{};
std::array<bool, 26> mapaPalpite{};
for (int i = 0; i < 15; ++i) {
int dezena = frequencias.at(i).second;
jogoSugerido.at(i) = dezena;
mapaPalpite.at(dezena) = true;
}
std::sort(jogoSugerido.begin(), jogoSugerido.end());
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
bool AnalisadorLotofacil::analisarEstrategias(int janelaHistorico) const {
if (concursos_.size() < 10) {
std::cerr << "Erro: Poucos concursos para realizar a analise comparativa.\n";
return false;
}
struct EstrategiaInfo {
std::string nome;
std::function<std::vector<std::array<int, 15>>(int, int)> gerador;
long long somaMelhoresAcertos = 0;
int concursosPremiados = 0; // Quantas vezes ao menos 1 dos 5 jogos fez 11+ pts
std::array<int, 16> distMelhorAcerto{};
};
std::vector<EstrategiaInfo>estrategias = {
{"1. Quentes (Top Frequencia)", [this](int ini, int fim) {
return estrategiaQuentes(ini, fim, 5);
}},
{"2. Frias (Menos Sorteadas) ", [this](int ini, int fim) {
return estrategiaFrias(ini, fim, 5);
}},
{"3. Atrasadas (Mais Tempo)  ", [this](int /*ini*/, int fim) {
return estrategiaAtrasadas(fim, 5);
}},
{"4. Repeticao (9 Ant + 6)   ", [this](int ini, int fim) {
return estrategiaRepeticaoAnterior(ini, fim, 5);
}},
{"5. Equilibrada (8 Imp/7Par)", [this](int ini, int fim) {
return estrategiaEquilibrada(ini, fim, 5);
}}
};
int totalConcursos = static_cast<int>(concursos_.size());
int inicioSimulacao = (janelaHistorico > 0 && janelaHistorico < totalConcursos) ? janelaHistorico : 1;
int concursosTestados = 0;
// Percorre todo o histórico testando as 5 estratégias (5 jogos por estratégia)
for (int alvo = inicioSimulacao; alvo < totalConcursos; ++alvo) {
int inicioJanela = (janelaHistorico > 0) ? std::max(0, alvo - janelaHistorico) : 0;
int fimJanela = alvo - 1;
const Concurso& concursoAlvo = concursos_.at(alvo);
for (auto& est : estrategias) {
auto palpites = est.gerador(inicioJanela, fimJanela);
int melhorAcertoRodada = 0;
for (const auto& jogo : palpites) {
int acertos = 0;
for (int dez : jogo) {
if (concursoAlvo.contemDezena(dez)) {
acertos++;
}
}
if (acertos > melhorAcertoRodada) {
melhorAcertoRodada = acertos;
}
}
est.somaMelhoresAcertos += melhorAcertoRodada;
est.distMelhorAcerto.at(melhorAcertoRodada)++;
if (melhorAcertoRodada >= 11) {
est.concursosPremiados++;
}
}
concursosTestados++;
}
// Ordena as estratégias pela maior quantidade de concursos premiados (e média de acertos)
std::sort(estrategias.begin(), estrategias.end(), [](const EstrategiaInfo& a, const EstrategiaInfo& b) {
if (a.concursosPremiados != b.concursosPremiados) {
return a.concursosPremiados > b.concursosPremiados;
}
return a.somaMelhoresAcertos > b.somaMelhoresAcertos;
});
std::cout << "=== ANALISE COMPARATIVA DE ESTRATEGIAS (5 PALPITES POR ESTRATEGIA) ===\n";
std::cout << "Base de calculo   : "
<< (janelaHistorico > 0 ? "Janela movel dos ultimos " + std::to_string(janelaHistorico) + " concursos"
: "Historico acumulado completo") << "\n";
std::cout << "Concursos testados: " << concursosTestados
<< " (Concurso " << concursos_.at(inicioSimulacao).getNumero()
<< " ao " << concursos_.back().getNumero() << ")\n\n";
std::cout << "Estrategia                  | Media Pts | Premiados (11+) | 11pts | 12pts | 13pts | 14pts | 15pts\n";
std::cout << "----------------------------+-----------+-------------------+-------+-------+-------+-------+------\n";
for (const auto& est : estrategias) {
double media = static_cast<double>(est.somaMelhoresAcertos) / concursosTestados;
double pctPrem = (static_cast<double>(est.concursosPremiados) / concursosTestados) * 100.0;
std::cout << est.nome << " | "
<< std::setw(9) << std::fixed << std::setprecision(2) << media << " | "
<< std::setw(6) << est.concursosPremiados << " (" << std::setw(5) << pctPrem << "%) | "
<< std::setw(5) << est.distMelhorAcerto.at(11) << " | "
<< std::setw(5) << est.distMelhorAcerto.at(12) << " | "
<< std::setw(5) << est.distMelhorAcerto.at(13) << " | "
<< std::setw(5) << est.distMelhorAcerto.at(14) << " | "
<< std::setw(5) << est.distMelhorAcerto.at(15) << "\n";
}
// Gera os 5 palpites da estratégia campeã usando os dados até o ÚLTIMO concurso
const auto& campea = estrategias.front();
int inicioAtual = (janelaHistorico > 0 && janelaHistorico < totalConcursos)
? (totalConcursos - janelaHistorico)
: 0;
int fimAtual = totalConcursos - 1;
auto palpitesProximo = campea.gerador(inicioAtual, fimAtual);
std::cout << "\n>>> ESTRATEGIA CAMPEA: " << campea.nome << "\n";
std::cout << "5 Palpites gerados para o proximo concurso (apos o "
<< concursos_.back().getNumero() << "):\n\n";
for (size_t i = 0; i < palpitesProximo.size(); ++i) {
std::cout << "Jogo " << (i + 1) << ": ";
for (int j = 0; j < 15; ++j) {
std::cout << std::setw(2) << std::setfill('0') << palpitesProximo.at(i).at(j);
if (j < 14) std::cout << " - ";
}
std::cout << "\n";
}
return true;
}