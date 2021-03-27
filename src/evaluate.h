#ifndef EVALUATE_H
#define EVALUATE_H

#include <cstdint>

#include "gamestate.h"

namespace Evaluate
{
    int positionScore(Gamestate::Bitboards bitboards, bool playingWhite);
    int evaluateMaterial(Gamestate::Bitboards bitboards, uint64_t myPieces);
}


#endif


