#include <cstdint>
#include <vector>

#include "gamestate.h"
#include "evaluate.h"
#include "moves.h"
#include "consts.h"

int Evaluate::positionScore(Gamestate::Bitboards *bitboards, bool *playingWhite)
{
    uint64_t myPieces, theirPieces, myPawns, myKing, theirPawns;

    bool notPlayingWhite = !*playingWhite;

    if (*playingWhite == true)
    {
        myPieces = bitboards->white;
        theirPieces = bitboards->black;
        myPawns = bitboards->p & bitboards->white;
        myKing = bitboards->k & bitboards->white;
        theirPawns = bitboards->black & bitboards->p;
    }
    else
    {
        myPieces = bitboards->black;
        theirPieces = bitboards->white;
        myPawns = bitboards->p & bitboards->black;
        myKing = bitboards->k & bitboards->black;
        theirPawns = bitboards->white & bitboards->p;
    }

    uint64_t unsafe = unsafeForMe(bitboards, playingWhite);
    uint64_t control = ~unsafeForMe(bitboards, &notPlayingWhite);

    int totalScore = 
        evaluateMaterial(bitboards, myPieces) - evaluateMaterial(bitboards, theirPieces) + 
        evaluateBoardControl(control) - evaluateBoardControl(unsafe) + 
        evaluateCenterControl(&control) + evaluateCenterControl(&unsafe) +
        evaluatePawns(&myPawns) - evaluatePawns(&theirPawns);

    return totalScore;
}

uint64_t Evaluate::unsafeForMe(Gamestate::Bitboards *bitboards, bool *playingWhite)
{

    uint64_t myControl, unsafe, myKing;
    uint64_t newOccupied = (bitboards->p | bitboards->r | bitboards->n | bitboards->b | bitboards->q | bitboards->k);
    
    if (*playingWhite == true)
    {
        myKing = bitboards->k & bitboards->white;
        unsafe = Moves::unsafeSpaces(
            &newOccupied, 
            bitboards->p & bitboards->black, 
            bitboards->r & bitboards->black, 
            bitboards->n & bitboards->black, 
            bitboards->b &
            bitboards->black, 
            bitboards->q & bitboards->black, 
            bitboards->k & bitboards->black, 
            &myKing, 
            playingWhite
        );
    }
    else
    {
        myKing = bitboards->k & bitboards->black;
        myControl = Moves::unsafeSpaces(
            &newOccupied, 
            bitboards->p & bitboards->white, 
            bitboards->r & bitboards->white, 
            bitboards->n & bitboards->white, 
            bitboards->b & bitboards->white, 
            bitboards->q & bitboards->white, 
            bitboards->k & bitboards->white, 
            &myKing, 
            playingWhite
        );
    }

    return myControl;
}

int Evaluate::evaluatePawns(uint64_t *myPawns)
{
    int runningScore = 0;

    int centerPawnScore = 4;
    int extendedCenterPawnScore = 2;
    
    // going to try and encourage it to grab space
    uint64_t centerPawns = *myPawns & Consts::CENTRE;
    uint64_t extendedCenterPawns = *myPawns & Consts::CENTRE;

    while (centerPawns > 0)
    {
        if (centerPawns & 1)
        {
            runningScore += centerPawnScore;
        }
        centerPawns = centerPawns >> 1;
    }

    while (extendedCenterPawns > 0)
    {
        if (extendedCenterPawns & 1)
        {
            runningScore += centerPawnScore;
        }
        extendedCenterPawns = extendedCenterPawns >> 1;
    }

    return runningScore;
}

int Evaluate::evaluateBoardControl(uint64_t myControl)
{
    int runningScore = 0;
    int squareControlValue = 2;

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

int Evaluate::evaluateCenterControl(uint64_t *myControl)
{
    uint64_t centerControl = *myControl & Consts::CENTRE;
    uint64_t extendedCenterControl = *myControl & Consts::EXTENDED_CENTRE;

    int runningScore = 0;
    int centerValue = 3;
    int extendedCenterValue = 1;

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
int Evaluate::evaluateMaterial(Gamestate::Bitboards *bitboards, uint64_t myPieces)
{
    int PAWN_VALUE = 100;
    int ROOK_VALUE = 500;
    int BISHOP_VALUE = 300;
    int KNIGHT_VALUE = 300;
    int QUEEN_VALUE = 900;
    int KING_VALUE = 20000;

    uint64_t temp = 0;
    int score = 0;

    temp = bitboards->p & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += PAWN_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards->r & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += ROOK_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards->b & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += BISHOP_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards->n & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += KNIGHT_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards->q & myPieces;
    while (temp > 0)
    {
        if (temp & 1)
        {
            score += QUEEN_VALUE;
        }
        temp = temp >> 1;
    }

    temp = bitboards->k & myPieces;
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