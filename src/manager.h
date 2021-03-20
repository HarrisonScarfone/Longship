#ifndef MANAGER_H
#define MANAGER_H

#include <cstdint>
#include <string>
#include <vector>

#include "gamestate.h"

class Manager
{
    public:
        std::string board[8][8];
        std::uint64_t p, r, n, b, q, k, black, white, enpassant;
        bool bkc, bqc, wkc, wqc;
        // vector<Gamestate::Bitboards> positions;

        Manager();
        void setBoard();
        void arrayToBitboards();
        std::uint64_t stringTo64Bit(std::string inString);
        void printArrayBoard();
        void drawFromBitboards();
        void showBitboardValues();
        void updateArrayFromBitboard(Gamestate::Bitboards bitboards);
        Gamestate::Bitboards getBitboards();

};

#endif  