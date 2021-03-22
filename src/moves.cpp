#include <cstdint>
#include <string>
#include <iostream>
#include <cstdlib>
#include <math.h>

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
        uint64_t notWhitePieces = 
            ~(((bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q) & bitboards.white) | (bitboards.black & bitboards.k));
        uint64_t occupied =
            (bitboards.p | bitboards.r | bitboards.n | bitboards.b | bitboards.q | bitboards.k);
        uint64_t emptySpaces = ~occupied;

        uint64_t whiteRook = bitboards.r & bitboards.white;
        uint64_t whiteBishop = bitboards.b & bitboards.white;
        uint64_t whiteQueen = bitboards.q & bitboards.white;
        uint64_t whiteKnight = bitboards.n & bitboards.white;

        std::string moves = 
            possibleWhitePawnMoves(bitboards, capturableBlackPieces, emptySpaces, history) + 
            possibleWhiteRook(occupied, notWhitePieces, whiteRook) + 
            possibleWhiteBishop(occupied, notWhitePieces, whiteBishop) + 
            possibleWhiteQueen(occupied, notWhitePieces, whiteQueen) + 
            possibleWhiteKnight(notWhitePieces, whiteKnight);   

        Utilities::showSplitMovestring(moves);

        return moves;    
    }

    void addPawnMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset, int x2offset, int y2offset)
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
            possibleMoves = possibleMoves >> 1;
            index++;
        }
    }

    void addSliderMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int location)
    {
        int index = 0;
        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {   
                std::cout << index << " ";
                *moveString += std::to_string(location / 8) + std::to_string(location % 8) + std::to_string(index / 8) + std::to_string(index % 8);  
            }
            possibleMoves = possibleMoves >> 1;
        }
    }

    uint64_t reverseUint64_t(uint64_t in)
    {
        uint64_t temp = 0;
        int index = 0;

        while (index < 64)
        {
            if (in & 1)
            {
                temp += pow(2, 63 - index);                
            }
            index++;
            in = in >> 1;
        }

        return temp;
    }

    uint64_t indexToBitboard(int index)
    {
        uint64_t temp = 1;
        return temp << index;
    }

    uint64_t hvMoves(int index, uint64_t occupied)
    {
        uint64_t indexBitboard = indexToBitboard(index);
        uint64_t horizontal = (occupied - 2 * indexBitboard) ^ reverseUint64_t(reverseUint64_t(occupied) - 2 * reverseUint64_t(indexBitboard));
        uint64_t vertical = ((occupied & Consts::FileMasks8[index % 8]) - (2 * indexBitboard)) ^ reverseUint64_t(reverseUint64_t(occupied & Consts::FileMasks8[index % 8]) - (2 * reverseUint64_t(indexBitboard)));

        uint64_t temp = (horizontal & Consts::RankMasks8[index / 8]) | (vertical & Consts::FileMasks8[index % 8]);

        Utilities::uint64AsBoard(temp);

        return temp;
    }

    uint64_t dAdMoves(int index, uint64_t occupied)
    {
        uint64_t indexBitboard = indexToBitboard(index);
        uint64_t diagonal = ((occupied & Consts::DiagonalMasks[(index / 8) + (index % 8)]) - (2 * indexBitboard) ^ reverseUint64_t(reverseUint64_t(occupied & Consts::DiagonalMasks[(index / 8) + (index % 8)]) - (2 * reverseUint64_t(indexBitboard))));
        uint64_t antidiagonal = ((occupied & Consts::AntiDiagonalMasks[(index / 8) + 7 - (index % 8)]) - (2 * indexBitboard) ^ reverseUint64_t(reverseUint64_t(occupied & Consts::AntiDiagonalMasks[(index / 8) + 7 - (index % 8)]) - (2 * reverseUint64_t(indexBitboard))));

        uint64_t temp = (diagonal & Consts::DiagonalMasks[(index / 8) + (index % 8)]) | (antidiagonal & Consts::AntiDiagonalMasks[(index / 8) + 7 - (index % 8)]);

        Utilities::uint64AsBoard(temp);

        return temp;
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
        addPawnMovesToMovestring(&moves, temp, 1, 0, 0, 0);

        // two squares forward
        temp = (whitePawns >> 16) & emptySpaces & Consts::RANK_4;
        addPawnMovesToMovestring(&moves, temp, 2, 0, 0, 0);

        // capture right
        temp = (whitePawns >> 7) & capturableBlackPeices & ~Consts::RANK_8 & ~Consts::FILE_A;
        addPawnMovesToMovestring(&moves, temp, 1, -1, 0, 0);

        // capture left
        temp = (whitePawns >> 9) & capturableBlackPeices & ~Consts::RANK_8 & ~Consts::FILE_H;
        addPawnMovesToMovestring(&moves, temp, 1, 1, 0, 0);

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

    std::string possibleWhiteRook(uint64_t occupied, uint64_t notWhite, uint64_t wr)
    {
        std::string temp = "";
        int location = 0;

        while (wr > 0)
        {
            if (wr & 1)
            {
                uint64_t moveBitboard = hvMoves(location, occupied) & notWhite;
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            wr = wr >> 1;
            location++;
        }

        return temp;

    }

    std::string possibleWhiteBishop(uint64_t occupied, uint64_t notWhite, uint64_t wb)
    {
        std::string temp = "";
        int location = 0;

        while (wb > 0)
        {
            if (wb & 1)
            {
                uint64_t moveBitboard = dAdMoves(location, occupied) & notWhite;
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            wb = wb >> 1;
            location++;
        }

        return temp;

    }

    std::string possibleWhiteQueen(uint64_t occupied, uint64_t notWhite, uint64_t wq)
    {
        std::string temp = "";
        int location = 0;

        while (wq > 0)
        {
            if (wq & 1)
            {
                uint64_t moveBitboard = (dAdMoves(location, occupied) | hvMoves(location, occupied)) & notWhite;
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            wq = wq >> 1;
            location++;
        }

        return temp;
    }

    std::string possibleWhiteKnight(uint64_t notWhite, uint64_t wn)
    {
        std::string temp = "";
        int location = 0;
        uint64_t potentialKnightMoves, moveBitboard;

        while (wn > 0)
        {
            if (wn & 1)
            {
                if (location > 18)
                {
                    potentialKnightMoves = Consts::KNIGHT_SPAN << (location - 18);
                }
                else
                {
                    potentialKnightMoves = Consts::KNIGHT_SPAN >> (18 - location);
                }

                if ((location % 8) < 4)
                {
                    moveBitboard = potentialKnightMoves & ~Consts::FILE_GH & notWhite;
                }
                else
                {
                    moveBitboard = potentialKnightMoves & ~Consts::FILE_AB & notWhite;
                }

                // can use the slider move add method for the knight as well
                // since it is a from location -> to location
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            wn = wn >> 1;
            location++;            
        }

        return temp;
    }
}
