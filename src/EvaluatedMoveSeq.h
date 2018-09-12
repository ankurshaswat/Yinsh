#include "Move.h"
#include <vector>

struct EvaluatedMoveSeq
{
    vector<Move> moveSequence;
    int evaluation;

    EvaluatedMoveSeq(vector<Move> moveSequence, int evaluation);
    bool operator<(const EvaluatedMoveSeq &other) const;
    bool operator>(const EvaluatedMoveSeq &other) const;
};