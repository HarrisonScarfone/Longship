#include <cstdint>
#include <string>
#include <iostream>
#include <cstdlib>

#include "moves.h"
#include "consts.h"
#include "manager.h"
#include "gamestate.h"
#include "utilities.h"

namespace Moves
{
    std::string possibleMoves(Gamestate::Bitboards &bitboards, std::string history)
    {
        uint64_t capturableBlackPieces = 
            (bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q) & bitboards.black; 
        uint64_t emptySpaces =
            ~(bitboards.p | bitboards.r | bitboards.n | bitboards.b | bitboards.q | bitboards.k);

        std::string moves = possibleWhitePawnMoves(bitboards, capturableBlackPieces, emptySpaces, history);   

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

    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int y1offset, int y2offset){
        int index = 0;

        if (possibleMoves & 1)
        {
            *moveString += std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PQ" +
                std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PR" + 
                std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PB" + 
                std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PN";    
        }

        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {
                *moveString += std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PQ" +
                    std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PR" + 
                    std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PB" + 
                    std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PN";      
            }
            possibleMoves = possibleMoves >> 1;
        }
    }

    void addEnPassantMove(std::string *moveString, uint64_t possibleMoves, int xoffset, int yoffset)
    {
        int index = 0;
        while (possibleMoves > 0)
        {
            if ((possibleMoves >> 1) & 1)
            {
                *moveString += std::to_string((index % 8) + xoffset) + std::to_string((index % 8) + yoffset) + "EP";
            }
            index++;
        }
    }

    std::string possibleWhitePawnMoves(Gamestate::Bitboards &bitboards, uint64_t capturableBlackPeices, uint64_t emptySpaces, std::string history)
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
        temp = (whitePawns >> 7) & capturableBlackPeices & ~Consts::RANK_8 & ~Consts::FILE_A;
        addMovesToMovestring(&moves, temp, 1, -1, 0, 0);

        // capture left
        temp = (whitePawns >> 9) & capturableBlackPeices & ~Consts::RANK_8 & ~Consts::FILE_H;
        addMovesToMovestring(&moves, temp, 1, 1, 0, 0);

        // promote by right capture
        temp = (whitePawns >> 7) & capturableBlackPeices & Consts::RANK_8 & ~Consts::FILE_A;
        addPromotionMovesToMovestring(&moves, temp, -1, 0);

        // promote by left capture
        temp = (whitePawns >> 9) & capturableBlackPeices & Consts::RANK_8 & ~Consts::FILE_H;
        Utilities::uint64AsBoard(temp);
        addPromotionMovesToMovestring(&moves, temp, 1, 0);

        // promote by forward movement
        temp = (whitePawns >> 8) & emptySpaces & Consts::RANK_8;
        // Utilities::uint64AsBoard(temp);
        addPromotionMovesToMovestring(&moves, temp, 0, 0);

        // en passant captures
        if (history.length()>=4)//1636
        {
            if ((history.at(history.length() - 1 )==history.at(history.length() - 3)) && abs(history.at(history.length() - 2)-history.at(history.length() - 4 )) == 2)
            {
                int eFile=history.at(history.length()-1)-'0';
                // en passant right
                // shows piece to remove, not the destination
                temp = (whitePawns << 1) & (bitboards.black & bitboards.p) & Consts::RANK_5 & ~Consts::FILE_A & Consts::FileMasks8[eFile];
                addEnPassantMove(&moves, temp, -1, 0);

                // en passant left
                // shows piece to remove, not the destination
                temp = (whitePawns >> 1) & (bitboards.black & bitboards.p) & Consts::RANK_5 & ~Consts::FILE_H & Consts::FileMasks8[eFile];
                addEnPassantMove(&moves, temp, 1, 0);
            }
        }

        return moves;
    }
}