#include <cstdint>
#include <string>
#include <iostream>

#include "moves.h"
#include "consts.h"
#include "manager.h"
#include "gamestate.h"
#include "utilities.h"

namespace Moves
{
    std::string possibleMoves(Gamestate::Bitboards &bitboards)
    {
        uint64_t capturableBlackPieces = 
            (bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q) & bitboards.black; 
        uint64_t emptySpaces =
            ~(bitboards.p | bitboards.r | bitboards.n | bitboards.b | bitboards.q | bitboards.k);

        std::string moves = possibleWhitePawnMoves(bitboards, capturableBlackPieces, emptySpaces);   

        Utilities::showSplitMovestring(moves);

        return moves;    
    }

    void addMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset, int x2offset, int y2offset)
    {
        int index = 0;
        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {
                *moveString += std::to_string((index / 8) + x1offset) + std::to_string((index % 8) + y1offset) + 
                    std::to_string((index / 8) + x2offset) + std::to_string((index % 8) + y2offset);       
            }
            possibleMoves = possibleMoves >> 1;
        }
    }

    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset){
        int index = 0;
        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {
                *moveString += std::to_string((index / 8) + x1offset) + std::to_string((index % 8) + y1offset) + "PQ" +
                    std::to_string((index / 8) + x1offset) + std::to_string((index % 8) + y1offset) + "PR" + 
                    std::to_string((index / 8) + x1offset) + std::to_string((index % 8) + y1offset) + "PB" + 
                    std::to_string((index / 8) + x1offset) + std::to_string((index % 8) + y1offset) + "PN";      
            }
            possibleMoves = possibleMoves >> 1;
        }
    }

    std::string possibleWhitePawnMoves(Gamestate::Bitboards &bitboards, uint64_t capturableBlackPeices, uint64_t emptySpaces)
    {
        std::string moves = "";

        /*
        Pawn moves steps through each variant of pawn move and 
        builds a move string with all the available moves
        */
        uint64_t whitePawns = bitboards.white & bitboards.p;

        // save a 64 bit for use in move calculation
        uint64_t temp;
        int index;

        // check move up single space
        temp = (whitePawns >> 8) & emptySpaces & ~Consts::RANK_8;
        addMovesToMovestring(&moves, temp, 1, 0, 0, 0);

        // two squares forward
        temp = (whitePawns >> 16) & emptySpaces & Consts::RANK_4;
        addMovesToMovestring(&moves, temp, 2, 0, 0, 0);

        // capture right
        temp = (whitePawns >> 9) & capturableBlackPeices & ~Consts::RANK_8 & ~Consts::FILE_A;
        addMovesToMovestring(&moves, temp, 1, -1, 0, 0);

        // capture left
        temp = (whitePawns >> 7) & capturableBlackPeices & ~Consts::RANK_8 & ~Consts::FILE_H;
        addMovesToMovestring(&moves, temp, 1, 1, 0, 0);

        // promote by right capture
        temp = (whitePawns >> 9) & capturableBlackPeices & Consts::RANK_8 & ~Consts::FILE_A;
        Utilities::uint64AsBoard(~Consts::FILE_A);
        addPromotionMovesToMovestring(&moves, temp, -1, 0);

        // promote by left capture
        temp = (whitePawns >> 7) & capturableBlackPeices & Consts::RANK_8 & ~Consts::FILE_H;
        Utilities::printuint64InBinary(temp);
        std::cout << "\n";
        addPromotionMovesToMovestring(&moves, temp, 1, 0);

        return moves;
    }
}