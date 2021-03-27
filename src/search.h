#ifndef SEARCH_H
#define SEARCH_H

#include "gamestate.h"


namespace Search
{

    int nwSearch(Gamestate::Bitboards bitboards, int beta, int depth, bool whiteToPlay);
    int pvSearch(Gamestate::Bitboards bitboards, int alpha, int beta, int depth, bool whiteToPlay);

}


#endif