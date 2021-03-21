#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdint>
#include <string>

#include "gamestate.h"

namespace Utilities
{
    void printuint64InBinary(uint64_t number);
    void newLine();
    void showBitboardValues(Gamestate::Bitboards &bitboards);
    void showSplitMovestring(std::string inString);
    void uint64AsBoard(uint64_t in);

};

#endif