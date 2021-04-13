#include <cstdint>
#include <string>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>

#include "moves.h"
#include "consts.h"
#include "manager.h"
#include "gamestate.h"
#include "utilities.h"
#include "movemaker.h"

std::vector <Move> Moves::possibleMoves(Gamestate::Bitboards *bitboards, bool *playingWhite)
{
    uint64_t capturablePieces = 
        (bitboards->p | bitboards->n | bitboards->r | bitboards->b | bitboards->q);
    uint64_t pieces = 
        (bitboards->p | bitboards->n | bitboards->r | bitboards->b | bitboards->q | bitboards->k);
    uint64_t occupied =
        (bitboards->p | bitboards->r | bitboards->n | bitboards->b | bitboards->q | bitboards->k);
    uint64_t emptySpaces = ~occupied;

    uint64_t    myPieces, myPawns, myRook, myBishop, myQueen, myKnight, myKing,  
                theirPieces, theirPawns, theirRook, theirBishop, theirQueen, theirKnight, theirKing,
                notMyPieces, unsafe;

    if (*playingWhite == 1)
    {
        myPieces = bitboards->white;
        theirPieces = bitboards->black;
    }
    else
    {
        myPieces = bitboards->black;
        theirPieces = bitboards->white;
    }

    capturablePieces = capturablePieces & theirPieces;
    notMyPieces = ~((pieces & myPieces) | (theirPieces & bitboards->k));
    myPawns = bitboards->p & myPieces;
    myRook = bitboards->r & myPieces;
    myBishop = bitboards->b & myPieces;
    myKnight = bitboards->n & myPieces;
    myQueen = bitboards->q & myPieces;
    myKing = bitboards->k & myPieces;
    theirPawns = bitboards->p & theirPieces;
    theirRook = bitboards->r & theirPieces;
    theirBishop = bitboards->b & theirPieces;
    theirKnight = bitboards->n & theirPieces;
    theirQueen = bitboards->q & theirPieces;
    theirKing = bitboards->k & theirPieces;

    unsafe = unsafeSpaces(&occupied, theirPawns, theirRook, theirKnight, theirBishop, theirQueen, theirKing, &myKing, playingWhite);

    std::vector <Move> moves;

    if (*playingWhite == 1)
    {
        possibleWhitePawnMoves(&moves, &myPawns, &theirPawns, &capturablePieces, &emptySpaces, &bitboards->enpassant, playingWhite);
        possibleWhiteCastleMoves(&moves, &occupied, &unsafe, &myKing, &bitboards->wkc, &bitboards->wqc,playingWhite);
    }
    else
    {
        possibleBlackPawnMoves(&moves, &myPawns, &theirPawns, &capturablePieces, &emptySpaces, &bitboards->enpassant, playingWhite);
        possibleBlackCastleMoves(&moves, &occupied, &unsafe, &myKing, &bitboards->bkc, &bitboards->bqc, playingWhite);
    }

        possibleRookMoves(&moves, &occupied, &notMyPieces, myRook, playingWhite);
        possibleBishopMoves(&moves, &occupied, &notMyPieces, myBishop, playingWhite);
        possibleQueenMoves(&moves, &occupied, &notMyPieces, myQueen, playingWhite);
        possibleKnightMoves(&moves, &notMyPieces, myKnight, playingWhite);
        possibleKingMoves(&moves, &notMyPieces, myKing, &unsafe, playingWhite);   

    return moveVectorValidator(bitboards, &moves, playingWhite);    
}

