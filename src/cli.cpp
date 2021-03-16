#include <cstdint>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <math.h>

#include "cli.h"
#include "boardutils.h"

Board::Board()
{
    wp = 0;
    wr = 0;
    wn = 0;
    wb = 0;
    wq = 0;
    wk = 0;
    bp = 0;
    br = 0;
    bn = 0;
    bb = 0;
    bq = 0;
    bk = 0;
    setBoard();
}

void Board::setBoard()
{
    std::string tmp_board[8][8] = {
        {"r","n","b","q","k","b","n","r"},
        {"p","p","p","p","p","p","p","p"},
        {" "," "," "," "," "," "," "," "},
        {" "," "," "," "," "," "," "," "},
        {" "," "," "," "," "," "," "," "},
        {" "," "," "," "," "," "," "," "},
        {"P","P","P","P","P","P","P","P"},
        {"R","N","B","Q","K","B","N","R"}};

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = tmp_board[i][j];
        }
    }

}

void Board::arrayToBitboards(){
    
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
                    bp = bp | tmpBinary;
                    break;
                case BoardUtils::r:
                    br = br | tmpBinary;
                    break;
                case BoardUtils::n:
                    bn = bn | tmpBinary;
                    break;
                case BoardUtils::b:
                    bb = bb | tmpBinary;
                    break;
                case BoardUtils::q:
                    bq = bq | tmpBinary;
                    break;
                case BoardUtils::k:
                    bk = bk | tmpBinary;
                    break;
                case BoardUtils::P:
                    wp = wp | tmpBinary;
                    break;
                case BoardUtils::R:
                    wr = wr | tmpBinary;
                    break;
                case BoardUtils::N:
                    wn = wn | tmpBinary;
                    break;
                case BoardUtils::B:
                    wb = wb | tmpBinary;
                    break;
                case BoardUtils::Q:
                    wq = wq | tmpBinary;
                    break;
                case BoardUtils::K:
                    wk = wk | tmpBinary;
                    break;
            }            
        }
    }
}

void Board::printuint64InBinary(uint64_t number)
{
    if (number >> 1) {
        printuint64InBinary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}

void Board::showBitboardValues(){
    printuint64InBinary(bp);
    newLine();
    printuint64InBinary(br);
    newLine();
    printuint64InBinary(bn);
    newLine();
    printuint64InBinary(bb);
    newLine();
    printuint64InBinary(bq);
    newLine();
    printuint64InBinary(bk);
    newLine();
    printuint64InBinary(wp);
    newLine();
    printuint64InBinary(wr);
    newLine();
    printuint64InBinary(wn);
    newLine();
    printuint64InBinary(wb);
    newLine();
    printuint64InBinary(wq);
    newLine();
    printuint64InBinary(wk);
    newLine();
}

void Board::newLine()
{
    std::cout << "\n";
}

void Board::printArrayBoard()
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

void Board::updateFromBitboard()
{
    for (int i = 0; i < 64; i++)
    {
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
        if (((bp>>i)&1)==1)
        {
            board[i/8][i%8] = "p";
        }
    }
}

std::uint64_t Board::stringTo64Bit(std::string inString)
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
