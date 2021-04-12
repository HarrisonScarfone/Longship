#ifndef MOVEMAKER_H
#define MOVEMAKER_H

#include "gamestate.h"
#include "moves.h"

namespace Movemaker
{
    void addPiece(Gamestate::Bitboards *bitboards, uint64_t *toBoard, char *piece, bool *isWhite);
    void removePiece(Gamestate::Bitboards *bitboards, uint64_t *removeBoard, bool *isWhite);
    Gamestate::Bitboards makeMove(Gamestate::Bitboards bitboards, Move *move);
}

#endif