std::vector <Move> Moves::moveVectorValidator(Gamestate::Bitboards *bitboards, std::vector <Move> *moves, bool *playingWhite)
{
    uint64_t myKing, unsafe;

    std::vector <Move> finalMoves;

    for (int i=0; i < moves->size(); i++)
    {
        Move currMove = moves->at(i);
        Gamestate::Bitboards potentialMoveBitboards = Movemaker::makeMove(*bitboards, &currMove);

        uint64_t newOccupied = (potentialMoveBitboards.p | potentialMoveBitboards.r | potentialMoveBitboards.n | potentialMoveBitboards.b | potentialMoveBitboards.q | potentialMoveBitboards.k);

        if (*playingWhite == true)
        {
            myKing = potentialMoveBitboards.k & potentialMoveBitboards.white;
            unsafe = unsafeSpaces(
                &newOccupied, 
                potentialMoveBitboards.p & potentialMoveBitboards.black, 
                potentialMoveBitboards.r & potentialMoveBitboards.black, 
                potentialMoveBitboards.n & potentialMoveBitboards.black, 
                potentialMoveBitboards.b & potentialMoveBitboards.black, 
                potentialMoveBitboards.q & potentialMoveBitboards.black, 
                potentialMoveBitboards.k & potentialMoveBitboards.black, 
                &myKing, 
                playingWhite
            );
        }
        else
        {
            myKing = potentialMoveBitboards.k & potentialMoveBitboards.black;
            unsafe = unsafeSpaces(
                &newOccupied, 
                potentialMoveBitboards.p & potentialMoveBitboards.white, 
                potentialMoveBitboards.r & potentialMoveBitboards.white, 
                potentialMoveBitboards.n & potentialMoveBitboards.white, 
                potentialMoveBitboards.b & potentialMoveBitboards.white, 
                potentialMoveBitboards.q & potentialMoveBitboards.white, 
                potentialMoveBitboards.k & potentialMoveBitboards.white, 
                &myKing, 
                playingWhite
            );

        }
        
        if ((myKing & unsafe) == 0)
        {
            finalMoves.push_back(currMove);
        }
    }

    return finalMoves;
}

void Moves::addPawnMovesToMoveVector(std::vector <Move> *moves, uint64_t *toBoard, char type, int offset, bool *playingWhite)
{
    Move thisMove;

    thisMove.isWhite = *playingWhite;
    thisMove.type = type;

    int index = 0;
    while (*toBoard > 0)
    {
        if (*toBoard & 1)
        {
            thisMove.toBoard = Consts::intToUINT.at(index);
            thisMove.fromBoard = Consts::intToUINT.at(index - offset);

            if (type == 'u')
            {
                thisMove.piece = 'Q';
                moves->push_back(thisMove);
                thisMove.piece = 'R';
                moves->push_back(thisMove);
                thisMove.piece = 'B';
                moves->push_back(thisMove);
                thisMove.piece = 'N';
                moves->push_back(thisMove);
            }
            else
            {
                thisMove.piece = 'P';
                moves->push_back(thisMove);
            }
        }
        *toBoard = *toBoard >> 1;
        index++;
    }
}

void Moves::addMovesToMoveVector(std::vector <Move> *moves, uint64_t *toBoard, uint64_t *fromBoard, char type, bool *playingWhite, char piece)
{
    Move newMove;

    newMove.type = type;
    newMove.fromBoard = *fromBoard;
    newMove.isWhite = *playingWhite;
    newMove.piece = piece;

    int index = 0;
    while (*toBoard > 0)
    {
        if (*toBoard & 1)
        {   
            newMove.toBoard = Consts::intToUINT.at(index);
            moves->push_back(newMove);
        }
        index++;
        *toBoard = *toBoard >> 1;
    }
}

uint64_t Moves::reverseUint64_t(uint64_t in)
{
    uint64_t temp = 0;
    int index = 0;

    while (index < 64)
    {
        temp = temp << 1;
        if (in & 1 == 1)
        {
            temp = temp ^ 1;                
        }
        index++;
        in = in >> 1;
    }

    return temp;
}

uint64_t Moves::hvMoves(int *index, uint64_t *fromBoard, uint64_t *occupied)
{
    uint64_t horizontal = (*occupied - (2 * *fromBoard)) ^ reverseUint64_t(reverseUint64_t(*occupied) - 2 * reverseUint64_t(*fromBoard));
    uint64_t vertical = ((*occupied & Consts::FileMasks8[*index % 8]) - (2 * *fromBoard)) ^ reverseUint64_t(reverseUint64_t(*occupied & Consts::FileMasks8[*index % 8]) - (2 * reverseUint64_t(*fromBoard)));

    uint64_t temp = (horizontal & Consts::RankMasks8[*index / 8]) | (vertical & Consts::FileMasks8[*index % 8]);

    return temp;
}

