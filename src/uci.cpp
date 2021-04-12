#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "uci.h"
#include "manager.h"
#include "moves.h"
#include "gamestate.h"
#include "search.h"
#include "utilities.h"
#include "movemaker.h"
#include "translator.h"

UCI::UCI()
{
    uciGo();
}

void UCI::uciGo(){
    
    std::string move;
    std::string token;
    Gamestate::Bitboards bitboards;

    bool whiteToPlay = true;

    std::cout.setf(std::ios::unitbuf);

    while (getline(std::cin, token))
    {
        if (token == "uci")
        {
            initiate();
        }
        else if (token == "ucinewgame")
        {
            
        }
        else if (token == "setoption")
        {
            optionsHandler();
        }
        else if (token == "ucinewgame")
        {
            newGameHandler();
        }
        else if (token == "isready")
        {
            sendReady();
        }
        else if (token.substr(0, 8) == "position")
        {
            State state = handlePositionToken(token.substr(9));
            bitboards = state.bitboards;
            whiteToPlay = state.whiteToPlay;
        }
        
        else if (token.substr(0, 2) == "go")
        {
            Search::SearchReturn sr = Search::getMove(bitboards, whiteToPlay);
            std::cout << "bestmove " << Translator::engineToUCIMove(&sr.selectedMove) << "\n";
        }
    }   
}

void UCI::initiate()
{
    std::cout << "id name testengine\n";
    std::cout << "id author Harrison Scarfone\n";

    Manager gm = Manager();
    gm.arrayToBitboards();

    std::vector<Gamestate::Bitboards> gameHistory;
    gameHistory.push_back(gm.getBitboards());

    std::cout << "uciok\n";
}

void UCI::sendReady()
{
    std::cout << "readyok\n";
}

void UCI::optionsHandler()
{
    return;
}

void UCI::newGameHandler()
{
    return;
}

void UCI::inputPosition()
{
    return;   
}

UCI::State UCI::handlePositionToken(std::string token)
{
    std::vector<std::string> tokens = vectorizeToken(token);
    gm.arrayToBitboards();
    Gamestate::Bitboards bitboards = gm.getBitboards();

    bool whiteToPlay = true;

    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens.at(i) == "startpos")
        {
            continue;
        }
        else if (tokens.at(i) == "moves")
        {
            continue;
        }
        else
        {            
            std::vector <Move> possibleMoves = Moves::possibleMoves(&bitboards, &whiteToPlay);

            Move move = Translator::uciMoveToEngineMove(&possibleMoves, &tokens.at(i), &whiteToPlay);
            bitboards = Movemaker::makeMove(bitboards, &move);
            whiteToPlay = !whiteToPlay;
        }
    }

    State state;
    state.bitboards = bitboards;
    state.whiteToPlay = whiteToPlay;

    return state;
}

std::vector <std::string> UCI::vectorizeToken(std::string tokenString)
{
    std::istringstream iss(tokenString);
    std::vector<std::string> tokens;

    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), back_inserter(tokens));

    return tokens;
}


