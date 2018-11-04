#include <limits>
#include <iostream>
#include <math.h>
#include "Board.h"
#include "Util.h"

#ifdef USEDEBUG
#define Debug(x) std::cout << "# " << x
#else
#define Debug(x)
#endif

const int INT_MAX = numeric_limits<int>::max() - 1000;
const int INT_MIN = numeric_limits<int>::min() + 1000;

Board::Board() : Board(5, 5) {}

Board::Board(int N, int K) : rings(2), directions(6)
{
    this->board_size = N;
    this->consecutiveMarkers = K;
    this->max_rings = (this->board_size == 5 && this->consecutiveMarkers == 5) ? 5 : 6;
    board = new int *[2 * N + 1];
    for (int i = 0; i < 2 * N + 2; i++)
    {
        board[i] = new int[2 * N + 1];
        for (int j = 0; j < 2 * N + 2; j++)
            board[i][j] = PositionStates::empty;
    }

    // Counts for 5 different types of markers
    counts = new int[5];
    for (int i = 0; i < 5; i++)
        counts[i] = 0;

    counts += 2;

    directions[0] = make_pair(0, 1);
    directions[1] = make_pair(0, -1);
    directions[2] = make_pair(1, 0);
    directions[3] = make_pair(-1, 0);
    directions[4] = make_pair(1, 1);
    directions[5] = make_pair(-1, -1);
}

int Board::getState(pair<int, int> position)
{
    return board[position.first + this->board_size][position.second + this->board_size];
}

int Board::getState(int pos1, int pos2)
{
    return board[pos1 + this->board_size][pos2 + this->board_size];
}

bool Board::validPosition(pair<int, int> position)
{
    int absPos1 = abs(position.first);
    int absPos2 = abs(position.second);
    if (position.first == 0 || position.second == 0 || position.first == position.second)
        return (absPos1 < this->board_size && absPos2 < this->board_size);
    else if (position.first * position.second > 0)
        return (absPos1 <= this->board_size && absPos2 <= this->board_size);
    else
        return (absPos1 + absPos2 <= this->board_size);
}

bool Board::validPlaceRing(pair<int, int> position)
{
    return (validPosition(position) && getState(position) == PositionStates::empty);
}

bool Board::validMoveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player)
{
    if (newPosition == currentPosition)
        return false;

    if (!(validPosition(newPosition)))
        return false;

    if (getState(newPosition) != PositionStates::empty)
        return false;

    if (!(newPosition.first == currentPosition.first || newPosition.second == currentPosition.second || newPosition.first - currentPosition.first == newPosition.second - currentPosition.second))
        return false;

    bool jumpedMarker = false;

    if (newPosition.first == currentPosition.first)
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i = currentPosition.second + increment; i != newPosition.second; i += increment)
        {
            int state = getState(currentPosition.first, i);
            if (state == PositionStates::whiteRing || state == PositionStates::blackRing || (jumpedMarker == true && state == PositionStates::empty))
                return false;
            else if (state == PositionStates::blackMarker || state == PositionStates::whiteMarker)
                jumpedMarker = true;
        }
    }
    else if (newPosition.second == currentPosition.second)
    {
        int increment = newPosition.first < currentPosition.first ? -1 : 1;

        for (int i = currentPosition.first + increment; i != newPosition.first; i += increment)
        {
            int state = getState(i, currentPosition.second);
            if (state == PositionStates::whiteRing || state == PositionStates::blackRing || (jumpedMarker == true && state == PositionStates::empty))
                return false;
            else if (state == PositionStates::blackMarker || state == PositionStates::whiteMarker)
                jumpedMarker = true;
        }
    }
    else
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;
        for (int i1 = currentPosition.first + increment, i2 = currentPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            int state = getState(i1, i2);
            if (state == PositionStates::whiteRing || state == PositionStates::blackRing || (jumpedMarker == true && state == PositionStates::empty))
                return false;
            else if (state == PositionStates::blackMarker || state == PositionStates::whiteMarker)
                jumpedMarker = true;
        }
    }

    return true;
}

