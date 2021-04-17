#include <iostream>

#include "manager.h"
#include "gamestate.h"
#include "utilities.h"
#include "consts.h"
#include "moves.h"
#include "game.h"
#include "consts.h"
#include "uci.h"
#include "zobrist.h"

int main(){
    // Game g = Game();
    // g.playCLIGame();

    UCI g = UCI();
    g.uciGo();    

    // Manager m = Manager();
    // m.arrayToBitboards();
    // Gamestate::Bitboards bitboards = m.getBitboards();

    // bool whiteToPlay = true;
    // bool test = false;

    // Zobrist z = Zobrist();
    // z.setStartingZobristHash(&bitboards, &whiteToPlay);
    // std::cout << z.startingHash << "\n";
    // z.setStartingZobristHash(&bitboards, &test);
    // std::cout << z.startingHash << "\n"; 
}