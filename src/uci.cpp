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

UCI::UCI()
{
    uciGo();
}

void UCI::uciGo(){
    
    std::string move;
    Gamestate::Bitboards bitboards;

    bool whiteToPlay = true;
    
    while (true)
    {
        std::string token;
        std::getline(std::cin >> std::ws, token);

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
        
        if (token.substr(0, 2) == "go")
        {
            Search::SearchReturn sr = Search::getMove(bitboards, whiteToPlay);
            std::string selection = sr.selectedMove;
            std::cout << "bestmove " << engineToUCIMove(selection) << "\n";
        }
        
    }
}

void UCI::initiate()
{
    std::cout << "id name UWinENGine\n";
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
            std::vector<std::string> possibleMoves = Moves::possibleMoves(bitboards, whiteToPlay);

            std::string move = uciMoveToEngineMove(possibleMoves, tokens.at(i));
            bitboards = Moves::makeMove(bitboards, move);
            whiteToPlay = !whiteToPlay;
        }
    }

    State state;
    state.bitboards = bitboards;
    state.whiteToPlay = whiteToPlay;

    return state;
}

std::vector<std::string> UCI::vectorizeToken(std::string tokenString)
{
    std::istringstream iss(tokenString);
    std::vector<std::string> tokens;

    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), back_inserter(tokens));

    return tokens;
}

std::string UCI::uciMoveToEngineMove(std::vector<std::string> possibleMoves, std::string token)
{
    std::string matcher = "";
    std::vector<std::string> converter;

    matcher += uciNumToEngineNum(token.at(1));
    matcher += letterToNum(token.at(0));
    matcher += uciNumToEngineNum(token.at(3));
    matcher += letterToNum(token.at(2));

    for (int i = 0; i < possibleMoves.size(); i++)
    {
        if (possibleMoves.at(i).substr(1) == matcher)
        {
            return possibleMoves.at(i);
        }
    }

    return "";
}

std::string UCI::engineToUCIMove(std::string move)
{
    std::string builder = "";

    builder += numToLetter(move.at(2));
    builder += engineNumToUCINum(move.at(1));
    builder += numToLetter(move.at(4));
    builder += engineNumToUCINum(move.at(3));

    return builder;
}

std::string UCI::letterToNum(char letter)
{
    switch (letter)
    {
    case 'a':
        return "0";
    case 'b':
        return "1";
    case 'c':
        return "2";
    case 'd':
        return "3";
    case 'e':
        return "4";
    case 'f':
        return "5";
    case 'g':
        return "6";
    case 'h':
        return "7";
    default:
        return "error";
    }
}

std::string UCI::numToLetter(char letter)
{
    switch (letter)
    {
    case '0':
        return "a";
    case '1':
        return "b";
    case '2':
        return "c";
    case '3':
        return "d";
    case '4':
        return "e";
    case '5':
        return "f";
    case '6':
        return "g";
    case '7':
        return "h";
    default:
        return "error";
    }
}

std::string UCI::uciNumToEngineNum(char num)
{
    switch (num)
    {
    case '1':
        return "7";
    case '2':
        return "6";
    case '3':
        return "5";
    case '4':
        return "4";
    case '5':
        return "3";
    case '6':
        return "2";
    case '7':
        return "1";
    case '8':
        return "0";
    default:
        return "error";
    }
}

std::string UCI::engineNumToUCINum(char num)
{
    switch (num)
    {
    case '7':
        return "1";
    case '6':
        return "2";
    case '5':
        return "3";
    case '4':
        return "4";
    case '3':
        return "5";
    case '2':
        return "6";
    case '1':
        return "7";
    case '0':
        return "8";
    default:
        return "error";
    }
}


