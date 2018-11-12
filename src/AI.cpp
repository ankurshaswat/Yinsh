#include <algorithm>
#include <iostream>
#include<time.h>

#include "AI.h"
#include "Util.h"


using namespace std;

#ifdef USEDEBUG
#define Debug(x) std::cout << "# " << x
#else
#define Debug(x)
#endif

const int INT_MAX = numeric_limits<int>::max() - 1000;
const int INT_MIN = numeric_limits<int>::min() + 1000;

const int MAX_DEPTH = 4;
const int DEPTH_REDUCTION = 3;

AI::AI(Board *board, bool player, int time, int n, int k)
{
    this->originalBoard = board;
    this->boardSize = n;
    this->consecutiveMarkers = k;
    this->maxRings = (n == 5 && k == 5) ? 5 : 6;
    Debug("AI::AI Constructor" << boardSize << maxRings << consecutiveMarkers << endl);
    this->player = player;
    this->timeRemaining = 1.0 * time;
    if (player)
        this->moveCount = 1;
    else
        this->moveCount = 0;
}

string AI::convertMoveToString(Move move)
{
    string moveString = "";

    if (move.type == MoveType::placeRing)
    {
        pair<int, int> position = axial2hex(move.initPosition);
        moveString += "P " + to_string(position.first) + ' ' + to_string(position.second) + ' ';
    }
    else if (move.type == MoveType::moveRing)
    {
        pair<int, int> position1 = axial2hex(move.initPosition);
        pair<int, int> position2 = axial2hex(move.finalPosition);
        moveString += "S " + to_string(position1.first) + ' ' + to_string(position1.second) + ' ';
        moveString += "M " + to_string(position2.first) + ' ' + to_string(position2.second) + ' ';
    }
    else if (move.type == MoveType::removeRow)
    {
        pair<int, int> position1 = axial2hex(move.initPosition);
        pair<int, int> position2 = axial2hex(move.finalPosition);
        moveString += "RS " + to_string(position1.first) + ' ' + to_string(position1.second) + ' ';
        moveString += "RE " + to_string(position2.first) + ' ' + to_string(position2.second) + ' ';
    }
    if (move.type == MoveType::removeRing)
    {
        pair<int, int> position = axial2hex(move.initPosition);
        moveString += "X " + to_string(position.first) + ' ' + to_string(position.second) + ' ';
    }

    return moveString;
}

