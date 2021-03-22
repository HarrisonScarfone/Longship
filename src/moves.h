#ifndef MOVES_H
#define MOVES_H

#include <string>

#include "gamestate.h"

namespace Moves
{
    std::string possibleMoves(Gamestate::Bitboards &bitboards, std::string history, bool playingWhite);
    std::string possibleWhitePawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePieces, uint64_t emptySpaces, std::string history);   
    std::string possibleBlackPawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePeices, uint64_t emptySpaces, std::string history);
    void addPawnMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset, int x2offset, int y2offset);
    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset);
    uint64_t indexToBitboard(int index);
    uint64_t reverseUint64_t(uint64_t in);
    uint64_t hvMoves(int index, uint64_t occupied);
    uint64_t dAdMoves(int index, uint64_t occupied);
    void addSliderMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int location);
    std::string possibleRookMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t r);
    std::string possibleBishopMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t b);
    std::string possibleQueenMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t q);
    std::string possibleKnightMoves(uint64_t notMyPieces, uint64_t n);
    std::string possibleKingMoves(uint64_t notMyPieces, uint64_t k);

};

#endif