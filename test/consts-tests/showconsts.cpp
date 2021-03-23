#include <iostream>
#include <cstdint>

#include "../../src/consts.h"

using namespace std;

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

int main(){

    cout << "************\nShowing non array constants\n************\n";
    
    cout << "FILE_A\n";
    uint64AsBoard(Consts::FILE_A);
    cout << "FILE_H\n";
    uint64AsBoard(Consts::FILE_H);
    cout << "FILE_AB\n";
    uint64AsBoard(Consts::FILE_AB);
    cout << "File_GH\n"; 
    uint64AsBoard(Consts::FILE_GH);
    cout << "RANK_1\n";
    uint64AsBoard(Consts::RANK_1);
    cout << "RANK_4\n";
    uint64AsBoard(Consts::RANK_4);
    cout << "RANK_5\n";
    uint64AsBoard(Consts::RANK_5);
    cout << "RANK_8\n";
    uint64AsBoard(Consts::RANK_8);
    cout << "CENTRE\n";
    uint64AsBoard(Consts::CENTRE);
    cout << "EXTENDED_CENTRE\n";
    uint64AsBoard(Consts::EXTENDED_CENTRE);
    cout << "KING_SIDE\n";
    uint64AsBoard(Consts::KING_SIDE);
    cout << "QUEEN_SIDE\n";
    uint64AsBoard(Consts::QUEEN_SIDE);
    cout << "KING_SPAN\n";
    uint64AsBoard(Consts::KING_SPAN);
    cout << "KNIGHT_SPAN";
    uint64AsBoard(Consts::KNIGHT_SPAN);


    cout << "************\nShowing array rank masks\n************\n";
    for (int i = 0; i < 8; i++)
    {
        uint64AsBoard(Consts::RankMasks8[i]);
    }

    cout << "************\nShowing array file masks\n************\n";
    for (int i = 0; i < 8; i++)
    {
        uint64AsBoard(Consts::FileMasks8[i]);
    }

    cout << "************\nShowing array diagonal masks\n************\n";
    for (int i = 0; i < 15; i++)
    {
        uint64AsBoard(Consts::DiagonalMasks[i]);
    }

    cout << "************\nShowing array anti diagonal masks\n************\n";
    for (int i = 0; i < 15; i++)
    {
        uint64AsBoard(Consts::AntiDiagonalMasks[i]);
    }

}