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

/*

Magic Numbers in the file:

- the offsets in the pawn move addition are add/subtract relative to the side and the pawn move
- the offsets in the king and knight moves are relative to position of the knight in the bitmask used in consts.h
- used 8s to represent the 8x8 board, fairly obvious where these are used

*/

namespace Moves
{
    std::string possibleMoves(Gamestate::Bitboards &bitboards, bool playingWhite)
    {
        uint64_t capturablePieces = 
            (bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q);
        uint64_t pieces = 
            (bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q | bitboards.k);
        uint64_t occupied =
            (bitboards.p | bitboards.r | bitboards.n | bitboards.b | bitboards.q | bitboards.k);
        uint64_t emptySpaces = ~occupied;

        uint64_t    myPieces, myPawns, myRook, myBishop, myQueen, myKnight, myKing,  
                    theirPieces, theirPawns, theirRook, theirBishop, theirQueen, theirKnight, theirKing,
                    notMyPieces, unsafe;
        
        std::string moves = "";

        if (playingWhite == 1)
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
        myPawns = bitboards.p & myPieces;
        myRook = bitboards.r & myPieces;
        myBishop = bitboards.b & myPieces;
        myKnight = bitboards.n & myPieces;
        myQueen = bitboards.q & myPieces;
        myKing = bitboards.k & myPieces;
        theirPawns = bitboards.p & theirPieces;
        theirRook = bitboards.r & theirPieces;
        theirBishop = bitboards.b & theirPieces;
        theirKnight = bitboards.n & theirPieces;
        theirQueen = bitboards.q & theirPieces;
        theirKing = bitboards.k & theirPieces;

        unsafe = unsafeSpaces(occupied, theirPawns, theirRook, theirKnight, theirBishop, theirQueen, theirKing, myKing, playingWhite);

        if (playingWhite == 1)
        {
            moves = 
                possibleWhitePawnMoves(myPawns, theirPawns, capturablePieces, emptySpaces, bitboards.enpassant, playingWhite) + 
                possibleWhiteCastleMoves(occupied, unsafe, myKing, bitboards.wkc, bitboards.wqc);
        }
        else
        {
            moves = 
                possibleBlackPawnMoves(myPawns, theirPawns, capturablePieces, emptySpaces, bitboards.enpassant, playingWhite) +
                possibleBlackCastleMoves(occupied, unsafe, myKing, bitboards.bkc, bitboards.bqc);
        }

        moves = moves +
            possibleRookMoves(occupied, notMyPieces, myRook, playingWhite) + 
            possibleBishopMoves(occupied, notMyPieces, myBishop, playingWhite) + 
            possibleQueenMoves(occupied, notMyPieces, myQueen, playingWhite) + 
            possibleKnightMoves(notMyPieces, myKnight, playingWhite) +
            possibleKingMoves(notMyPieces, myKing, unsafe, playingWhite);   

        /*
        we need to be able to reduce our moveset if we are in check. note that the king already can't move into check.

        we know what square the king is on and what squares are unsafe.  if the king is on an unsafe square, we can block 
        any move that doesn't make that square unsafe by checking to see if the king is still unsafe after that move.

        for now, we can pull moves from the movestring and check them after. i dont think your really losing much here
        because you have to generate the move anyway and you have to generate the unsafe and check it at some point

        the reduced movestring can also be used to determine the games ending
        */

        std::string finalMoves = "";
        int i = 0;
        while (moves.length() > i)
        {
            std::string currMove = moves.substr(i, 5);
            Gamestate::Bitboards potentialMoveBitboards = makeMove(bitboards, currMove);

            uint64_t newOccupied = (potentialMoveBitboards.p | potentialMoveBitboards.r | potentialMoveBitboards.n | potentialMoveBitboards.b | potentialMoveBitboards.q | potentialMoveBitboards.k);

            if (playingWhite == true)
            {
                myKing = potentialMoveBitboards.k & potentialMoveBitboards.white;
            }
            else
            {
                myKing = potentialMoveBitboards.k & potentialMoveBitboards.black;
            }

            unsafe = unsafeSpaces(newOccupied, theirPawns, theirRook, theirKnight, theirBishop, theirQueen, theirKing, myKing, playingWhite);
            
            if ((myKing & unsafe) == 0)
            {
                finalMoves += currMove;
            }
            i += 5;
        }
            
        return finalMoves;    
    }