void Board::removeMarker(pair<int, int> position)
{
    setState(position, PositionStates::empty);
}

void Board::removeRing(pair<int, int> position)
{
    bool player = getState(position) > 0;
    int ringIndex = player ? 1 : 0;
    for (auto it = rings[ringIndex].begin(); it != rings[ringIndex].end(); ++it)
        if ((*it).first == position.first && (*it).second == position.second)
        {
            rings[ringIndex].erase(it);
            break;
        }
    setState(position, PositionStates::empty);
}

void Board::setState(pair<int, int> position, int positionVal)
{
    int initialVal = board[position.first + this->board_size][position.second + this->board_size];
    counts[initialVal]--;
    board[position.first + this->board_size][position.second + this->board_size] = positionVal;
    counts[positionVal]++;
}

void Board::setState(int position1, int position2, int positionVal)
{
    int initialVal = board[position1 + this->board_size][position2 + this->board_size];
    counts[initialVal]--;
    board[position1 + this->board_size][position2 + this->board_size] = positionVal;
    counts[positionVal]++;
}

void Board::invertState(int pos1, int pos2)
{
    int initialVal = board[pos1 + this->board_size][pos2 + this->board_size];
    counts[initialVal]--;
    board[pos1 + this->board_size][pos2 + this->board_size] = -1 * initialVal;
    counts[-initialVal]++;
}

bool Board::placeRing(pair<int, int> position, bool player)
{
    int playerRing = player ? PositionStates::whiteRing : PositionStates::blackRing;

    int ringIndex = player ? 1 : 0;
    rings[ringIndex].push_back(position);
    setState(position, playerRing);
    return true;
}

bool Board::moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player)
{
    int playerRing = player ? PositionStates::whiteRing : PositionStates::blackRing;
    int playerMarker = player ? PositionStates::whiteMarker : PositionStates::blackMarker;
    setState(currentPosition, playerMarker);
    setState(newPosition, playerRing);

    if (newPosition.first == currentPosition.first)
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i = currentPosition.second + increment; i != newPosition.second; i += increment)
            invertState(currentPosition.first, i);
    }
    else if (newPosition.second == currentPosition.second)
    {
        int increment = newPosition.first < currentPosition.first ? -1 : 1;

        for (int i = currentPosition.first + increment; i != newPosition.first; i += increment)
            invertState(i, currentPosition.second);
    }
    else
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i1 = currentPosition.first + increment, i2 = currentPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
            invertState(i1, i2);
    }

    int ringIndex = player ? 1 : 0;
    for (int i = 0; i < rings[ringIndex].size(); i++)
    {
        pair<int, int> pos = rings[ringIndex][i];
        if (pos.first == currentPosition.first && pos.second == currentPosition.second)
        {
            rings[ringIndex][i] = newPosition;
            break;
        }
    }
    return true;
}

pair<bool, pair<pair<int, int>, pair<int, int>>> Board::checkMarkersLocal(pair<int, int> position, pair<int, int> direction, bool player)
{

    int playerMarker = player ? PositionStates::whiteMarker : PositionStates::blackMarker;

    if (getState(position) == playerMarker)
    {
        int count = 1;

        pair<int, int> startMarker = position;
        startMarker.first += direction.first;
        startMarker.second += direction.second;

        while (validPosition(startMarker))
        {
            count++;
            if (getState(startMarker) != playerMarker)
            {
                count--;
                break;
            }

            startMarker.first += direction.first;
            startMarker.second += direction.second;
        }
        startMarker.first -= direction.first;
        startMarker.second -= direction.second;

        pair<int, int> endMarker = position;
        endMarker.first -= direction.first;
        endMarker.second -= direction.second;

        while (validPosition(endMarker))
        {
            count++;
            if (getState(endMarker) != playerMarker)
            {
                count--;
                break;
            }
            endMarker.first -= direction.first;
            endMarker.second -= direction.second;
        }
        endMarker.first += direction.first;
        endMarker.second += direction.second;

        if (count >= this->consecutiveMarkers)
            return make_pair(true, make_pair(startMarker, endMarker));
    }

    return make_pair(false, make_pair(position, position));
}

