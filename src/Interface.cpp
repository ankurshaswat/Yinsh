#include <iostream>
#include "Interface.h"
#include "Util.h"

#ifdef USEDEBUG
#define Debug(x) std::cout << "# " << x
#else
#define Debug(x)
#endif

Interface::Interface(bool initBoard, string boardPath)
{
    int playerInt = 0, n = 0, time_ = 0, k = 0;
    string str;
    getline(cin, str);
    int i = 0;
    while (str[i] != ' ')
        playerInt = 10 * playerInt + str[i++] - '0';
    i++;
    while (str[i] != ' ')
        n = 10 * n + str[i++] - '0';
    i++;
    while (str[i] != ' ')
        time_ = 10 * time_ + str[i++] - '0';
    i++;
    while (str[i] != ' ' && i < str.length())
        k = 10 * k + str[i++] - '0';
    playerInt--;

    bool player = playerInt == 1 ? true : false;

    Debug("Interface::Interface - n=" << n << " Player=" << player << " Time=" << time_ << " K=" << k << endl);

    board = new Board(n, k);
    ai = new AI(board, player, time_, n, k);

    Move m;

    if (initBoard)
    {
        pair<pair<int, int>, pair<int, int>> movePos = board->setBoard(boardPath);
        ai->setMoveCount(26);
        m.type = MoveType::moveRing;
        m.initPosition = movePos.first;
        m.finalPosition = movePos.second;
    }

    if (player == 1)
    {
        readInput();
        currentPlayer = !currentPlayer;
    }

    while (true)
    {
        Debug("Moving on to get AI output" << endl);

        string move = ai->playMoveSeq(m);
        cout << move;
        cout<<"# "<<ai->getEvaluation()<<endl;
        currentPlayer = !currentPlayer;
        Debug("Moving on to get input" << endl);
        m = readInput();
        cout<<"# "<<ai->getEvaluation()<<endl;
        currentPlayer = !currentPlayer;
    }
}

Move Interface::readInput()
{
    Debug("Inteface::readInput" << endl);

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
                Debug("Inteface::readInput Shouldn't be here" << endl);
    }

    return m;
}