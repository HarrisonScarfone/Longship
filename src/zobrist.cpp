#include <iostream>
#include <random>
#include <cmath>
#include <cstdint>

#include "zobrist.h"
#include "consts.h"
#include "gamestate.h"

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

    // do the pieces first
    for (int square = 0; square < 64; square++)
    {
        if (((bitboards->white & bitboards->p) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[0][square];
        }
        else if (((bitboards->black & bitboards->p) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[1][square];
        }
        else if (((bitboards->white & bitboards->r) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[2][square];
        }
        else if (((bitboards->black & bitboards->r) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[3][square];
        }
        else if (((bitboards->white & bitboards->n) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[4][square];
        }
        else if (((bitboards->black & bitboards->n) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[5][square];
        }
        else if (((bitboards->white & bitboards->b) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[6][square];
        }
        else if (((bitboards->black & bitboards->b) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[7][square];
        }
        else if (((bitboards->white & bitboards->q) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[8][square];
        }
        else if (((bitboards->black & bitboards->q) >> square) & 1)
        {
            hashKey = hashKey ^ piecePosition[9][square];
        }
        else if (((bitboards->white & bitboards->k) >> square) & 1)
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
        if ((bitboards->enpassant) & Consts::FileMasks8[i] > 1)
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