vector<pair<pair<int, int>, pair<int, int>>> Board::checkMarkersLine(pair<int, int> position, pair<int, int> direction, bool player)
{
    vector<pair<pair<int, int>, pair<int, int>>> combinationSequences;
    int playerMarker = player ? PositionStates::whiteMarker : PositionStates::blackMarker;

    pair<int, int> newStartMarker = position;
    newStartMarker.first += direction.first;
    newStartMarker.second += direction.second;

    while (validPosition(newStartMarker))
        if (getState(newStartMarker) != playerMarker)
        {
            newStartMarker.first += direction.first;
            newStartMarker.second += direction.second;
        }
        else
        {
            int count = 1;
            pair<int, int> startMarker = newStartMarker;
            pair<int, int> endMarker = startMarker;
            endMarker.first += direction.first;
            endMarker.second += direction.second;
            while (validPosition(endMarker))
            {
                count++;
                if (getState(endMarker) != playerMarker)
                {
                    count--;
                    break;
                }
                endMarker.first += direction.first;
                endMarker.second += direction.second;
            }
            endMarker.first -= direction.first;
            endMarker.second -= direction.second;

            if (count >= this->consecutiveMarkers)
                combinationSequences.push_back(make_pair(startMarker, endMarker));

            newStartMarker.first = endMarker.first + 2 * direction.first;
            newStartMarker.second = endMarker.second + 2 * direction.second;
        }

    return combinationSequences;
}

vector<pair<pair<int, int>, pair<int, int>>> Board::checkMarkers(pair<int, int> newPosition, pair<int, int> oldPosition, bool player)
{

    vector<pair<pair<int, int>, pair<int, int>>> combinationSequences;

    if (newPosition.first == oldPosition.first && newPosition.second == oldPosition.second)
        return combinationSequences;

    pair<bool, pair<pair<int, int>, pair<int, int>>> returnedSequence;

    if (newPosition.first == oldPosition.first)
    {
        int increment = newPosition.second < oldPosition.second ? -1 : 1;

        combinationSequences = checkMarkersLine(newPosition, make_pair(0, increment * -1), player);

        for (int i = oldPosition.second; i != newPosition.second; i += increment)
        {
            returnedSequence = checkMarkersLocal(make_pair(newPosition.first, i), make_pair(1, 0), player);
            if (returnedSequence.first == true)
                combinationSequences.push_back(returnedSequence.second);

            returnedSequence = checkMarkersLocal(make_pair(newPosition.first, i), make_pair(1, 1), player);
            if (returnedSequence.first == true)
                combinationSequences.push_back(returnedSequence.second);
        }
    }
    else if (newPosition.second == oldPosition.second)
    {
        int increment = newPosition.first < oldPosition.first ? -1 : 1;

        combinationSequences = checkMarkersLine(newPosition, make_pair(increment * -1, 0), player);

        for (int i = oldPosition.first; i != newPosition.first; i += increment)
        {
            returnedSequence = checkMarkersLocal(make_pair(i, newPosition.second), make_pair(0, 1), player);
            if (returnedSequence.first == true)
                combinationSequences.push_back(returnedSequence.second);

            returnedSequence = checkMarkersLocal(make_pair(i, newPosition.second), make_pair(1, 1), player);
            if (returnedSequence.first == true)
                combinationSequences.push_back(returnedSequence.second);
        }
    }
    else
    {
        int increment = newPosition.second < oldPosition.second ? -1 : 1;

        combinationSequences = checkMarkersLine(newPosition, make_pair(increment * -1, increment * -1), player);

        for (int i1 = oldPosition.first, i2 = oldPosition.second; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            returnedSequence = checkMarkersLocal(make_pair(i1, i2), make_pair(0, 1), player);
            if (returnedSequence.first == true)
                combinationSequences.push_back(returnedSequence.second);

            returnedSequence = checkMarkersLocal(make_pair(i1, i2), make_pair(1, 0), player);
            if (returnedSequence.first == true)
                combinationSequences.push_back(returnedSequence.second);
        }
    }

    return combinationSequences;
}

