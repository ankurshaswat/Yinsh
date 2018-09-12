#include "AI.h"
#include "Util.h"
#include "EvaluatedMoveSeq.h"
#include <algorithm>
#include <limits>

#ifdef USEDEBUG
#define Debug(x) std::cout << "# " << x
#else
#define Debug(x)
#endif

const int INT_MAX = numeric_limits<int>::max();
const int INT_MIN = numeric_limits<int>::min();

const int MAX_DEPTH = 4;
const int SHALLOW_DEPTH = 4;

AI::AI(Board *board, bool player, int time, int n)
{
    this->originalBoard = board;
    this->boardSize = n;
    this->player = player;
    this->time = 1.0 * time;
    this->moveCount = 0;
}

void AI::printMove(Move move)
{
    if (move.type == MoveType::placeRing)
    {
        pair<int, int> position = axial2hex(move.initPosition);
        cout << "P " << position.first << ' ' << position.second << ' ';
        // break;
    }
    else if (move.type == MoveType::moveRing)
    {
        pair<int, int> position1 = axial2hex(move.initPosition);
        pair<int, int> position2 = axial2hex(move.finalPosition);
        cout << "S " << position1.first << ' ' << position1.second << ' ';
        cout << "M " << position2.first << ' ' << position2.second << ' ';
        // break;
    }
    else if (move.type == MoveType::removeRow)
    {
        pair<int, int> position1 = axial2hex(move.initPosition);
        pair<int, int> position2 = axial2hex(move.finalPosition);
        cout << "RS " << position1.first << ' ' << position1.second << ' ';
        cout << "RE " << position2.first << ' ' << position2.second << ' ';
        // break;
    }
    if (move.type == MoveType::removeRing)
    {
        pair<int, int> position = axial2hex(move.initPosition);
        cout << "X " << position.first << ' ' << position.second << ' ';
        // break;
    }
}

void AI::playMoveSeq(Move prevMove)
{
    Debug("# AI::playMoveSeq" << endl);

    // Board *board = new Board(this->boardSize, this->originalBoard);

    // if (m.type == MoveType::moveRing)
    // {

    //     // This needs to be used.....
    // }

    // Start time

    // pair<vector<Move>, int> maxValue(INT_MIN, INT_MAX, board, )

    //Convert All moves to hex here and print
    // Debug( << "P 4 5" << endl;
    // // Stop Time

    pair<vector<Move>, int> returnedMovePair = maxValue(INT_MIN, INT_MAX, MAX_DEPTH, *originalBoard, prevMove, player, moveCount);
    vector<Move> moves = returnedMovePair.first;

    // Debug("# Got Returned Moves" << endl);

    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves[i];
        this->originalBoard->playMove(move, player);
    }
    for (auto move : moves)
        printMove(move);
    cout << endl;
    moveCount += 2;
}

pair<vector<Move>, int> AI::maxValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int internalMoveCount)
{
    Debug("# AI::maxValue - alpha=" << alpha << " Beta=" << beta << " Depth=" << depth << " Player=" << player << endl);
    vector<Move> bestMoveSeq;
    // Check for win
    if (board.isWin(!player) && internalMoveCount > 10)
    {
        // cout << "# AI::maxValue 0 win state" << endl;
        return make_pair(bestMoveSeq, INT_MIN);
    }
    else if (board.isWin(player) && internalMoveCount > 10)
    {
        // cout << "# AI::maxValue 1 win state" << endl;
        return make_pair(bestMoveSeq, INT_MAX);
    }

    //Base case for depth
    if (depth <= 0)
    {
        return make_pair(bestMoveSeq, board.evaluate(player));
    }

    // Debug(<<"# "<<bestMoveSeq.size();
    vector<vector<Move>> moveSequences;
    if (internalMoveCount < 10)
    {
        // Rings placement phase
        vector<Move> moveSeqeunce;

        vector<Move> placeRingMoves;
        board.getValidPlaceRingMoves(placeRingMoves, player);
        Debug("# AI::maxValue Got Valid Ring Placements" << endl);

        for (auto move : placeRingMoves)
        {
            moveSeqeunce.push_back(move);
            // Evaluate position till shallow depth (for move ordering)
            moveSequences.push_back(moveSeqeunce);
            moveSeqeunce.pop_back();
        }

        Debug("# AI::maxValue Pushed" << endl);
    }
    else
    {
        // Rings movement phase
        vector<Move> moveSeqeunce;

        vector<Move> removeRowMoves;
        board.getValidRowMoves(prevMove, removeRowMoves, player);
        if (removeRowMoves.size() == 0)
        {
            moveMarkerMoves(board, moveSeqeunce, moveSequences, player);
        }
        else
        {
            rowMoves(board, player, removeRowMoves, moveSeqeunce, moveSequences, true);
        }
    }
    Debug("# AI::maxValue Out" << endl);

    Move bestMove;
    int bestEval = INT_MIN, evaluation;
    for (auto moveSeq : moveSequences)
    {
        // Move *lastRingMove = &prevMove;
        Move lastRingMove = prevMove;
        for (auto move = moveSeq.begin(); move != moveSeq.end(); ++move)
        {
            if ((*move).type == MoveType::moveRing)
                lastRingMove = *move;
            board.playMove(*move, player);
        }

        evaluation = minValue(alpha, beta, depth - 1, board, (lastRingMove), !player, internalMoveCount + 1).second;

        for (auto move = moveSeq.rbegin(); move != moveSeq.rend(); ++move)
        {
            board.undoMove(*move, player);
        }

        if (evaluation >= bestEval)
        {
            bestMoveSeq = moveSeq;
            bestEval = evaluation;
        }
        alpha = max(alpha, evaluation);
        if (alpha >= beta)
        {
            Debug("# AI::maxValue Returning alpha>=beta Alpha=" << alpha << " Beta=" << beta << endl);
            return make_pair(moveSeq, evaluation);
        }
    }
    return make_pair(bestMoveSeq, bestEval);
}

