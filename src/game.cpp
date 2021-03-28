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
    int moveIndex = -1;

    std::string move;
    std::vector<std::string> possibleMoves;
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

        if (possibleMoves.size() == 0)
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

        std::cout << getTurnColorString(whiteToPlay) << "'s turn. \n";
        std::cout << "Current position evaluated at: " << positionScore << "\n";

        if (!whiteToPlay)
        {
            Search::SearchReturn searchReturn = Search::getMove(currBitboards, whiteToPlay);
            newBitboards = Moves::makeMove(currBitboards, searchReturn.selectedMove);
        }

        else
        {
            // std::cout << "Best move vector size is " << searchReturn.allBestMoves.size() << "\n";
            // // Utilities::showMoveVector(searchReturn.allBestMoves);
            // std::cout << "We recommend " << searchReturn.selectedMove << "\n";
            std::cout << "Enter a move: ";
            std::cin >> move;
            std::cout << "\n";

            newBitboards = Moves::makeMove(currBitboards, move);
            // Utilities::showAllBitboardsAsBoards(newBitboards);
        }
        



        gameHistory.push_back(newBitboards);
        currBitboards = newBitboards;
        // Utilities::showAllBitboardsAsBoards(currBitboards);

        whiteToPlay = !whiteToPlay;
        turnCount++;
    }

}
