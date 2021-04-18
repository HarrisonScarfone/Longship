#include <iostream>
#include <fstream>
#include <string>

#include "gamerecorder.h"
#include "gamestate.h"
#include "moves.h"

GameRecorder::GameRecorder()
{
    std::ofstream outfile;
    outfile.open("gamerecord.txt");
    outfile.close();
}

GameRecorder::~GameRecorder()
{
}

void GameRecorder::writeBitboards(Gamestate::Bitboards *bitboards)
{
    std::ofstream outfile;
    outfile.open("gamerecord.txt", std::ios_base::app);
    outfile << getBitboardString(bitboards);
    outfile.close();
}

void GameRecorder::writeMove(Move *move)
{
    std::ofstream outfile;
    outfile.open("gamerecord.txt", std::ios_base::app);
    outfile << moveAsString(move);
    outfile.close();
}

void GameRecorder::writeTurnInformation(int turnCount)
{
    std::ofstream outfile;
    outfile.open("gamerecord.txt", std::ios_base::app);
    outfile << "\n" << std::to_string(turnCount) << "\n\n";
    outfile.close();
}

std::string GameRecorder::moveAsString(Move *move)
{
    std::string builder = "";

    builder += "From Board" + uint64AsBoard(move->fromBoard);
    builder += "To Board" + uint64AsBoard(move->toBoard);
    builder += "\nPiece: " + std::to_string(move->piece);
    builder += "\nType: " + std::to_string(move->type);
    builder += "\nIs White: " + std::to_string(move->isWhite);
    builder += "\n\n";

    return builder;
}

std::string GameRecorder::getBitboardString(Gamestate::Bitboards *bitboards)
{
    std::string builder = "";
    builder += "White" + uint64AsBoard(bitboards->white);
    builder += "Black" + uint64AsBoard(bitboards->black);
    builder += "Pawns" + uint64AsBoard(bitboards->p);
    builder += "Rooks" + uint64AsBoard(bitboards->r);
    builder += "Bishops" + uint64AsBoard(bitboards->b);
    builder += "Knights" + uint64AsBoard(bitboards->n);
    builder += "Queens" + uint64AsBoard(bitboards->q);
    builder += "Kings" + uint64AsBoard(bitboards->k);
    builder += "En Passant Capture Available" + uint64AsBoard(bitboards->enpassant);
    builder += "\n\n";

    return builder;
}

std::string GameRecorder::uint64AsBoard(uint64_t in)
{

    int count = 0;
    std::string outString;

    while (count < 64)
    {
        if (count % 8 == 0)
        {
            outString += "\n";
        }

        if (in % 2 == 0)
        {
            outString += "0";
        }
        else
        {
            outString += "1";
        }

        in = in / 2;
        count++;
    }
    return "\n" + outString + "\n\n";
}

void GameRecorder::writeTurnStart()
{
    std::ofstream outfile;
    outfile.open("gamerecord.txt", std::ios_base::app);
    outfile << "\n----------------------------------------------------------------------------\n\n";
    outfile.close();
}

void GameRecorder::writeTurnEnd()
{
    std::ofstream outfile;
    outfile.open("gamerecord.txt", std::ios_base::app);
    outfile << "\n----------------------------------------------------------------------------\n\n";
    outfile.close();
}

void GameRecorder::writePositionHash(uint64_t *hash)
{
    std::ofstream outfile;
    outfile.open("gamerecord.txt", std::ios_base::app);
    outfile << *hash << "\n";
    outfile.close();
}