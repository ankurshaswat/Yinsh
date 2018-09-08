// #include <iostream>
#include "Interface.h"
// #include "Board.h"
#include "Util.h"
// #include "AI.h"

using namespace std;

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

    board = new Board(n);
    ai = new AI(board, player, time);

    cout << "# Got metadata" << endl;

    if (player == 1)
    {
        readInput();
        currentPlayer = !currentPlayer;
    }

    while (true)
    {
        cout << "# Moving on to get AI output" << endl;
        ai->playMoveSeq();
        currentPlayer = !currentPlayer;
        cout << "# Moving on to get input" << endl;
        readInput();
        currentPlayer = !currentPlayer;
    }
}

void Inteface::readInput()
{
    string str;
    getline(cin, str);
    pair<int, pair<int, int>> returnedVal;
    int i;

    cout << "# Taken input" << endl;

    if (str[0] == 'P')
    {
        i = 2;

        returnedVal = extractCoordinates(str, i);
        pair<int, int> coord = returnedVal.second;
        i = returnedVal.first;
        board->placeRing(coord, currentPlayer);
    }
    else if (str[0] == 'S')
    {
        i = 2;

        returnedVal = extractCoordinates(str, i);
        pair<int, int> oldPosition = returnedVal.second;
        i = returnedVal.first;

        i += 3;

        returnedVal = extractCoordinates(str, i);
        pair<int, int> newPosition = returnedVal.second;
        i = returnedVal.first;

        board->moveRing(newPosition, oldPosition, currentPlayer);

        if (str[i] != '\n')
        {
            i += 4;

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
        }
    }
    else if (str[i] == 'R')
    {
        i = 3;

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

        i += 3;

        returnedVal = extractCoordinates(str, i);
        pair<int, int> oldPosition = returnedVal.second;
        i = returnedVal.first;

        i += 3;

        returnedVal = extractCoordinates(str, i);
        pair<int, int> newPosition = returnedVal.second;
        i = returnedVal.first;

        board->moveRing(newPosition, oldPosition, currentPlayer);

        if (str[i] != '\n')
        {
            i += 4;

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
        }
    }
}