string AI::playMoveSeq(Move prevMove)
{
    time_t start_time,end_time;
    time(&start_time);
    clock_t startClock = clock();
    Debug("AI::playMoveSeq" << endl);
    pair<vector<Move>, int> returnedMovePair;
    // cout<<"# Time remaining: "<<this->timeRemaining<<endl;
    if (this->timeRemaining < 15.0){
        if(this->boardSize==6) {
            cout<<"#CRITICAL- DEPTH: 3\n";
            returnedMovePair = maxValue(INT_MIN, INT_MAX, 3, *originalBoard, prevMove, player, moveCount);    
        }
        else{
            cout<<"#CRITICAL- Depth: 3\n";           
            returnedMovePair = maxValue(INT_MIN, INT_MAX, 3, *originalBoard, prevMove, player, moveCount);
        }
    }
    else if (this->timeRemaining > 15.0 && this->timeRemaining < 30.0){
        if(this->boardSize==6) {
            cout<<"#CRITICAL- DEPTH: 3\n";
            returnedMovePair = maxValue(INT_MIN, INT_MAX, 3, *originalBoard, prevMove, player, moveCount);    
        }
        else{
            cout<<"#CRITICAL- Depth: 4\n";           
            returnedMovePair = maxValue(INT_MIN, INT_MAX, 4, *originalBoard, prevMove, player, moveCount);
        }
    }
    else
    {
        if(this->consecutiveMarkers==5 && this->boardSize==5){
            // && this->originalBoard->getRingsCount(player) + this->originalBoard->getRingsCount(!player) >= this->maxRings * 2 - 3)
            if (moveCount < this->maxRings * 2)
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 1, *originalBoard, prevMove, player, moveCount);
            else if (moveCount < 35 || this->originalBoard->getRingsCount(player) + this->originalBoard->getRingsCount(!player) >= this->maxRings * 2 - 1)
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 4, *originalBoard, prevMove, player, moveCount);
            else if (moveCount < 50 || this->originalBoard->getRingsCount(player) + this->originalBoard->getRingsCount(!player) >= this->maxRings * 2 - 2)
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 5, *originalBoard, prevMove, player, moveCount);
            // else if (moveCount>50 || this->originalBoard->getRingsCount(player) + this->originalBoard->getRingsCount(!player) <= this->maxRings * 2 - 3)
                // returnedMovePair = maxValue(INT_MIN, INT_MAX, 6, *originalBoard, prevMove, player, moveCount);
            else
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 6, *originalBoard, prevMove, player, moveCount);
        }
        else if(this->consecutiveMarkers==5 && this->boardSize==6){
            if (moveCount < this->maxRings * 2)
            {
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 1, *originalBoard, prevMove, player, moveCount);
            }
            else if (moveCount < 40 || this->originalBoard->getRingsCount(player) + this->originalBoard->getRingsCount(!player) >= this->maxRings * 2 - 1)
            {
                cout<<"# Depth: 4\n";             
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 4, *originalBoard, prevMove, player, moveCount);
            }
            else
            { 
                cout<<"# Depth: 4..\n";                
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 4, *originalBoard, prevMove, player, moveCount);
            }   
        }
        else{
            //consecutiveMarkers=6 & boardSize=6
            if (moveCount < this->maxRings * 2)
            {
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 1, *originalBoard, prevMove, player, moveCount);
            }
            else if (moveCount < 40 || this->originalBoard->getRingsCount(player) + this->originalBoard->getRingsCount(!player) >= this->maxRings * 2 - 1)
            {
                cout<<"# Depth: 4\n";
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 4, *originalBoard, prevMove, player, moveCount);
            }
            else
            { 
                cout<<"# Depth: 5\n";
                returnedMovePair = maxValue(INT_MIN, INT_MAX, 5, *originalBoard, prevMove, player, moveCount);
            }

        }
    }

    vector<Move> moves = returnedMovePair.first;

    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves[i];
        this->originalBoard->playMove(move, player);
    }

    string moveString = "";

    for (auto move : moves)
        moveString += convertMoveToString(move);
    moveString += '\n';

    clock_t endClock = clock();
    // double timeTaken = double(endClock - startClock) / CLOCKS_PER_SEC;

    time(&end_time);
    double timeTaken=(end_time-start_time);
    this->timeRemaining -= timeTaken;
    // cout<<"# Time taken: "<<timeTaken<<endl;
    moveCount += 2;

    return moveString;
}

