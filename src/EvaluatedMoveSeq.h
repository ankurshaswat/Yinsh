#include "Move.h"
#include <vector>

struct EvaluatedMoveSeq
{
    vector<Move> moveSequence;
    int evaluation;

    EvaluatedMoveSeq(vector<Move> moveSequence, int evaluation);
    bool operator<(const EvaluatedMoveSeq &rhs);
    bool operator>(const EvaluatedMoveSeq &rhs);
};