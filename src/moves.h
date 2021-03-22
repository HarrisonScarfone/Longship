#ifndef MOVES_H
#define MOVES_H

#include <string>

#include "gamestate.h"

namespace Moves
{
    std::string possibleMoves(Gamestate::Bitboards &bitboards, std::string history);
    std::string possibleWhitePawnMoves(Gamestate::Bitboards &bitboards, uint64_t capturableBlackPeices, uint64_t emptySpaces, std::string history);
    void addPawnMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset, int x2offset, int y2offset);
    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset);
    uint64_t indexToBitboard(int index);
    uint64_t reverseUint64_t(uint64_t in);
    uint64_t hvMoves(int index, uint64_t occupied);
    uint64_t dAdMoves(int index, uint64_t occupied);
    void addSliderMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int location);
    std::string possibleWhiteRook(uint64_t occupied, uint64_t notWhite, uint64_t wr);
    std::string possibleWhiteBishop(uint64_t occupied, uint64_t notWhite, uint64_t wb);
    std::string possibleWhiteQueen(uint64_t occupied, uint64_t notWhite, uint64_t wq);
    std::string possibleWhiteKnight(uint64_t notWhite, uint64_t wn);

};

#endif