#include <cstdint>
#include <string>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <time.h>

#include "search.h"
#include "gamestate.h"
#include "consts.h"
#include "evaluate.h"
#include "moves.h"
#include "utilities.h"
#include "movemaker.h"

SearchReturn Search::getMove(Gamestate::Bitboards bitboards, bool playingWhite)
    {

        int depth = 3;

        Move selectedMove;
        std::vector <Move> bestMoves;
        SearchReturn searchReturn;

        bestMoves = negaMaxHandler(bitboards, INT32_MIN, INT32_MAX, depth, playingWhite);

        // psuedorandom select a move
        srand(time(NULL));
        int randIndex = rand() % bestMoves.size();

        if (bestMoves.size() > 1)
        {
            selectedMove = bestMoves.at(randIndex);
        }
        else
        {
            selectedMove = bestMoves.at(0);
        }

        searchReturn.selectedMove = selectedMove;
        searchReturn.allBestMoves = bestMoves;

        return searchReturn;

    }

std::vector <Move> Search::negaMaxHandler(Gamestate::Bitboards bitboards, int alpha, int beta, int depth, bool playingWhite)
{
    std::vector <Move> possibleMoves = Moves::possibleMoves(&bitboards, &playingWhite);

    int bestMoveVal = INT32_MIN;

    // we can have more than one move that evaluates to a "best value" so store them in a vector
    std::vector <Move> bestMoves;

    for (int i = 0; i < possibleMoves.size(); i++)
    {
        Gamestate::Bitboards variation = Movemaker::makeMove(bitboards, &possibleMoves.at(i));

        int variationScore = -1 * negaMaxAB(variation, depth - 1, -beta, -alpha, !playingWhite);

        if (variationScore == bestMoveVal)
        {
            bestMoves.push_back(possibleMoves.at(i));
        }
        else if (variationScore > bestMoveVal)
        {
            bestMoveVal = variationScore;
            bestMoves.clear();
            bestMoves.push_back(possibleMoves.at(i));
        }

        if (bestMoveVal > alpha)
        {
            alpha = bestMoveVal;
        }

        if (alpha > beta)
        {
            break;
        }
    }

    return bestMoves;
}

int Search::negaMaxAB(Gamestate::Bitboards bitboards, int depth, int alpha, int beta, bool playingWhite)
{

    int moveScore = INT32_MIN;

    if (depth == 0)
    {
        return Evaluate::positionScore(&bitboards, &playingWhite);
    }

    int val = INT32_MIN;
    std::vector <Move> possibleMoves = Moves::possibleMoves(&bitboards, &playingWhite);

    if (possibleMoves.size() == 0)
    {
        uint64_t myKing;
        if (playingWhite == 1)
        {
            myKing = bitboards.white & bitboards.k;
        }
        else
        {
            myKing = bitboards.black & bitboards.k;
        }
        if ((Evaluate::unsafeForMe(&bitboards, &playingWhite) & myKing) > 1)
        {
            return -100000;
        }
    }

    for (int i = 0; i < possibleMoves.size(); i++)
    {
        Gamestate::Bitboards variation = Movemaker::makeMove(bitboards, &possibleMoves.at(i));

        moveScore = -1 * negaMaxAB(variation, depth - 1, -1 * beta, -1 * alpha, !playingWhite);

        if (moveScore > val)
        {
            val = moveScore;
        }

        if (moveScore > alpha)
        {
            alpha = moveScore;
        }
    }
    return val;
}
