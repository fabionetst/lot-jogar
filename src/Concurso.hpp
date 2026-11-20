#pragma once

#include <array>

class Concurso {
    private:

    int numero_;
    std::array<int, 15> dezenas_;

    public:
        
    int getNumero() const;
    const std::array<int, 15>& getDezenas() const;
    bool contemDezena(int dezena) const;

    Concurso(int numero, const std::array<int, 15>& dezenas);
};
