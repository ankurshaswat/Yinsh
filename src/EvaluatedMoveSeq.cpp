#include "EvaluatedMoveSeq.h"

EvaluatedMoveSeq::EvaluatedMoveSeq(vector<Move> moveSequence, int evaluation) : moveSequence(moveSequence), evaluation(evaluation) {}

bool EvaluatedMoveSeq::operator<(const EvaluatedMoveSeq &other) const
{
    return (evaluation < other.evaluation);
}

bool EvaluatedMoveSeq::operator>(const EvaluatedMoveSeq &other) const
{
    return (evaluation > other.evaluation);
}