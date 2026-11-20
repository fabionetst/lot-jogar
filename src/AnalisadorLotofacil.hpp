#pragma once

#include <vector>
#include <array>
#include <string>
#include <utility>
#include "Concurso.hpp"

class AnalisadorLotofacil {
private:
    const std::vector<Concurso>& concursos_;

    std::vector<std::pair<int, int>> calcularFrequencias(int indiceInicio, int indiceFim) const;
    std::vector<std::pair<int, int>> calcularAtrasos(int indiceFim) const;

    // 5 Estratégias diferentes que geram até 5 palpites cada (baseadas no histórico até indiceFim)
    std::vector<std::array<int, 15>> estrategiaQuentes(int indiceInicio, int indiceFim, int qtdPalpites = 5) const;
    std::vector<std::array<int, 15>> estrategiaFrias(int indiceInicio, int indiceFim, int qtdPalpites = 5) const;
    std::vector<std::array<int, 15>> estrategiaAtrasadas(int indiceFim, int qtdPalpites = 5) const;
    std::vector<std::array<int, 15>> estrategiaRepeticaoAnterior(int indiceInicio, int indiceFim, int qtdPalpites = 5) const;
    std::vector<std::array<int, 15>> estrategiaEquilibrada(int indiceInicio, int indiceFim, int qtdPalpites = 5) const;

public:
    explicit AnalisadorLotofacil(const std::vector<Concurso>& concursos);

    bool exibirGrafico(int quantidadeContagem = 0) const;
    bool simularEstrategia(int janelaHistorico = 0) const;
    bool gerarPalpite(int janelaHistorico = 0) const;
    bool analisarEstrategias(int janelaHistorico = 0) const;
};
