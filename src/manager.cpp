#include <cstdint>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <math.h>

#include "manager.h"
#include "boardutils.h"
#include "gamestate.h"
#include "utilities.h"

Manager::Manager()
{
    p = 0;
    r = 0;
    n = 0;
    b = 0;
    q = 0;
    k = 0;
    black = 0;
    white = 0;
    enpassant = 0;
    bkc = 1;
    bqc = 1;
    wkc = 1;
    wqc = 1;
    setBoard();
}

void Manager::setBoard()
{
    std::string tmp_board[8][8] = {
        {" "," "," "," "," "," "," "," "},
        {" "," "," ","p","p","p"," "," "},
        {" "," "," ","p","k","p"," "," "},
        {" "," "," ","p"," ","p"," "," "},
        {" "," "," "," "," "," "," "," "},
        {" "," "," "," "," "," "," "," "},
        {"P","P","P","Q","P","P","P","P"},
        {"R"," "," "," ","K"," "," ","R"}};


        // {" "," "," "," "," "," "," "," "},
        // {" ","p"," "," "," "," "," "," "},
        // {" "," ","B"," "," "," "," "," "},
        // {" "," "," "," "," "," "," "," "},
        // {" "," "," "," "," "," "," "," "},
        // {" "," "," "," "," "," "," "," "},
        // {" "," "," "," "," "," "," "," "},
        // {" "," "," "," "," "," "," "," "}};

        /*
        {"r","n","b","q","k","b","n","r"},
        {"p","p","p","p","p","p","p","p"},
        {" "," "," "," "," "," "," "," "},
        {" "," "," "," "," "," "," "," "},
        {" "," "," "," "," "," "," "," "},
        {" "," "," "," "," "," "," "," "},
        {"P","P","P","P","P","P","P","P"},
        {"R","N","B","Q","K","B","N","R"}};
        */

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = tmp_board[i][j];
        }
    }

}

std::uint64_t Manager::stringTo64Bit(std::string inString)
{
    uint64_t tmpVal = 0;
    for (int i = 0; i < 64 ; i++)
    {
        if (inString[63 - i] == '1')
        {
            tmpVal += 1 * pow(2.0, i);
        }
    }
    return tmpVal;
}

void Manager::arrayToBitboards(){
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            BoardUtils::pieces piece;
            piece = BoardUtils::stringToPiece(board[i][j]);

            if (piece == BoardUtils::nopiece)
            {
                continue;
            }

            std::string tmp;
            for (int k = 0; k < 64; k++)
            {
                tmp.append("0");
            }

            int index = 63 - ((i * 8) + j);
            tmp[index] = '1';

            uint64_t tmpBinary = stringTo64Bit(tmp);

            switch(piece)
            {
                case BoardUtils::p:
                    p = p | tmpBinary;
                    black = black | tmpBinary;
                    break;
                case BoardUtils::r:
                    r = r | tmpBinary;
                    black = black | tmpBinary;
                    break;
                case BoardUtils::n:
                    n = n | tmpBinary;
                    black = black | tmpBinary;
                    break;
                case BoardUtils::b:
                    b = b | tmpBinary;
                    black = black | tmpBinary;
                    break;
                case BoardUtils::q:
                    q = q | tmpBinary;
                    black = black | tmpBinary;
                    break;
                case BoardUtils::k:
                    k = k | tmpBinary;
                    black = black | tmpBinary;
                    break;
                case BoardUtils::P:
                    p = p | tmpBinary;
                    white = white | tmpBinary;
                    break;
                case BoardUtils::R:
                    r = r | tmpBinary;
                    white = white | tmpBinary;
                    break;
                case BoardUtils::N:
                    n = n | tmpBinary;
                    white = white | tmpBinary;
                    break;
                case BoardUtils::B:
                    b = b | tmpBinary;
                    white = white | tmpBinary;
                    break;
                case BoardUtils::Q:
                    q = q | tmpBinary;
                    white = white | tmpBinary;
                    break;
                case BoardUtils::K:
                    k = k | tmpBinary;
                    white = white | tmpBinary;
                    break;
            }            
        }
    }
}

void Manager::updateArrayFromBitboard(Gamestate::Bitboards bitboards)
{
    for (int i = 0; i < 64; i++)
    {
        if ((((bitboards.p & bitboards.white)>>i)&1)==1)
        {
            board[i/8][i%8] = "P";
        }
        else if ((((bitboards.r & bitboards.white)>>i)&1)==1)
        {
            board[i/8][i%8] = "R";
        }
        else if ((((bitboards.n & bitboards.white)>>i)&1)==1)
        {
            board[i/8][i%8] = "N";
        }
        else if ((((bitboards.b & bitboards.white)>>i)&1)==1)
        {
            board[i/8][i%8] = "B";
        }
        else if ((((bitboards.q & bitboards.white)>>i)&1)==1)
        {
            board[i/8][i%8] = "Q";
        }
        else if ((((bitboards.k & bitboards.white)>>i)&1)==1)
        {
            board[i/8][i%8] = "K";
        }
        else if ((((bitboards.p & bitboards.black)>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        else if ((((bitboards.r & bitboards.black)>>i)&1)==1)
        {
            board[i/8][i%8] = "r";
        }
        else if ((((bitboards.n & bitboards.black)>>i)&1)==1)
        {
            board[i/8][i%8] = "n";
        }
        else if ((((bitboards.b & bitboards.black)>>i)&1)==1)
        {
            board[i/8][i%8] = "b";
        }
        else if ((((bitboards.q & bitboards.black)>>i)&1)==1)
        {
            board[i/8][i%8] = "q";
        }
        else if ((((bitboards.k & bitboards.black)>>i)&1)==1)
        {
            board[i/8][i%8] = "k";
        }
        else
        {
            board[i/8][i%8] = " ";
        }
    }
}

Gamestate::Bitboards Manager::getBitboards(){
    Gamestate::Bitboards currBitboards;
    currBitboards.p = p;
    currBitboards.r = r;
    currBitboards.n = n;
    currBitboards.b = b;
    currBitboards.q = q;
    currBitboards.k = k;
    currBitboards.white = white;
    currBitboards.black = black;
    currBitboards.enpassant = enpassant;
    currBitboards.bkc = bkc;
    currBitboards.bqc = bqc;
    currBitboards.wkc = wkc;
    currBitboards.wqc = wqc;
    return currBitboards;
}

void Manager::printArrayBoard()
{
    for (int i = 0; i < 8; i++)
    {
        std::cout << "---------------------------------" << "\n";
        for (int j = 0; j < 8; j++)
        {
            std::cout << "| " << board[i][j] << " ";
        }
        std::cout << "|" << "\n";
    }
    std::cout << "---------------------------------" << "\n";
}