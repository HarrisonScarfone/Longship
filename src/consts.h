#ifndef CONSTS_H
#define CONSTS_H

#include <cstdint>

namespace Consts
{
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
    const uint64_t KING_B7 = 460039L;
    const uint64_t KNIGHT_C6 = 43234889994L;

    const uint64_t RankMasks8[] =/*from rank1 to rank8*/
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
}



#endif