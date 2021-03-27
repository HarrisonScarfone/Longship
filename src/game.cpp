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

    std::string move;
    std::string possibleMoves;
    std::vector<std::string> moveVector;

    Gamestate::Bitboards currBitboards;
    Gamestate::Bitboards newBitboards;
    currBitboards = gameHistory.at(0);

    while (true)
    {
        currBitboards = gameHistory.at(turnCount);

        gm.updateArrayFromBitboard(currBitboards);
        gm.printArrayBoard();

        possibleMoves = Moves::possibleMoves(gameHistory.at(turnCount), whiteToPlay);
        // Utilities::showSplitMovestring(possibleMoves);
        moveVector = gm.moveStringToVector(possibleMoves);

        if (possibleMoves.length() == 0)
        {
            if (whiteToPlay == true)
            {
                std::cout << "Black wins by checkmate.";
            }
            else
            {
                std::cout << "White wins by checkmate.";
            }
            break;
        }

        positionScore = Evaluate::positionScore(currBitboards, whiteToPlay);

        std::cout << getTurnColorString(whiteToPlay) << "'s turn. " << "There are " << possibleMoves.length() / 5 << " moves.\n";
        std::cout << possibleMoves << "\n";
        std::cout << "Current position evaluated at: " << positionScore << "\n";
        std::cout << "We recommend move at index " << Search::negaMax(currBitboards, 0, whiteToPlay) << "\n";
        std::cout << "Select a move: ";
        std::cin >> move;
        std::cout << "\n";

        newBitboards = Moves::makeMove(currBitboards, move);

        gameHistory.push_back(newBitboards);
        currBitboards = newBitboards;
        // Utilities::showAllBitboardsAsBoards(currBitboards);

        whiteToPlay = !whiteToPlay;
        turnCount++;
    }

}