uint64_t Moves::dAdMoves(int *index, uint64_t *fromBoard, uint64_t *occupied)
{
    uint64_t diagonal = ((*occupied & Consts::DiagonalMasks[(*index / 8) + (*index % 8)]) - (2 * *fromBoard) ^ reverseUint64_t(reverseUint64_t(*occupied & Consts::DiagonalMasks[(*index / 8) + (*index % 8)]) - (2 * reverseUint64_t(*fromBoard))));
    uint64_t antidiagonal = ((*occupied & Consts::AntiDiagonalMasks[(*index / 8) + 7 - (*index % 8)]) - (2 * *fromBoard) ^ reverseUint64_t(reverseUint64_t(*occupied & Consts::AntiDiagonalMasks[(*index / 8) + 7 - (*index % 8)]) - (2 * reverseUint64_t(*fromBoard))));

    uint64_t temp = (diagonal & Consts::DiagonalMasks[(*index / 8) + (*index % 8)]) | (antidiagonal & Consts::AntiDiagonalMasks[(*index / 8) + 7 - (*index % 8)]);

    return temp;
}

void Moves::possibleWhitePawnMoves(std::vector <Move> *moves, uint64_t *myPawns, uint64_t *theirPawns, uint64_t *capturablePieces, uint64_t *emptySpaces, uint64_t *enpassant, bool *playingWhite)
{
    /*
    Pawn moves steps through each variant of pawn move and 
    builds a move string with all the available moves
    */
    // save a 64 bit for use in move calculation
    uint64_t temp;
    int index;

    // check move up single space
    temp = (*myPawns >> 8) & *emptySpaces & ~Consts::RANK_8;
    addPawnMovesToMoveVector(moves, &temp, 's', -8, playingWhite);

    // two squares forward
    temp = (*myPawns >> 16) & *emptySpaces & (*emptySpaces >> 8) & Consts::RANK_4;
    addPawnMovesToMoveVector(moves, &temp, 'd', -16, playingWhite);

    // capture right
    temp = (*myPawns >> 7) & *capturablePieces & ~Consts::RANK_8 & ~Consts::FILE_A;
    addPawnMovesToMoveVector(moves, &temp, 's', -7, playingWhite);

    // capture left
    temp = (*myPawns >> 9) & *capturablePieces & ~Consts::RANK_8 & ~Consts::FILE_H;
    addPawnMovesToMoveVector(moves, &temp, 's', -9, playingWhite);

    // promote by right capture
    temp = (*myPawns >> 7) & *capturablePieces & Consts::RANK_8 & ~Consts::FILE_A;
    addPawnMovesToMoveVector(moves, &temp, 'u', -7, playingWhite);

    // promote by left capture
    temp = (*myPawns >> 9) & *capturablePieces & Consts::RANK_8 & ~Consts::FILE_H;
    addPawnMovesToMoveVector(moves, &temp, 'u', -9, playingWhite);

    // promote by forward movement
    temp = (*myPawns >> 8) & *emptySpaces & Consts::RANK_8;
    addPawnMovesToMoveVector(moves, &temp, 'u', -8, playingWhite);

    // en passant right
    temp = (*myPawns << 1) & (*theirPawns) & Consts::RANK_5 & ~Consts::FILE_A & *enpassant;
    addPawnMovesToMoveVector(moves, &temp, 'e', -7, playingWhite);

    // en passant left
    temp = (*myPawns >> 1) & (*theirPawns) & Consts::RANK_5 & ~Consts::FILE_H & *enpassant;
    addPawnMovesToMoveVector(moves, &temp, 'e', -9, playingWhite);
}

