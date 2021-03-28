#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>

#include "gamestate.h"


namespace Search
{

    struct SearchReturn
    {
        std::string selectedMove;
        std::vector<std::string> allBestMoves;
    };

    std::vector<std::string> negaMaxHandler(Gamestate::Bitboards bitboards, int alpha, int beta, int depth, bool playingWhite);
    int negaMaxAB(Gamestate::Bitboards bitboards, int depth, int alpha, int beta, bool playingWhite);
    SearchReturn getMove(Gamestate::Bitboards bitboards, bool playingWhite);

}

#endif