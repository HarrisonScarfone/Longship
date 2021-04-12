#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>

#include "gamestate.h"
#include "moves.h"


namespace Search
{

    struct SearchReturn
    {
        Move selectedMove;
        std::vector <Move> allBestMoves;
    };

    std::vector <Move> negaMaxHandler(Gamestate::Bitboards bitboards, int alpha, int beta, int depth, bool playingWhite);
    int negaMaxAB(Gamestate::Bitboards bitboards, int depth, int alpha, int beta, bool playingWhite);
    SearchReturn getMove(Gamestate::Bitboards bitboards, bool playingWhite);

}

#endif