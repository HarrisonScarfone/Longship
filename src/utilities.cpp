#include <iostream>

#include "gamestate.h"

namespace Utilities
{
    void printuint64InBinary(uint64_t number)
    {
        if (number >> 1) {
            printuint64InBinary(number >> 1);
        }
        putc((number & 1) ? '1' : '0', stdout);
    }

    void newLine()
    {
        std::cout << "\n";
    }

    void showBitboardValues(Gamestate::Bitboards bitboards){
        printuint64InBinary(bitboards.p);
        std::cout << "\n";
        printuint64InBinary(bitboards.r);
        std::cout << "\n";
        printuint64InBinary(bitboards.n);
        std::cout << "\n";
        printuint64InBinary(bitboards.b);
        std::cout << "\n";
        printuint64InBinary(bitboards.q);
        std::cout << "\n";
        printuint64InBinary(bitboards.k);
        std::cout << "\n";
        printuint64InBinary(bitboards.enpassant);
        std::cout << "\n" << bitboards.bkc << bitboards.bqc << bitboards.wkc << bitboards.wqc << "\n";
    }
};
