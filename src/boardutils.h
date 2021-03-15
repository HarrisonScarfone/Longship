#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H

#include <string>
#include <stdexcept>

namespace BoardUtils
{

    enum pieces
    {
        p,
        r,
        n,
        b,
        q,
        k,
        P,
        R,
        N,
        B,
        Q,
        K,
        nopiece
    };

    BoardUtils::pieces stringToPiece(std::string inString)
    {
        if (inString == "p")
        {
            return p;
        }else if (inString == "r")
        {
            return r;
        }else if (inString == "n")
        {
            return n;
        }else if (inString == "b")
        {
            return b;
        }else if (inString == "q")
        {
            return q;
        }else if (inString == "k")
        {
            return k;
        }else if (inString == "P")
        {
            return P;
        }else if (inString == "R")
        {
            return R;
        }else if (inString == "N")
        {
            return N;
        }else if (inString == "B")
        {
            return B;
        }else if (inString == "Q")
        {
            return Q;
        }else if (inString == "K")
        {
            return K;
        }else
        {
            return nopiece;
        }
        
    }

};

#endif