pair<vector<Move>, double> AI::maxValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int internalMoveCount)
{
    Debug("AI::maxValue - alpha=" << alpha << " Beta=" << beta << " Depth=" << depth << " Player=" << player << endl);
    vector<Move> bestMoveSeq;
    // Check for win
    if (board.isWin(!this->player) && internalMoveCount > this->maxRings * 2)
        return make_pair(bestMoveSeq, INT_MIN + internalMoveCount);
    else if (board.isWin(this->player) && internalMoveCount > this->maxRings * 2)
        return make_pair(bestMoveSeq, INT_MAX - internalMoveCount);

    //Base case for depth
    if (depth <= 0)
        return make_pair(bestMoveSeq, board.evaluate(this->player, internalMoveCount, this->featureWeights));

    vector<EvaluatedMoveSeq> moveSequences;
    if (internalMoveCount < this->maxRings * 2)
    {
        // Rings placement phase
        vector<Move> moveSeqeunce;

        vector<Move> placeRingMoves;
        board.getValidPlaceRingMoves(placeRingMoves, player);
        Debug("AI::maxValue Got Valid Ring Placements" << endl);

        for (auto move : placeRingMoves)
        {
            moveSeqeunce.push_back(move);
            // Evaluate position till shallow depth (for move ordering)
            moveSequences.push_back(EvaluatedMoveSeq(moveSeqeunce, 0));
            moveSeqeunce.pop_back();
        }

        Debug("AI::maxValue Pushed" << endl);
    }
    else
    {
        // Rings movement phase
        vector<Move> moveSeqeunce;

        vector<Move> removeRowMoves;
        board.getValidRowMoves(prevMove, removeRowMoves, player);
        if (removeRowMoves.size() == 0)
            moveMarkerMoves(board, moveSeqeunce, moveSequences, player, depth);
        else
            rowMoves(board, player, removeRowMoves, moveSeqeunce, moveSequences, true, depth);
    }

    Debug("AI::maxValue Out" << endl);

    sort(moveSequences.begin(), moveSequences.end(), greater<EvaluatedMoveSeq>());

    Move bestMove;
    int bestEval = INT_MIN, evaluation;
    for (auto moveSeq : moveSequences)
    {
        Move lastRingMove = prevMove;
        for (auto move = moveSeq.moveSequence.begin(); move != moveSeq.moveSequence.end(); ++move)
        {
            if ((*move).type == MoveType::moveRing)
                lastRingMove = *move;
            board.playMove(*move, player);
        }

        evaluation = minValue(alpha, beta, depth - 1, board, (lastRingMove), !player, internalMoveCount + 1).second;

        for (auto move = moveSeq.moveSequence.rbegin(); move != moveSeq.moveSequence.rend(); ++move)
            board.undoMove(*move, player);

        if (evaluation >= bestEval)
        {
            bestMoveSeq = moveSeq.moveSequence;
            bestEval = evaluation;
        }
        if(bestEval>=2147380000) {
            if(depth>=5) {
                cout<<"########## FOUND WIN :)))"<<endl;
                cout<<"# DEPTH: "<<depth<<endl;
            }

            break;
        }
        alpha = max(alpha, evaluation);
        if (alpha > beta)
        {
            Debug("AI::maxValue Returning alpha>=beta Alpha=" << alpha << " Beta=" << beta << endl);
            return make_pair(moveSeq.moveSequence, evaluation);
        }
    }
    return make_pair(bestMoveSeq, bestEval);
}

