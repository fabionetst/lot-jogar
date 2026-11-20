#pragma once

#include <string>
#include <vector>
#include "Concurso.hpp"

class LeitorPlanilha {
private:
    std::string caminhoArquivo_;
    static std::string localizarArquivoPadrao();

public:
    explicit LeitorPlanilha(const std::string& caminhoCustomizado = "");

    bool arquivoExiste() const;
    bool carregar(std::vector<Concurso>& concursos) const;
};
