#ifndef GAMERECORDER_H
#define GAMERECORDER_H

#include <iostream>
#include <fstream>
#include <string>

#include "gamestate.h"
#include "moves.h"

class GameRecorder
{
    public:
        GameRecorder();

        void writeBitboards(Gamestate::Bitboards *bitboards);
        void writeMove(Move *move);
        void writeTurnInformation(int turnCount);
        void writeTurnStart();
        void writeTurnEnd();

        std::string getBitboardString(Gamestate::Bitboards *bitboards);
        std::string uint64AsBoard(uint64_t in);
        std::string moveAsString(Move *move);
};

#endif