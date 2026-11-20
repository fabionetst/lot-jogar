#pragma once

#include <string>
#include <vector>

class AplicacaoCLI {

    private:

    std::vector<std::string> args_;

    void exibirAjuda() const;

    static bool converterInteiroPositivo(const std::string &texto, int &valorSaida);

    public:

    AplicacaoCLI(int argc, char* argv[]);
    int executar() const;
};
