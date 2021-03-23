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
            (bitboards.p | bitboards.n | bitboards.r | bitboards.b | bitboards.q);
        uint64_t occupied =
            (bitboards.p | bitboards.r | bitboards.n | bitboards.b | bitboards.q | bitboards.k);
        uint64_t emptySpaces = ~occupied;

        uint64_t    myPieces, myPawns, myRook, myBishop, myQueen, myKnight, myKing,  
                    theirPieces, theirPawns, theirRook, theirBishop, theirQueen, theirKnight, theirKing,
                    notMyPieces;
        
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

        if (playingWhite == 1)
        {
            moves = 
                possibleWhitePawnMoves(myPawns, theirPawns, capturablePieces, emptySpaces, bitboards.enpassant, playingWhite) + 
                possibleWhiteCastleMoves(occupied, bitboards.wkc, bitboards.wqc);
        }
        else
        {
            moves = 
                possibleBlackPawnMoves(myPawns, theirPawns, capturablePieces, emptySpaces, bitboards.enpassant, playingWhite) +
                possibleBlackCastleMoves(occupied, bitboards.bkc, bitboards.wqc);
        }

        moves = moves +
            possibleRookMoves(occupied, notMyPieces, myRook, playingWhite) + 
            possibleBishopMoves(occupied, notMyPieces, myBishop, playingWhite) + 
            possibleQueenMoves(occupied, notMyPieces, myQueen, playingWhite) + 
            possibleKnightMoves(notMyPieces, myKnight, playingWhite) +
            possibleKingMoves(notMyPieces, myKing, playingWhite);   
            
        return moves;    
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

    void addPromotionMovesToMovestring(std::string *moveString, uint64_t possibleMoves, int y1offset, int y2offset, bool playingWhite){
        int index = 0;

        if (possibleMoves & 1)
        {
            char toAdd;
            if (playingWhite)
            {
                toAdd = 'P';
            }
            else
            {
                toAdd = 'p';
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
                    toAdd = 'P';
                }
                else
                {
                    toAdd = 'p';
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
                *moveString += std::to_string((index % 8) + xoffset) + std::to_string((index % 8) + yoffset) + "EP";
            }
            possibleMoves = possibleMoves >> 1;
            index++;
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
        temp = (myPawns << 8) & emptySpaces & ~Consts::RANK_1;
        // Utilities::uint64AsBoard(myPawns);
        // Utilities::uint64AsBoard(temp);
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

    std::string possibleKingMoves(uint64_t notMyPieces, uint64_t k, bool playingWhite)
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

                // same as with the knight we can use add slider move method for the kings
                // bitboard as well
                addSliderMovesToMovestring(&temp, moveBitboard, location, toPass);
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

    uint64_t unsafeSpaces(uint64_t occupied, uint64_t theirPawns, uint64_t theirRook, uint64_t theirKnight, uint64_t theirBishop, uint64_t theirQueen, uint64_t theirKing, bool playingWhite)
    {
        uint64_t unsafe;
        uint64_t temp;
        int location;

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

    std::string possibleWhiteCastleMoves(uint64_t occupied, bool wkc, bool wqc)
    {
        std::string temp = "";
        if (wkc == 1 && ((occupied & Consts::WKC_INBETWEEN) == 0))
        {
            temp += "C7476";
        }
        if (wqc == 1 && ((occupied & Consts::WQC_INBETWEEN) == 0))
        {
            temp += "C7472";
        }
        return temp;
    }

    std::string possibleBlackCastleMoves(uint64_t occupied, bool bkc, bool bqc)
    {
        std::string temp = "";
        if (bkc == 1 && ((occupied & Consts::BKC_INBETWEEN) == 0))
        {
            temp += "c0406";
        }
        if (bqc == 1 && ((occupied & Consts::BQC_INBETWEEN) == 0))
        {
            temp += "c0402";
        }
        return temp;
    }

    uint64_t getMoveBoard(uint64_t inBoard, int fromLocation, int toLocation)
    {
        uint64_t fromBitboard = indexToBitboard(fromLocation);
        uint64_t toBitboard = indexToBitboard(toLocation);
        
        // this should actually remove a piece if the taken piece is the same type
        // can be left as a redundant check for now
        return inBoard ^ fromBitboard ^ toBitboard;
    }

    Gamestate::Bitboards removeTakenPiece(Gamestate::Bitboards newBitboards, int toLocation)
    {
        uint64_t toBoard = indexToBitboard((toLocation));
        // this should take away any piece that is on the square we are moving to
        if (newBitboards.r & toBoard > 0)
        {
            newBitboards.r = newBitboards.r & toBoard;
        }
        else if (newBitboards.n & toBoard > 0)
        {
            newBitboards.n = newBitboards.n & toBoard;
        }
        else if (newBitboards.b & toBoard > 0)
        {
            newBitboards.b = newBitboards.b & toBoard;
        }
        else if (newBitboards.q & toBoard > 0)
        {
            newBitboards.q = newBitboards.q & toBoard;
        }

        if (newBitboards.white & toBoard > 0)
        {
            newBitboards.white = newBitboards.white & toBoard;
        }
        else if (newBitboards.black & toBoard > 0)
        {
            newBitboards.black = newBitboards.black & toBoard;
        }
        return newBitboards;
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

        // note the char to int conversion = -48
        int fromX = move.at(1) - 48;
        int fromY = move.at(2) - 48;
        int toX = move.at(3) - 48;
        int toY = move.at(4) - 48;

        int fromLocation = (fromX * 8) + fromY;
        int toLocation = (toX * 8) + toY;
        
        switch (pieceToMove)
        {
        case 'c':
            break;
        case 'C':
            break;
        case 'e':
            break;
        case 'E':
            break;
        case 'p':
            newBitboards.p = getMoveBoard(newBitboards.p, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.black, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'P':
            newBitboards.p = getMoveBoard(newBitboards.p, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.white, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'r':
            newBitboards.r = getMoveBoard(newBitboards.r, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.black, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);

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
            if ((newBitboards.r & Consts::WQC) == 0)
            {
                newBitboards.wqc = 0;
            }
            if ((newBitboards.r & Consts::WKC) == 0)
            {
                newBitboards.wkc = 0;
            }
            break;
        case 'R':
            newBitboards.r = getMoveBoard(newBitboards.r, fromLocation, toLocation);
            newBitboards.white = getMoveBoard(newBitboards.white, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'b':
            newBitboards.b = getMoveBoard(newBitboards.b, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.black, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'B':
            newBitboards.b = getMoveBoard(newBitboards.b, fromLocation, toLocation);
            newBitboards.white = getMoveBoard(newBitboards.white, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'n':
            newBitboards.n = getMoveBoard(newBitboards.n, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.black, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'N':
            newBitboards.n = getMoveBoard(newBitboards.n, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.white, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'q':
            newBitboards.q = getMoveBoard(newBitboards.q, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.black, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'Q':
            newBitboards.q = getMoveBoard(newBitboards.q, fromLocation, toLocation);
            newBitboards.white = getMoveBoard(newBitboards.white, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'k':
            newBitboards.k = getMoveBoard(newBitboards.k, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.black, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        case 'K':
            newBitboards.k = getMoveBoard(newBitboards.k, fromLocation, toLocation);
            newBitboards.black = getMoveBoard(newBitboards.white, fromLocation, toLocation);
            newBitboards = removeTakenPiece(newBitboards, toLocation);
            break;
        default:
            throw 111;
        }

        return newBitboards;
    }
}