void Board::removeMarkers(pair<int, int> startSeries, pair<int, int> endSeries)
{
    if (startSeries.first == endSeries.first)
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i = endSeries.second; i != startSeries.second + increment; i += increment)
            setState(endSeries.first, i, PositionStates::empty);
    }
    else if (startSeries.second == endSeries.second)
    {
        int increment = startSeries.first < endSeries.first ? -1 : 1;

        for (int i = endSeries.first; i != startSeries.first + increment; i += increment)
            setState(i, endSeries.second, PositionStates::empty);
    }
    else
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i1 = endSeries.first, i2 = endSeries.second; i1 != startSeries.first + increment; i1 += increment, i2 += increment)
            setState(i1, i2, PositionStates::empty);
    }
}

void Board::placeMarkers(pair<int, int> startSeries, pair<int, int> endSeries, bool player)
{
    PositionStates marker = player ? whiteMarker : blackMarker;
    if (startSeries.first == endSeries.first)
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i = endSeries.second; i != startSeries.second + increment; i += increment)
            setState(endSeries.first, i, marker);
    }
    else if (startSeries.second == endSeries.second)
    {
        int increment = startSeries.first < endSeries.first ? -1 : 1;

        for (int i = endSeries.first; i != startSeries.first + increment; i += increment)
            setState(i, endSeries.second, marker);
    }
    else
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i1 = endSeries.first, i2 = endSeries.second; i1 != startSeries.first + increment; i1 += increment, i2 += increment)
            setState(i1, i2, marker);
    }
}

void Board::playMove(Move m, bool player)
{
    MoveType type = m.type;
    if (type == MoveType::placeRing)
    {
        Debug("Board::playMove PlaceRing - Position=(" << m.initPosition.first << ',' << m.initPosition.second << ')' << endl);
        placeRing(m.initPosition, player);
    }
    else if (type == MoveType::moveRing)
    {
        Debug("Board::playMove MoveRing - CurrentPosition=(" << m.initPosition.first << ',' << m.initPosition.second << ") NewPosition=(" << m.finalPosition.first << ',' << m.finalPosition.second << ')' << endl);
        moveRing(m.finalPosition, m.initPosition, player);
    }
    else if (type == MoveType::removeRing)
    {
        Debug("Board::playMove RemoveRing - Position=(" << m.initPosition.first << ',' << m.initPosition.second << ')' << endl);
        removeRing(m.initPosition);
    }
    else
    {
        Debug("Board::playMove RemoveMarkers - StartPosition=(" << m.initPosition.first << ',' << m.initPosition.second << ") EndPosition=(" << m.finalPosition.first << ',' << m.finalPosition.second << ')' << endl);
        removeMarkers(m.initPosition, m.finalPosition);
    }
}

void Board::undoMove(Move m, bool player)
{
    MoveType type = m.type;
    if (type == MoveType::placeRing)
    {
        Debug("Board::undoMove PlaceRing - Position=(" << m.initPosition.first << ',' << m.initPosition.second << ')' << endl);
        removeRing(m.initPosition);
    }
    else if (type == MoveType::moveRing)
    {
        Debug("Board::undoMove MoveRing - CurrentPosition=(" << m.initPosition.first << ',' << m.initPosition.second << ") NewPosition=(" << m.finalPosition.first << ',' << m.finalPosition.second << ')' << endl);
        setState(m.initPosition, PositionStates::empty);
        moveRing(m.initPosition, m.finalPosition, player);
        setState(m.finalPosition, PositionStates::empty);
    }
    else if (type == MoveType::removeRing)
    {
        Debug("Board::undoMove RemoveRing - Position=(" << m.initPosition.first << ',' << m.initPosition.second << ')' << endl);
        placeRing(m.initPosition, player);
    }
    else
    {
        Debug("Board::undoMove RemoveMarkers - StartPosition=(" << m.initPosition.first << ',' << m.initPosition.second << ") EndPosition=(" << m.finalPosition.first << ',' << m.finalPosition.second << ')' << endl);
        placeMarkers(m.initPosition, m.finalPosition, player);
    }
}

