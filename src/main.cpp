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
    b.printArrayBoard();

    // Utilities::printuint64InBinary(Consts::RANK_1);

    Moves::possibleMoves(c, "", 0);

    // uint64_t test = 4567;
    // uint64_t reverse = Moves::reverseUint64_t(test);

    // Utilities::printuint64InBinary(test);
    // Utilities::printuint64InBinary(reverse);

    Gamestate::Bitboards test = Moves::makeMove(c, "p1121");
    b.updateArrayFromBitboard(test);
    Utilities::showAllBitboardsAsBoards(test);
    b.printArrayBoard();

}