pair<vector<Move>, double> AI::minValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int internalMoveCount)
{
    Debug("AI::minValue - alpha=" << alpha << " Beta=" << beta << " Depth=" << depth << " Player=" << player << endl);

    vector<Move> bestMoveSeq;
    // Check for win
    if (board.isWin(!this->player) && internalMoveCount > this->maxRings * 2)
        return make_pair(bestMoveSeq, INT_MIN + internalMoveCount);
    else if (board.isWin(this->player) && internalMoveCount > this->maxRings * 2)
        return make_pair(bestMoveSeq, INT_MAX - internalMoveCount);

    //Base case for depth
    if (depth <= 0)
        return make_pair(bestMoveSeq, board.evaluate(this->player, internalMoveCount, this->featureWeights));

    vector<EvaluatedMoveSeq> moveSequences;

    if (internalMoveCount < this->maxRings * 2)
    {
        // Rings placement phase
        vector<Move> placeRingMoves, moveSeq;
        board.getValidPlaceRingMoves(placeRingMoves, player);
        for (auto m : placeRingMoves)
        {
            moveSeq.push_back(m);
            // Evaluate position till shallow depth (for move ordering)
            moveSequences.push_back(EvaluatedMoveSeq(moveSeq, 0));
            moveSeq.pop_back();
        }
    }
    else
    {
        // Rings movement phase
        vector<Move> removeRowMoves, moveSeq;
        board.getValidRowMoves(prevMove, removeRowMoves, player);
        if (removeRowMoves.size() == 0)
            moveMarkerMoves(board, moveSeq, moveSequences, player, depth);
        else
            rowMoves(board, player, removeRowMoves, moveSeq, moveSequences, true, depth);
    }

    sort(moveSequences.begin(), moveSequences.end());

    Move bestMove;
    int bestEval = INT_MAX, evaluation;
    for (auto moveSeq : moveSequences)
    {
        Move lastRingMove = prevMove;
        for (auto m = moveSeq.moveSequence.begin(); m != moveSeq.moveSequence.end(); ++m)
        {
            if ((*m).type == MoveType::moveRing)
                lastRingMove = (*m);
            board.playMove(*m, player);
        }

        evaluation = maxValue(alpha, beta, depth - 1, board, (lastRingMove), !player, internalMoveCount + 1).second;

        for (auto m = moveSeq.moveSequence.rbegin(); m != moveSeq.moveSequence.rend(); ++m)
            board.undoMove(*m, player);

        if (evaluation <= bestEval)
        {
            bestMoveSeq = moveSeq.moveSequence;
            bestEval = evaluation;
        }
        beta = min(beta, evaluation);
        if(bestEval<=-2147380000) {
            // if(depth>=5) {
            //     cout<<"### FOUND WIN FOR OPPONENT:("<<endl;
            //     cout<<"# DEPTH: "<<depth<<endl;
            // }
            break;
        }
        
        if (beta < alpha)
        {
            Debug("AI::maxValue Returning alpha>=beta Alpha=" << alpha << " Beta=" << beta << endl);
            return make_pair(moveSeq.moveSequence, evaluation);
        }
    }
    return make_pair(bestMoveSeq, bestEval);
}

