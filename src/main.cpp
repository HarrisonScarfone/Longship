#include <iostream>

#include "manager.h"
#include "gamestate.h"
#include "utilities.h"
#include "consts.h"
#include "moves.h"
#include "game.h"
#include "consts.h"

int main(){

    // Utilities::uint64AsBoard((18446462598732906495));
    // Utilities::uint64AsBoard((1153202979583557615));
    // Utilities::uint64AsBoard(9295429630892703744);

    Game g = Game();
    g.playCLIGame();

}