    void addPawnMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int x1offset, int y1offset, int x2offset, int y2offset, bool playingWhite)
    {
        int index = 0;
        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {
                if (playingWhite)
                {
                    *moveString += "P";
                }
                else
                {
                    *moveString += "p";
                }
                *moveString += std::to_string((index / 8) + x1offset) + std::to_string((index % 8) + y1offset) + 
                    std::to_string((index / 8) + x2offset) + std::to_string((index % 8) + y2offset);       
            }
            possibleMoves = possibleMoves >> 1;
        }
    }

    // gonna use U and u for upgrade haha P(romotion) was taken :P
    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int y1offset, int y2offset, bool playingWhite){
        int index = 0;

        if (possibleMoves & 1)
        {
            char toAdd;
            if (playingWhite)
            {
                toAdd = 'U';
            }
            else
            {
                toAdd = 'u';
            }
            *moveString += toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PQ" +
                toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PR" + 
                toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PB" + 
                toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PN";    
        }

        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {
                char toAdd;
                if (playingWhite)
                {
                    toAdd = 'U';
                }
                else
                {
                    toAdd = 'u';
                }
                *moveString += toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PQ" +
                    toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PR" + 
                    toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PB" + 
                    toAdd + std::to_string((index % 8) + y1offset) + std::to_string((index % 8) + y2offset) + "PN";      
            }
            possibleMoves = possibleMoves >> 1;
        }
    }

    void addEnPassantMove(std::string *moveString, uint64_t possibleMoves, int xoffset, int yoffset, bool playingWhite)
    {
        int index = 0;
        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {
                if (playingWhite)
                {
                    *moveString += "E";
                }
                else
                {
                    *moveString += "e";
                }
                *moveString += std::to_string((index % 8) + xoffset) + std::to_string((index % 8) + yoffset) + "EP";
            }
            possibleMoves = possibleMoves >> 1;
        }
    }

    void addSliderMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int location, char charToAdd)
    {
        int index = 0;
        while (possibleMoves > 0)
        {
            index++;
            if ((possibleMoves >> 1) & 1)
            {   
                *moveString += charToAdd + std::to_string(location / 8) + std::to_string(location % 8) + std::to_string(index / 8) + std::to_string(index % 8);  
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
            temp = temp << 1;
            if (in & 1 == 1)
            {
                temp = temp ^ 1;                
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
        uint64_t horizontal = (occupied - (2 * indexBitboard)) ^ reverseUint64_t(reverseUint64_t(occupied) - 2 * reverseUint64_t(indexBitboard));
        uint64_t vertical = ((occupied & Consts::FileMasks8[index % 8]) - (2 * indexBitboard)) ^ reverseUint64_t(reverseUint64_t(occupied & Consts::FileMasks8[index % 8]) - (2 * reverseUint64_t(indexBitboard)));

        uint64_t temp = (horizontal & Consts::RankMasks8[index / 8]) | (vertical & Consts::FileMasks8[index % 8]);

        return temp;
    }

    uint64_t dAdMoves(int index, uint64_t occupied)
    {
        uint64_t indexBitboard = indexToBitboard(index);
        uint64_t diagonal = ((occupied & Consts::DiagonalMasks[(index / 8) + (index % 8)]) - (2 * indexBitboard) ^ reverseUint64_t(reverseUint64_t(occupied & Consts::DiagonalMasks[(index / 8) + (index % 8)]) - (2 * reverseUint64_t(indexBitboard))));
        uint64_t antidiagonal = ((occupied & Consts::AntiDiagonalMasks[(index / 8) + 7 - (index % 8)]) - (2 * indexBitboard) ^ reverseUint64_t(reverseUint64_t(occupied & Consts::AntiDiagonalMasks[(index / 8) + 7 - (index % 8)]) - (2 * reverseUint64_t(indexBitboard))));

        uint64_t temp = (diagonal & Consts::DiagonalMasks[(index / 8) + (index % 8)]) | (antidiagonal & Consts::AntiDiagonalMasks[(index / 8) + 7 - (index % 8)]);

        return temp;
    }

    std::string possibleWhitePawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePieces, uint64_t emptySpaces, uint64_t enpassant, bool playingWhite)
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
        addPawnMovesToMovestring(&moves, temp, 1, 0, 0, 0, playingWhite);

        // two squares forward
        temp = (myPawns >> 16) & emptySpaces & (emptySpaces >> 8) & Consts::RANK_4;
        addPawnMovesToMovestring(&moves, temp, 2, 0, 0, 0, playingWhite);

        // capture right
        temp = (myPawns >> 7) & capturablePieces & ~Consts::RANK_8 & ~Consts::FILE_A;
        addPawnMovesToMovestring(&moves, temp, 1, -1, 0, 0, playingWhite);

        // capture left
        temp = (myPawns >> 9) & capturablePieces & ~Consts::RANK_8 & ~Consts::FILE_H;
        addPawnMovesToMovestring(&moves, temp, 1, 1, 0, 0, playingWhite);

        // promote by right capture
        temp = (myPawns >> 7) & capturablePieces & Consts::RANK_8 & ~Consts::FILE_A;
        addPromotionMovesToMovestring(&moves, temp, -1, 0, playingWhite);

        // promote by left capture
        temp = (myPawns >> 9) & capturablePieces & Consts::RANK_8 & ~Consts::FILE_H;
        addPromotionMovesToMovestring(&moves, temp, 1, 0, playingWhite);

        // promote by forward movement
        temp = (myPawns >> 8) & emptySpaces & Consts::RANK_8;
        addPromotionMovesToMovestring(&moves, temp, 0, 0, playingWhite);

        // en passant right
        // shows piece to remove, not the destination
        temp = (myPawns << 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_A & enpassant;
        addEnPassantMove(&moves, temp, -1, 0, playingWhite);

        // en passant left
        // shows piece to remove, not the destination
        temp = (myPawns >> 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_H & enpassant;
        addEnPassantMove(&moves, temp, 1, 0, playingWhite);
           

        return moves;
    }

    std::string possibleBlackPawnMoves(uint64_t myPawns, uint64_t theirPawns, uint64_t capturablePeices, uint64_t emptySpaces, uint64_t enpassant, bool playingWhite)
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
        temp = (myPawns << 8) & emptySpaces & ~Consts::RANK_1;;
        addPawnMovesToMovestring(&moves, temp, -1, 0, 0, 0, playingWhite);

        // two squares forward
        temp = (myPawns << 16) & emptySpaces & (emptySpaces << 8) & Consts::RANK_5;
        addPawnMovesToMovestring(&moves, temp, -2, 0, 0, 0, playingWhite);

        // capture right
        temp = (myPawns << 7) & capturablePeices & ~Consts::RANK_1 & ~Consts::FILE_H;
        addPawnMovesToMovestring(&moves, temp, -1, 1, 0, 0, playingWhite);

        // capture left
        temp = (myPawns << 9) & capturablePeices & ~Consts::RANK_1 & ~Consts::FILE_A;
        addPawnMovesToMovestring(&moves, temp, -1, -1, 0, 0, playingWhite);

        // promote by right capture
        temp = (myPawns << 7) & capturablePeices & Consts::RANK_1 & ~Consts::FILE_H;
        addPromotionMovesToMovestring(&moves, temp, 1, 0, playingWhite);

        // promote by left capture
        temp = (myPawns << 9) & capturablePeices & Consts::RANK_1 & ~Consts::FILE_A;
        addPromotionMovesToMovestring(&moves, temp, -1, 0, playingWhite);

        // promote by forward movement
        temp = (myPawns << 8) & emptySpaces & Consts::RANK_1;
        addPromotionMovesToMovestring(&moves, temp, 0, 0, playingWhite);

        // en passant right
        // shows piece to remove, not the destination
        temp = (myPawns >> 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_A & enpassant;
        addEnPassantMove(&moves, temp, -1, 0, playingWhite);

        // en passant left
        // shows piece to remove, not the destination
        temp = (myPawns << 1) & (theirPawns) & Consts::RANK_5 & ~Consts::FILE_H & enpassant;
        addEnPassantMove(&moves, temp, 1, 0, playingWhite);

        return moves;
    }

    std::string possibleRookMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t r, bool playingWhite)
    {
        std::string temp = "";
        int location = 0;
        char toPass;

        while (r > 0)
        {
            if (r & 1)
            {
                if (playingWhite)
                {
                    toPass = 'R';
                }
                else
                {
                    toPass = 'r';
                }
                
                uint64_t moveBitboard = hvMoves(location, occupied) & notMyPieces;
                addSliderMovesToMovestring(&temp, moveBitboard, location, toPass);
            }
            r = r >> 1;
            location++;
        }

        return temp;

    }

    std::string possibleBishopMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t b, bool playingWhite)
    {
        std::string temp = "";
        int location = 0;
        char toPass;

        while (b > 0)
        {
            if (b & 1)
            {
                if (playingWhite)
                {
                    toPass = 'B';
                }
                else
                {
                    toPass = 'b';
                }
                uint64_t moveBitboard = dAdMoves(location, occupied) & notMyPieces;
                addSliderMovesToMovestring(&temp, moveBitboard, location, toPass);
            }
            b = b >> 1;
            location++;
        }

        return temp;

    }

    std::string possibleQueenMoves(uint64_t occupied, uint64_t notMyPieces, uint64_t q, bool playingWhite)
    {
        std::string temp = "";
        int location = 0;
        char toPass;

        while (q > 0)
        {
            if (q & 1)
            {
                if (playingWhite)
                {
                    toPass = 'Q';
                }
                else
                {
                    toPass = 'q';
                }
                uint64_t moveBitboard = (dAdMoves(location, occupied) | hvMoves(location, occupied)) & notMyPieces;
                addSliderMovesToMovestring(&temp, moveBitboard, location, toPass);
            }
            q = q >> 1;
            location++;
        }

        return temp;
    }

    std::string possibleKnightMoves(uint64_t notMyPieces, uint64_t n, bool playingWhite)
    {
        std::string temp = "";
        int location = 0;
        uint64_t potentialKnightMoves, moveBitboard;
        char toPass;

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

                if (playingWhite)
                {
                    toPass = 'N';
                }
                else
                {
                    toPass = 'n';
                }

                // can use the slider move add method for the knight as well
                // since it is a from location -> to location
                addSliderMovesToMovestring(&temp, moveBitboard, location, toPass);
            }
            n = n >> 1;
            location++;            
        }

        return temp;
    }

    std::string possibleKingMoves(uint64_t notMyPieces, uint64_t k, uint64_t unsafe, bool playingWhite)
    {
        std::string temp = "";
        int location = 0;
        uint64_t potentialKingMoves, moveBitboard;
        char toPass;

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

                if (playingWhite)
                {
                    toPass = 'K';
                }
                else
                {
                    toPass = 'k';
                }

                moveBitboard = moveBitboard & ~unsafe;

                // if we are trying to move to an unsafe square its not allowed and dont add the move
                addSliderMovesToMovestring(&temp, moveBitboard, location, toPass);;

                // same as with the knight we can use add slider move method for the kings
                // bitboard as well
                
            }
            k = k >> 1;
            location++;
        }

        return temp;
    }

    uint64_t unsafeFromPawns(uint64_t theirPawns, bool playingWhite)
    {
        if (playingWhite)
        {
            return ((theirPawns << 7) & ~Consts::FILE_H) | ((theirPawns << 9) & ~Consts::FILE_A);
        }
        else
        {
            return ((theirPawns >> 7) & ~Consts::FILE_A) | ((theirPawns >> 9) & ~Consts::FILE_H);
        }
    }

    uint64_t unsafeSpaces(uint64_t occupied, uint64_t theirPawns, uint64_t theirRook, uint64_t theirKnight, uint64_t theirBishop, uint64_t theirQueen, uint64_t theirKing, uint64_t myKing, bool playingWhite)
    {
        uint64_t unsafe;
        uint64_t temp;
        int location;

        occupied = occupied & ~myKing;

        /*
        the variable temp carries all through and used for control calculations at each step
        note that it is reassigned in each section it is used in this method
        */

        // pawn capture right | left
        unsafe = unsafeFromPawns(theirPawns, playingWhite);

        // handle squares controlled by knights
        location = 0;
        while (theirKnight > 0)
        {
            if (theirKnight & 1)
            {
                if (location > 18)
                {
                    temp = Consts::KNIGHT_SPAN << (location - 18);
                }
                else
                {
                    temp = Consts::KNIGHT_SPAN >> (18 - location);
                }

                if ((location % 8) < 4)
                {
                    temp = temp & ~Consts::FILE_GH;
                }
                else
                {
                    temp = temp & ~Consts::FILE_AB;
                }

                unsafe = unsafe | temp;
            }
            theirKnight = theirKnight >> 1;
            location++;
        }

        // handle squares controlled diagonally by bishop &| queen
        uint64_t theirQB = theirBishop | theirQueen;        
        location = 0;
        while (theirQB > 0)
        {
            if (theirQB & 1)
            {
                temp = dAdMoves(location, occupied);
                unsafe = unsafe | temp;
            }
            location++;
            theirQB = theirQB >> 1;
        }

        // handle squares controlled diagonally by rook &| queen
        uint64_t theirQR = theirRook | theirQueen;
        location = 0;
        while (theirQR > 0)
        {
            if (theirQR & 1)
            {
                temp = hvMoves(location, occupied);
                unsafe = unsafe | temp;
            }
            location++;
            theirQR = theirQR >> 1;
        }

        // handle the squares controlled by the king
        location = 0;
        while (theirKing > 0)
        {
            if (theirKing & 1)
            {
                if (location > 9)
                {
                    temp = Consts::KING_SPAN << (location - 9);
                }
                else
                {
                    temp = Consts::KING_SPAN >> (9 - location);
                }

                if ((location % 8) < 4)
                {
                    temp = temp & ~Consts::FILE_GH;
                }
                else
                {
                    temp = temp & ~Consts::FILE_AB;
                }

                unsafe = unsafe | temp;
            }
            theirKing = theirKing >> 1;
            location++;
        }

        unsafe = unsafe & ~occupied;
        return unsafe;
    }

    std::string possibleWhiteCastleMoves(uint64_t occupied, uint64_t unsafe, uint64_t myKing, bool wkc, bool wqc)
    {
        std::string temp = "";
        if (wkc == 1 && ((occupied & Consts::WKC_INBETWEEN) == 0) && ((unsafe & Consts::WKC_INBETWEEN) == 0) && ((myKing & Consts::WHITE_KING_START) > 0))
        {
            temp += "C7476";
        }
        if (wqc == 1 && ((occupied & Consts::WQC_INBETWEEN) == 0) && ((unsafe & Consts::WQC_INBETWEEN) == 0) && ((myKing & Consts::WHITE_KING_START) > 0))
        {
            temp += "C7472";
        }
        return temp;
    }

    std::string possibleBlackCastleMoves(uint64_t occupied, uint64_t unsafe, uint64_t myKing, bool bkc, bool bqc)
    {
        std::string temp = "";
        if (bkc == 1 && ((occupied & Consts::BKC_INBETWEEN) == 0) && ((unsafe & Consts::BKC_INBETWEEN) == 0) && ((myKing & Consts::BLACK_KING_START) > 0))
        {
            temp += "c0406";
        }
        if (bqc == 1 && ((occupied & Consts::BQC_INBETWEEN) == 0) && ((unsafe & Consts::BQC_INBETWEEN) == 0) && ((myKing & Consts::BLACK_KING_START) > 0))
        {
            temp += "c0402";
        }
        return temp;
    }

    /*
    think im going to have to remove the piece from both and add it back, getting weird bugs
    with pieces taking pieces of the same type and an even weirder bug with the color boards
    when taking pieces of different types.  implement a lazy solution for now and come back
    when its time to optimize

    although maybe not the best solution i am preforming at max 1 struct copy, 3 64 bit bitwise
    operations per call with one call each time to remove the from and to piece.  i can 
    if/else if all of them because i'm assuming the engine did it properly and is only sitting
    1 piece per square.  then set the piece back in the switch depending on which piece you moved
    using the to board. not great but not terrible.

    tldr -> remove bit locations in to and from, add back the to
    */

    Gamestate::Bitboards removeLocationSquareFromBitboards(Gamestate::Bitboards bitboards, uint64_t *board)
    {
        // remove the piece        
        if ((bitboards.p & *board) > 0)
        {
            bitboards.p = bitboards.p & ~*board;
        }
        else if ((bitboards.r & *board )> 0)
        {
            bitboards.r = bitboards.r & ~*board;
        }
        else if ((bitboards.n & *board) > 0)
        {
            bitboards.n = bitboards.n & ~*board;
        }
        else if ((bitboards.b & *board) > 0)
        {
            bitboards.b = bitboards.b & ~*board;
        }
        else if ((bitboards.q & *board) > 0)
        {
            bitboards.q = bitboards.q & ~*board;
        }
        // this should be ok to allow king removal since its never part of a legal take/move gen
        else if ((bitboards.k & *board) > 0)
        {
            bitboards.k = bitboards.k & ~*board;
        }

        // always remove from both colors to avoid weird bug ive been getting can come back to this
        // 4 64 bit ops instead of 3 to check both because we can skip the if
        bitboards.white = bitboards.white & ~*board;
        bitboards.black = bitboards.black & ~*board;

        return bitboards;
    }

    Gamestate::Bitboards removeRookFromCastling(Gamestate::Bitboards bitboards, const uint64_t *board)
    {
        // remove the piece, it can only be a rook unless we hit a major error somewhere else so this
        // shoudl be fine     
        if (bitboards.r & *board > 0)
        {
            bitboards.r = bitboards.r & ~*board;
        }

        // we can leave the double color removal as its roughly the same for checking which color it is
        bitboards.white = bitboards.white & ~*board;
        bitboards.black = bitboards.black & ~*board;

        return bitboards;

    }

    Gamestate::Bitboards makeMove(Gamestate::Bitboards bitboards, std::string move)
    {

        /*
        String is going to come as 5 characters ex P1122
        
        0 position represents piece type which we can switch off of.
        Postitions 1 and 2 are the from location, 3 and 4 are the to location.
        Use these pairs to map to a single bitmask allowing move and piece removal.
        */

        char pieceToMove = move.at(0);
        Gamestate::Bitboards newBitboards = bitboards;

        /*
        if we get a move that isn't en passant then we can clear the bitboard since
        that move must be made directly after it becomes available or it can't be made

        if it is a pawn move - 
            - if it is an en passant take, clear the ep bitboard
            - if it is a different pawn move, its not ep and the ep board is cleared
            - if it is a move that opens ep on another file, the logic overwrites the board anyway
        */

        // note the char to int conversion = -48
        int fromX = move.at(1) - 48;
        int fromY = move.at(2) - 48;
        int toX = move.at(3) - 48;
        int toY = move.at(4) - 48;

        int fromLocation = (fromX * 8) + fromY;
        int toLocation = (toX * 8) + toY;

        uint64_t toBoard = indexToBitboard(toLocation);
        uint64_t fromBoard = indexToBitboard(fromLocation);

        if (tolower(pieceToMove) != 'e')
        {
            newBitboards.enpassant = 0;
            newBitboards = removeLocationSquareFromBitboards(newBitboards, &toBoard);
            newBitboards = removeLocationSquareFromBitboards(newBitboards, &fromBoard);
        }

        int eFromY;
        int pawnFromAndTakeX;
        int eToandTakeY;
        int eToX;

        int eFromLocation;
        int eToLocation;

        uint64_t r1Board;
        uint64_t r2Board;
        uint64_t addBoard;

        // getting error if I include this inside the switch statement in seperate cases
        uint64_t rookDestinationBoard;
        
        switch (pieceToMove)
        {
        case 'c':
            /*  comes in like "c0406"
                default to board gets 06, from board gets 04 which can handle the king movement.
                need to clear the rook position and add it back 

                can use the kingside or queenside rook location constant based on the too location
                6 for kingside, 2 for queenside. this also applies to white castling   
            */
            newBitboards.k = newBitboards.k | toBoard;
            newBitboards.black = newBitboards.black | toBoard;

            uint64_t rookDestinationBoard;
            
            if (toLocation == 6)
            {
                newBitboards = removeRookFromCastling(newBitboards, &Consts::BKC);
                rookDestinationBoard = indexToBitboard(5);
            }
            else if (toLocation == 2)
            {
                newBitboards = removeRookFromCastling(newBitboards, &Consts::BQC);
                rookDestinationBoard = indexToBitboard(3);
            }
            else
            {
                throw 177;
            }

            newBitboards.r = newBitboards.r | rookDestinationBoard;
            newBitboards.black = newBitboards.black | rookDestinationBoard;
            newBitboards.bkc = false;
            newBitboards.bqc = false;
            break;            
        case 'C':
            // see the 'c' case for explanation
            newBitboards.k = newBitboards.k | toBoard;
            newBitboards.white = newBitboards.white | toBoard;
            
            if (toLocation == 62)
            {
                newBitboards = removeRookFromCastling(newBitboards, &Consts::WKC);
                rookDestinationBoard = indexToBitboard(61);
            }
            else if (toLocation == 58)
            {
                newBitboards = removeRookFromCastling(newBitboards, &Consts::WQC);
                rookDestinationBoard = indexToBitboard(59);
            }
            else
            {
                throw 177;
            }

            newBitboards.r = newBitboards.r | rookDestinationBoard;
            newBitboards.white = newBitboards.white | rookDestinationBoard;
            newBitboards.wkc = false;
            newBitboards.wqc = false;
            break;
        case 'e':
            /*
            en passant can only occur in two locations.  the to and from locations
            are going to need more calculation logic than can be stored in a 5 char string.

            could maybe change the movestring to something this can use easier

            for now - fromX and fromY translate to the fromY and toY
            can only take from a single rank, 4 -> 5 for black, 3 -> 2 for white
            */
            eFromY = fromX;
            pawnFromAndTakeX = 4;
            eToandTakeY = fromY;
            eToX = 5;

            eFromLocation = (fromX * 8) + fromY;
            eToLocation = (toX * 8) + toY;

            r1Board = indexToBitboard((pawnFromAndTakeX * 8) + eFromY);
            r2Board = indexToBitboard((pawnFromAndTakeX * 8) + eToandTakeY);
            addBoard = indexToBitboard((eToX * 8) + eToandTakeY);

            newBitboards = removeLocationSquareFromBitboards(newBitboards, &r1Board);
            newBitboards = removeLocationSquareFromBitboards(newBitboards, &r2Board);
            newBitboards.p = newBitboards.p | addBoard;
            newBitboards.black = newBitboards.black | addBoard;
            newBitboards.enpassant = 0;
            break;
        case 'E':
            eFromY = fromX;
            pawnFromAndTakeX = 3;
            eToandTakeY = fromY;
            eToX = 2;

            eFromLocation = (fromX * 8) + fromY;
            eToLocation = (toX * 8) + toY;

            r1Board = indexToBitboard((pawnFromAndTakeX * 8) + eFromY);
            r2Board = indexToBitboard((pawnFromAndTakeX * 8) + eToandTakeY);
            addBoard = indexToBitboard((eToX * 8) + eToandTakeY);

            newBitboards = removeLocationSquareFromBitboards(newBitboards, &r1Board);
            newBitboards = removeLocationSquareFromBitboards(newBitboards, &r2Board);
            newBitboards.p = newBitboards.p | addBoard;
            newBitboards.white = newBitboards.white | addBoard;
            newBitboards.enpassant = 0;
            break;
        case 'u':
            /*
            fromX and fromY translate to fromY and toY same as ep
            the fromX can only ever be 6 and toX can only ever be 7
            */
            toY = fromY;
            fromY = fromX;
            fromX = 6;
            toX = 7;

            fromBoard = indexToBitboard((fromX * 8) + fromY);
            toBoard = indexToBitboard((toX * 8) + toY);

            newBitboards = removeLocationSquareFromBitboards(newBitboards, &fromBoard);
            newBitboards = removeLocationSquareFromBitboards(newBitboards, &toBoard);
            
            // to and from locations are now cleared.  we must add back the chosen promo
            // piece at the location of the toBoard
            switch (move.at(4))
            {
            case 'R':
                newBitboards.r = newBitboards.r | toBoard;
                break;
            case 'B':
                newBitboards.b = newBitboards.b | toBoard;
                break;
            case 'N':
                newBitboards.n = newBitboards.n | toBoard;
            case 'Q':
                newBitboards.q = newBitboards.q | toBoard;
            }

            // now add the black or white piece holder
            newBitboards.black = newBitboards.black | toBoard;
            break;
        case 'U':
            toY = fromY;
            fromY = fromX;
            fromX = 1;
            toX = 0;

            fromBoard = indexToBitboard((fromX * 8) + fromY);
            toBoard = indexToBitboard((toX * 8) + toY);

            newBitboards = removeLocationSquareFromBitboards(newBitboards, &fromBoard);
            newBitboards = removeLocationSquareFromBitboards(newBitboards, &toBoard);
            
            switch (move.at(4))
            {
            case 'R':
                newBitboards.r = newBitboards.r | toBoard;
                break;
            case 'B':
                newBitboards.b = newBitboards.b | toBoard;
                break;
            case 'N':
                newBitboards.n = newBitboards.n | toBoard;
            case 'Q':
                newBitboards.q = newBitboards.q | toBoard;
            }
            
            newBitboards.white = newBitboards.white | toBoard;
            break;
        case 'p':
            newBitboards.p = newBitboards.p | toBoard;
            newBitboards.black = newBitboards.black | toBoard;

            if (fromX == 1 && toX == 3)
            {
                newBitboards.enpassant = Consts::FileMasks8[fromY];
            }
            break;
        case 'P':
            newBitboards.p = newBitboards.p | toBoard;
            newBitboards.white = newBitboards.white | toBoard;

            if (fromX == 6 & toX == 4)
            {
                newBitboards.enpassant = Consts::FileMasks8[fromY];
            }
            break;
        case 'r':
            newBitboards.r = newBitboards.r | toBoard;
            newBitboards.black = newBitboards.black | toBoard;
            // newBitboards = removeTakenPiece(newBitboards, toLocation, pieceToMove);

            // remove castling rights by setting the intially 1 valued castle bit
            // look at which castle we moved and switch that bit, can only ever switch off
            // probably a better way to do this as it will have uncessary executions

            if ((newBitboards.r & Consts::BQC) == 0)
            {
                newBitboards.bqc = 0;
            }
            if ((newBitboards.r & Consts::BKC) == 0)
            {
                newBitboards.bkc = 0;
            }
            break;
        case 'R':
            newBitboards.r = newBitboards.r | toBoard;
            newBitboards.white = newBitboards.white | toBoard;
            // newBitboards = removeTakenPiece(newBitboards, toLocation, pieceToMove);

            // see above comment for black rooks
            if ((newBitboards.r & Consts::WQC) == 0)
            {
                newBitboards.wqc = 0;
            }
            if ((newBitboards.r & Consts::WKC) == 0)
            {
                newBitboards.wkc = 0;
            }
            break;
        case 'b':
            newBitboards.b = newBitboards.b | toBoard;
            newBitboards.black = newBitboards.black | toBoard;
            break;
        case 'B':
            newBitboards.b = newBitboards.b | toBoard;
            newBitboards.white = newBitboards.white | toBoard;
            break;
        case 'n':
            newBitboards.n = newBitboards.n | toBoard;
            newBitboards.black = newBitboards.black | toBoard;
            break;
        case 'N':
            newBitboards.n = newBitboards.n | toBoard;
            newBitboards.white = newBitboards.white | toBoard;
            break;
        case 'q':
            newBitboards.q = newBitboards.q | toBoard;
            newBitboards.black = newBitboards.black | toBoard;
            break;
        case 'Q':
            newBitboards.q = newBitboards.q | toBoard;
            newBitboards.white = newBitboards.white | toBoard;
            break;
        case 'k':
            newBitboards.k = newBitboards.k | toBoard;
            newBitboards.black = newBitboards.black | toBoard;
            break;
        case 'K':
            newBitboards.k = newBitboards.p | toBoard;
            newBitboards.k = newBitboards.white | toBoard;
            break;
        default:
            throw 111;
        }

        return newBitboards;
    }
}
