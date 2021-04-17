#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <cstdint>

#include "gamestate.h"

class Zobrist
{
    public:
        // 12 pieces (b&w, p, r, n, b, q, k)
        uint64_t piecePosition [12][64];
        // 4 castling bits
        uint64_t castling [4];
        // 8 enpassant files
        uint64_t enpassant [8];
        // whos turn is it
        uint64_t whosTurn [2];

        uint64_t startingHash;

        Zobrist();

        void initZobristNumbers();   
        void setStartingZobristHash(Gamestate::Bitboards *bitboards, bool *whiteToPlay);  

};

#endif