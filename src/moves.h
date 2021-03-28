#ifndef MOVES_H
#define MOVES_H

#include <string>
#include <vector>

#include "gamestate.h"

namespace Moves
{
    std::vector<std::string> possibleMoves(Gamestate::Bitboards &bitboards, bool playingWhite);
    std::string possibleWhitePawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePieces, uint64_t emptySpaces, uint64_t enpassant, bool playingWhite);   
    std::string possibleBlackPawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePeices, uint64_t emptySpaces, uint64_t enpassant, bool playingWhite);
    void addPawnMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset, int x2offset, int y2offset, bool playingWhite);
    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int y1offset, int y2offset, bool playingWhite);
    void addEnPassantMove(std::string *moveString, uint64_t possibleMoves, int xoffset, int yoffset, bool playingWhite);
    uint64_t indexToBitboard(int index);
    uint64_t reverseUint64_t(uint64_t in);
    uint64_t hvMoves(int index, uint64_t occupied);
    uint64_t dAdMoves(int index, uint64_t occupied);
    void addSliderMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int location, char charToAdd);
    std::string possibleRookMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t r, bool playingWhite);
    std::string possibleBishopMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t b, bool playingWhite);
    std::string possibleQueenMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t q, bool playingWhite);
    std::string possibleKnightMoves(uint64_t notMyPieces, uint64_t n, bool playingWhite);
    std::string possibleKingMoves(uint64_t notMyPieces, uint64_t k, uint64_t unsafe, bool playingWhite);
    uint64_t unsafeSpaces(uint64_t occupied, uint64_t theirPawns, uint64_t theirRook, uint64_t theirKnight, uint64_t theirBishop, uint64_t theirQueen, uint64_t theirKing, uint64_t myKing, bool playingWhite);
    std::string possibleWhiteCastleMoves(uint64_t occupied, uint64_t unsafe, uint64_t myKing, bool wkc, bool wqc);
    std::string possibleBlackCastleMoves(uint64_t occupied, uint64_t unsafe, uint64_t myKing, bool bkc, bool bqc);
    Gamestate::Bitboards removeLocationSquareFromBitboards(Gamestate::Bitboards bitboards, uint64_t *board);    uint64_t getMoveBoard(uint64_t inBoard, int fromLocation, int toLocation);
    Gamestate::Bitboards makeMove(Gamestate::Bitboards bitboards, std::string move);
    

};

#endif