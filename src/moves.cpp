#include <moves.h>
#include <consts.h>
#include <boardutils.h>
#include <string>
#include <cstdint>
#include <cli.h>

namespace Moves
{
    std::string possibleMoves(Bitboards bitboards)
    {
        uint64_t notWhitePieces = ~(
            bitboards.wp |
            bitboards.wr |
            bitboards.wn | 
            bitboards.wb |
            bitboards.wq |
            bitboards.wk |
            bitboards.bk
        );
        uint64_t blackPieces = (
            bitboards.bp |
            bitboards.br |
            bitboards.bn |
            bitboards.bb |
            bitboards.bq
        );
        uint64_t empty = (
            bitboards.wp |
            bitboards.wr |
            bitboards.wn | 
            bitboards.wb |
            bitboards.wq |
            bitboards.wk |
            bitboards.bp |
            bitboards.br |
            bitboards.bk |
            bitboards.bb |
            bitboards.bq |
            bitboards.bk
        )
    }
}