bool Board::isWin(bool player)
{
    PositionStates ring = player ? whiteRing : blackRing;
    return counts[ring] <= (this->max_rings - 3);
}

void Board::getValidRowMoves(Move prevMoveRing, vector<Move> &moves, bool player)
{
    vector<pair<pair<int, int>, pair<int, int>>> rows = checkMarkers(prevMoveRing.finalPosition, prevMoveRing.initPosition, player);

    for (int i = 0; i < rows.size(); i++)
    {
        pair<pair<int, int>, pair<int, int>> row = rows[i];
        pair<int, int> start = row.first, end = row.second;

        pair<int, int> direction;
        direction.first = end.first - start.first;
        direction.second = end.second - start.second;
        direction = makeUnit(direction);

        int count = inclusiveMarkerCount(row.first, row.second) - (this->consecutiveMarkers - 1);
        for (int offset = 0; offset < count; offset++)
        {
            pair<int, int> alternateStart;
            alternateStart.first = start.first + offset * direction.first;
            alternateStart.second = start.second + offset * direction.second;

            pair<int, int> alternateEnd;
            alternateEnd.first = start.first + (offset + (this->consecutiveMarkers - 1)) * direction.first;
            alternateEnd.second = start.second + (offset + (this->consecutiveMarkers - 1)) * direction.second;

            moves.push_back(Move(MoveType::removeRow, alternateStart, alternateEnd));
        }
    }
}

void Board::getValidRingMoves(vector<Move> &moves, bool player)
{

    int ringIndex = player ? 1 : 0;
    for (int i = 0; i < rings[ringIndex].size(); i++)
    {
        pair<int, int> ringPosition = rings[ringIndex][i];
        for (int j = 0; j < directions.size(); j++)
        {
            pair<int, int> direction = directions[j];
            pair<int, int> checkPosition = ringPosition;

            bool jumpedMarker = false;

            while (true)
            {
                checkPosition.first += direction.first;
                checkPosition.second += direction.second;

                if (!validPosition(checkPosition))
                    break;

                int positionState = getState(checkPosition);

                jumpedMarker = jumpedMarker || (positionState == PositionStates::blackMarker) || (positionState == PositionStates::whiteMarker);

                if (positionState == PositionStates::whiteRing || positionState == PositionStates::blackRing)
                    break;
                else if (positionState == PositionStates::empty)
                {
                    moves.push_back(Move(MoveType::moveRing, ringPosition, checkPosition));

                    if (jumpedMarker)
                        break;
                }
            }
        }
    }
}

void Board::getValidPlaceRingMoves(vector<Move> &moves, bool player)
{
    Debug("Board::getValidPlaceRingMoves - Player=" << player << endl);
    int count = 0, i = 0, j = 0, a, b;

    // vector<pair<int, int>> ringMoves{make_pair(1, 0), make_pair(1, 1), make_pair(3, 0), make_pair(1, 4), make_pair(2, 0)};

    while (count < 1)
    {
        // std::cout << "#TEST: " << test << "\n";
        // i = ringMoves[test].first;
        // j = ringMoves[test].second;

        // pair<int, int> pos = hex2axial(make_pair(i, j));
        pair<int, int> pos = make_pair(i, j);
        if (validPosition(pos) && getState(pos) == PositionStates::empty)
        {
            moves.push_back(Move(MoveType::placeRing, pos, pos));
            count++;
        }
        i = (rand() % (4)) - 2;
        j = (rand() % (4)) - 2;
    }
    std::cout << "#Ring Moves: " << moves.size() << "\n";
    test++;
};

void Board::getValidRemoveRingMoves(vector<Move> &moves, bool player)
{
    int ringIndex = player ? 1 : 0;
    vector<pair<int, int>> *ringsPos = &rings[ringIndex];
    for (auto pos : (*ringsPos))
        moves.push_back(Move(MoveType::removeRing, pos, pos));
}

