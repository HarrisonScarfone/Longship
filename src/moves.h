#ifndef MOVES_H
#define MOVES_H

#include <string>

#include "gamestate.h"

namespace Moves
{
    std::string possibleMoves(Gamestate::Bitboards &bitboards);
    std::string possibleWhitePawnMoves(Gamestate::Bitboards &bitboards, uint64_t capturableBlackPeices, uint64_t emptySpaces);
    void addMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset, int x2offset, int y2offset);
    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset);

};

#endif