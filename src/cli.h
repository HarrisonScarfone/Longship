#ifndef CLI_H
#define CLI_H

#include <cstdint>
#include <string>

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

};

#endif  