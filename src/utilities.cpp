#include <iostream>
#include <string>

#include "gamestate.h"
#include "utilities.h"

namespace Utilities
{

    void printuint64InBinary(uint64_t number)
    {
        std::string temp = "";
        int index = 0;

        while (index < 64)
        {
            if (number & 1)
            {
                temp = "1" + temp;
            }
            else
            {
                temp = "0" + temp;
            }

            index++;
            number = number >> 1;
        }

        std::cout << temp << "\n"; 
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

        int count = 0;
        std::string outString;
        
        std::cout << in << "\n";

        while (count < 64)
        {
            if (count % 8 == 0)
            {
                outString += "\n";
            }

            if (in % 2 == 0)
            {
                outString += "0";
            }
            else
            {
                outString += "1";
            }

            in = in / 2;
            count++;
        }
        std::cout << outString << "\n";
    }
};
