#include <iostream>

#include "manager.h"
#include "gamestate.h"
#include "utilities.h"
#include "consts.h"
#include "moves.h"
#include "game.h"
#include "consts.h"
#include "uci.h"

int main(){

    // Game g = Game();
    // g.playCLIGame();

    UCI g = UCI();
    g.uciGo();    


}