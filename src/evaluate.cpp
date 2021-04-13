#include <cstdint>

#include "gamestate.h"
#include "evaluate.h"
#include "moves.h"
#include "consts.h"

int Evaluate::positionScore(Gamestate::Bitboards bitboards, bool playingWhite)
{
    uint64_t myPieces = 0;
    uint64_t theirPieces = 0;

    if (playingWhite == true)
    {
        myPieces = bitboards.white;
        theirPieces = bitboards.black;
    }
    else
    {
        myPieces = bitboards.black;
        theirPieces = bitboards.white;
    }

    uint64_t control = myControl(bitboards, playingWhite);

    int totalScore = 
        evaluateMaterial(bitboards, myPieces) - evaluateMaterial(bitboards, theirPieces) + 
        evaluateBoardControl(control) + 
        evaluateCenterControl(control);

    return totalScore;
}

uint64_t Evaluate::myControl(Gamestate::Bitboards bitboards, bool playingWhite)
{

    uint64_t myControl, myKing;
    uint64_t newOccupied = (bitboards.p | bitboards.r | bitboards.n | bitboards.b | bitboards.q | bitboards.k);
    
    if (playingWhite == true)
    {
        myKing = bitboards.k & bitboards.white;
        myControl = Moves::unsafeSpaces(
            &newOccupied, 
            bitboards.p & bitboards.black, 
            bitboards.r & bitboards.black, 
            bitboards.n & bitboards.black, 
            bitboards.b &
                bitboards.black, 
            bitboards.q & bitboards.black, 
            bitboards.k & bitboards.black, 
            &myKing, 
            &playingWhite
        );
    }
    else
    {
        myKing = bitboards.k & bitboards.black;
        myControl = Moves::unsafeSpaces(
            &newOccupied, 
            bitboards.p & bitboards.white, 
            bitboards.r & bitboards.white, 
            bitboards.n & bitboards.white, 
            bitboards.b & bitboards.white, 
            bitboards.q & bitboards.white, 
            bitboards.k & bitboards.white, 
            &myKing, 
            &playingWhite
        );
    }

    return myControl;
}

int Evaluate::evaluateBoardControl(uint64_t myControl)
{
    int runningScore = 0;
    int squareControlValue = 3;

    while (myControl > 0)
    {
        if (myControl & 1)
        {
            runningScore += squareControlValue;
        }
        myControl = myControl >> 1;
    }

    return runningScore;
}

int Evaluate::evaluateCenterControl(uint64_t myControl)
{
    uint64_t centerControl = myControl & Consts::CENTRE;
    uint64_t extendedCenterControl = myControl & Consts::EXTENDED_CENTRE;

    int runningScore = 0;
    int centerValue = 11;
    int extendedCenterValue = 7;

    while (centerControl > 0)
    {
        if (centerControl & 1)
        {
            runningScore += centerValue;
        }
        centerControl = centerControl >> 1;
    }

    while (extendedCenterControl > 0)
    {
        if (extendedCenterControl & 1)
        {
            runningScore += centerValue;
        }
        extendedCenterControl = extendedCenterControl >> 1;
    }        

    return runningScore;
}

// just do a raw material count for now
int Evaluate::evaluateMaterial(Gamestate::Bitboards bitboards, uint64_t myPieces)
{
    int PAWN_VALUE = 100;
    int ROOK_VALUE = 500;
    int BISHOP_VALUE = 300;
    int KNIGHT_VALUE = 300;
    int QUEEN_VALUE = 900;
    int KING_VALUE = 20000;

    uint64_t temp = 0;
    int score = 0;

    temp = bitboards.p & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += PAWN_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards.r & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += ROOK_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards.b & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += BISHOP_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards.n & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += KNIGHT_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards.q & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += QUEEN_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards.k & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += KING_VALUE;
        }
        temp = temp >> 1;
    }

    return score;
}