#include "Board.h"
#include "Util.h"

Board::Board() : Board(5) {}

Board::Board(int n)
{
    this->n = n;
    board = new int *[2 * n + 1];
    for (int i = 0; i < 2 * n + 2; i++)
    {
        board[i] = new int[2 * n + 1];
        for (int j = 0; j < 2 * n + 2; j++)
        {
            board[i][j] = PositionStates::empty;
        }
    }

    counts = new int[5];
    for (int i = 0; i < 5; i++)
    {
        counts[i] = 0;
    }

    counts += 2;

    directions.push_back(make_pair(0, 1));
    directions.push_back(make_pair(0, -1));
    directions.push_back(make_pair(1, 0));
    directions.push_back(make_pair(-1, 0));
    directions.push_back(make_pair(1, 1));
    directions.push_back(make_pair(-1, -1));
}

int Board::getState(pair<int, int> position)
{
    return board[position.first + n][position.second + n];
}

int Board::getState(int pos1, int pos2)
{
    return board[pos1 + n][pos2 + n];
}

bool Board::validPosition(pair<int, int> position)
{
    if (position.first == 0 || position.second == 0 || position.first == position.second)
    {
        return (position.first < n && position.second < n && position.first > -1 * n && position.second > -1 * n);
    }

    return (position.first <= n && position.second <= n && position.first >= -1 * n && position.second >= -1 * n);
}

bool Board::validPlaceRing(pair<int, int> position)
{
    return (validPosition(position) && getState(position) == PositionStates::empty);
}

bool Board::validMoveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player)
{
    if (newPosition == currentPosition)
    {
        return false;
    }

    if (!(validPosition(newPosition)))
    {
        return false;
    }

    if (getState(newPosition) != PositionStates::empty)
    {
        return false;
    }

    if (!(newPosition.first == currentPosition.first || newPosition.second == currentPosition.second || newPosition.first - currentPosition.first == newPosition.second - currentPosition.second))
    {
        return false;
    }

    bool jumpedMarker = false;

    if (newPosition.first == currentPosition.first)
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i = currentPosition.second + increment; i != newPosition.second; i += increment)
        {
            int state = getState(currentPosition.first, i);
            if (state == PositionStates::whiteRing || state == PositionStates::blackRing || (jumpedMarker == true && state == PositionStates::empty))
            {
                return false;
            }
            else if (state == PositionStates::blackMarker || state == PositionStates::whiteMarker)
            {
                jumpedMarker = true;
            }
        }
    }
    else if (newPosition.second == currentPosition.second)
    {
        int increment = newPosition.first < currentPosition.first ? -1 : 1;

        for (int i = currentPosition.first + increment; i != newPosition.first; i += increment)
        {
            int state = getState(i, currentPosition.second);
            if (state == PositionStates::whiteRing || state == PositionStates::blackRing || (jumpedMarker == true && state == PositionStates::empty))
            {
                return false;
            }
            else if (state == PositionStates::blackMarker || state == PositionStates::whiteMarker)
            {
                jumpedMarker = true;
            }
        }
    }
    else
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;
        for (int i1 = currentPosition.first + increment, i2 = currentPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            int state = getState(i1, i2);
            if (state == PositionStates::whiteRing || state == PositionStates::blackRing || (jumpedMarker == true && state == PositionStates::empty))
            {
                return false;
            }
            else if (state == PositionStates::blackMarker || state == PositionStates::whiteMarker)
            {
                jumpedMarker = true;
            }
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
    vector<pair<int, int>> searchList = player ? rings1 : rings0;
    for (auto it = searchList.begin(); it != searchList.end(); ++it)
    {
        if ((*it).first == position.first && (*it).second == position.second)
        {
            searchList.erase(it);
            break;
        }
    }
    setState(position, PositionStates::empty);
}

void Board::setState(pair<int, int> position, int positionVal)
{
    int initialVal = board[position.first + n][position.second + n];
    counts[initialVal]--;
    board[position.first + n][position.second + n] = positionVal;
    counts[positionVal]++;
}

void Board::setState(int position1, int position2, int positionVal)
{
    int initialVal = board[position1 + n][position2 + n];
    counts[initialVal]--;
    board[position1 + n][position2 + n] = positionVal;
    counts[positionVal]++;
}

