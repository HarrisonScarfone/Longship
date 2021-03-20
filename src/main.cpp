#include <iostream>

#include "manager.h"
#include "gamestate.h"
#include "utilities.h"

int main(){

    Manager b = Manager();
    b.setBoard();
    b.printArrayBoard();
    b.arrayToBitboards();
    Gamestate::Bitboards c = b.getBitboards();
    Utilities::showBitboardValues(c);
    b.updateArrayFromBitboard(c);
    b.printArrayBoard();

}