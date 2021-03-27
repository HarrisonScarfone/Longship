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
            if (i % 5 == 0)
            {
                std::cout << "\n";
            }
            else if ((i - 3) % 5 == 0)
            {
                std::cout << "->";
            }
            
            std::cout << inString.at(i); // 0 1 2 3 4 5 / 
        }

        std::cout << "\n";
    }

    void uint64AsBoard(uint64_t in)
    {

        int count = 0;
        std::string outString;

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
        std::cout << "\n" << outString << "\n\n";
    }

    void showAllBitboardsAsBoards(Gamestate::Bitboards bitboards)
    {
        std::cout << "White";
        uint64AsBoard(bitboards.white);
        std::cout << "Black";
        uint64AsBoard(bitboards.black);
        std::cout << "Pawns";
        uint64AsBoard(bitboards.p);
        std::cout << "Rooks";
        uint64AsBoard(bitboards.r);
        std::cout << "Bishops";
        uint64AsBoard(bitboards.b);
        std::cout << "Knights";
        uint64AsBoard(bitboards.n);
        std::cout << "Queens";
        uint64AsBoard(bitboards.q);
        std::cout << "Kings";
        uint64AsBoard(bitboards.k);
        std::cout << "En Passant Capture Available";
        uint64AsBoard(bitboards.enpassant);
        std::cout << "White Pawns";
        uint64AsBoard(bitboards.p & bitboards.white);
        std::cout << "White Rooks";
        uint64AsBoard(bitboards.r & bitboards.white);
        std::cout << "White Knights";
        uint64AsBoard(bitboards.n & bitboards.white);
        std::cout << "White Bishops";
        uint64AsBoard(bitboards.b & bitboards.white);
        std::cout << "White Queens";
        uint64AsBoard(bitboards.q & bitboards.white);
        std::cout << "White Kings";
        uint64AsBoard(bitboards.k & bitboards.white);
        std::cout << "black Pawns";
        uint64AsBoard(bitboards.p & bitboards.black);
        std::cout << "black Rooks";
        uint64AsBoard(bitboards.r & bitboards.black);
        std::cout << "black Knights";
        uint64AsBoard(bitboards.n & bitboards.black);
        std::cout << "black Bishops";
        uint64AsBoard(bitboards.b & bitboards.black);
        std::cout << "black Queens";
        uint64AsBoard(bitboards.q & bitboards.black);
        std::cout << "black Kings";
        uint64AsBoard(bitboards.k & bitboards.black);
    }

    void showMoveVector(std::vector<std::string> moveVector)
    {
        for (int i = 0; i < moveVector.size(); i++)
        {
            std::cout << moveVector.at(i) << "\n";
        }
    }
};
