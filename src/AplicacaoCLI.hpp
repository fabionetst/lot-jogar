#pragma once

#include <string>
#include <vector>

class AplicacaoCLI {
private:
    static constexpr const char* VERSAO = "0.2.0";
    std::vector<std::string> args_;

    void exibirAjuda() const;
    static bool converterInteiroPositivo(const std::string& texto, int& valorSaida);

public:
    AplicacaoCLI(int argc, char* argv[]);
    int executar() const;
};
