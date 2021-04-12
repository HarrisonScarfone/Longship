#ifndef MOVES_H
#define MOVES_H

#include <string>
#include <vector>

#include "gamestate.h"

struct Move
{
    uint64_t fromBoard;
    uint64_t toBoard;

    char type;
    char piece;
    
    bool isWhite;
};

namespace Moves
{

    std::vector <Move> possibleMoves(Gamestate::Bitboards &bitboards, bool playingWhite);

    void addPawnMovesToMoveVector(std::vector <Move> *moves, uint64_t *toBoard, char type, int offset, bool *playingWhite);
    void addMovesToMoveVector(std::vector <Move> *moves, uint64_t *toBoard, uint64_t *fromBoard, char type, bool *playingWhite, char piece);
    void possibleWhitePawnMoves(std::vector <Move> *moves, uint64_t *myPawns, uint64_t *theirPawns, uint64_t *capturablePieces, uint64_t *emptySpaces, uint64_t *enpassant, bool *playingWhite);
    void possibleBlackPawnMoves(std::vector <Move> *moves, uint64_t *myPawns, uint64_t *theirPawns, uint64_t *capturablePeices, uint64_t *emptySpaces, uint64_t *enpassant, bool *playingWhite);
    void possibleRookMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *notMyPieces, uint64_t r, bool *playingWhite);
    void possibleBishopMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *notMyPieces, uint64_t b, bool *playingWhite);
    void possibleQueenMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *notMyPieces, uint64_t q, bool *playingWhite);
    void possibleKnightMoves(std::vector <Move> *moves, uint64_t *notMyPieces, uint64_t n, bool *playingWhite);
    void possibleKingMoves(std::vector <Move> *moves, uint64_t *notMyPieces, uint64_t k, uint64_t *unsafe, bool *playingWhite);
    void possibleWhiteCastleMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *unsafe, uint64_t *myKing, bool *wkc, bool *wqc, bool *playingWhite);
    void possibleBlackCastleMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *unsafe, uint64_t *myKing, bool *bkc, bool *bqc, bool *playingWhite);
    

    uint64_t reverseUint64_t(uint64_t in);
    uint64_t hvMoves(int *index, uint64_t *fromBoard, uint64_t *occupied);
    uint64_t dAdMoves(int *index, uint64_t *fromBoard, uint64_t *occupied);
    uint64_t unsafeFromPawns(uint64_t *theirPawns, bool *playingWhite);
    uint64_t unsafeSpaces(uint64_t *startingOccupied, uint64_t theirPawns, uint64_t theirRook, uint64_t theirKnight, uint64_t theirBishop, uint64_t theirQueen, uint64_t theirKing, uint64_t *myKing, bool *playingWhite);
    

};



#endif