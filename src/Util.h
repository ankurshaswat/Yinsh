/* 
 * File:   Util.h
 * Author: Shashwat Shivam
 *
 */

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <utility>
using namespace std;

pair<int, int> hex2axial(pair<int, int> hexCoord)
{


    int c1 = hexCoord.first;
    if(c1 == 0) {
        return make_pair(0,0);
    }
    int c2 = hexCoord.second % hexCoord.first;
    int branch = hexCoord.second / hexCoord.first;

    switch (branch)
    {
    case 0:
        return make_pair(c2, c1);
    case 1:
        return make_pair(c1, c1 - c2);
    case 2:
        return make_pair(c1 - c2, -c2);
    case 3:
        return make_pair(-c2, -c1);
    case 4:
        return make_pair(-c1, c2 - c1);
    case 5:
        return make_pair(c2 - c1, c2);
    default:
        cout << "# Unknown Case";
        return make_pair(0, 0);
    }
}

pair<int, int> axial2hex(pair<int, int> axialCoord)
{
    int a = axialCoord.first;
    int b = axialCoord.second;

    int hex, pos;

    if (a == 0 && b == 0)
    {
        hex = 0;
        pos = 0;
    }
    else if (a <= 0 && b >= 0)
    {
        hex = b - a;
        pos = (5 * hex + b) % (6 * hex);
    }
    else if (a >= 0 && b <= 0)
    {
        hex = a - b;
        pos = 3 * hex - a;
    }
    else if (a >= 0 && b >= 0)
    {
        if (b > a)
        {
            hex = b;
            pos = a;
        }
        else
        {
            hex = a;
            pos = 2 * hex - b;
        }
    }
    else
    {
        if (b > a)
        {
            hex = -a;
            pos = 5 * hex + b;
        }
        else
        {
            hex = -b;
            pos = 3 * hex - a;
        }
    }

    return make_pair(hex, pos);
}

pair<int, pair<int, int>> extractCoordinates(string str, int startPosition)
{

    int i = startPosition;

    int coord1 = 0;
    while (str[i] != ' ')
    {
        coord1 = 10 * coord1 + str[i++] - '0';
    }
    i++;
    int coord2 = 0;
    while (str[i] != ' ' && i < str.length())
    {
        coord1 = 10 * coord1 + str[i++] - '0';
    }

    return make_pair(i, hex2axial(make_pair(coord1, coord2)));
}

#endif /* UTIL_H */
