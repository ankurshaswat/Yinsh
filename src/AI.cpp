#include "AI.h"
#include "Util.h"
#include <iostream>
#include <limits>

const int INT_MAX = numeric_limits<int>::max();
const int INT_MIN = numeric_limits<int>::min();

const int MAX_DEPTH = 5;

AI::AI(Board *board, bool player, int time, int n)
{
    this->originalBoard = board;
    this->boardSize = n;
    this->player = player;
    this->time = 1.0 * time;
    this->moveCount = 0;
}

void AI::playMoveSeq(Move prevMove)
{
    cout << "# AI::playMoveSeq" << endl;

    Board *board = new Board(this->boardSize, this->originalBoard);

    // if (m.type == MoveType::moveRing)
    // {

    //     // This needs to be used.....
    // }

    // Start time

    // pair<vector<Move>, int> maxValue(INT_MIN, INT_MAX, board, )

    //Convert All moves to hex here and print
    // cout << "P 4 5" << endl;
    // // Stop Time

    pair<vector<Move>, int> returnedMovePair = maxValue(INT_MIN, INT_MAX, 6, *board, prevMove, 0);
    vector<Move> moves = returnedMovePair.first;
    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves[i];
        if (move.type == MoveType::placeRing)
        {
            pair<int, int> position = axial2hex(move.initPosition);
            cout << "P " << position.first << ' ' << position.second << endl;
            break;
        }
    }
    moveCount++;
}

pair<vector<Move>, int> AI::maxValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player)
{
    cout << "# AI::maxValue" << endl;
    vector<Move> bestMoveSeq;
    // Check for win
    if (board.isWin(!player) && moveCount > 5)
    {
        cout << "# AI::maxValue 0 win state" << endl;
        bestMoveSeq.push_back(Move());
        return make_pair(bestMoveSeq, INT_MIN);
    }
    else if (board.isWin(player) && moveCount > 5)
    {
        cout << "# AI::maxValue 1 win state" << endl;
        bestMoveSeq.push_back(Move());
        return make_pair(bestMoveSeq, INT_MAX);
    }

    //Base case for depth
    if (depth <= 0)
    {
        return make_pair(bestMoveSeq, board.evaluate(player));
    }

    vector<vector<Move>> moveSequences;
    if (moveCount < boardSize)
    {
        // Rings placement phase
        cout << "# AI::maxValue Rings Placement phase" << endl;
        vector<Move> moveSeq;

        vector<Move> placeRingMoves;
        board.getValidPlaceRingMoves(placeRingMoves, player);
        cout << "# AI::maxValue Got Valid Ring Placements" << endl;

        for (auto m : placeRingMoves)
        {
            moveSeq.push_back(m);
            // Evaluate position till shallow depth (for move ordering)
            moveSequences.push_back(moveSeq);
        }

        cout << "# AI::maxValue Pushed" << endl;

    }
    else
    {
        // Rings movement phase
        vector<Move> moveSeq;

        vector<Move> removeRowMoves;
        board.getValidRowMoves(prevMove, removeRowMoves, player);
        if (removeRowMoves.size() == 0)
        {
            moveMarkerMoves(board, moveSeq, moveSequences, player);
        }
        else
        {
            rowMoves(board, player, removeRowMoves, moveSeq, moveSequences, true);
        }
    }
        cout << "# AI::maxValue Out" << endl;


    Move bestMove;
    int bestEval = INT_MIN, evaluation;
    for (auto moveSeq : moveSequences)
    {
        Move *lastRingMove = &prevMove;
        for (auto m = moveSeq.begin(); m != moveSeq.end(); ++m)
        {
            if ((*m).type == MoveType::moveRing)
                lastRingMove = &(*m);
            board.playMove(*m, player);
        }

        evaluation = minValue(alpha, beta, depth - 1, board, (*lastRingMove), !player).second;

        for (auto m = moveSeq.rbegin(); m != moveSeq.rend(); ++m)
        {
            board.undoMove(*m, player);
        }

        if (evaluation > bestEval)
        {
            bestMoveSeq = moveSeq;
            bestEval = evaluation;
        }
        alpha = max(alpha, evaluation);
        if (alpha >= beta)
            return make_pair(moveSeq, evaluation);
    }
    return make_pair(bestMoveSeq, bestEval);
}