void AI::rowMoves(Board &board, bool player, vector<Move> &removeRowMoves, vector<Move> &moveSeq, vector<EvaluatedMoveSeq> &moveSequences, bool continuePlaying, int depth)
{
    Debug("AI::rowMoves - Player=" << player << " ContinuePlaying=" << continuePlaying << endl);

    int ringsLeft = board.getRingsCount(player);

    for (int i = 0; i < removeRowMoves.size(); i++)
    {
        bool moveSeqFound = false;
        // play remove row move
        board.playMove(removeRowMoves[i], player);
        moveSeq.push_back(removeRowMoves[i]);

        vector<Move> removeRingMoves;
        board.getValidRemoveRingMoves(removeRingMoves, player);
        for (auto m1 : removeRingMoves)
        {
            // play moveRing move
            moveSeqFound = false;
            board.playMove(m1, player);
            moveSeq.push_back(m1);

            for (int j = i + 1; j < removeRowMoves.size(); j++)
            {
                if (!isIntersecting(removeRowMoves[i], removeRowMoves[j]))
                {
                    // play remove row move
                    board.playMove(removeRowMoves[j], player);
                    moveSeq.push_back(removeRowMoves[j]);

                    vector<Move> removeRingMoves2;
                    board.getValidRemoveRingMoves(removeRingMoves2, player);
                    for (auto m2 : removeRingMoves2)
                    {
                        // play moveRing move
                        moveSeqFound = false;
                        board.playMove(m2, player);
                        moveSeq.push_back(m2);

                        for (int k = j + 1; k < removeRowMoves.size(); k++)
                        {
                            if (!isIntersecting(removeRowMoves[k], removeRowMoves[j]) && !isIntersecting(removeRowMoves[k], removeRowMoves[i]))
                            {
                                // play remove row move
                                board.playMove(removeRowMoves[k], player);
                                moveSeq.push_back(removeRowMoves[k]);

                                vector<Move> removeRingMoves3;
                                board.getValidRemoveRingMoves(removeRingMoves3, player);

                                for (auto m3 : removeRingMoves3)
                                {
                                    moveSeq.push_back(m3);
                                    // play removeRing move
                                    board.playMove(m3, player);
                                    moveSeqFound = true;

                                    // Evaluate new board position till shallow depth (for move ordering)

                                    int retVal;
                                    if (player == this->player)
                                        retVal = minValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m3, !player, this->maxRings * 2 + 1).second;
                                    else
                                        retVal = maxValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m3, !player, this->maxRings * 2 + 1).second;
                                    moveSequences.push_back(EvaluatedMoveSeq(moveSeq, retVal));

                                    board.undoMove(m3, player);
                                    moveSeq.pop_back();
                                }
                                board.undoMove(removeRowMoves[k], player);
                                moveSeq.pop_back();
                            }
                        }
                        if (!moveSeqFound)
                        {
                            moveSeqFound = true;
                            if (board.getRingsCount(player) == this->maxRings - 3 || !continuePlaying)
                            {
                                // Evaluate new board position till shallow depth (for move ordering)

                                int retVal;
                                if (player == this->player)
                                    retVal = minValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m2, !player, this->maxRings * 2 + 1).second;
                                else
                                    retVal = maxValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m2, !player, this->maxRings * 2 + 1).second;
                                moveSequences.push_back(EvaluatedMoveSeq(moveSeq, retVal));
                            }
                            else
                                moveMarkerMoves(board, moveSeq, moveSequences, player, depth);
                        }

                        board.undoMove(m2, player);
                        moveSeq.pop_back();
                    }
                    board.undoMove(removeRowMoves[j], player);
                    moveSeq.pop_back();
                }
            }

            if (!moveSeqFound)
            {
                // Evaluate new board position till shallow depth (for move ordering)
                if (board.getRingsCount(player) == this->maxRings - 3 || !continuePlaying)
                {
                    // Evaluate new board position till shallow depth (for move ordering)
                    int retVal;
                    if (player == this->player)
                        retVal = minValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m1, !player, this->maxRings * 2 + 1).second;
                    else
                        retVal = maxValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m1, !player, this->maxRings * 2 + 1).second;
                    moveSequences.push_back(EvaluatedMoveSeq(moveSeq, retVal));
                }
                else
                    moveMarkerMoves(board, moveSeq, moveSequences, player, depth);
            }

            board.undoMove(m1, player);
            moveSeq.pop_back();
        }
        board.undoMove(removeRowMoves[i], player);
        moveSeq.pop_back();
    }
}

void AI::moveMarkerMoves(Board &board, vector<Move> &moveSeq, vector<EvaluatedMoveSeq> &moveSequences, bool player, int depth)
{
    Debug("AI::moveMarkerMoves - Player=" << player << endl);

    vector<Move> moveRingMoves;
    board.getValidRingMoves(moveRingMoves, player);
    for (auto m : moveRingMoves)
    {
        // play move m
        board.playMove(m, player);
        moveSeq.push_back(m);

        vector<Move> removeRowMovesAgain;
        board.getValidRowMoves(m, removeRowMovesAgain, player);
        if (removeRowMovesAgain.size() > 0)
            rowMoves(board, player, removeRowMovesAgain, moveSeq, moveSequences, false, depth);
        else
        {
            // Evaluate position till shallow depth (for move ordering)
            int retVal;
            if (player == this->player)
                retVal = minValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m, !player, this->maxRings * 2 + 1).second;
            else
                retVal = maxValue(INT_MIN, INT_MAX, depth - DEPTH_REDUCTION, board, m, !player, this->maxRings * 2 + 1).second;
            moveSequences.push_back(EvaluatedMoveSeq(moveSeq, retVal));
        }

        board.undoMove(m, player);

        moveSeq.pop_back();
    }
}

void AI::setWeights(vector<double> featureWeights)
{
    this->featureWeights = featureWeights;
}

void AI::setMoveCount(int moveCount)
{
    this->moveCount = moveCount;
}

double AI::getEvaluation()
{
    return originalBoard->evaluate(this->player, this->moveCount, this->featureWeights);
}