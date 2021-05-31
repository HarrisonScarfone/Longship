
#include <iostream>

#include "moves.h"
#include "gamestate.h"
#include "movemaker.h"
#include "manager.h"
#include "evaluate.h"

struct PerftData
{
    int totalNodes = 0;
    int pawnMoves = 0;
    int rookMoves = 0;
    int bishopMoves = 0;
    int knightMoves = 0;
    int queenMoves = 0;
    int kingMoves = 0;
    int castleMoves = 0;
    int enpassant = 0;
    int pawnDoubleMoves = 0;
};

void perftDataToStdout(PerftData thisRun);
void analyzeMove(Move *move, Gamestate::Bitboards bitboards, bool playingWhite, bool hasMoves, PerftData *pd);
void perft(Gamestate::Bitboards bitboards, int depth, bool playingWhite, PerftData *pd);
PerftData perft_handler(Gamestate::Bitboards bitboards, bool whiteToPlay, int depth);

int main(){

    bool whiteToPlay = true;
    int testDepth = 6;

    std::cout << "Depth: " << testDepth << "\n";

    Manager gm = Manager();
    gm.arrayToBitboards();
    Gamestate::Bitboards bitboards = gm.getBitboards();

    PerftData thisRun = perft_handler(bitboards, whiteToPlay, testDepth);

    perftDataToStdout(thisRun);

    return 0;
}

PerftData perft_handler(Gamestate::Bitboards bitboards, bool whiteToPlay, int depth){

    bool wp = whiteToPlay;
    PerftData pd;

    std::vector <Move> possibleMoves = Moves::possibleMoves(&bitboards, &wp);
    
    for (int i = 0; i < possibleMoves.size(); i++)
    {
        Gamestate::Bitboards variation = Movemaker::makeMove(bitboards, &possibleMoves.at(i));
        analyzeMove(&possibleMoves.at(i), bitboards, !whiteToPlay, true, &pd);
        perft(variation, depth - 1, !whiteToPlay, &pd);     
    }

    return pd;

}

void perft(Gamestate::Bitboards bitboards, int depth, bool playingWhite, PerftData *pd)
{
    if (depth == 0)
    {
        pd->totalNodes++;
        return;
    }

    std::vector <Move> possibleMoves = Moves::possibleMoves(&bitboards, &playingWhite);

    for (int i = 0; i < possibleMoves.size(); i++)
    {  
        Gamestate::Bitboards variation = Movemaker::makeMove(bitboards, &possibleMoves.at(i));
        analyzeMove(&possibleMoves.at(i), bitboards, !playingWhite, true, pd);
        perft(variation, depth - 1, !playingWhite, pd);
    }

}

void analyzeMove(Move *move, Gamestate::Bitboards bitboards, bool playingWhite, bool hasMoves, PerftData *pd)
{
    switch (move->piece)
    {
        case 'P':
            pd->pawnMoves++;
            break;
        case 'K':
            pd->kingMoves++;
            break;
        case 'Q':
            pd->queenMoves++;
            break;
        case 'R':
            pd->rookMoves++;
            break;
        case 'B':
            pd->bishopMoves++;
            break;
        case 'N':
            pd->knightMoves++;
            break;    
        default:
            break;
    }

    switch (move->type)
    {
        case 'e':
            pd->enpassant++;
            break;
        case 'd':
            pd->pawnDoubleMoves++;
            break;
        case '1':
            pd->castleMoves++;
            break;
        case '2':
            pd->castleMoves++;
            break;
        case '3':
            pd->castleMoves++;
            break;
        case '4':
            pd->castleMoves++;
            break;
        default:
            break;
    }

}

void perftDataToStdout(PerftData thisRun)
{
    std::cout << "Total Nodes Searched: " << thisRun.totalNodes << "\n\n";
    std::cout << "Pawn Moves: " << thisRun.pawnMoves << "\n";
    std::cout << "Rook Moves: " << thisRun.rookMoves << "\n";
    std::cout << "Bishop Moves: " << thisRun.bishopMoves << "\n";
    std::cout << "Knight Moves: " << thisRun.knightMoves << "\n";
    std::cout << "Queen Moves: " << thisRun.queenMoves << "\n";
    std::cout << "King Moves: " << thisRun.kingMoves << "\n";
    std::cout << "Castle Moves: " << thisRun.castleMoves << "\n";
    std::cout << "Pawn Double Moves: " << thisRun.pawnDoubleMoves << "\n";
    std::cout << "Enpassants: " << thisRun.enpassant << "\n";
}