pair<vector<Move>, int> AI::minValue(int alpha, int beta, int depth, Board &board, Move prevMove, bool player, int internalMoveCount)
{
    Debug("# AI::minValue - alpha=" << alpha << " Beta=" << beta << " Depth=" << depth << " Player=" << player << endl);

    vector<Move> bestMoveSeq;
    // Check for win
    if (board.isWin(!player) && internalMoveCount > 10)
    {
        // cout << "# AI::maxValue 0 win state" << endl;
        return make_pair(bestMoveSeq, INT_MAX);
    }
    else if (board.isWin(player) && internalMoveCount > 10)
    {
        // cout << "# AI::maxValue 1 win state" << endl;
        return make_pair(bestMoveSeq, INT_MIN);
    }

    //Base case for depth
    if (depth <= 0)
    {
        return make_pair(bestMoveSeq, board.evaluate(player));
    }

    vector<vector<Move>> moveSequences;

    if (internalMoveCount < 10)
    {
        // Rings placement phase
        vector<Move> placeRingMoves, moveSeq;
        board.getValidPlaceRingMoves(placeRingMoves, player);
        for (auto m : placeRingMoves)
        {
            moveSeq.push_back(m);
            // Evaluate position till shallow depth (for move ordering)
            moveSequences.push_back(moveSeq);
            moveSeq.pop_back();
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
    int bestEval = INT_MAX, evaluation;
    for (auto moveSeq : moveSequences)
    {
        Move lastRingMove = prevMove;
        for (auto m = moveSeq.begin(); m != moveSeq.end(); ++m)
        {
            if ((*m).type == MoveType::moveRing)
                lastRingMove = (*m);
            board.playMove(*m, player);
        }

        evaluation = maxValue(alpha, beta, depth - 1, board, (lastRingMove), !player, internalMoveCount + 1).second;

        for (auto m = moveSeq.rbegin(); m != moveSeq.rend(); ++m)
        {
            board.undoMove(*m, player);
        }

        if (evaluation <= bestEval)
        {
            bestMoveSeq = moveSeq;
            bestEval = evaluation;
        }
        beta = min(beta, evaluation);
        if (beta <= alpha)
        {
            Debug("# AI::maxValue Returning alpha>=beta Alpha=" << alpha << " Beta=" << beta << endl);
            return make_pair(moveSeq, evaluation);
        }
    }
    return make_pair(bestMoveSeq, bestEval);
}

void AI::rowMoves(Board &board, bool player, vector<Move> &removeRowMoves, vector<Move> &moveSeq, vector<vector<Move>> &moveSequences, bool continuePlaying)
{
    Debug("# AI::rowMoves - Player=" << player << " ContinuePlaying=" << continuePlaying << endl);

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
            moveSeqFound=false;
            board.playMove(m1, player);
            moveSeq.push_back(m1);
            // cout << "# Row made after opponent's move- "
            //  << " " << board.getRingsCount(player) << " " << removeRowMoves[i].initPosition.first << ", " << removeRowMoves[i].initPosition.second << endl;

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
                        moveSeqFound=false;
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
                            if (board.getRingsCount(player) == 2 || !continuePlaying)
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
                // moveSeqFound = true;
                if (board.getRingsCount(player) == 2 || !continuePlaying)
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
    Debug("# AI::moveMarkerMoves - Player=" << player << endl);
    vector<EvaluatedMoveSeq> tempMoveSequences;

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
            pair<vector<Move>, int> retVal;
            if (player == this->player)
            {
                retVal = minValue(INT_MIN, INT_MAX, SHALLOW_DEPTH, board, m, !player, 11);
            }
            else
            {
                retVal = maxValue(INT_MIN, INT_MAX, SHALLOW_DEPTH, board, m, !player, 11);
            }
            tempMoveSequences.push_back(EvaluatedMoveSeq(retVal.first, retVal.second));
            // moveSequences.push_back(moveSeq); //push this with evaluated value
        }

        board.undoMove(m, player);

        moveSeq.pop_back();
    }

    if (player == this->player)
    {
        sort(tempMoveSequences.begin(), tempMoveSequences.end(),greater<EvaluatedMoveSeq>());
    }
    else
    {
        sort(tempMoveSequences.begin(), tempMoveSequences.end());
    }

    for (int i =0;i<tempMoveSequences.size();i++) {
        moveSequences.push_back(tempMoveSequences[i].moveSequence);
    }
}
