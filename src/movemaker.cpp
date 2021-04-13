#include <string>
#include <iostream>

#include "movemaker.h"
#include "gamestate.h"
#include "consts.h"
#include "moves.h"

void Movemaker::addPiece(Gamestate::Bitboards *bitboards, uint64_t *toBoard, char *piece, bool *isWhite)
{
    switch (*piece)
    {
    case 'P':
        bitboards->p = bitboards->p | *toBoard;
        break;
    case 'K':
        bitboards->k = bitboards->k | *toBoard;
        break;
    case 'Q':
        bitboards->q = bitboards->q | *toBoard;
        break;
    case 'R':
        bitboards->r = bitboards->r | *toBoard;
        break;
    case 'B':
        bitboards->b = bitboards->b | *toBoard;
        break;
    case 'N':
        bitboards->n = bitboards->n | *toBoard;
        break;    
    default:
        throw std::invalid_argument("Invalid piece to add");
        break;
    }

    if (*isWhite == 1)
    {
        bitboards->white = bitboards->white | *toBoard;
    }
    else
    {
        bitboards->black = bitboards->black | *toBoard;
    }
}

void Movemaker::removePiece(Gamestate::Bitboards *bitboards, uint64_t *removeBoard)
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
    else if ((bitboards->k & *removeBoard) > 0)
    {
        bitboards->k = bitboards->k & ~*removeBoard;
    }

    bitboards->white = bitboards->white & ~*removeBoard;
    bitboards->black = bitboards->black & ~*removeBoard;
}

Gamestate::Bitboards Movemaker::makeMove(Gamestate::Bitboards bitboards, Move *move)
{
    Gamestate::Bitboards newBitboards = bitboards;

    removePiece(&newBitboards, &move->fromBoard);
    removePiece(&newBitboards, &move->toBoard);
    addPiece(&newBitboards, &move->toBoard, &move->piece, &move->isWhite);

    if (move->type == 'e')
    {
        uint64_t removeBoard;
        if (move->isWhite == 1)
        {
            removeBoard = move->toBoard << 8;
        }
        else
        {
            removeBoard = move->toBoard >> 8;
        }
        removePiece(&newBitboards, &removeBoard);
    }

    uint64_t toBoard;
    char piece = 'R';
    // wkc = 1, wqc = 2, bkc = 3, bqc = 4
    if (isdigit(move->type))
    {
        switch(move->type)
        {
            case '1':
                toBoard = Consts::intToUINT.at(61);
                break;
            case '2':
                toBoard = Consts::intToUINT.at(59);
                break;
            case '3':
                toBoard = Consts::intToUINT.at(13);
                break;
            case '4':
                toBoard = Consts::intToUINT.at(11);
                break;
            default:
                throw std::invalid_argument("Rook toboard placement error");
        }
        addPiece(&newBitboards, &toBoard, &piece, &move->isWhite);
    }
    return newBitboards;
}