# Longship

A UCI compatible chess engine written in C++.

## Instructions on Playing a GUI Game Against the Engine

The engine is barebones UCI complient which means it can use UCI GUIs.  I recommened downloading and using Arena, which can be found [here](http://www.playwitharena.de/).  All GUI instructions will be relative to that.

1. Get a copy of Arena (above).
2. Pull this repo.
3. Build an executable called `main` with:
```shell
cd engine
make engine
```
4. Open Arena and select top menu bar options `Engines --> Install New Engine`.
5. Select the `longship` executable from inside the `engine/build` folder.
6. Back to the top menu bar, navigate through `Engines --> Load Engine...` and select `Main`
7. `File --> New Game` to start a new game against the engine.
8.  On your first move (assuming human as white), Arena will start the engine.

*Note*: You can view the debug window while playing with `Engines --> Log - Window` to see the Engine/UCI communication. Helpful to see if the executable has failed.  Note that UCI standard is still not fully implemented so a restart of Arena or a force stop may be necessary in some situations (evident in the log window).

## Other Usage

You can compile a `perft` (Preformance Testing) executable with:
```shell
cd engine
make perft
```
It will calculate the number of moves it sees at a given depth.

Compile a `longship` executeable with debug flags using:
```shell
cd engine
make dengine
```