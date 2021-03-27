#include <cstdint>
#include <string>
#include <vector>
#include <limits>
#include <stdlib.h>

#include "search.h"
#include "gamestate.h"
#include "consts.h"
#include "evaluate.h"
#include "moves.h"

namespace Search
{

    int nwSearch(Gamestate::Bitboards bitboards, int beta, int depth, bool whiteToPlay)
    {
        int score = INT32_MIN;

        if (depth >= 6)
        {
            score = Evaluate::positionScore(bitboards, whiteToPlay);
            return score;
        }

        std::string moves = Moves::possibleMoves(bitboards, whiteToPlay);

        /*
        will eventually sort the moves to be more in line with pvs
        */

       for (int i = 0; i < moves.length(); i+= 5)
       {
           Gamestate::Bitboards variation = Moves::makeMove(bitboards, moves.substr(i, 5));
           score = -1 * nwSearch(variation, 1 - beta, depth + 1, !whiteToPlay);
           if (score >= beta)
           {
               return score;
           }
       }

       return beta - 1;        
    }

    int pvSearch(Gamestate::Bitboards bitboards, int alpha, int beta, int depth, bool whiteToPlay)
    {
        int bestScore;
        int bestIndex = -1;

        if (depth >= 6)
        {
            bestScore = Evaluate::positionScore(bitboards, whiteToPlay);
            return bestScore;
        }

        std::string moves = Moves::possibleMoves(bitboards, whiteToPlay);

        /* 
        will eventually sort the moves to help pvs
        */
       
        int firstLegalMove = 0;

        if (firstLegalMove == -1)
        {
            return whiteToPlay ? Consts::CHECKMATE_SCORE : -1 * Consts::CHECKMATE_SCORE;
        }

        Gamestate::Bitboards variation = Moves::makeMove(bitboards, moves.substr(firstLegalMove, 5));

        bestScore = -1 * pvSearch(variation, -1 * beta, -1 * alpha, depth + 1, !whiteToPlay);

        if (abs(bestScore) == Consts::CHECKMATE_SCORE)
        {
            return bestScore;
        }

        if (bestScore > alpha)
        {
            if (bestScore >= beta)
            {
                return bestScore;
            }
            alpha = bestScore;
        }

        bestIndex = firstLegalMove;

        for (int i = firstLegalMove + 5; i < moves.length(); i += 5)
        {
            int score;
            variation = Moves::makeMove(bitboards, moves.substr(i, 5));

            score = -1 * nwSearch(variation, -1 * alpha, depth + 1, !whiteToPlay);

            if ((score > alpha) && (score < beta))
            {
                bestScore = -1 * pvSearch(variation, -1 * beta, -1 * alpha, depth + 1, !whiteToPlay);

                if (score > alpha)
                {
                    bestIndex = i;
                    alpha = score;
                }

                if ((score != Consts::CHECKMATE_SCORE) && (score > bestScore))
                {
                    if (score >= beta)
                    {
                        bestIndex = i;
                    }
                    bestScore = score;
                    if (abs(bestScore) == Consts::CHECKMATE_SCORE)
                    {
                        bestIndex = i;
                    }
                }
            }
        }
        return bestIndex;
    }

}