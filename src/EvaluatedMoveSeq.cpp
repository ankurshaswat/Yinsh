#include "EvaluatedMoveSeq.h"

EvaluatedMoveSeq::EvaluatedMoveSeq(vector<Move> moveSequence, int evaluation)
{
    this->moveSequence = moveSequence;
    this->evaluation = evaluation;
}

bool EvaluatedMoveSeq::operator<(const EvaluatedMoveSeq &rhs)
{
    if (evaluation < rhs.evaluation)
    {
        return true;
    }
    return false;
}