pair<vector<Move>, int> AI::minValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player)
{
    cout << "# AI::minValue" << endl;

    vector<Move> bestMoveSeq;
    // Check for win
    if (board.isWin(!player))
    {
        bestMoveSeq.push_back(Move());
        return make_pair(bestMoveSeq, INT_MIN);
    }
    else if (board.isWin(player))
    {
        bestMoveSeq.push_back(Move());
        return make_pair(bestMoveSeq, INT_MAX);
    }

    //Base case for depth
    if (depth <= 0)
    {
        return make_pair(bestMoveSeq, board.evaluate(player));
    }

    vector<vector<Move>> moveSequences;

    if (moveCount < boardSize)
    {
        // Rings placement phase
        vector<Move> placeRingMoves, moveSeq;
        board.getValidPlaceRingMoves(placeRingMoves, player);
        for (auto m : placeRingMoves)
        {
            moveSeq.push_back(m);
            // Evaluate position till shallow depth (for move ordering)
            moveSequences.push_back(moveSeq);
        }
    }
    else
    {
        // Rings movement phase
        vector<Move> removeRowMoves, moveSeq;
        board.getValidRowMoves(prevMove, removeRowMoves, player);
        if (removeRowMoves.size() == 0)
        {
            moveMarkerMoves(board, moveSeq, moveSequences, player);
        }

        else
        {
            rowMoves(board, player, removeRowMoves, moveSeq, moveSequences, true);
        }
    }

    Move bestMove;
    int bestEval = INT_MIN, evaluation;
    for (auto moveSeq : moveSequences)
    {
        Move *lastRingMove = &prevMove;
        for (auto m = moveSeq.begin(); m != moveSeq.end(); ++m)
        {
            if ((*m).type == MoveType::moveRing)
                lastRingMove = &(*m);
            board.playMove(*m, player);
        }

        evaluation = maxValue(alpha, beta, depth - 1, board, (*lastRingMove), !player).second;

        for (auto m = moveSeq.rbegin(); m != moveSeq.rend(); ++m)
        {
            board.undoMove(*m, player);
        }

        if (evaluation < bestEval)
        {
            bestMoveSeq = moveSeq;
            bestEval = evaluation;
        }
        beta = min(beta, evaluation);
        if (beta <= alpha)
            return make_pair(moveSeq, evaluation);
    }
    return make_pair(bestMoveSeq, bestEval);
}

void AI::rowMoves(Board &board, bool player, vector<Move> &removeRowMoves, vector<Move> &moveSeq, vector<vector<Move>> &moveSequences, bool continuePlaying)
{
    cout << "# AI::rowMoves" << endl;

    int ringsLeft = board.getRingsCount(player);

    for (int i = 0; i < removeRowMoves.size(); i++)
    {
        bool moveSeqFound = false;
        // play remove row move
        board.playMove(removeRowMoves[i], player);
        moveSeq.push_back(removeRowMoves[i]);

        vector<Move> moveRingMoves;
        board.getValidRemoveRingMoves(moveRingMoves, player);
        for (auto m1 : moveRingMoves)
        {
            // play moveRing move
            board.playMove(m1, player);
            moveSeq.push_back(m1);

            for (int j = i + 1; j < removeRowMoves.size(); j++)
            {
                if (!board.isIntersecting(removeRowMoves[i], removeRowMoves[j]))
                {
                    // play remove row move
                    board.playMove(removeRowMoves[j], player);
                    moveSeq.push_back(removeRowMoves[j]);

                    vector<Move> moveRingMoves2;
                    board.getValidRemoveRingMoves(moveRingMoves2, player);
                    for (auto m2 : moveRingMoves2)
                    {
                        // play moveRing move
                        board.playMove(m2, player);
                        moveSeq.push_back(m2);

                        for (int k = j + 1; k < removeRowMoves.size(); k++)
                        {
                            if (!board.isIntersecting(removeRowMoves[k], removeRowMoves[j]) && !board.isIntersecting(removeRowMoves[k], removeRowMoves[i]))
                            {
                                // play remove row move
                                board.playMove(removeRowMoves[k], player);
                                moveSeq.push_back(removeRowMoves[k]);

                                vector<Move> moveRingMoves3;
                                board.getValidRemoveRingMoves(moveRingMoves3, player);

                                for (auto m3 : moveRingMoves3)
                                {
                                    moveSeq.push_back(m3);
                                    // play moveRing move
                                    board.playMove(m3, player);
                                    // Evaluate new board position till shallow depth (for move ordering)
                                    moveSeqFound = true;
                                    moveSequences.push_back(moveSeq); //push this with evaluated value

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
                            if (ringsLeft == 2 || !continuePlaying)
                            {
                                // Evaluate new board position till shallow depth (for move ordering)
                                moveSequences.push_back(moveSeq); //push this with evaluated value
                            }
                            else
                            {
                                moveMarkerMoves(board, moveSeq, moveSequences, player);
                            }
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
                moveSeqFound = true;
                if (ringsLeft == 1 || !continuePlaying)
                {
                    // Evaluate new board position till shallow depth (for move ordering)
                    moveSequences.push_back(moveSeq); //push this with evaluated value
                }
                else
                {
                    moveMarkerMoves(board, moveSeq, moveSequences, player);
                }
            }

            board.undoMove(m1, player);
            moveSeq.pop_back();
        }
        board.undoMove(removeRowMoves[i], player);
        moveSeq.pop_back();
    }
}

void AI::moveMarkerMoves(Board &board, vector<Move> &moveSeq, vector<vector<Move>> &moveSequences, bool player)
{
    cout << "# AI::moveMarkerMoves" << endl;

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
        {
            rowMoves(board, player, removeRowMovesAgain, moveSeq, moveSequences, false);
        }
        else
        {
            // Evaluate position till shallow depth (for move ordering)
            moveSequences.push_back(moveSeq); //push this with evaluated value
        }

        board.undoMove(m, player);

        moveSeq.pop_back();
    }
}
