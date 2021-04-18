#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>
#include <cstdint>

#include "gamestate.h"
#include "moves.h"
#include "zobrist.h"

struct SearchReturn
{
    Move selectedMove;
    std::vector <Move> allBestMoves;
};

namespace Search
{
    std::vector <Move> negaMaxHandler(Gamestate::Bitboards bitboards, int alpha, int beta, int depth, bool playingWhite, Zobrist *z, uint64_t *hash);
    int negaMaxAB(Gamestate::Bitboards bitboards, int depth, int alpha, int beta, bool playingWhite, Zobrist *z, uint64_t hash);
    SearchReturn getMove(Gamestate::Bitboards bitboards, bool playingWhite, Zobrist *z, uint64_t *hash);
}

#endif