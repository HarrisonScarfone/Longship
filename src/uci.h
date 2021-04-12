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

        Move uciMoveToEngineMove();
        std::string engineToUCIMove(std::string move);
        
        std::vector <std::string> vectorizeToken(std::string tokenString);
        
        std::string letterToNum(char letter);
        std::string numToLetter(char letter);
        std::string uciMoveToEngineMove(std::vector<std::string> possibleMoves, std::string token, bool playingWhite);
        std::string uciNumToEngineNum(char num);
        std::string engineNumToUCINum(char num);
};

#endif