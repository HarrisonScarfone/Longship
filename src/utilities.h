#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdint>

#include "gamestate.h"

namespace Utilities
{
    void printuint64InBinary(uint64_t number);
    void newLine();
    void showBitboardValues(Gamestate::Bitboards bitboards);

};


#endif