void Board::invertState(int pos1, int pos2)
{
    int initialVal = board[pos1 + n][pos2 + n];
    counts[initialVal]--;
    board[pos1 + n][pos2 + n] = -1 * initialVal;
    counts[-initialVal]++;
}

bool Board::placeRing(pair<int, int> position, bool player)
{

    int playerRing = player ? PositionStates::whiteRing : PositionStates::blackRing;

    // if (validPlaceRing(position))
    // {
    vector<pair<int, int>> searchList = player ? rings1 : rings0;
    searchList.push_back(position);
    setState(position, playerRing);
    return true;
    // }

    // return false;
}

bool Board::moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player)
{
    // if (!validMoveRing(newPosition, currentPosition, player))
    // {
    //     return false;
    // }

    int playerRing = player ? PositionStates::whiteRing : PositionStates::blackRing;
    int playerMarker = player ? PositionStates::whiteMarker : PositionStates::blackMarker;
    setState(currentPosition, playerMarker);
    setState(newPosition, playerRing);

    if (newPosition.first == currentPosition.first)
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i = currentPosition.second + increment; i != newPosition.second; i += increment)
        {
            invertState(currentPosition.first, i);
        }
    }
    else if (newPosition.second == currentPosition.second)
    {
        int increment = newPosition.first < currentPosition.first ? -1 : 1;

        for (int i = currentPosition.first + increment; i != newPosition.first; i += increment)
        {
            invertState(i, currentPosition.second);
        }
    }
    else
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i1 = currentPosition.first + increment, i2 = currentPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            invertState(i1, i2);
        }
    }

    vector<pair<int, int>> searchList;
    searchList = player ? rings1 : rings0;
    for (int i = 0; i < searchList.size(); i++)
    {
        pair<int, int> pos = searchList[i];
        if (pos.first == currentPosition.first && pos.second == currentPosition.second)
        {
            searchList[i] = newPosition;
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
        int count = 0;

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

        if (count >= 5)
        {
            return make_pair(true, make_pair(startMarker, endMarker));
        }
    }

    return make_pair(false, make_pair(position, position));
}

vector<pair<pair<int, int>, pair<int, int>>> Board::checkMarkersLine(pair<int, int> position, pair<int, int> direction, bool player)
{
    vector<pair<pair<int, int>, pair<int, int>>> combinationSequences;
    int playerMarker = player ? PositionStates::whiteMarker : PositionStates::blackMarker;

    int count = 0;

    pair<int, int> startMarker = position;
    startMarker.first -= direction.first;
    startMarker.second -= direction.second;

    while (validPosition(startMarker))
    {
        count++;
        if (getState(startMarker) != playerMarker)
        {
            count--;
            break;
        }

        startMarker.first -= direction.first;
        startMarker.second -= direction.second;
    }
    startMarker.first += direction.first;
    startMarker.second += direction.second;

    pair<int, int> endMarker = position;
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

    if (count >= 5)
    {
        combinationSequences.push_back(make_pair(startMarker, endMarker));
    }

    pair<int, int> newStartMarker = endMarker;
    newStartMarker.first += 2 * direction.first;
    newStartMarker.second += 2 * direction.second;

    while (validPosition(newStartMarker))
    {
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

            if (count >= 5)
            {
                combinationSequences.push_back(make_pair(startMarker, endMarker));
            }

            newStartMarker.first = endMarker.first + 2 * direction.first;
            newStartMarker.second = endMarker.second + 2 * direction.second;
        }
    }

    return combinationSequences;
}

