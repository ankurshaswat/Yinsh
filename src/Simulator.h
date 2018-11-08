#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include "AI.h"

enum Policy
{
  Greedy,
};

class Simulator
{
private:
  vector<double> featureWeights;
  string resultsPath;

  void readWeights(string weightsFilePath);
  Move extractLastMoveRing(string str);

public:
  Simulator(string resultsPath, int selfPolicy, int oppPolicy, string weightsFilePath);

  void simulate(int numGames);
};

#endif