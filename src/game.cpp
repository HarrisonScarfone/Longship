#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include "game.h"
#include "gamestate.h"
#include "manager.h"
#include "moves.h"
#include "utilities.h"
#include "evaluate.h"
#include "search.h"
#include "movemaker.h"

Game::Game(){
    gm = Manager();
    gm.arrayToBitboards();
    gameHistory.push_back(gm.getBitboards());
}

std::string Game::getTurnColorString(bool whiteToPlay){
    if (whiteToPlay > 0)
    {
        return "White";
    }
    else
    {
        return "Black";
    }
}

void Game::playCLIGame()
{

    int MAX_SEARCH_DEPTH = 5;

    bool whiteToPlay = true;
    int turnCount = 0;

    int positionScore = 0;
    int moveIndex = -1;

    std::vector <Move> possibleMoves;

    Gamestate::Bitboards currBitboards;
    Gamestate::Bitboards newBitboards;
    currBitboards = gameHistory.at(0);

    while (true)
    {
        currBitboards = gameHistory.at(turnCount);

        gm.updateArrayFromBitboard(currBitboards);
        gm.printArrayBoard();

        possibleMoves = Moves::possibleMoves(&gameHistory.at(turnCount), &whiteToPlay);

        int index;
        std::cout << "Enter move index: ";
        std::cin >> index;
        std::cout << "\n";

        newBitboards = Movemaker::makeMove(currBitboards, &possibleMoves.at(index));
        gameHistory.push_back(newBitboards);

        whiteToPlay = !whiteToPlay;
        turnCount++;
    }
}
