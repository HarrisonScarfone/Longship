#include <cstdint>

#include "gamestate.h"
#include "evaluate.h"

/*

Scoring system

*/

namespace Evaluate
{

    int positionScore(Gamestate::Bitboards bitboards, bool playingWhite)
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

        int totalScore = 
            evaluateMaterial(bitboards, myPieces) - evaluateMaterial(bitboards, theirPieces);

        return totalScore;
    }

    // just do a raw material count for now
    int evaluateMaterial(Gamestate::Bitboards bitboards, uint64_t myPieces)
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
    
}