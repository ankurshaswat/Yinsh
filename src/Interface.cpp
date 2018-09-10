// #include <iostream>
#include "Interface.h"
// #include "Board.h"
#include "Util.h"
// #include "AI.h"

using namespace std;

#ifdef USEDEBUG
#define Debug(x) cout << "# " << x << endl
#else
#define Debug(x)
#endif

Inteface::Inteface()
{
    int playerInt = 0, n = 0, time = 0;
    string str;
    getline(cin, str);
    int i = 0;
    while (str[i] != ' ')
    {
        playerInt = 10 * playerInt + str[i++] - '0';
    }
    i++;
    while (str[i] != ' ')
    {
        n = 10 * n + str[i++] - '0';
    }
    i++;
    while (str[i] != ' ' && i < str.length())
    {
        time = 10 * time + str[i++] - '0';
    }
    playerInt--;

    bool player = playerInt == 1 ? true : false;

    Debug("Inteface::Inteface - n=" << n << " Player=" << player << " Time=" << time );

    board = new Board(n);
    ai = new AI(board, player, time, n);

    // Debug("Got metadata");

    Move m;

    if (player == 1)
    {
        readInput();
        currentPlayer = !currentPlayer;
    }

    while (true)
    {
        Debug("# Moving on to get AI output");
        ai->playMoveSeq(m);
        currentPlayer = !currentPlayer;
        Debug("# Moving on to get input");
        m = readInput();
        currentPlayer = !currentPlayer;
    }
}

Move Inteface::readInput()
{
    Debug("# Inteface::readInput");

    Move m;
    string str;
    getline(cin, str);
    pair<int, pair<int, int>> returnedVal;
    int i;

    if (str[0] == 'P')
    {
        i = 2;

        returnedVal = extractCoordinates(str, i);
        pair<int, int> coord = returnedVal.second;
        i = returnedVal.first;
        board->placeRing(coord, currentPlayer);

        m = Move(MoveType::placeRing, coord, coord);
    }
    else
    {
        i = 0;
        while (i < str.size())
        {
            if (str[i] == 'S')
            {
                i += 2;

                returnedVal = extractCoordinates(str, i);
                pair<int, int> oldPosition = returnedVal.second;
                i = returnedVal.first;

                i += 3;

                returnedVal = extractCoordinates(str, i);
                pair<int, int> newPosition = returnedVal.second;
                i = returnedVal.first;

                board->moveRing(newPosition, oldPosition, currentPlayer);

                m = Move(MoveType::moveRing, oldPosition, newPosition);
                i += 1;
            }
            else if (str[i] == 'R')
            {
                i += 3;

                returnedVal = extractCoordinates(str, i);
                pair<int, int> startSeries = returnedVal.second;
                i = returnedVal.first;

                i += 4;

                returnedVal = extractCoordinates(str, i);
                pair<int, int> endSeries = returnedVal.second;
                i = returnedVal.first;

                board->removeMarkers(startSeries, endSeries);

                i += 3;

                returnedVal = extractCoordinates(str, i);
                pair<int, int> ringPos = returnedVal.second;
                i = returnedVal.first;

                board->removeRing(ringPos);

                i += 1;
            }
            else
            {
                Debug("# Inteface::readInput Shouldn't be here");
            }
        }
    }

    return m;
}