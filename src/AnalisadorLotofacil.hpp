#pragma once

#include <vector>
#include <utility>
#include "Concurso.hpp"

class AnalisadorLotofacil {
private:
    const std::vector<Concurso>& concursos_;
    std::vector<std::pair<int, int>> calcularFrequencias(int indiceInicio, int indiceFim) const;

public:
    explicit AnalisadorLotofacil(const std::vector<Concurso>& concursos);

    bool exibirGrafico(int quantidadeContagem = 0) const;
    bool simularEstrategia(int janelaHistorico = 0) const;
    bool gerarPalpite(int janelaHistorico = 0) const;

};
