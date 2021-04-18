#include <iostream>
#include <random>
#include <cmath>
#include <cstdint>
#include <map>

#include "zobrist.h"
#include "consts.h"
#include "gamestate.h"
#include "moves.h"

Zobrist::Zobrist()
{
    initZobristNumbers();
}

void Zobrist::initZobristNumbers()
{
    std::random_device rd;

    // c++ 11+ supports a native implementation of a Mersenne Twister algorithm for rand gen on interval [0, 2^w -1]
    std::mt19937_64 e2(rd());
    // seed our mt19937_64 object so we are always replicating hash keys
    e2.seed(7);

    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2,61)), std::llround(std::pow(2,62)));

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 64; j++)
        {
            piecePosition[i][j] = dist(e2);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        castling[i] = dist(e2);
    }

    for (int i = 0; i < 8; i++)
    {
        enpassant[i] = dist(e2);
    }
    
    for (int i = 0; i < 2; i++)
    {
        whosTurn[i] = dist(e2);
    }
}

void Zobrist::setStartingZobristHash(Gamestate::Bitboards *bitboards, bool *whiteToPlay)
{
    uint64_t hashKey = 0;

    // define our piece integers here

    // efficiany doesn't matter here, just need to generate the starting has
    // since xor is its own inverse, we can change the hash as we go

    // do the pieces first -> w/b p/r/n/b/q/k
    for (int square = 0; square < 64; square++)
    {
        if (((bitboards->white & bitboards->p) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[0][square];
        }
        else if (((bitboards->white & bitboards->r) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[1][square];
        }
        else if (((bitboards->white & bitboards->n) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[2][square];
        }
        else if (((bitboards->white & bitboards->b) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[3][square];
        }
        else if (((bitboards->white & bitboards->q) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[4][square];
        }
        else if (((bitboards->white & bitboards->k) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[5][square];
        }
        else if (((bitboards->black & bitboards->p) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[6][square];
        }
        else if (((bitboards->black & bitboards->r) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[7][square];
        }
        else if (((bitboards->black & bitboards->n) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[8][square];
        }
        else if (((bitboards->black & bitboards->b) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[9][square];
        }
        else if (((bitboards->black & bitboards->q) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[10][square];
        }
        else if (((bitboards->black & bitboards->k) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[11][square];
        }
    }

    // castling bits from bitboards
    if (bitboards->wkc)
    {
        hashKey = hashKey ^ castling[0];
    }
    else if (bitboards->wqc)
    {
        hashKey = hashKey ^ castling[1];
    }
    else if (bitboards->bkc)
    {
        hashKey = hashKey ^ castling[2];
    }
    else if (bitboards->bqc)
    {
        hashKey = hashKey ^ castling[3];
    }

    // hash enpassnt if its set
    for (int i = 0; i < 8; i++)
    {
        if ((bitboards->enpassant & Consts::FileMasks8[i]) > 1)
        {
            hashKey = hashKey ^ enpassant[i];
        }
    }

    // finally get the turn
    if (*whiteToPlay == 1)
    {
        hashKey = hashKey ^ whosTurn[0];
    }
    else
    {
        hashKey = hashKey ^ whosTurn[1];
    }

    startingHash = hashKey;
}

void Zobrist::clearSquare(Gamestate::Bitboards *bitboards, uint64_t *hash, uint64_t *location)
{
    int offset;
    if ((bitboards->white & *location) > 0)
    {
        offset = 0;
    }
    else if ((bitboards->black & *location) > 0)
    {
        offset = 6;
    }
    
    if ((bitboards->p & *location) > 0)
    {
        *hash = *hash ^ piecePosition[0 + offset][Consts::uintToInt.at(*location)];
    }
    else if ((bitboards->r & *location) > 0)
    {
        *hash = *hash ^ piecePosition[1 + offset][Consts::uintToInt.at(*location)];
    }
    else if ((bitboards->n & *location) > 0)
    {
        *hash = *hash ^ piecePosition[2 + offset][Consts::uintToInt.at(*location)];
    }
    else if ((bitboards->b & *location) > 0)
    {
        *hash = *hash ^ piecePosition[3 + offset][Consts::uintToInt.at(*location)];
    }
    else if ((bitboards->q & *location) > 0)
    {
        *hash = *hash ^ piecePosition[4 + offset][Consts::uintToInt.at(*location)];
    }
    else if ((bitboards->k & *location) > 0)
    {
        *hash = *hash ^ piecePosition[5 + offset][Consts::uintToInt.at(*location)];
    }
}

void Zobrist::addPiece(char *piece, uint64_t *location, uint64_t *hash, bool *isWhite)
{
    int offset;

    if (*isWhite == 1)
    {
        offset = 0;
    }
    else
    {
        offset = 6;
    }

    switch (*piece)
    {
        case 'P':
            *hash = *hash ^ piecePosition[0 + offset][Consts::uintToInt.at(*location)];
            break;
        case 'R':
            *hash = *hash ^ piecePosition[1 + offset][Consts::uintToInt.at(*location)];
            break;
        case 'N':
            *hash = *hash ^ piecePosition[2 + offset][Consts::uintToInt.at(*location)];
            break;
        case 'B':
            *hash = *hash ^ piecePosition[3 + offset][Consts::uintToInt.at(*location)];
            break;
        case 'Q':
            *hash = *hash ^ piecePosition[4 + offset][Consts::uintToInt.at(*location)];
            break;
        case 'K':
            *hash = *hash ^ piecePosition[5 + offset][Consts::uintToInt.at(*location)];
            break;
        default:
            throw std::invalid_argument("error adding peice to hash");
    }
}

uint64_t Zobrist::getUpdatedHashKey(Gamestate::Bitboards *bitboards, Move *move, uint64_t hash)
{
    clearSquare(bitboards, &hash, &move->toBoard);
    clearSquare(bitboards, &hash, &move->fromBoard);
    addPiece(&move->piece, &move->toBoard, &hash, &move->isWhite);

    if (isdigit(move->type) == 1)
    {
        uint64_t clearRook, addRook;
        char piece = 'R';
        switch(move->type)
        {
            case '1':
                clearRook = Consts::intToUINT.at(63);
                addRook = Consts::intToUINT.at(61);
                break;
            case '2':
                clearRook = Consts::intToUINT.at(56);
                addRook = Consts::intToUINT.at(59);
                break;
            case '3':
                clearRook = Consts::intToUINT.at(7);
                addRook = Consts::intToUINT.at(5);
                break;
            case '4':
                clearRook = Consts::intToUINT.at(0);
                addRook = Consts::intToUINT.at(3);
                break;
        }
        clearSquare(bitboards, &hash, &clearRook);
        addPiece(&piece, &addRook, &hash, &move->isWhite);
    }
    else
    {
        switch (move->type)
        {
            case 's':
                break;
            case 'u':
                break;
            case 'd':
                break;
            case 'e':
                uint64_t toBoard;
                if (move->isWhite == 1)
                {
                    toBoard = move->toBoard << 8;
                    clearSquare(bitboards, &hash, &toBoard);
                }
                else
                {
                    toBoard = move->toBoard >> 8;
                    clearSquare(bitboards, &hash, &toBoard);
                }
            default:
                throw std::invalid_argument("failed to find move type while updating hash key");
        }
    }
    return hash;
}