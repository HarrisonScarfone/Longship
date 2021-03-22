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
    std::string possibleMoves(Gamestate::Bitboards &bitboards, std::string history, bool playingWhite)
    {
        uint64_t capturablePieces = 
            (bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q);
        uint64_t pieces = 
            (bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q);
        uint64_t occupied =
            (bitboards.p | bitboards.r | bitboards.n | bitboards.b | bitboards.q | bitboards.k);
        uint64_t emptySpaces = ~occupied;

        uint64_t myPieces, theirPieces, notMyPieces, myRook, myBishop, myQueen, myKnight, myKing, myPawns, theirPawns;
        
        std::string moves = "";

        if (playingWhite)
        {
            myPieces = bitboards.white;
            theirPieces = bitboards.black;
        }
        else
        {
            myPieces = bitboards.black;
            theirPieces = bitboards.white;
        }

        capturablePieces = capturablePieces & theirPieces;
        notMyPieces = ~((pieces & myPieces) | (theirPieces & bitboards.k));
        myRook = bitboards.r & myPieces;
        myBishop = bitboards.b & myPieces;
        myKnight = bitboards.n & myPieces;
        myQueen = bitboards.q & myPieces;
        myKing = bitboards.k & myPieces;
        myPawns = bitboards.p & myPieces;
        theirPawns = bitboards.p & theirPieces;

        if (playingWhite)
        {
            moves = possibleWhitePawnMoves(myPawns, theirPawns, capturablePieces, emptySpaces, history);
        }
        else
        {
            moves = possibleBlackPawnMoves(myPawns, theirPawns, capturablePieces, emptySpaces, history);
        }

        Utilities::uint64AsBoard(bitboards.black);
        Utilities::uint64AsBoard(bitboards.p);
        Utilities::uint64AsBoard(myPawns);
        Utilities::uint64AsBoard(theirPawns);

        moves = moves +
            possibleRookMoves(occupied, notMyPieces, myRook) + 
            possibleBishopMoves(occupied, notMyPieces, myBishop) + 
            possibleQueenMoves(occupied, notMyPieces, myQueen) + 
            possibleKnightMoves(notMyPieces, myKnight) +
            possibleKingMoves(notMyPieces, myKing);   

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

    std::string possibleWhitePawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePieces, uint64_t emptySpaces, std::string history)
    {
        std::string moves = "";

        /*
        Pawn moves steps through each variant of pawn move and 
        builds a move string with all the available moves
        */
        // save a 64 bit for use in move calculation
        uint64_t temp;
        int index;

        // check move up single space
        temp = (myPawns >> 8) & emptySpaces & ~Consts::RANK_8;
        addPawnMovesToMovestring(&moves, temp, 1, 0, 0, 0);

        // two squares forward
        temp = (myPawns >> 16) & emptySpaces & (emptySpaces >> 8) & Consts::RANK_4;
        addPawnMovesToMovestring(&moves, temp, 2, 0, 0, 0);

        // capture right
        temp = (myPawns >> 7) & capturablePieces & ~Consts::RANK_8 & ~Consts::FILE_A;
        addPawnMovesToMovestring(&moves, temp, 1, -1, 0, 0);

        // capture left
        temp = (myPawns >> 9) & capturablePieces & ~Consts::RANK_8 & ~Consts::FILE_H;
        addPawnMovesToMovestring(&moves, temp, 1, 1, 0, 0);

        // promote by right capture
        temp = (myPawns >> 7) & capturablePieces & Consts::RANK_8 & ~Consts::FILE_A;
        addPromotionMovesToMovestring(&moves, temp, -1, 0);

        // promote by left capture
        temp = (myPawns >> 9) & capturablePieces & Consts::RANK_8 & ~Consts::FILE_H;
        addPromotionMovesToMovestring(&moves, temp, 1, 0);

        // promote by forward movement
        temp = (myPawns >> 8) & emptySpaces & Consts::RANK_8;
        addPromotionMovesToMovestring(&moves, temp, 0, 0);

        // en passant captures
        if (history.length()>=4)//1636
        {
            if ((history.at(history.length() - 1 )==history.at(history.length() - 3)) && abs(history.at(history.length() - 2)-history.at(history.length() - 4 )) == 2)
            {
                int eFile=history.at(history.length()-1)-'0';
                // en passant right
                // shows piece to remove, not the destination
                temp = (myPawns << 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_A & Consts::FileMasks8[eFile];
                addEnPassantMove(&moves, temp, -1, 0);

                // en passant left
                // shows piece to remove, not the destination
                temp = (myPawns >> 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_H & Consts::FileMasks8[eFile];
                addEnPassantMove(&moves, temp, 1, 0);
            }
        }

        return moves;
    }

    std::string possibleBlackPawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePeices, uint64_t emptySpaces, std::string history)
    {
        std::string moves = "";

        /*
        Pawn moves steps through each variant of pawn move and 
        builds a move string with all the available moves
        */

        // save a 64 bit for use in move calculation
        uint64_t temp;
        int index;

        // check move up single space
        temp = (myPawns << 8) & emptySpaces & ~Consts::RANK_1;
        // Utilities::uint64AsBoard(myPawns);
        // Utilities::uint64AsBoard(temp);
        addPawnMovesToMovestring(&moves, temp, -1, 0, 0, 0);

        // two squares forward
        temp = (myPawns << 16) & emptySpaces & (emptySpaces << 8) & Consts::RANK_5;
        addPawnMovesToMovestring(&moves, temp, -2, 0, 0, 0);

        // capture right
        temp = (myPawns << 7) & capturablePeices & ~Consts::RANK_1 & ~Consts::FILE_H;
        addPawnMovesToMovestring(&moves, temp, -1, 1, 0, 0);

        // capture left
        temp = (myPawns << 9) & capturablePeices & ~Consts::RANK_1 & ~Consts::FILE_A;
        addPawnMovesToMovestring(&moves, temp, -1, -1, 0, 0);

        // promote by right capture
        temp = (myPawns << 7) & capturablePeices & Consts::RANK_1 & ~Consts::FILE_H;
        addPromotionMovesToMovestring(&moves, temp, 1, 0);

        // promote by left capture
        temp = (myPawns << 9) & capturablePeices & Consts::RANK_1 & ~Consts::FILE_A;
        addPromotionMovesToMovestring(&moves, temp, -1, 0);

        // promote by forward movement
        temp = (myPawns << 8) & emptySpaces & Consts::RANK_1;
        addPromotionMovesToMovestring(&moves, temp, 0, 0);

        // en passant captures
        if (history.length()>=4)//1636
        {
            if ((history.at(history.length() - 1 )==history.at(history.length() - 3)) && abs(history.at(history.length() - 2)-history.at(history.length() - 4 )) == 2)
            {
                int eFile=history.at(history.length()-1)-'0';
                // en passant right
                // shows piece to remove, not the destination
                temp = (myPawns >> 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_A & Consts::FileMasks8[eFile];
                addEnPassantMove(&moves, temp, -1, 0);

                // en passant left
                // shows piece to remove, not the destination
                temp = (myPawns << 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_H & Consts::FileMasks8[eFile];
                addEnPassantMove(&moves, temp, 1, 0);
            }
        }

        return moves;
    }

    std::string possibleRookMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t r)
    {
        std::string temp = "";
        int location = 0;

        while (r > 0)
        {
            if (r & 1)
            {
                uint64_t moveBitboard = hvMoves(location, occupied) & notMyPieces;
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            r = r >> 1;
            location++;
        }

        return temp;

    }

    std::string possibleBishopMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t b)
    {
        std::string temp = "";
        int location = 0;

        while (b > 0)
        {
            if (b & 1)
            {
                uint64_t moveBitboard = dAdMoves(location, occupied) & notMyPieces;
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            b = b >> 1;
            location++;
        }

        return temp;

    }

    std::string possibleQueenMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t q)
    {
        std::string temp = "";
        int location = 0;

        while (q > 0)
        {
            if (q & 1)
            {
                uint64_t moveBitboard = (dAdMoves(location, occupied) | hvMoves(location, occupied)) & notMyPieces;
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            q = q >> 1;
            location++;
        }

        return temp;
    }

    std::string possibleKnightMoves(uint64_t notMyPieces, uint64_t n)
    {
        std::string temp = "";
        int location = 0;
        uint64_t potentialKnightMoves, moveBitboard;

        while (n > 0)
        {
            if (n & 1)
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
                    moveBitboard = potentialKnightMoves & ~Consts::FILE_GH & notMyPieces;
                }
                else
                {
                    moveBitboard = potentialKnightMoves & ~Consts::FILE_AB & notMyPieces;
                }

                // can use the slider move add method for the knight as well
                // since it is a from location -> to location
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            n = n >> 1;
            location++;            
        }

        return temp;
    }

    std::string possibleKingMoves(uint64_t notMyPieces, uint64_t k)
    {
        std::string temp = "";
        int location = 0;
        uint64_t potentialKingMoves, moveBitboard;

        while (k > 0)
        {
            if (k & 1)
            {
                if (location > 9)
                {
                    potentialKingMoves = Consts::KING_SPAN << (location - 9);
                }
                else
                {
                    potentialKingMoves = Consts::KING_SPAN >> (9 - location);
                }

                if ((location % 8) < 4)
                {
                    moveBitboard = potentialKingMoves & ~Consts::FILE_GH & notMyPieces;
                }
                else
                {
                    moveBitboard = potentialKingMoves & ~Consts::FILE_AB & notMyPieces;
                }

                // same as with the knight we can use add slider move method for the kings
                // bitboard as well
                addSliderMovesToMovestring(&temp, moveBitboard, location);
            }
            k = k >> 1;
            location++;
        }

        return temp;
    }
}