int Board::evaluate(bool player, int moveCount, vector<double> &featureWeights, vector<double> &featureWeightsOpp)
{
    if (getRingsCount(player) <= 2)
        return INT_MAX - moveCount;

    if (getRingsCount(!player) <= 2)
        return INT_MIN + moveCount;

    vector<vector<double>> featureCounts(2, vector<double>(this->numFeatures));

    this->featureGenerator(featureCounts);

    double score1 = 0, score0 = 0;

    for (int i = 0; i < featureWeights.size(); i++)
        if (player)
            score1 += featureCounts[1][i] * featureWeights[i];
        else
            score1 += featureCounts[1][i] * featureWeightsOpp[i];

    for (int i = 0; i < featureWeights.size(); i++)
        if (player)
            score0 += featureCounts[0][i] * featureWeightsOpp[i];
        else
            score0 += featureCounts[0][i] * featureWeights[i];

    return player ? score1 - score0 : score0 - score1;
}

void Board::featureGenerator(vector<vector<double>> &featureCounts)
{
    // Marker and ring count

    featureCounts[0][FeatureIndexes::MarkersCount] = this->counts[PositionStates::blackMarker];
    featureCounts[0][FeatureIndexes::RingsCount] = this->counts[PositionStates::blackRing];
    featureCounts[1][FeatureIndexes::MarkersCount] = this->counts[PositionStates::whiteMarker];
    featureCounts[1][FeatureIndexes::RingsCount] = this->counts[PositionStates::whiteRing];

    // Calculate counts for structures

    for (int i = -this->board_size; i <= this->board_size; i++)
    {
        bool validStartFound = false;
        int prevState = -3;
        int countSingleType = 0, countWithRing = 0;
        for (int j = -this->board_size; j <= this->board_size; j++)
        {
            pair<int, int> position = make_pair(i, j);
            bool breaker = counter(position, validStartFound, prevState, countSingleType, countWithRing, featureCounts);
            if (breaker)
                break;
        }
    }

    for (int j = -this->board_size; j <= this->board_size; j++)
    {
        bool validStartFound = false;
        int prevState = -3;
        int countSingleType = 0, countWithRing = 0;
        for (int i = -this->board_size; i <= this->board_size; i++)
        {
            pair<int, int> position = make_pair(i, j);
            bool breaker = counter(position, validStartFound, prevState, countSingleType, countWithRing, featureCounts);
            if (breaker)
                break;
        }
    }

    for (int i = -this->board_size; i <= 0; i++)
        for (int j = -this->board_size; j <= 0; j++)
        {
            if (abs(i) != this->board_size && abs(j) != this->board_size)
                continue;
            bool validStartFound = false;
            int prevState = -3;
            int countWithRing = 0, countSingleType = 0;
            for (int k = 0; k < 2 * this->board_size; k++)
            {
                pair<int, int> position = make_pair(i + k, j + k);
                bool breaker = counter(position, validStartFound, prevState, countSingleType, countWithRing, featureCounts);
                if (breaker)
                    break;
            }
        }

    // Calculate Counts for distances

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < rings[i].size(); j++)
        {
            pair<int, int> position = rings[i][j];
            double dist = sqrt(pow(position.first, 2) + pow(position.second, 2));
            featureCounts[i][j + 10] = dist;
        }

    // Calculate degree of freedom and available Jump spaces

    for (int player = 0; player < 2; player++)
    {
        for (int i = 0; i < rings[player].size(); i++)
        {
            pair<int, int> ringPosition = rings[player][i];
            int jumpPositions = 0;
            vector<bool> freedoms(6);

            for (int j = 0; j < directions.size(); j++)
            {
                pair<int, int> direction = directions[j];
                pair<int, int> checkPosition = ringPosition;

                bool jumpedMarker = false;

                while (true)
                {
                    checkPosition.first += direction.first;
                    checkPosition.second += direction.second;

                    if (!validPosition(checkPosition))
                        break;

                    int positionState = getState(checkPosition);

                    jumpedMarker = jumpedMarker || (positionState == PositionStates::blackMarker) || (positionState == PositionStates::whiteMarker);

                    if (positionState == PositionStates::whiteRing || positionState == PositionStates::blackRing)
                        break;
                    else if (positionState == PositionStates::empty)
                    {
                        jumpPositions++;
                        freedoms[j] = true;
                        if (jumpedMarker)
                            break;
                    }
                }
            }

            featureCounts[player][FeatureIndexes::Ring1JumpPositions + i] = jumpPositions;
            for (int freedomsCount = 0; freedomsCount < freedoms.size(); freedomsCount++)
                if (freedoms[freedomsCount])
                    featureCounts[player][FeatureIndexes::Ring1Freedom + i]++;
        }
    }
}

