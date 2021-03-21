#include <iostream>

#include "manager.h"
#include "gamestate.h"
#include "utilities.h"
#include "consts.h"
#include "moves.h"

int main(){

    Manager b = Manager();
    b.setBoard();
    // b.printArrayBoard();
    b.arrayToBitboards();
    Gamestate::Bitboards c = b.getBitboards();
    // Utilities::showBitboardValues(c);
    // b.updateArrayFromBitboard(c);
    // b.printArrayBoard();

    // Utilities::printuint64InBinary(Consts::RANK_1);

    Moves::possibleMoves(c, "");

}