void Moves::possibleBlackPawnMoves(std::vector <Move> *moves, uint64_t *myPawns, uint64_t *theirPawns, uint64_t *capturablePeices, uint64_t *emptySpaces, uint64_t *enpassant, bool *playingWhite)
{
    uint64_t temp;
    int index;

    temp = (*myPawns << 8) & *emptySpaces & ~Consts::RANK_1;;
    addPawnMovesToMoveVector(moves, &temp, 's', 8, playingWhite);

    temp = (*myPawns << 16) & *emptySpaces & (*emptySpaces << 8) & Consts::RANK_5;
    addPawnMovesToMoveVector(moves, &temp, 'd', 16, playingWhite);

    temp = (*myPawns << 7) & *capturablePeices & ~Consts::RANK_1 & ~Consts::FILE_H;
    addPawnMovesToMoveVector(moves, &temp, 's', 7, playingWhite);

    temp = (*myPawns << 9) & *capturablePeices & ~Consts::RANK_1 & ~Consts::FILE_A;
    addPawnMovesToMoveVector(moves, &temp, 's', 9, playingWhite);

    temp = (*myPawns << 7) & *capturablePeices & Consts::RANK_1 & ~Consts::FILE_H;
    addPawnMovesToMoveVector(moves, &temp, 'u', 7, playingWhite);

    temp = (*myPawns << 9) & *capturablePeices & Consts::RANK_1 & ~Consts::FILE_A;
    addPawnMovesToMoveVector(moves, &temp, 'u', 9, playingWhite);

    temp = (*myPawns << 8) & *emptySpaces & Consts::RANK_1;
    addPawnMovesToMoveVector(moves, &temp, 'u', 8, playingWhite);

    temp = (*myPawns >> 1) & (*theirPawns) & Consts::RANK_5 & ~Consts::FILE_A & *enpassant;
    addPawnMovesToMoveVector(moves, &temp, 'e', 7, playingWhite);

    temp = (*myPawns << 1) & (*theirPawns) & Consts::RANK_5 & ~Consts::FILE_H & *enpassant;
    addPawnMovesToMoveVector(moves, &temp, 'e', 9, playingWhite);
}

void Moves::possibleRookMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *notMyPieces, uint64_t r, bool *playingWhite)
{
    int location = 0;
    while (r > 0)
    {
        if (r & 1)
        {        
            uint64_t fromBoard = Consts::intToUINT.at(location);        
            uint64_t moveBitboard = hvMoves(&location, &fromBoard, occupied) & *notMyPieces;
            addMovesToMoveVector(moves, &moveBitboard, &fromBoard, 's', playingWhite, 'R');
        }
        r = r >> 1;
        location++;
    }
}

void Moves::possibleBishopMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *notMyPieces, uint64_t b, bool *playingWhite)
{
    int location = 0;
    while (b > 0)
    {
        if (b & 1)
        {
            uint64_t fromBoard = Consts::intToUINT.at(location); 
            uint64_t moveBitboard = dAdMoves(&location, &fromBoard, occupied) & *notMyPieces;
            addMovesToMoveVector(moves, &moveBitboard, &fromBoard, 's', playingWhite, 'B');
        }
        b = b >> 1;
        location++;
    }
}

void Moves::possibleQueenMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *notMyPieces, uint64_t q, bool *playingWhite)
{
    int location = 0;
    while (q > 0)
    {
        if (q & 1)
        {
            uint64_t fromBoard = Consts::intToUINT.at(location);
            uint64_t moveBitboard = (dAdMoves(&location, &fromBoard, occupied) | hvMoves(&location, &fromBoard, occupied)) & *notMyPieces;
            addMovesToMoveVector(moves, &moveBitboard, &fromBoard, 's', playingWhite, 'Q');
        }
        q = q >> 1;
        location++;
    }
}

