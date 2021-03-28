# UCI Chess Engine

Still doesn't have an official name, but it will one day.

## To Play a Command Line Game

NOTE THAT IT WILL CURRENTLY ALLOW WHITE TO MAKE WHATEVER MOVE IT WANTS (regardless of legality).  It's useful for debugging.

Forgive the super hacky way I have of running the currently unfinished project.

1. Pull repo.
2. 
    ```shell
    cd src
    make
    ./main
    ```

    *Note* - makefile currently builds with debug flags.

Note the input commands are not algebraic. They run in 5 character strings as shown.

`(Piece char as displayed on board)(from row)(from column)(to row)(to column)`

where the rows/columns are as depicted in the command line print out.

Special move notation is as follows (remains 5 char string):

Promotion - (P for white, p for black)(from column)(to column)P(piece to promote, not case sensitive, Q, B, N, R)
En Passant - (E for white, e for black)(from column)(to column)EP
Castling - (C for white, c for black)(king from row)(king from column)(king to row)(king from column)

Will fiddle with the notation as UCI gets implmented (next on to do list).