#ifndef SEARCH_H
#define SEARCH_H

#include <string>

#include "gamestate.h"


namespace Search
{

    int search(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay);
    int getMove(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay, int lookingFor);
    int negaMax(Gamestate::Bitboards bitboards, int depth, bool whiteToPlay);
}


#endif