#include "EvaluatedMoveSeq.h"

EvaluatedMoveSeq::EvaluatedMoveSeq(vector<Move> moveSequence, int evaluation)
{
    this->moveSequence = moveSequence;
    this->evaluation = evaluation;
}

bool EvaluatedMoveSeq::operator<(const EvaluatedMoveSeq &rhs)
{
    return (evaluation < rhs.evaluation);
}

bool EvaluatedMoveSeq::operator>(const EvaluatedMoveSeq &rhs)
{
    return (evaluation > rhs.evaluation);
}