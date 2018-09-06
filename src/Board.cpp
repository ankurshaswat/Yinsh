#include "Board.h"

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
}

int Board::getPosition(pair<int, int> position)
{
    return board[position.first + n][position.second + n];
}

int Board::getPosition(int pos1, int pos2)
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
    return (validPosition(position) && getPosition(position) == PositionStates::empty);
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

    if (!(newPosition.first == currentPosition.first || newPosition.second == currentPosition.second || newPosition.first - currentPosition.first == newPosition.second - currentPosition.second))
    {
        return false;
    }

    int opposingRingState = player ? PositionStates::blackRing : PositionStates::whiteRing;

    if (newPosition.first == currentPosition.first)
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;

        for (int i = currentPosition.second + increment; i != newPosition.second; i += increment)
        {
            if (getPosition(currentPosition.first, i) == opposingRingState)
            {
                return false;
            }
        }
    }
    else if (newPosition.second == currentPosition.second)
    {
        int increment = newPosition.first < currentPosition.first ? -1 : 1;

        for (int i = currentPosition.first + increment; i != newPosition.first; i += increment)
        {
            if (getPosition(i, currentPosition.second) == opposingRingState)
            {
                return false;
            }
        }
    }
    else
    {
        int increment = newPosition.second < currentPosition.second ? -1 : 1;
        for (int i1 = currentPosition.first + increment, i2 = currentPosition.second + increment; i1 != newPosition.first; i1 += increment, i2 += increment)
        {
            if (getPosition(i1, i2) == opposingRingState)
            {
                return false;
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
    setState(position, PositionStates::empty);
}

void Board::setState(pair<int, int> position, int positionVal)
{
    board[position.first + n][position.second + n] = positionVal;
}

void Board::invertState(int pos1, int pos2)
{
    board[pos1 + n][pos2 + n] *= -1;
}

bool Board::placeRing(pair<int, int> position, bool player)
{

    int playerRing = player ? PositionStates::whiteRing : PositionStates::blackRing;

    if (validPlaceRing(position))
    {
        setState(position, playerRing);
        return true;
    }

    return false;
}

bool Board::moveRing(pair<int, int> newPosition, pair<int, int> currentPosition, bool player)
{
    if (!validMoveRing(newPosition, currentPosition, player))
    {
        return false;
    }

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

    return true;
}

pair<bool, pair<pair<int, int>, pair<int, int>>> Board::checkMarkersLocal(pair<int, int> position, pair<int, int> direction, bool player)
{

    int playerMarker = player ? PositionStates::whiteMarker : PositionStates::blackMarker;

    if (getPosition(position) == playerMarker)
    {
        int count = 0;

        pair<int, int> startMarker = position;
        startMarker.first += direction.first;
        startMarker.second += direction.second;

        while (validPosition(startMarker))
        {
            count++;
            if (getPosition(startMarker) != playerMarker)
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
            if (getPosition(endMarker) != playerMarker)
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
        if (getPosition(startMarker) != playerMarker)
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
        if (getPosition(endMarker) != playerMarker)
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
        if (getPosition(newStartMarker) != playerMarker)
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
                if (getPosition(endMarker) != playerMarker)
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