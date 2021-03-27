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

    std::string negaMax(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay)
    {
        int maxScore = search(bitboards, 0, whiteToPlay);
        return getMove(bitboards, 0, whiteToPlay, maxScore);


    }

    int search(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay)
    {
        if (depth == 4)
        {
            return Evaluate::positionScore(bitboards, whiteToPlay);
        }

        int max = INT32_MIN;
        std::string move;

        std::string moves = Moves::possibleMoves(bitboards, whiteToPlay);

        for (int i = 0; i < moves.length(); i += 5)
        {
            Gamestate::Bitboards variation = Moves::makeMove(bitboards, moves.substr(i, 5));
            int score = -1 * search(variation, depth + 1, whiteToPlay);
            if (score > max)
            {
                max = score;
                move = moves.substr(i, 5);
            }
        }
        return max;
    }

    std::string getMove(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay, int lookingFor)
    {
        std::string move;

        if (depth == 4)
        {
            return move;
        }

        int max = INT32_MIN;

        std::string moves = Moves::possibleMoves(bitboards, whiteToPlay);

        for (int i = 0; i < moves.length(); i += 5)
        {
            Gamestate::Bitboards variation = Moves::makeMove(bitboards, moves.substr(i, 5));
            int score = -1 * search(variation, depth + 1, whiteToPlay);
            if (score == lookingFor && i < moves.length() - 5)
            {
                move = move.substr(i, 5);
            }
        }
        return move;
    }
}