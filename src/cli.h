#ifndef CLI_H
#define CLI_H

#include <cstdint>
#include <string>

namespace Game
{
    struct Bitboards
    {
        uint64_t wp;
        uint64_t wr;
        uint64_t wn;
        uint64_t wb;
        uint64_t wq;
        uint64_t wk;
        uint64_t bp;
        uint64_t br;
        uint64_t bn;
        uint64_t bb;
        uint64_t bq;
        uint64_t bk;
    };
}

class Board
{
    public:
        std::string board[8][8];
        std::uint64_t wp, wr, wn, wb, wq, wk, bp, br, bn, bb, bq, bk;

        Board();
        void setBoard();
        void arrayToBitboards();
        std::uint64_t stringTo64Bit(std::string inString);
        void printArrayBoard();
        void showBitboardValues();
        void printuint64InBinary(uint64_t number);
        void newLine();
        void updateFromBitboard();
        Game::Bitboards getBitboards();

};

#endif  