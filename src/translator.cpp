#include <iostream>
#include <cstdint>

#include "translator.h"
#include "moves.h"
#include "consts.h"

Move Translator::uciMoveToEngineMove(std::vector <Move> *possibleMoves, std::string *token, bool *playingWhite)
{
    uint64_t fromBoard, toBoard;

    int fromBoardAsInt = uciNumToEngineNum(token->at(1)) * 8;
    fromBoardAsInt += letterToNum(token->at(0));

    int toBoardAsInt = uciNumToEngineNum(token->at(3)) * 8;
    toBoardAsInt += letterToNum(token->at(2));

    fromBoard = Consts::intToUINT.at(fromBoardAsInt);
    toBoard = Consts::intToUINT.at(toBoardAsInt);

    // find the engine move in possible moves
    for (int i = 0; i < possibleMoves->size(); i++)
    {
        if (possibleMoves->at(i).fromBoard == fromBoard && possibleMoves->at(i).toBoard == toBoard)
        {
            return possibleMoves->at(i);
        }
    }

    throw std::invalid_argument("Couldn't translate from UCI to engine move");
}

std::string Translator::engineToUCIMove(Move *move)
{
    int toBoardAsInt = Consts::uintToInt.at(move->toBoard);
    int fromBoardAsInt = Consts::uintToInt.at(move->fromBoard);

    int t1 = toBoardAsInt / 8;
    int t2 = toBoardAsInt % 8;

    int f1 = fromBoardAsInt / 8;
    int f2 = fromBoardAsInt % 8;

    std::string engineMove = numToLetter(f2) + engineNumToUCINum(f1) + numToLetter(t2) + engineNumToUCINum(t1);

    return engineMove;
}

int Translator::letterToNum(char letter)
{
    switch (letter)
    {
    case 'a':
        return 0;
    case 'b':
        return 1;
    case 'c':
        return 2;
    case 'd':
        return 3;
    case 'e':
        return 4;
    case 'f':
        return 5;
    case 'g':
        return 6;
    case 'h':
        return 7;
    default:
        throw std::invalid_argument("Move conversion error occured");
    }
}

std::string Translator::numToLetter(int letter)
{
    switch (letter)
    {
    case 0:
        return "a";
    case 1:
        return "b";
    case 2:
        return "c";
    case 3:
        return "d";
    case 4:
        return "e";
    case 5:
        return "f";
    case 6:
        return "g";
    case 7:
        return "h";
    default:
        throw std::invalid_argument("Move conversion error occured");
    }
}

int Translator::uciNumToEngineNum(char num)
{
    switch (num)
    {
    case '1':
        return 7;
    case '2':
        return 6;
    case '3':
        return 5;
    case '4':
        return 4;
    case '5':
        return 3;
    case '6':
        return 2;
    case '7':
        return 1;
    case '8':
        return 0;
    default:
        throw std::invalid_argument("Move conversion error occured");
    }
}

std::string Translator::engineNumToUCINum(int num)
{
    switch (num)
    {
    case 7:
        return "1";
    case 6:
        return "2";
    case 5:
        return "3";
    case 4:
        return "4";
    case 3:
        return "5";
    case 2:
        return "6";
    case 1:
        return "7";
    case 0:
        return "8";
    default:
        throw std::invalid_argument("Move conversion error occured");
    }
}