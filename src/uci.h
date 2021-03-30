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
        std::string uciMoveToEngineString();
        State handlePositionToken(std::string token);
        std::vector<std::string> vectorizeToken(std::string tokenString);
        std::string engineToUCIMove(std::string move);
        std::string letterToNum(char letter);
        std::string numToLetter(char letter);
        std::string uciMoveToEngineMove(std::vector<std::string> possibleMoves, std::string token, bool playingWhite);
        std::string uciNumToEngineNum(char num);
        std::string engineNumToUCINum(char num);
};

#endif

/*
2021-03-28 20:19:24.903-->1:uci
2021-03-28 20:19:24.904<--1:id name UWinENGine
2021-03-28 20:19:24.904<--1:id author Harrison Scarfone
2021-03-28 20:19:24.904<--1:uciok
2021-03-28 20:19:24.904-->1:isready
2021-03-28 20:19:24.942<--1:readyok
2021-03-28 20:19:25.106*1*Start calc, move no: 1
2021-03-28 20:19:25.106-->1:ucinewgame
2021-03-28 20:19:25.106-->1:isready
2021-03-28 20:19:25.107<--1:readyok
2021-03-28 20:19:25.113-->1:position startpos moves e2e4
2021-03-28 20:19:25.113-->1:go wtime 300000 btime 300000 winc 0 binc 0
2021-03-28 20:24:26.753-->1:stop
*/