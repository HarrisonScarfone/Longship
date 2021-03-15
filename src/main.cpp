#include <iostream>

#include "cli.h"


int main(){

    Board b = Board();
    b.setBoard();
    b.printArrayBoard();
    b.arrayToBitboards();
    b.showBitboardValues();
    
}