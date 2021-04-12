#ifndef UCI_H
#define UCI_H

#include <iostream>
#include <string>
#include <vector>

#include "manager.h"

class UCI
{
    struct State
    {
        Gamestate::Bitboards bitboards;
        bool whiteToPlay;
    };

    public:
        Manager gm;

        UCI();

        void uciGo();
        void initiate();
        void sendReady();
        void optionsHandler();
        void newGameHandler();
        void inputPosition();

        State handlePositionToken(std::string token);

        std::vector<std::string> vectorizeToken(std::string tokenString);
};

#endif