vector<pair<pair<int, int>, pair<int, int>>> Board::checkMarkers(pair<int, int> newPosition, pair<int, int> oldPosition, bool player)
{

    vector<pair<pair<int, int>, pair<int, int>>> combinationSequences;

    pair<bool, pair<pair<int, int>, pair<int, int>>> returnedSequence;

    if (newPosition.first == oldPosition.first)
    {
        int increment = newPosition.second < oldPosition.second ? -1 : 1;

        combinationSequences = checkMarkersLine(newPosition, make_pair(0, increment * 1), player);

        for (int i = oldPosition.second + increment; i != newPosition.second; i += increment)
        {
            returnedSequence = checkMarkersLocal(make_pair(newPosition.first, i), make_pair(1, 0), player);
            if (returnedSequence.first == true)
            {
                combinationSequences.push_back(returnedSequence.second);
            }

            returnedSequence = checkMarkersLocal(make_pair(newPosition.first, i), make_pair(1, 1), player);
            if (returnedSequence.first == true)
            {
                combinationSequences.push_back(returnedSequence.second);
            }
        }
    }

    else if (newPosition.second == oldPosition.second)
    {
        int increment = newPosition.first < oldPosition.first ? -1 : 1;

        combinationSequences = checkMarkersLine(newPosition, make_pair(increment * 1, 0), player);

        for (int i = oldPosition.first + increment; i != newPosition.first; i += increment)
        {
            returnedSequence = checkMarkersLocal(make_pair(newPosition.first, i), make_pair(0, 1), player);
            if (returnedSequence.first == true)
            {
                combinationSequences.push_back(returnedSequence.second);
            }

            returnedSequence = checkMarkersLocal(make_pair(newPosition.first, i), make_pair(1, 1), player);
            if (returnedSequence.first == true)
            {
                combinationSequences.push_back(returnedSequence.second);
            }
        }
    }
    else
    {
        int increment = newPosition.second < oldPosition.second ? -1 : 1;

        combinationSequences = checkMarkersLine(newPosition, make_pair(increment * 1, increment * 1), player);

        for (int i1 = oldPosition.first + increment, i2 = oldPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            returnedSequence = checkMarkersLocal(make_pair(i1, i2), make_pair(0, 1), player);
            if (returnedSequence.first == true)
            {
                combinationSequences.push_back(returnedSequence.second);
            }

            returnedSequence = checkMarkersLocal(make_pair(i1, i2), make_pair(1, 0), player);
            if (returnedSequence.first == true)
            {
                combinationSequences.push_back(returnedSequence.second);
            }
        }
    }

    return combinationSequences;
}

void Board::removeMarkers(pair<int, int> startSeries, pair<int, int> endSeries)
{
    if (startSeries.first == endSeries.first)
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i = endSeries.second + increment; i != startSeries.second; i += increment)
        {
            setState(endSeries.first, i, PositionStates::empty);
        }
    }
    else if (startSeries.second == endSeries.second)
    {
        int increment = startSeries.first < endSeries.first ? -1 : 1;

        for (int i = endSeries.first + increment; i != startSeries.first; i += increment)
        {
            setState(i, endSeries.second, PositionStates::empty);
        }
    }
    else
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i1 = endSeries.first + increment, i2 = endSeries.second + increment; i1 != startSeries.first; i1 += increment, i2 += increment)
        {
            setState(i1, i2, PositionStates::empty);
        }
    }
}

void Board::placeMarkers(pair<int, int> startSeries, pair<int, int> endSeries, bool player)
{
    PositionStates marker = player ? whiteMarker : blackMarker;
    if (startSeries.first == endSeries.first)
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i = endSeries.second + increment; i != startSeries.second; i += increment)
        {
            setState(endSeries.first, i, marker);
        }
    }
    else if (startSeries.second == endSeries.second)
    {
        int increment = startSeries.first < endSeries.first ? -1 : 1;

        for (int i = endSeries.first + increment; i != startSeries.first; i += increment)
        {
            setState(i, endSeries.second, marker);
        }
    }
    else
    {
        int increment = startSeries.second < endSeries.second ? -1 : 1;

        for (int i1 = endSeries.first + increment, i2 = endSeries.second + increment; i1 != startSeries.first; i1 += increment, i2 += increment)
        {
            setState(i1, i2, marker);
        }
    }
}

void Board::playMove(Move m, bool player)
{

    MoveType type = m.type;
    if (type == MoveType::placeRing)
    {
        placeRing(m.initPosition, player);
    }
    else if (type == MoveType::moveRing)
    {
        moveRing(m.finalPosition, m.initPosition, player);
    }
    else if (type == MoveType::removeRing)
    {
        removeRing(m.initPosition);
    }
    else
    {
        removeMarkers(m.initPosition, m.finalPosition);
    }
}

