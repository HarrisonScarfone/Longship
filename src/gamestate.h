#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdint>

namespace Gamestate 
{
    struct Bitboards
    {
        uint64_t p;
        uint64_t r;
        uint64_t n;
        uint64_t b;
        uint64_t q;
        uint64_t k;
        uint64_t white;
        uint64_t black;
        uint64_t enpassant;
        bool bkc;
        bool bqc;
        bool wkc;
        bool wqc;
    }; 
}

#endif