void Moves::possibleKnightMoves(std::vector <Move> *moves, uint64_t *notMyPieces, uint64_t n, bool *playingWhite)
{

    int location = 0;
    uint64_t potentialKnightMoves, moveBitboard;

    while (n > 0)
    {
        if (n & 1)
        {
            if (location > 18)
            {
                potentialKnightMoves = Consts::KNIGHT_SPAN << (location - 18);
            }
            else
            {
                potentialKnightMoves = Consts::KNIGHT_SPAN >> (18 - location);
            }

            if ((location % 8) < 4)
            {
                moveBitboard = potentialKnightMoves & ~Consts::FILE_GH & *notMyPieces;
            }
            else
            {
                moveBitboard = potentialKnightMoves & ~Consts::FILE_AB & *notMyPieces;
            }

            uint64_t fromBoard = Consts::intToUINT.at(location);
            addMovesToMoveVector(moves, &moveBitboard, &fromBoard, 's', playingWhite, 'N');
        }
        n = n >> 1;
        location++;            
    }
}

void Moves::possibleKingMoves(std::vector <Move> *moves, uint64_t *notMyPieces, uint64_t k, uint64_t *unsafe, bool *playingWhite)
{
    int location = 0;
    uint64_t potentialKingMoves, moveBitboard;
    while (k > 0)
    {
        if (k & 1)
        {
            if (location > 9)
            {
                potentialKingMoves = Consts::KING_SPAN << (location - 9);
            }
            else
            {
                potentialKingMoves = Consts::KING_SPAN >> (9 - location);
            }

            if ((location % 8) < 4)
            {
                moveBitboard = potentialKingMoves & ~Consts::FILE_GH & *notMyPieces;
            }
            else
            {
                moveBitboard = potentialKingMoves & ~Consts::FILE_AB & *notMyPieces;
            }

            moveBitboard = moveBitboard & ~*unsafe;

            // if we are trying to move to an unsafe square its not allowed and dont add the move
            uint64_t fromBoard = Consts::intToUINT.at(location);
            addMovesToMoveVector(moves, &moveBitboard, &fromBoard, 's', playingWhite, 'K');

            // same as with the knight we can use add slider move method for the kings
            // bitboard as well
            
        }
        k = k >> 1;
        location++;
    }
}

uint64_t Moves::unsafeFromPawns(uint64_t *theirPawns, bool *playingWhite)
{
    if (*playingWhite)
    {
        return ((*theirPawns << 7) & ~Consts::FILE_H) | ((*theirPawns << 9) & ~Consts::FILE_A);
    }
    else
    {
        return ((*theirPawns >> 7) & ~Consts::FILE_A) | ((*theirPawns >> 9) & ~Consts::FILE_H);
    }
}

uint64_t Moves::unsafeSpaces(uint64_t *startingOccupied, uint64_t theirPawns, uint64_t theirRook, uint64_t theirKnight, uint64_t theirBishop, uint64_t theirQueen, uint64_t theirKing, uint64_t *myKing, bool *playingWhite)
{
    uint64_t unsafe;
    uint64_t temp;
    int location;

    uint64_t occupied = *startingOccupied & ~*myKing;

    /*
    the variable temp carries all through and used for control calculations at each step
    note that it is reassigned in each section it is used in this method
    */

    // pawn capture right | left
    unsafe = unsafeFromPawns(&theirPawns, playingWhite);

    // handle squares controlled by knights
    location = 0;
    while (theirKnight > 0)
    {
        if (theirKnight & 1)
        {
            if (location > 18)
            {
                temp = Consts::KNIGHT_SPAN << (location - 18);
            }
            else
            {
                temp = Consts::KNIGHT_SPAN >> (18 - location);
            }

            if ((location % 8) < 4)
            {
                temp = temp & ~Consts::FILE_GH;
            }
            else
            {
                temp = temp & ~Consts::FILE_AB;
            }

            unsafe = unsafe | temp;
        }
        theirKnight = theirKnight >> 1;
        location++;
    }

    // handle squares controlled diagonally by bishop &| queen
    uint64_t theirQB = theirBishop | theirQueen;        
    location = 0;
    while (theirQB > 0)
    {
        if (theirQB & 1)
        {
            uint64_t fromBoard = Consts::intToUINT.at(location);
            temp = dAdMoves(&location, &fromBoard, &occupied);
            unsafe = unsafe | temp;
        }
        location++;
        theirQB = theirQB >> 1;
    }

    // handle squares controlled diagonally by rook &| queen
    uint64_t theirQR = theirRook | theirQueen;
    location = 0;
    while (theirQR > 0)
    {
        if (theirQR & 1)
        {
            uint64_t fromBoard = Consts::intToUINT.at(location);
            temp = hvMoves(&location, &fromBoard, &occupied);
            unsafe = unsafe | temp;
        }
        location++;
        theirQR = theirQR >> 1;
    }

    // handle the squares controlled by the king
    location = 0;
    while (theirKing > 0)
    {
        if (theirKing & 1)
        {
            if (location > 9)
            {
                temp = Consts::KING_SPAN << (location - 9);
            }
            else
            {
                temp = Consts::KING_SPAN >> (9 - location);
            }

            if ((location % 8) < 4)
            {
                temp = temp & ~Consts::FILE_GH;
            }
            else
            {
                temp = temp & ~Consts::FILE_AB;
            }

            unsafe = unsafe | temp;
        }
        theirKing = theirKing >> 1;
        location++;
    }

    unsafe = unsafe & ~occupied;
    return unsafe;
}

