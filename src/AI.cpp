#include "AI.h"
// #include "Util.h"
#include <iostream>
#include <limits>

const int INT_MAX=numeric_limits<int>::max();
const int INT_MIN=numeric_limits<int>::min();

const int MAX_DEPTH=5;

AI::AI(Board *board, bool player, int time)
{
    this->board = board;
    this->player = player;
    this->time = 1.0 * time;
}

void AI::playMoveSeq()
{
    // Start time




    //Convert All moves to hex here and print

    cout << "P 4 5"<<endl;
    // Stop Time
}

pair<Move,int> AI::maxValue(int alpha, int beta, int depth, Board& board){
    // Check for win
    if(board.isWin(!player)) return make_pair(Move(), INT_MIN);
    else if(board.isWin(player)) return make_pair(Move(),INT_MAX);

    vector<Move> moves;
    board.getValidMoves(moves,player);
    // Add Shallow search for move ordering

    Move bestMove;
    int bestEval=INT_MIN;
    for(auto m: moves){
        board.playMove(m,player);
        int evaluation=minValue(alpha, beta, depth-1, board).second;
        board.undoMove(m,player);
        if(evaluation>bestEval) {
            bestMove=m;
            bestEval=evaluation;
        }
        alpha=max(alpha,evaluation);
        if(alpha>=beta) return make_pair(m,evaluation);
    }
    return make_pair(bestMove,bestEval);
    
}

pair<Move,int> AI::minValue(int alpha, int beta, int depth, Board& board){
    // Check for win
    if(board.isWin(!player)) return make_pair(Move(), INT_MIN);
    else if(board.isWin(player)) return make_pair(Move(),INT_MAX);

    vector<Move> moves;
    board.getValidMoves(moves,player);
    //Add Shallow search for move ordering

    Move bestMove;
    int bestEval=INT_MAX;
    for(auto m: moves){
        board.playMove(m,player);
        int evaluation=maxValue(alpha, beta, depth-1, board).second;
        board.undoMove(m,player);
        if(evaluation<bestEval) {
            bestMove=m;
            bestEval=evaluation;
        }
        beta=min(beta,evaluation);
        if(beta<=alpha) return make_pair(m,evaluation);
    }
    return make_pair(bestMove,bestEval);


}
