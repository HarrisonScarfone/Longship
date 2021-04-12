#include <string>
#include <iostream>

#include "movemaker.h"
#include "gamestate.h"
#include "consts.h"
#include "moves.h"

void addPiece(Gamestate::Bitboards *bitboards, Move *move)
{
    switch (move->piece)
    {
    case 'P':
        bitboards->p = bitboards->p | move->toBoard;
        break;
    case 'K':
        bitboards->k = bitboards->k | move->toBoard;
        break;
    case 'Q':
        bitboards->q = bitboards->q | move->toBoard;
        break;
    case 'R':
        bitboards->r = bitboards->r | move->toBoard;
        break;
    case 'B':
        bitboards->b = bitboards->b | move->toBoard;
        break;
    case 'N':
        bitboards->n = bitboards->n | move->toBoard;
        break;    
    default:
        throw std::invalid_argument("Invalid piece to add");
        break;
    }

    if (move->isWhite == 1)
    {
        bitboards->white = bitboards->white | move->toBoard;
    }
    else
    {
        bitboards->black = bitboards->black | move->toBoard;
    }
}

void removePiece(Gamestate::Bitboards *bitboards, uint64_t *removeBoard, bool *isWhite)
{
    if ((bitboards->p & *removeBoard) > 0)
    {
        bitboards->p = bitboards->p & ~*removeBoard;
    }
    else if ((bitboards->q & *removeBoard) > 0)
    {
        bitboards->q = bitboards->q & ~*removeBoard;
    }
    else if ((bitboards->n & *removeBoard) > 0)
    {
        bitboards->n = bitboards->n & ~*removeBoard;
    }
    else if ((bitboards->b & *removeBoard) > 0)
    {
        bitboards->b = bitboards->b & ~*removeBoard;
    }
    else if ((bitboards->r & *removeBoard) > 0)
    {
        bitboards->r = bitboards->r & ~*removeBoard;
    }

    if (*isWhite == 1)
    {
        bitboards->white = bitboards->white | *removeBoard;
    }
    else
    {
        bitboards->black = bitboards->black | *removeBoard;
    }
}

Gamestate::Bitboards makeMove(Gamestate::Bitboards bitboards, Move move)
{

}