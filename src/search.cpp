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

    int negaMax(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay)
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

        std::string moves = Moves::possibleMoves(bitboards, whiteToPlay);

        for (int i = 0; i < moves.length(); i += 5)
        {
            Gamestate::Bitboards variation = Moves::makeMove(bitboards, moves.substr(i, 5));
            int score = -1 * search(variation, depth + 1, whiteToPlay);
            if (score > max)
            {
                max = score;
            }
        }
        return max;
    }

    int getMove(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay, int lookingFor)
    {
        int move;

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
            if (score > max)
            {
                max = score;
                move = i;
            }
        }
        return move;
    }
}