bool Board::counter(pair<int, int> &position, bool &validStartFound, int &prevState, int &countSingleType, int &countWithRing, vector<vector<double>> &featureCounts)
{
    if (validPosition(position))
    {
        validStartFound = true;
        int state = getState(position);
        if (state == prevState)
        {
            countSingleType++;
            countWithRing++;
        }
        else
        {
            if (countSingleType > 1)
                switch (prevState)
                {
                case PositionStates::whiteMarker:
                case PositionStates::whiteRing:
                    featureCounts[1][min(countSingleType - 2, 4)] += 1;
                    countSingleType = 1;

                    break;
                case PositionStates::blackMarker:
                case PositionStates::blackRing:
                    featureCounts[0][min(countSingleType - 2, 4)] += 1;
                    countSingleType = 1;

                    break;
                default:
                    countSingleType = 0;

                    break;
                }
            else
                countSingleType = 1;

            if (countWithRing > 1)
                switch (prevState)
                {
                case PositionStates::whiteMarker:
                case PositionStates::whiteRing:
                    if (state == PositionStates::whiteMarker || state == PositionStates::whiteRing)
                        countWithRing++;
                    else
                    {
                        featureCounts[1][min(countWithRing + 3, 9)] += 1;
                        countWithRing = 1;
                    }
                    break;
                case PositionStates::blackMarker:
                case PositionStates::blackRing:
                    if (state == PositionStates::blackMarker || state == PositionStates::blackRing)
                        countWithRing++;
                    else
                    {
                        featureCounts[0][min(countWithRing + 3, 9)] += 1;
                        countWithRing = 1;
                    }
                    break;
                default:
                    countWithRing = 1;
                    break;
                }
            else
                countWithRing = 1;
        }
        prevState = state;
    }
    else if (validStartFound)
    {
        if (countSingleType > 1)
            switch (prevState)
            {
            case PositionStates::whiteMarker:
            case PositionStates::whiteRing:
                featureCounts[1][min(countSingleType - 2, 4)] += 1;
                break;
            case PositionStates::blackMarker:
            case PositionStates::blackRing:
                featureCounts[0][min(countSingleType - 2, 4)] += 1;
                break;
            default:
                break;
            }

        if (countWithRing > 1)
            switch (prevState)
            {
            case PositionStates::whiteMarker:
            case PositionStates::whiteRing:
                featureCounts[1][min(countWithRing + 3, 9)] += 1;
                break;
            case PositionStates::blackMarker:
            case PositionStates::blackRing:
                featureCounts[0][min(countWithRing + 3, 9)] += 1;
                break;
            default:
                break;
            }

        return true;
    }

    return false;
}

int Board::getSize()
{
    return this->board_size;
}

int Board::getRingsCount(bool player)
{
    int ringIndex = player ? 1 : 0;
    return rings[ringIndex].size();
}

Board::~Board()
{
    for (int i = 0; i < 2 * this->board_size + 2; i++)
        delete[] board[i];
    delete[] board;
    delete[] counts;
}

vector<pair<int, int>> Board::getRingPositions(bool player)
{
    int ringIndex = player ? 1 : 0;
    return rings[ringIndex];
}