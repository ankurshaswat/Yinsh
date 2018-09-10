/* 
 * File:   Util.h
 * Author: Shashwat Shivam
 *
 */

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <utility>
#include <vector>
#include "Board.h"

using namespace std;

pair<int, int> hex2axial(pair<int, int> hexCoord);
pair<int, int> axial2hex(pair<int, int> axialCoord);
pair<int, pair<int, int>> extractCoordinates(string str, int startPosition);
int axialDistance(pair<int, int> p1, pair<int, int> p2);
pair<int, int> makeUnit(pair<int, int> pos);
void printPointers(vector<vector<Move>> moveSequences);

#endif /* UTIL_H */
