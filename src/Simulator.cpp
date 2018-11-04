// #include <iostream>
#include <iostream>
#include <fstream>
#include "Simulator.h"
#include "Util.h"

#ifdef USEDEBUG
#define Debug(x) std::cout << "# " << x
#else
#define Debug(x)
#endif

Simulator::Simulator(string resultsPath, int selfPolicy, int oppPolicy, string weightsFilePath)
{
    this->resultsPath = resultsPath;
    this->readWeights(weightsFilePath);
}

void Simulator::simulate(int numGames)
{
    int n = 5; // Board Size
    int k = 5; //
    int time_ = 120;
    for (int i = 0; i < numGames; i++)
    {

        // open file in resultsPath with game number
        string path = "gameRes/" + resultsPath + "_" + to_string(i) + ".log";
        ofstream gameFile(path, ios::out | ios::app);

        Board *board = new Board(n, k);
        AI *ai1 = new AI(board, 0, time_, n, k);
        ai1->setWeights(featureWeights, featureWeightsOpp);
        AI *ai2 = new AI(board, 1, time_, n, k);
        ai2->setWeights(featureWeights, featureWeightsOpp);

        Move m;
        bool currentPlayer = false;
        string move;
        int moveCount = 0;

        if (i % 2 == 1)
        {
            move = ai2->playMoveSeq(m);

            gameFile << move;
            vector<vector<double>> featuresCounts(2, vector<double>(board->numFeatures));
            board->featureGenerator(featuresCounts);
            for (int player = 0; player < 2; player++)
            {
                for (int featureNum = 0; featureNum < featuresCounts[player].size(); featureNum++)
                {
                    gameFile << featuresCounts[player][featureNum];
                    gameFile << ' ';
                }
            }
            gameFile << '\n';
            moveCount++;
            currentPlayer = !currentPlayer;
            m = extractLastMoveRing(move);
        }

        while ((board->getRingsCount(1) > 2 && board->getRingsCount(0) > 2) || moveCount < 15)
        {
            move = ai1->playMoveSeq(m);

            gameFile << move;
            // cout << move;
            vector<vector<double>> featuresCounts(2, vector<double>(board->numFeatures));
            board->featureGenerator(featuresCounts);
            for (int player = 0; player < 2; player++)
            {
                for (int featureNum = 0; featureNum < featuresCounts[player].size(); featureNum++)
                {
                    gameFile << featuresCounts[player][featureNum];
                    gameFile << ' ';
                }
            }
            gameFile << '\n';
            moveCount++;

            currentPlayer = !currentPlayer;
            m = extractLastMoveRing(move);
            gameFile.flush();

            if ((board->getRingsCount(1) <= 2 || board->getRingsCount(0) <= 2) && moveCount > 15)
            {
                break;
            }

            move = ai2->playMoveSeq(m);

            gameFile << move;
            // cout << move;

            vector<vector<double>> featuresCounts_(2, vector<double>(board->numFeatures));
            board->featureGenerator(featuresCounts_);
            for (int player = 0; player < 2; player++)
            {
                for (int featureNum = 0; featureNum < featuresCounts_[player].size(); featureNum++)
                {
                    gameFile << featuresCounts_[player][featureNum];
                    gameFile << ' ';
                }
            }
            gameFile << '\n';
            moveCount++;

            currentPlayer = !currentPlayer;
            m = extractLastMoveRing(move);

            gameFile.flush();
        }
        gameFile << "END\n";
        gameFile.flush();
        gameFile.close();

    }
}

void Simulator::readWeights(string weightsFilePath)
{
    ifstream weightsFile(weightsFilePath);

    int numFeatures;
    weightsFile >> numFeatures;
    double temp;

    for (int i = 0; i < numFeatures; i++)
    {
        weightsFile >> temp;
        this->featureWeights.push_back(temp);
    }

    for (int i = 0; i < numFeatures; i++)
    {
        weightsFile >> temp;
        this->featureWeightsOpp.push_back(temp);
    }

    weightsFile.close();
}

Move Simulator::extractLastMoveRing(string str)
{

    Move m;
    pair<int, pair<int, int>> returnedVal;
    int i;

    if (str[0] == 'P')
    {
        i = 2;

        returnedVal = extractCoordinates(str, i);
        pair<int, int> coord = returnedVal.second;
        i = returnedVal.first;

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

                i += 3;

                returnedVal = extractCoordinates(str, i);
                pair<int, int> ringPos = returnedVal.second;
                i = returnedVal.first;

                i += 1;
            }
            else if (str[i] == ' ')
            {
                i++;
            }
            else if (str[i] == '\n')
            {
                break;
            }
            else
            {

                Debug("Simulator::extractLastMoveRing Shouldn't be here " << str[i] << endl);
            }
    }

    return m;
}