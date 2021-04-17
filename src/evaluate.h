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

    uint64_t unsafeForMe(Gamestate::Bitboards *bitboards, bool *playingWhite);
}

#endif
