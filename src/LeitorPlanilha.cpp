#include "LeitorPlanilha.hpp"
#include <iostream>
#include <array>
#include <algorithm>
#include <filesystem>
#include <xlsxio_read.h>

namespace fs = std::filesystem;

std::string LeitorPlanilha::localizarArquivoPadrao() {
    if (fs::exists("data/lotofacil.xlsx")) return "data/lotofacil.xlsx";
    if (fs::exists("../data/lotofacil.xlsx")) return "../data/lotofacil.xlsx";
    return "";
}

LeitorPlanilha::LeitorPlanilha(const std::string& caminhoCustomizado) {
    if (!caminhoCustomizado.empty()) {
        caminhoArquivo_ = caminhoCustomizado;
    } else {
        caminhoArquivo_ = localizarArquivoPadrao();
    }
}

bool LeitorPlanilha::arquivoExiste() const {
    return !caminhoArquivo_.empty() && fs::exists(caminhoArquivo_);
}

bool LeitorPlanilha::carregar(std::vector<Concurso>& concursos) const {
    if (!arquivoExiste()) {
        std::cerr << "Erro: Arquivo 'lotofacil.xlsx' nao encontrado na pasta data/.\n";
        return false;
    }

    xlsxioreader leitor = xlsxioread_open(caminhoArquivo_.c_str());
    if (leitor == nullptr) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << caminhoArquivo_ << "'.\n";
        return false;
    }

    xlsxioreadersheet planilha = xlsxioread_sheet_open(leitor, nullptr, XLSXIOREAD_SKIP_NONE);
    if (planilha == nullptr) {
        std::cerr << "Erro: Nao foi possivel abrir a primeira aba da planilha.\n";
        xlsxioread_close(leitor);
        return false;
    }

    char* valorCelula = nullptr;

    while (xlsxioread_sheet_next_row(planilha)) {
        int numeroConcurso = 0;
        std::array<int, 15> dezenas{};
        int coluna = 0;
        int dezenasLidas = 0;
        bool numeroValido = false;

        while ((valorCelula = xlsxioread_sheet_next_cell(planilha)) != nullptr) {
            if (*valorCelula != '\0') {
                try {
                    if (coluna == 0) {
                        numeroConcurso = std::stoi(valorCelula);
                        if (numeroConcurso > 0) {
                            numeroValido = true;
                        }
                    } else if (coluna >= 2 && coluna <= 16) {
                        int dezena = std::stoi(valorCelula);
                        if (dezena >= 1 && dezena <= 25) {
                            dezenas.at(coluna - 2) = dezena;
                            dezenasLidas++;
                        }
                    }
                } catch (...) {
                }
            }

            xlsxioread_free(valorCelula);
            coluna++;
        }

        if (numeroValido && dezenasLidas == 15) {
            concursos.emplace_back(numeroConcurso, dezenas);
        }
    }

    xlsxioread_sheet_close(planilha);
    xlsxioread_close(leitor);

    if (concursos.empty()) {
        std::cerr << "Erro: Nenhum concurso valido encontrado em '" << caminhoArquivo_ << "'.\n";
        return false;
    }

    if (concursos.front().getNumero() > concursos.back().getNumero()) {
        std::reverse(concursos.begin(), concursos.end());
    }

    return true;
}
