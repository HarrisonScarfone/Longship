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

    // since an en passant move in invalidated on the next turn zero the enpassant board
    // before we start.  it gets set back if its needed for the next move gen
    newBitboards.enpassant = 0;

    // check for extra removal due to en passant
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

    if (move->type == 'd')
    {
        int enpassantSquare = Consts::uintToInt.at(move->toBoard);
        newBitboards.enpassant = Consts::FileMasks8[enpassantSquare % 8];
    }
    
    // need to remove castling rights if a rook or king moves
    if (move->piece == 'K')
    {
        if (move->isWhite == 1)
        {
            newBitboards.wkc = false;
            newBitboards.wqc = false;
        }
        else
        {
            newBitboards.bkc = false;
            newBitboards.wqc = false;
        }
    }

    // remove castle rights if rook moves from a square
    if (move->piece == 'R')
    {
        switch (move->fromBoard)
        {
        case Consts::BKC:
            newBitboards.bkc = false;
            break;
        case Consts::BQC:
            newBitboards.bqc = false;
            break;
        case Consts::WKC:
            newBitboards.wkc = false;
            break;
        case Consts::WQC:
            newBitboards.wqc = false;
            break;
        }
    }

    // handle a castiling move. remove and replace the rook
    // wkc = 1, wqc = 2, bkc = 3, bqc = 4
    if (isdigit(move->type))
    {
        uint64_t toBoard;
        uint64_t fromBoard;
        char piece = 'R';

        switch(move->type)
        {
            case '1':
                toBoard = Consts::intToUINT.at(61);
                fromBoard = Consts::WKC;
                break;
            case '2':
                toBoard = Consts::intToUINT.at(59);
                fromBoard = Consts::WQC;
                break;
            case '3':
                toBoard = Consts::intToUINT.at(5);
                fromBoard = Consts::BKC;
                break;
            case '4':
                toBoard = Consts::intToUINT.at(3);
                fromBoard = Consts::BQC;
                break;
            default:
                throw std::invalid_argument("Rook toboard placement error");
        }
        removePiece(&newBitboards, &fromBoard);
        addPiece(&newBitboards, &toBoard, &piece, &move->isWhite);
    }

    // set enpassant to 0 since if we dont make that move it dissapears
    return newBitboards;
}