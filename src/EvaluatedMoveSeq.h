#ifndef EVALUATEDMOVESEQ_H
#define EVALUATEDMOVESEQ_H

#include <vector>
#include "Move.h"

struct EvaluatedMoveSeq
{
    vector<Move> moveSequence;
    int evaluation;

    EvaluatedMoveSeq(vector<Move> moveSequence, int evaluation);
    bool operator<(const EvaluatedMoveSeq &other) const;
    bool operator>(const EvaluatedMoveSeq &other) const;
};

#endif