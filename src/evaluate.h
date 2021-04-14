#ifndef EVALUATE_H
#define EVALUATE_H

#include <cstdint>
#include <vector>

#include "gamestate.h"
#include "moves.h"

namespace Evaluate
{
    int positionScore(Gamestate::Bitboards *bitboards, bool *playingWhite);
    int evaluateMaterial(Gamestate::Bitboards *bitboards, uint64_t myPieces);
    int evaluateBoardControl(uint64_t myControl);
    int evaluateCenterControl(uint64_t *myControl);
    int evaluatePawns(uint64_t *myPawns);

    bool mateCheck(std::vector <Move> *moves, uint64_t *myKing, uint64_t *unsafe);

    uint64_t myControl(Gamestate::Bitboards *bitboards, bool *playingWhite);
}


#endif