void Board::undoMove(Move m, bool player)
{
    MoveType type = m.type;
    if (type == MoveType::placeRing)
    {
        removeRing(m.initPosition);
    }
    else if (type == MoveType::moveRing)
    {
        setState(m.initPosition, PositionStates::empty);
        moveRing(m.initPosition, m.finalPosition, player);
        setState(m.finalPosition, PositionStates::empty);
    }
    else if (type == MoveType::removeRing)
    {
        placeRing(m.initPosition, player);
    }
    else
    {
        placeMarkers(m.initPosition, m.finalPosition, player);
    }
}

bool Board::isWin(bool player)
{
    PositionStates ring = player ? whiteRing : blackRing;
    if (counts[ring] <= 2)
    {
        return true;
    }

    return false;
}

void Board::getValidRowMoves(Move prevMoveRing, vector<Move> &moves, bool player)
{
    vector<pair<pair<int, int>, pair<int, int>>> rows = checkMarkers(prevMoveRing.finalPosition, prevMoveRing.initPosition, player);

    for (int i = 0; i < rows.size(); i++)
    {
        pair<pair<int, int>, pair<int, int>> row = rows[i];
        pair<int, int> start = row.first, end = row.second;
        if (axialDistance(row.first, row.second) > 5)
        {
            pair<int, int> direction;
            direction.first = end.first - start.first;
            direction.second = end.second - start.second;
            direction = makeUnit(direction);

            pair<int, int> alternateEnd;
            alternateEnd.first = start.first + 4 * direction.first;
            alternateEnd.second = start.second + 4 * direction.second;

            moves.push_back(Move(MoveType::removeRow, start, alternateEnd));

            pair<int, int> alternateStart;
            alternateStart.first = end.first - 4 * direction.first;
            alternateStart.second = end.second - 4 * direction.second;

            moves.push_back(Move(MoveType::removeRow, alternateStart, end));
        }
        else
        {
            moves.push_back(Move(MoveType::removeRow, start, end));
        }
    }
}

void Board::getValidRingMoves(vector<Move> &moves, bool player)
{

    vector<pair<int, int>> searchList = player ? rings1 : rings0;
    for (int i = 0; i < searchList.size(); i++)
    {
        pair<int, int> ringPosition = searchList[i];
        for (int j = 0; j < directions.size(); j++)
        {
            pair<int, int> direction = directions[j];
            pair<int, int> checkPosition = ringPosition;

            bool jumpedMarker = false;

            while (true)
            {
                checkPosition.first += direction.first;
                checkPosition.second += direction.second;

                int positionState = getState(checkPosition);

                jumpedMarker = (positionState == PositionStates::blackMarker) || (positionState == PositionStates::whiteMarker);

                if (!validPosition(checkPosition) || positionState == PositionStates::whiteRing || positionState == PositionStates::blackRing)
                {
                    break;
                }
                else if (positionState == PositionStates::empty)
                {
                    moves.push_back(Move(MoveType::moveRing, ringPosition, checkPosition));

                    // Move moveRing(MoveType::moveRing, ringPosition, checkPosition);

                    // // Apply Move to board
                    // playMove(moveRing, player);

                    // // Check Markers for match
                    // vector<pair<pair<int, int>, pair<int, int>>> rows = checkMarkers(checkPosition, ringPosition, player);

                    // if (rows.size() == 0)
                    // {

                    //     // Append only move ring to moves
                    // }
                    // else
                    // {

                    //     // Append possible remove rows

                    //     for (int k = 0; k < rows.size(); k++)
                    //     {
                    //         pair<pair<int, int>, pair<int, int>> row = rows[k];
                    //         pair<int, int> startPos = row.first, endPos = row.second;
                    //     }
                    // }

                    // // Undo move from board
                    // undoMove(moveRing, player);

                    if (jumpedMarker)
                    {
                        break;
                    }
                }
            }
        }
    }
}

int Board::evaluate(bool player)
{
    int markersCount, ringsCount, score;
    int MARKERS_WEIGHT = 1, RINGS_WEIGHT = -10;
    if (player)
    { // player 1 - white
        markersCount = this->counts[PositionStates::whiteMarker];
        ringsCount = this->counts[PositionStates::whiteRing];
    }
    else
    {
        markersCount = this->counts[PositionStates::blackMarker];
        ringsCount = this->counts[PositionStates::blackRing];
    }
    score = MARKERS_WEIGHT * markersCount + RINGS_WEIGHT * ringsCount;
    return score;
}
