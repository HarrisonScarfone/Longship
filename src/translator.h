#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>

#include "moves.h"

namespace Translator
{
    Move uciMoveToEngineMove(std::vector <Move> *possibleMoves, std::string *token, bool *playingWhite);

    std::string engineToUCIMove(Move *move);
    std::string numToLetter(int letter);
    std::string engineNumToUCINum(int num);

    int letterToNum(char letter);
    int uciNumToEngineNum(char num);
}

#endif