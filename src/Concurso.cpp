#include "Concurso.hpp"
#include <algorithm>

Concurso::Concurso(int numero, const std::array<int, 15> &dezenas): numero_(numero), dezenas_(dezenas) {}

int Concurso::getNumero() const {

    return numero_;
}

const std::array<int, 15>& Concurso::getDezenas() const {

    return dezenas_;
}

bool Concurso::contemDezena(int dezena) const {
    
    return std::find(dezenas_.begin(), dezenas_.end(), dezena) != dezenas_.end();
}
