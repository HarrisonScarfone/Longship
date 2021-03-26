#ifndef CONSTS_H
#define CONSTS_H

#include <cstdint>

namespace Consts
{
    // logic for general moves
    const uint64_t FILE_A = 0x101010101010101L;
    const uint64_t FILE_H = 0x8080808080808080L;
    const uint64_t FILE_AB = 217020518514230019L;
    const uint64_t FILE_GH = -4557430888798830400L;
    const uint64_t RANK_1 = -72057594037927936L;
    const uint64_t RANK_4 = 1095216660480L;
    const uint64_t RANK_5 = 4278190080L;
    const uint64_t RANK_8 = 255L;
    const uint64_t CENTRE = 103481868288L;
    const uint64_t EXTENDED_CENTRE = 66229406269440L;
    const uint64_t KING_SIDE = -1085102592571150096L;
    const uint64_t QUEEN_SIDE = 1085102592571150095L;
    const uint64_t KING_SPAN = 460039L;
    const uint64_t KNIGHT_SPAN = 43234889994L;

    // bitmasks needed for castling logic
    const uint64_t BQC = 0x1;
    const uint64_t BQC_INBETWEEN = 0xE;
    const uint64_t BKC = 0x80;
    const uint64_t BKC_INBETWEEN = 0x60;
    
    const uint64_t WQC = 0x100000000000000;
    const uint64_t WQC_INBETWEEN = 0xE00000000000000;
    const uint64_t WKC = 0x8000000000000000;
    const uint64_t WKC_INBETWEEN = 0x6000000000000000;


    // rank 1 to rank 8
    const uint64_t RankMasks8[] =
    {
        0xFFL, 
        0xFF00L, 
        0xFF0000L, 
        0xFF000000L, 
        0xFF00000000L, 
        0xFF0000000000L, 
        0xFF000000000000L, 
        0xFF00000000000000L
    };

    // file a to file h
    const uint64_t FileMasks8[] =/*from fileA to FileH*/
    {
        0x101010101010101L, 
        0x202020202020202L, 
        0x404040404040404L, 
        0x808080808080808L,
        0x1010101010101010L, 
        0x2020202020202020L, 
        0x4040404040404040L, 
        0x8080808080808080L
    };

    // top left to bottom right
    const uint64_t DiagonalMasks[]
    {
        0x1L,
        0x102L,
        0x10204L,
        0x1020408L,
        0x102040810L,
        0x10204081020L,
        0x1020408102040L,
        0x102040810204080L,
        0x204081020408000L,
        0x408102040800000L,
        0x810204080000000L,
    	0x1020408000000000L,
        0x2040800000000000L,
        0x4080000000000000L,
        0x8000000000000000L
    };

    // top right to bottom left
    const uint64_t AntiDiagonalMasks[]
    {
        0x80L,
        0x8040L,
        0x804020L,
        0x80402010L,
        0x8040201008L,
        0x804020100804L,
        0x80402010080402L,
        0x8040201008040201L,
        0x4020100804020100L,
        0x2010080402010000L,
        0x1008040201000000L,
        0x804020100000000L,
        0x402010000000000L,
        0x201000000000000L,
        0x100000000000000L
    };
}

#endif