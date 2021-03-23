#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

#include "manager.h"
#include "gamestate.h"

class Game
{
    public:
        Manager gm;
        std::vector<Gamestate::Bitboards> gameHistory;

        Game();
        void playCLIGame();
        std::string getTurnColorString(bool whiteToPlay);
};


#endif