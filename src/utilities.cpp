#include <iostream>
#include <string>

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

    void showBitboardValues(Gamestate::Bitboards &bitboards){
        printuint64InBinary(bitboards.p);
        std::cout << "\npawns above\n";
        printuint64InBinary(bitboards.r);
        std::cout << "\nrooks above\n";
        printuint64InBinary(bitboards.n);
        std::cout << "\nknights above\n";
        printuint64InBinary(bitboards.b);
        std::cout << "\nbishops above\n";
        printuint64InBinary(bitboards.q);
        std::cout << "\nqueens above\n";
        printuint64InBinary(bitboards.k);
        std::cout << "\nkings above\n";
        printuint64InBinary(bitboards.white);
        std::cout << "\nwhite above\n";
        printuint64InBinary(bitboards.black);
        std::cout << "\nblack above\n" << bitboards.bkc << bitboards.bqc << bitboards.wkc << bitboards.wqc << "\n";
    }

    void showSplitMovestring(std::string inString)
    {
        for (int i = 0; i < inString.length(); i++)
        {
            if (i % 4 == 0)
            {
                std::cout << "\n";
            }
            else if (i % 2 == 0)
            {
                std::cout << "->";
            }
            
            std::cout << inString.at(i);
        }

        std::cout << "\n";
    }

    void uint64AsBoard(uint64_t in)
    {

        std::string binary = "";

        while (in != 0)
        {
            if (in % 2 == 0)
            {
                binary = "0" + binary;
            }
            else
            {
                binary = "1" + binary;
            }
            in = in / 2;
        }

        for (int i=0; i < binary.length(); i++)
        {
            if (i % 8 == 0)
            {
                std::cout << "\n";
            }

            std::cout << binary.at(i);
        }

        std::cout << "\n";
    }
};