void Moves::possibleWhiteCastleMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *unsafe, uint64_t *myKing, bool *wkc, bool *wqc, bool *playingWhite)
{
    std::string temp = "";
    if (*wkc == 1 && ((*occupied & Consts::WKC_INBETWEEN) == 0) && ((*unsafe & Consts::WKC_INBETWEEN & Consts::WHITE_KING_START) == 0) && ((*myKing & Consts::WHITE_KING_START) > 0))
    {
        Move thisMove = {
            .fromBoard = Consts::intToUINT.at(60),
            .toBoard = Consts::intToUINT.at(62),
            .type = '1',
            .piece = 'K',
            .isWhite = *playingWhite
        };

        moves->push_back(thisMove);
    }
    if (*wqc == 1 && ((*occupied & Consts::WQC_INBETWEEN) == 0) && ((*unsafe & Consts::WQC_INBETWEEN & Consts::WHITE_KING_START) == 0) && ((*myKing & Consts::WHITE_KING_START) > 0))
    {
        Move thisMove = {
            .fromBoard = Consts::intToUINT.at(60),
            .toBoard = Consts::intToUINT.at(58),
            .type = '2',
            .piece = 'K',
            .isWhite = *playingWhite,   
        };

        moves->push_back(thisMove);
    }
}

void Moves::possibleBlackCastleMoves(std::vector <Move> *moves, uint64_t *occupied, uint64_t *unsafe, uint64_t *myKing, bool *bkc, bool *bqc, bool *playingWhite)
{
    std::string temp = "";
    if (*bkc == 1 && ((*occupied & Consts::BKC_INBETWEEN) == 0) && ((*unsafe & Consts::BKC_INBETWEEN & Consts::BLACK_KING_START) == 0) && ((*myKing & Consts::BLACK_KING_START) > 0))
    {
        Move thisMove = {
            .fromBoard = Consts::intToUINT.at(4),
            .toBoard = Consts::intToUINT.at(6),
            .type = '3',
            .piece = 'K',
            .isWhite = *playingWhite,  
        };

        moves->push_back(thisMove);
    }
    if (*bqc == 1 && ((*occupied & Consts::BQC_INBETWEEN) == 0) && ((*unsafe & Consts::BQC_INBETWEEN & Consts::BLACK_KING_START) == 0) && ((*myKing & Consts::BLACK_KING_START) > 0))
    {
        Move thisMove = {
            .fromBoard = Consts::intToUINT.at(4),
            .toBoard = Consts::intToUINT.at(2),
            .type = '4',
            .piece = 'K',
            .isWhite = *playingWhite, 
        };

        moves->push_back(thisMove);
    }
}
